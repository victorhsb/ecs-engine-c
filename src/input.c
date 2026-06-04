#include "input.h"
#include <raylib.h>

InputState InputState_init(void) {
    return (InputState){0};
}

bool pool_events(InputState *input_state) {
    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) {
        input_state->horizontal_axis = -1;
    } else if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) {
        input_state->horizontal_axis = 1;
    } else {
        input_state->horizontal_axis = 0;
    }
    input_state->action = IsKeyPressed(KEY_SPACE);
    return true;
}
