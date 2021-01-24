#include <SFML/Graphics.h>


#ifndef SPACE_INVADERS_OBJECTS_H
#define SPACE_INVADERS_OBJECTS_H

// Sprite object, basically every object on the screen
typedef struct {
    sfSprite *pointer;
    sfTexture *texture;
    sfVector2f position;
    double speed;
    int isAlive;
} Sprite;

typedef struct {
    sfSprite *pointer;
    sfTexture *texture;
    sfVector2f position;
} StaticSprite;

#endif //SPACE_INVADERS_OBJECTS_H
