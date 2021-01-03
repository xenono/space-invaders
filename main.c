#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <math.h>
#include <SFML/Graphics.h>
#include <SFML/System.h>
#include <SFML/Audio.h>
#include <SFML/Window.h>

int main() {
    char windowTitle[20] = "CSFML WINDOW";
    sfVideoMode mode = { 800, 600, 32 };
    sfRenderWindow* window;
    sfEvent event;
    sfText* text;
    sfFont* monstserratFont;
    sfVector2i mousePosition;
    sfVector2f spritePosition = { 250, 500 };
    sfSprite* sprite;
    sfTexture* spriteTexture;

    sfVector2f spaceshipSticksToLeftBorder = { 0, 500 };
    sfVector2f spaceshipSticksToRightBorder = { 736, 500 };


    int isOnRectangle = 0;

    /* Creating sprite */

    sprite = sfSprite_create();
    spriteTexture = sfTexture_createFromFile("player.png", NULL);

    sfSprite_setTexture(sprite, spriteTexture, sfTrue);
    sfSprite_setPosition(sprite, spritePosition);

    window = sfRenderWindow_create(mode, windowTitle, sfClose, NULL);

    while (sfRenderWindow_isOpen(window)) {
        mousePosition = sfMouse_getPosition(window);

        while (sfRenderWindow_pollEvent(window, &event)) {
            if (event.type == sfEvtClosed) {
                sfRenderWindow_close(window);
            }
        }
        /* Moves player on click */
        if (sfKeyboard_isKeyPressed(sfKeyRight)) {
            spritePosition.x += 0.3;
            sfSprite_setPosition(sprite, spritePosition);
        }
        else if (sfKeyboard_isKeyPressed(sfKeyLeft)) {
            spritePosition.x -= 0.3;
            sfSprite_setPosition(sprite, spritePosition);
        }
        /* Checks if player is not out of border */
        if (spritePosition.x + 64 >= 800) {
            sfSprite_setPosition(sprite, spaceshipSticksToRightBorder);
        }
        else if (spritePosition.x <= 0) {
            sfSprite_setPosition(sprite, spaceshipSticksToLeftBorder);
        }

        sfRenderWindow_clear(window, sfBlack);
        sfRenderWindow_drawSprite(window, sprite, NULL);
        sfRenderWindow_display(window);
    }

    /* Cleanup */
    sfSprite_destroy(sprite);
    sfTexture_destroy(spriteTexture);
    sfRenderWindow_destroy(window);
    return 0;
}