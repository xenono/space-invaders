#include "objects.h"
#include <stdlib.h>
#include <stdio.h>

void fillAxisLinesArrays(int* xAxisPointer, int* yAxisPointer, int width,int height, sfVector2f position);

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
    int checkX = 0;
    int checkY = 0;

    int* firstObjectXLine = (int*)calloc(firstObject.width,sizeof(int));
    int* firstObjectYLine = (int*)calloc(firstObject.height, sizeof(int));
    fillAxisLinesArrays(firstObjectXLine,firstObjectYLine, firstObject.width, firstObject.height, firstObject.position);

    int* secondObjectXLine = (int*)calloc(secondObject.width,sizeof(int));
    int* secondObjectYLine = (int*)calloc(secondObject.height, sizeof(int));
    fillAxisLinesArrays(secondObjectXLine,secondObjectYLine, secondObject.width, secondObject.height, secondObject.position);



    for(int firstXAxis = 0; firstXAxis < firstObject.width; firstXAxis++){
        for(int secondXAxis = 0; secondXAxis < secondObject.width; secondXAxis++){
            if(firstObjectXLine[firstXAxis] == secondObjectXLine[secondXAxis]) {
                checkX = 1;
                break;
            }
        }
        if (checkX == 1) break;
    };
    for(int firstYAxis = 0; firstYAxis < firstObject.height; firstYAxis++){
        for(int secondYAxis = 0; secondYAxis < secondObject.height; secondYAxis++){
            if(firstObjectYLine[firstYAxis] == secondObjectYLine[secondYAxis]){
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

int checkCollisionStatic(StaticSprite firstObject, StaticSprite secondObject) {

    int checkX = 0;
    int checkY = 0;

    float* firstObjectXLine = (float*)calloc(firstObject.width,sizeof(float));
    float* firstObjectYLine = (float*)calloc(firstObject.height, sizeof(float));
    fillAxisLinesArrays(firstObjectXLine,firstObjectYLine, firstObject.width, firstObject.height, firstObject.position);

    float* secondObjectXLine = (float*)calloc(secondObject.width,sizeof(float));
    float* secondObjectYLine = (float*)calloc(secondObject.height, sizeof(float));
    fillAxisLinesArrays(secondObjectXLine,secondObjectYLine, secondObject.width, secondObject.height, secondObject.position);



    for(int firstXAxis = 0; firstXAxis < firstObject.width; firstXAxis++){
        for(int secondXAxis = 0; secondXAxis < secondObject.width; secondXAxis++){
            if(firstObjectXLine[firstXAxis] == secondObjectXLine[secondXAxis]) {
                checkX = 1;
                break;
            }
        }
        if (checkX == 1) break;
    };
    for(int firstYAxis = 0; firstYAxis < firstObject.height; firstYAxis++){
        for(int secondYAxis = 0; secondYAxis < secondObject.height; secondYAxis++){
            if(firstObjectYLine[firstYAxis] == secondObjectYLine[secondYAxis]){
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

void fillAxisLinesArrays(int* xAxisPointer,int* yAxisPointer, int width, int height, sfVector2f position){
    for(int i = 0; i < width; i++){
        xAxisPointer[i] = position.x + i;
    }
    for(int i = 0; i < height; i++){
        yAxisPointer[i] = position.y + i;
    }
}