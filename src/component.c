#include <raylib.h>
#include "component.h"

Color get_brick_color(const Brick brick) {
    if (brick.hp < 10) {
        return RED;
    }
    return brick.color;
}
