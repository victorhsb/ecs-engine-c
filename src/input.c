#include "input.h"
#include <raylib.h>

InputState InputState_init(void) {
    return (InputState){0};
}

bool pool_events(InputState *input_state) {
    if (IsKeyDown(KEY_A)) {
        input_state->horizontal_axis = -1;
    } else if (IsKeyDown(KEY_D)) {
        input_state->horizontal_axis = 1;
    } else {
        input_state->horizontal_axis = 0;
    }
    return true;
}
