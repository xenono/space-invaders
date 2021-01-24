#include "objects.h"
#include <stdlib.h>

Sprite createSprite(char textureName[20], int isAlive, sfVector2f startingPosition, double speed) {
    Sprite sprite;
    sprite.pointer = sfSprite_create();
    sprite.isAlive = isAlive;
    sprite.speed = speed;
    sprite.position = startingPosition;
    sprite.texture = sfTexture_createFromFile(textureName, NULL);
    sfSprite_setTexture(sprite.pointer, sprite.texture, sfTrue);
    return sprite;
}

StaticSprite createStaticSprite(char textureName[20], sfVector2f startingPosition) {
    StaticSprite sprite;
    sprite.pointer = sfSprite_create();
    sprite.position = startingPosition;
    sprite.texture = sfTexture_createFromFile(textureName, NULL);
    sfSprite_setTexture(sprite.pointer, sprite.texture, sfTrue);
    return sprite;
}

int generateNumberInRange(int lower, int upper) {
    int number;
    number = (rand() % (upper - lower)) + lower;
    return number;
}