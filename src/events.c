#include "events.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_EVENTS 100

EventQueue init_event_queue(size_t size, size_t capacity) {
    return (EventQueue){
        .events = malloc(size * capacity),
        .event_size = size,
        .capacity = capacity,
        .count = 0,
    };
}

EventBus init_event_bus(void) {
    EventBus bus = (EventBus){0};
    bus.queues[EVENT_TYPE_SCORE] =
        init_event_queue(sizeof(EventScore), MAX_EVENTS);
    bus.queues[EVENT_TYPE_EXIT] = init_event_queue(sizeof(EventExit), 1);
    return bus;
}

// deallocs the events array and resets it to 0
void destroy_queue(EventQueue *queue) {
    free(queue->events);

    queue->events = NULL;
    queue->event_size = 0;
    queue->count = 0;
    queue->capacity = 0;
}

// deallocs the event bus
void destroy_event_bus(EventBus *bus) {
    destroy_queue(&bus->queues[EVENT_TYPE_EXIT]);
    destroy_queue(&bus->queues[EVENT_TYPE_SCORE]);
}

// resets the counter
void reset_queue(EventQueue *q) { q->count = 0; }

bool EventQueue_push(EventQueue *q, void *e) {
    printf("pushing event with size: %d\n", (int)q->event_size);
    assert(q->events != NULL);
    assert(e != NULL);

    if (q->count >= q->capacity) // we don't support resize now. if the event
                                 // does not fit we return false.
        return false;            // does not fit

    // cast the events to a char pointer so we can do byte stuff with it
    char *base = q->events;
    // we iterate over base jumping to the count*size space of it.
    void *dst = base + (q->count * q->event_size);
    // then we copy the bytes of e into the offset destination copying the
    // event_size.
    memcpy(dst, e, q->event_size);
    // finally increment the counter.
    q->count++;
    return true;
}

void *EventQueue_pull(EventQueue *q, size_t index) {
    assert(q != NULL);
    assert(index < q->count);

    return ((char *)q->events) + index * q->event_size;
}

#define GENERATE_BUS_API_FN(type, idx)                                         \
    bool EventBus_add_##type(EventBus *eb, void *e) {                          \
        return EventQueue_push(&eb->queues[idx], e);                           \
    }                                                                          \
    void *EventBus_get_##type(EventBus *eb, size_t i) {                        \
        return EventQueue_pull(&eb->queues[idx], i);                           \
    }                                                                          \
    void *EventBus_pull_##type(EventBus *eb) {                                 \
        EventQueue *eq = &eb->queues[idx];                                     \
        if (eq->count == 0)                                                    \
            return NULL;                                                       \
        void *v = EventQueue_pull(eq, eq->count);                              \
        eq->count--;                                                           \
        return v;                                                              \
    }                                                                          \
    size_t EventBus_has_event_##type(EventBus *eb) {                           \
        return eb->queues[idx].count;                                          \
    }

GENERATE_BUS_API_FN(exit, EVENT_TYPE_EXIT)
GENERATE_BUS_API_FN(score, EVENT_TYPE_SCORE)
