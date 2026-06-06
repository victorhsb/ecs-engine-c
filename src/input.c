#include "input.h"
#include <raylib.h>

InputState InputState_init(void) { return (InputState){0}; }

bool pool_events(InputState *input_state) {
    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) {
        input_state->horizontal_axis = -1;
    } else if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) {
        input_state->horizontal_axis = 1;
    } else {
        input_state->horizontal_axis = 0;
    }
#define POLL_EVENT(t, k) input_state->events[t] = IsKeyPressed(k);

    POLL_EVENT(EVENT_ACTION, KEY_SPACE);
    POLL_EVENT(EVENT_ESC, KEY_ESCAPE);

    return true;
}
