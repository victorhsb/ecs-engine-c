#pragma once

#include <stdbool.h>

// right now we only have sideways movement on the player
// this means that only the X axis is caught. which simplifies the
// input handling.
// Later i'll probably also have an action event for triggering effects
// or even a "parry" to boost ball damage.
typedef struct InputState {
    int horizontal_axis;
} InputState;

InputState InputState_init(void);
bool pool_events(InputState *input_state);
