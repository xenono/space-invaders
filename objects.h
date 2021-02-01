#include <SFML/Graphics.h>


#ifndef SPACE_INVADERS_OBJECTS_H
#define SPACE_INVADERS_OBJECTS_H

// Sprite object, basically every object on the screen
typedef struct {
    sfSprite *pointer;
    sfTexture *texture;
    sfVector2f position;
    float speed;
    int isAlive;
    float width;
    float height;
} Sprite;

typedef struct {
    sfSprite *pointer;
    sfTexture *texture;
    sfVector2f position;
    int width;
    int height;
} StaticSprite;

#endif //SPACE_INVADERS_OBJECTS_H
