#include "component.h"
#include "raylib.h"
#include <stdlib.h>

ShapeRectangle init_player_rectangle(void) {
    ShapeRectangle shape;
    shape.color = WHITE;
    shape.size.height = 10;
    shape.size.width = 10;
    return shape;
}

void deinit_player_rectangle(ShapeRectangle *shape) {
    shape->color = BLACK;
    shape->size.height = 0;
    shape->size.width = 0;
    free(shape);
}
