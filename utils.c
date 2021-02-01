#include "objects.h"
#include <stdlib.h>
#include <stdio.h>

void fillAxisLines(float* arrayPointer, float* yAxisPointer, int width,int height, sfVector2f position);

Sprite
createSprite(char textureName[20], int isAlive, sfVector2f startingPosition, double speed, int width, int height) {
    Sprite sprite;
    sprite.pointer = sfSprite_create();
    sprite.isAlive = isAlive;
    sprite.width = width;
    sprite.height = height;
    sprite.speed = speed;
    sprite.position = startingPosition;
    sprite.texture = sfTexture_createFromFile(textureName, NULL);
    sfSprite_setTexture(sprite.pointer, sprite.texture, sfTrue);
    return sprite;
}

StaticSprite createStaticSprite(char textureName[20], sfVector2f startingPosition, int width, int height) {
    StaticSprite sprite;
    sprite.pointer = sfSprite_create();
    sprite.position = startingPosition;
    sprite.width = width;
    sprite.height = height;
    sprite.texture = sfTexture_createFromFile(textureName, NULL);
    sfSprite_setTexture(sprite.pointer, sprite.texture, sfTrue);
    return sprite;
}


int generateNumberInRange(int lower, int upper) {
    int number;
    number = (rand() % (upper - lower)) + lower;
    return number;
}



int checkCollision(Sprite firstObject, Sprite secondObject) {
    if ((firstObject.position.x <= secondObject.position.x &&
         secondObject.position.x <= firstObject.position.x + firstObject.width) ||
         (firstObject.position.x <= secondObject.position.x + secondObject.width &&
         secondObject.position.x + secondObject.width <= firstObject.position.x + firstObject.width)
         ){
        if(secondObject.position.y <= firstObject.position.y + firstObject.height){
            return 1;
        }
    }

    return 0;
}

int checkCollisionStatic(StaticSprite firstObject, StaticSprite secondObject) {
//    if ((firstObject.position.x <= secondObject.position.x &&
//         secondObject.position.x <= firstObject.position.x + firstObject.width) ||
//        (firstObject.position.x <= secondObject.position.x + secondObject.width &&
//         secondObject.position.x + secondObject.width <= firstObject.position.x + firstObject.width)
//            ){
//        if(secondObject.position.y <= firstObject.position.y + firstObject.height && secondObject.position.y + secondObject.height >= firstObject.position.y){
//            return 1;
//        }
    int checkX = 0;
    int checkY = 0;
    float* firstObjectXLine = (float*)calloc(firstObject.width,sizeof(float));
    float* firstObjectYLine = (float*)calloc(firstObject.height, sizeof(float));
    fillAxisLines(firstObjectXLine,firstObjectYLine, firstObject.width, firstObject.height, firstObject.position);

    float* secondObjectXLine = (float*)calloc(secondObject.width,sizeof(float));
    float* secondObjectYLine = (float*)calloc(secondObject.height, sizeof(float));
    fillAxisLines(secondObjectXLine,secondObjectYLine, secondObject.width, secondObject.height, secondObject.position);



    for(int i = 0; i < firstObject.width; i++){
        for(int j = 0; j < secondObject.width; j++){
            if(firstObjectXLine[i] == secondObjectXLine[j]) {
                checkX = 1;
                break;
            }
        }
        if (checkX == 1) break;
    };
    for(int i = 0; i < firstObject.height; i++){
        for(int j = 0; j < secondObject.height; j++){
            if(firstObjectYLine[i] == secondObjectYLine[j]){
                checkY = 1;
                break;
            }
        }
        if (checkY == 1) break;
    }

    free(firstObjectXLine);
    free(secondObjectXLine);
    if ( checkX == 1 && checkY == 1)
        return 1;
    return 0;
}

void fillAxisLines(float* xAxisPointer,float* yAxisPointer, int width, int height, sfVector2f position){
    for(int i = 0; i < width; i++){
        xAxisPointer[i] = position.x + (float)i;
    }
    for(int i = 0; i < height; i++){
        yAxisPointer[i] = position.y + (float)i;
    }
}