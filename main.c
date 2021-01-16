#include <stdio.h>
#include <math.h>
#include <SFML/Graphics.h>
#include <SFML/System.h>
#include <SFML/Audio.h>
#include <SFML/Window.h>
#include <time.h>

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

void spriteDie(Sprite sprite);

int main() {
    char windowTitle[20] = "CSFML WINDOW";
    sfVideoMode windowSize = {1000, 800, 32};
    sfRenderWindow *window;
    sfEvent event;

    time_t start, end;
    double timeDifference;

    time(&start);

    // Spaceship shoot sound
    sfSoundBuffer* shootBuffer;
    shootBuffer = sfSoundBuffer_createFromFile("artillery.wav");
    sfSound* shootSound;
    shootSound = sfSound_create();
    sfSound_setBuffer(shootSound, shootBuffer);

    // Invader explosion sound
    sfSoundBuffer* invaderExplosionBuffer;
    invaderExplosionBuffer = sfSoundBuffer_createFromFile("Explosion+7.wav");
    sfSound* invaderExplosionSound;
    invaderExplosionSound = sfSound_create();
    sfSound_setBuffer(invaderExplosionSound, invaderExplosionBuffer);

    // Setting up window
    window = sfRenderWindow_create(windowSize, windowTitle, sfClose, NULL);

    // Setting up player's spaceship
    sfVector2f playerStartingPosition = {windowSize.width - 32, 700};

    // Moves spaceship to the window border if it tries to go outside
    sfVector2f spaceshipSticksToLeftBorder = {0, 700};
    sfVector2f spaceshipSticksToRightBorder = {736, 700};
    Sprite player;
    player = createSprite("player.png", 1, playerStartingPosition, 0.2);
    sfSprite_setPosition(player.pointer, playerStartingPosition);

    // Setting up spaceship's bullet
    Sprite bullet;
    bullet = createSprite("bullet.png", 0, playerStartingPosition, 0.1);
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
            if (sfKeyboard_isKeyPressed(sfKeySpace) && bullet.isAlive == 0) {
                bullet.isAlive = 1;
                bullet.position.x = player.position.x + 16.00;
                bullet.position.y = player.position.y - 32.00;
                sfSound_play(shootSound);
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
        if (player.position.x + 64 >= windowSize.width) {
            player.position.x = windowSize.width - 64;
            sfSprite_setPosition(player.pointer, player.position);
        } else if (player.position.x <= 0) {
            player.position.x = 0;
            sfSprite_setPosition(player.pointer, player.position);
        }

        // Moves invaders
        time(&end);
        timeDifference = difftime(end, start);
//        if (timeDifference >= 2.5) {
//            time(&start);
//            timeDifference = 0;
        for (int i = 0; i < 20; i++) {
            if (Invaders[i].isAlive == 1) {
                Invaders[i].position.x += 0.01;
                if (Invaders[i].position.x + 48 >= windowSize.width) {
                    Invaders[i].position.x = 10;
                    Invaders[i].position.y += 72;
                    sfSprite_setPosition(Invaders[i].pointer, Invaders[i].position);
                }
            }
        }
//        }

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
            // First checks if invader is alive then checks for if left or right border of the bullet is between two borders of the invader's sprite
            if (Invaders[i].isAlive == 1 && bullet.isAlive == 1 &&
                ((Invaders[i].position.x <= bullet.position.x && bullet.position.x <= Invaders[i].position.x + 35) ||
                 (Invaders[i].position.x <= bullet.position.x + 20 &&
                  bullet.position.x + 20 <= Invaders[i].position.x + 35))) {
                // If x position matches then it checks if bullet's Y is less than Invader's bottom border Y
                if (bullet.position.y <= Invaders[i].position.y + 32) {
                    // If bullet hits, invader and bullet dies
                    Invaders[i].isAlive = 0;
                    bullet.isAlive = 0;
                    bullet.position.y = 950;
                    sfSound_play(invaderExplosionSound);
                    sfSprite_setPosition(bullet.pointer, bullet.position);
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
                sfSprite_setPosition(Invaders[i].pointer, Invaders[i].position);
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
    sfMusic_destroy(shootSound);
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

void spriteDie(Sprite sprite) {
    sprite.isAlive = 0;
    sprite.position.x = 0;
    sprite.position.y = 0;
    sfSprite_setPosition(sprite.pointer, sprite.position);
}
