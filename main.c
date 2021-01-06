#include <stdio.h>
#include <math.h>
#include <SFML/Graphics.h>
#include <SFML/System.h>
#include <SFML/Audio.h>
#include <SFML/Window.h>

// Sprite object, basically every object on the screen
typedef struct {
    sfSprite *pointer;
    sfTexture *texture;
    sfVector2f position;
    double speed;
    int isAlive;
} Sprite;

// Function to set up basic values for every Sprite
Sprite createSprite(char textureName[20], int isAlive, sfVector2f startingPosition, double speed);

int main() {
    char windowTitle[20] = "CSFML WINDOW";
    sfVideoMode mode = {800, 600, 32};
    sfRenderWindow *window;
    sfEvent event;

    // Setting up window
    window = sfRenderWindow_create(mode, windowTitle, sfClose, NULL);

    // Setting up player's spaceship
    sfVector2f playerStartingPosition = {368, 500};

    // Moves spaceship to the window border if it tries to go outside
    sfVector2f spaceshipSticksToLeftBorder = {0, 500};
    sfVector2f spaceshipSticksToRightBorder = {736, 500};
    Sprite player;
    player = createSprite("player.png", 1, playerStartingPosition, 0.3);
    sfSprite_setPosition(player.pointer, playerStartingPosition);

    // Setting up spaceship's bullet
    Sprite bullet;
    bullet = createSprite("bullet.png", 0, playerStartingPosition, 0.4);
    sfSprite_setPosition(bullet.pointer, playerStartingPosition);

    // Setting up Earth Invaders
    Sprite Invaders[20];
    int invaderPositioner = 50;
    for (int i = 0; i < 20; ++i) {
        sfVector2f invaderStartingPosition = {10 + (i % 5 * (invaderPositioner + 30)), 35};
        Invaders[i] = createSprite("ufo.png", 1, invaderStartingPosition, 0.1);
        if (i >= 5) {
            invaderStartingPosition.y = (i / 5) * 70 + invaderStartingPosition.y;
        }
        Invaders[i].position = invaderStartingPosition;
        sfSprite_setPosition(Invaders[i].pointer, invaderStartingPosition);
    }


    while (sfRenderWindow_isOpen(window)) {
        while (sfRenderWindow_pollEvent(window, &event)) {
            if (event.type == sfEvtClosed) {
                sfRenderWindow_close(window);
            }
            if (event.key.code == sfKeySpace && bullet.isAlive == 0) {
                bullet.isAlive = 1;
                bullet.position.x = player.position.x + 16.00;
                bullet.position.y = player.position.y - 32.00;
                sfSprite_setPosition(bullet.pointer, bullet.position);
            }
        }

        /* Moves player on click */
        if (sfKeyboard_isKeyPressed(sfKeyRight)) {
            player.position.x += player.speed;
            sfSprite_setPosition(player.pointer, player.position);
        } else if (sfKeyboard_isKeyPressed(sfKeyLeft)) {
            player.position.x -= player.speed;
            sfSprite_setPosition(player.pointer, player.position);
        }

        /* Checks if player is not out of border */
        if (player.position.x + 64 >= 800) {
            player.position.x = 800 - 64;
            sfSprite_setPosition(player.pointer, spaceshipSticksToRightBorder);
        } else if (player.position.x <= 0) {
            player.position.x = 0;
            sfSprite_setPosition(player.pointer, spaceshipSticksToLeftBorder);
        }

        // Updates window
        sfRenderWindow_clear(window, sfBlack);
        // Updates bullet
        if (bullet.isAlive == 1) {
            bullet.position.y -= bullet.speed;
            sfSprite_setPosition(bullet.pointer, bullet.position);
            sfRenderWindow_drawSprite(window, bullet.pointer, NULL);
        }
        // Checks collision between bullet and each Invader
        for (int i = 0; i < 20; i++) {
            if (Invaders[i].isAlive == 1 && bullet.position.x >= Invaders[i].position.x &&
                bullet.position.x <= Invaders[i].position.x + 48) {
                if (bullet.position.y <= Invaders[i].position.y + 100) {
                    printf("%f ", bullet.position.y);
                    Invaders[i].isAlive = 0;
                    bullet.isAlive = 0;
                }
            }
        }
        // Destroys bullet if its out of top border
        if (bullet.position.y <= -64) {
            bullet.isAlive = 0;
        }
        // Draws all alive invaders
        for (int i = 0; i < 20; ++i) {
            if (Invaders[i].isAlive != 0) {
                sfRenderWindow_drawSprite(window, Invaders[i].pointer, NULL);
            }
        }
        sfRenderWindow_drawSprite(window, player.pointer, NULL);
        sfRenderWindow_display(window);
    }

    // Cleanup
    sfSprite_destroy(player.pointer);
    sfTexture_destroy(player.texture);
    sfSprite_destroy(bullet.pointer);
    sfTexture_destroy(bullet.texture);
    sfRenderWindow_destroy(window);
    return 0;
}

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