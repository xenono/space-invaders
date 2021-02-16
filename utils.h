#include "objects.h";

#ifndef SPACE_INVADERS_UTILS_H
#define SPACE_INVADERS_UTILS_H

// Function to set up basic values for every Sprite
Sprite createSprite(char textureName[20], int isAlive, sfVector2f startingPosition, double speed, int width, int height );

StaticSprite createStaticSprite(char textureName[20], sfVector2f startingPosition, int width, int height);

int generateNumberInRange(int lower, int upper);

int checkCollision(Sprite firstObject,Sprite secondObject);
int checkCollisionStatic(StaticSprite firstObject, StaticSprite secondObject);
#endif //SPACE_INVADERS_UTILS_H
