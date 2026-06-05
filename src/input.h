#pragma once

#include <stdbool.h>
#include <stdint.h>

typedef enum NotificationEvent {
    EVENT_ACTION,
    EVENT_EXIT,
    NOTIFICATION_EVENT_COUNT,
} NotificationEvent;

// right now we only have sideways movement on the player
// this means that only the X axis is caught. which simplifies the
// input handling.
// Later i'll probably also have an action event for triggering effects
// or even a "parry" to boost ball damage.
typedef struct InputState {
    int8_t horizontal_axis;
    bool events[NOTIFICATION_EVENT_COUNT];
} InputState;

InputState InputState_init(void);
bool pool_events(InputState *input_state);
