#pragma once

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
typedef struct ExitEvent {
    
} ExitEvent;