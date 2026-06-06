#pragma once

#include <stdbool.h>
#include <stddef.h>

typedef enum EventType {
    EVENT_TYPE_EXIT,
    EVENT_TYPE_SCORE,
    EVENT_COUNT
} EventType;

typedef struct EventQueue {
    void *events;
    size_t event_size;
    size_t count;
    size_t capacity;
} EventQueue;

typedef struct EventBus {
    EventQueue queues[EVENT_COUNT];
} EventBus;

// event types
typedef bool EventExit;
typedef int EventScore;

void destroy_event_bus(EventBus*);
EventBus init_event_bus(void);

#define GENERATE_BUS_API(type, name) \
    bool EventBus_add_##name(EventBus *, void*); \
    void *EventBus_get_##name(EventBus *, size_t); \
    void *EventBus_pull_##name(EventBus *eb); \
    size_t EventBus_has_event_##name(EventBus*);

GENERATE_BUS_API(EventExit, exit)
GENERATE_BUS_API(EventScore, score)
