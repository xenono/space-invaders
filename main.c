#include "objects.h"
#include "utils.h"
#include <stdio.h>
#include <math.h>
#include <SFML/Graphics.h>
#include <SFML/System.h>
#include <SFML/Audio.h>
#include <SFML/Window.h>
#include <time.h>
#include <stdlib.h>
#include <chipmunk/chipmunk.h>

#define GameOver 2
#define Starting 3
#define Running 4
#define True 1
#define False 0

int main() {
    char windowTitle[20] = "Space Invaders";
    sfVideoMode windowSize = {1000, 800, 32};
    sfRenderWindow *window;
    sfEvent event;
    cpSpace *space = cpSpaceNew();
    int gameStatus = Starting;

    // Setting up window
    window = sfRenderWindow_create(windowSize, windowTitle, sfClose, NULL);
    sfVector2f backgroundPos = {0, 0};
    StaticSprite background = createStaticSprite("background.jpg", backgroundPos, 1000, 800);
    sfSprite_setPosition(background.pointer, background.position);

    // Holds mouse object
    StaticSprite mouseCoordinates;
    mouseCoordinates.width = 1;
    mouseCoordinates.height = 1;

    // Starting title
    sfVector2f startPagePos = {(float) windowSize.width / 2 - 490, 70};
    StaticSprite startPageSprite = createStaticSprite("startingTitle.png", startPagePos, 1000, 200);
    sfSprite_setPosition(startPageSprite.pointer, startPageSprite.position);

    // Play button
    sfVector2f playButtonPos = {(float) windowSize.width / 2 - 280, 370};
    StaticSprite playButton = createStaticSprite("playButton.png", playButtonPos, 540, 92);
    sfTexture *playButtonActiveTexture = sfTexture_createFromFile("playButtonActive.png", NULL);

    sfSprite_setPosition(playButton.pointer, playButton.position);

    // Exit button
    sfVector2f exitButtonPos = {(float) windowSize.width / 2 - 280, 570};
    StaticSprite exitButton = createStaticSprite("exitButton.png", exitButtonPos, 540, 92);

    sfSprite_setPosition(exitButton.pointer, exitButton.position);

    // Font
    sfFont *font;
    font = sfFont_createFromFile("montserrat.ttf");

    // Initialize Time
    time_t start, end;
    time_t now = time(0);
    int lowerRange = 2;
    int upperRange = 6; // upperRange + 1
    double timeDifference;
    int timeInterval = generateNumberInRange(lowerRange, upperRange);
    time(&start);



    // Spaceship shoot sound
    sfSoundBuffer *shootBuffer;
    sfSound *shootSound;
    shootBuffer = sfSoundBuffer_createFromFile("artillery.wav");
    shootSound = sfSound_create();
    sfSound_setBuffer(shootSound, shootBuffer);

    // Invader explosion sound
    sfSoundBuffer *invaderExplosionBuffer;
    sfSound *invaderExplosionSound;
    invaderExplosionBuffer = sfSoundBuffer_createFromFile("Explosion+7.wav");
    invaderExplosionSound = sfSound_create();
    sfSound_setBuffer(invaderExplosionSound, invaderExplosionBuffer);

    // Laser shoot sound
    sfSoundBuffer *laserShootBuffer;
    sfSound *laserShootSound;
    float volume = 10;
    laserShootBuffer = sfSoundBuffer_createFromFile("Laser.wav");
    laserShootSound = sfSound_create();
    sfSound_setBuffer(laserShootSound, laserShootBuffer);
    sfSound_setVolume(laserShootSound, volume);

    // Setting up player's spaceship
    sfVector2f playerStartingPosition = {(float) windowSize.width - 32, 700};

    // Moves spaceship to the window border if it tries to go outside
    sfVector2f spaceshipSticksToLeftBorder = {0, 700};
    sfVector2f spaceshipSticksToRightBorder = {736, 700};
    Sprite player;
    player = createSprite("player.png", 1, playerStartingPosition, 0.2, 64, 64);
    sfSprite_setPosition(player.pointer, playerStartingPosition);

    // Setting up spaceship's bullet
    int randomInvader;
    Sprite bullet;
    bullet = createSprite("bullet.png", 0, playerStartingPosition, 0.1, 20, 32);
    sfSprite_setPosition(bullet.pointer, playerStartingPosition);

    // Setting up Earth Invaders x
    Sprite Invaders[20];
    int invaderPositioner = 50;
    for (int i = 0; i < 20; ++i) {
        sfVector2f invaderStartingPosition = {(float) 10 + (i % 5 * (invaderPositioner + 30)), 35};
        Invaders[i] = createSprite("ufo.png", 1, invaderStartingPosition, 0.025, 35, 32);
        if (i >= 5) {
            invaderStartingPosition.y = (i / 5) * 70 + invaderStartingPosition.y;
        }
        Invaders[i].position = invaderStartingPosition;
        sfSprite_setPosition(Invaders[i].pointer, invaderStartingPosition);
    }
    // Setting up Invaders' lasers
    Sprite laser;
    laser = createSprite("beams.png", 0, Invaders[0].position, 0.4, 15, 19);
    sfSprite_setPosition(laser.pointer, laser.position);



    while (sfRenderWindow_isOpen(window)) {
        while (sfRenderWindow_pollEvent(window, &event)) {
            if (event.type == sfEvtClosed) {
                sfRenderWindow_close(window);
            }
            if (sfKeyboard_isKeyPressed(sfKeyEnter) && gameStatus == Starting || gameStatus == GameOver) {
                laser.position.x = 0;
                laser.position.y = 0;
                player.isAlive = 1;
                gameStatus = Running;
            };
            if (sfKeyboard_isKeyPressed(sfKeySpace) && bullet.isAlive == 0 && gameStatus == Running) {
                bullet.isAlive = 1;
                bullet.position.x = player.position.x + 16;
                bullet.position.y = player.position.y - 32;
                sfSound_play(shootSound);
                sfSprite_setPosition(bullet.pointer, bullet.position);
            };

        }
        if (gameStatus == Starting) {
            mouseCoordinates.position.x = (float)sfMouse_getPosition(window).x;
            mouseCoordinates.position.y = (float)sfMouse_getPosition(window).y;
            if (sfSprite_getTexture(playButton.pointer) == playButtonActiveTexture) {
                sfSprite_setTexture(playButton.pointer, playButton.texture, sfTrue);
            }
            sfRenderWindow_clear(window, sfBlack);
            sfRenderWindow_drawSprite(window, background.pointer, NULL);
            sfRenderWindow_drawSprite(window, startPageSprite.pointer, NULL);
//            if (mouseCoordinates.x >= playButton.position.x + 10.0 &&
//                mouseCoordinates.x <= playButton.position.x + 530.0) {
//                if (mouseCoordinates.y <= playButton.position.y + 85.0 &&
//                    mouseCoordinates.y >= playButton.position.y + 10.0) {
//                    sfSprite_setTexture(playButton.pointer, playButtonActiveTexture, sfTrue);
//                }
//            }

//            checkCollisionStatic(playButton,mouseCoordinates);
            if(checkCollisionStatic(mouseCoordinates,playButton)){
                sfSprite_setTexture(playButton.pointer, playButtonActiveTexture, sfTrue);

            }
            sfRenderWindow_drawSprite(window, playButton.pointer, NULL);
            sfRenderWindow_drawSprite(window, exitButton.pointer, NULL);
            sfRenderWindow_display(window);
            continue;
        } else if (gameStatus == GameOver) {
            sfRenderWindow_clear(window, sfBlack);
            sfText *startButtonText = sfText_create();
            sfVector2f startButtonTextPosition = {0, (float) windowSize.height / 2};
            sfText_setString(startButtonText, "Press Enter to play");
            sfText_setFillColor(startButtonText, sfWhite);
            sfText_setFont(startButtonText, font);
            sfText_setCharacterSize(startButtonText, 50);
            startButtonTextPosition.x =
                    ((float) windowSize.width / 2) - (sfText_getLocalBounds(startButtonText).width / 2);
            sfText_setPosition(startButtonText, startButtonTextPosition);

            sfRenderWindow_drawText(window, startButtonText, NULL);
            sfRenderWindow_display(window);
            continue;
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
        if (player.position.x + player.width >= (float) windowSize.width) {
            player.position.x = (float) windowSize.width - player.width;
            sfSprite_setPosition(player.pointer, player.position);
        } else if (player.position.x <= 0) {
            player.position.x = 0;
            sfSprite_setPosition(player.pointer, player.position);
        }


        time(&end);
        timeDifference = difftime(end, start);
        if (timeDifference >= timeInterval) {
            time(&start);
            timeDifference = 0;
            timeInterval = generateNumberInRange(lowerRange, upperRange);
            printf("%d", timeInterval);
        }

        if (laser.isAlive == 0) {
            randomInvader = rand() % 20;
            while (Invaders[randomInvader].isAlive != 1) {
                randomInvader = rand() % 20;
            }
            laser.isAlive = 1;
            laser.position = Invaders[randomInvader].position;
            laser.position.x += 24;
            laser.position.y += 48;
            sfSprite_setPosition(laser.pointer, laser.position);
            sfSound_play(laserShootSound);
        }


        for (int i = 0; i < 20; i++) {
            if (Invaders[i].isAlive == 1) {
                Invaders[i].position.x += Invaders[i].speed;
                if (Invaders[i].position.x + 48 >= (float) windowSize.width) {
                    Invaders[i].position.x = 10;
                    Invaders[i].position.y += 72;
                    sfSprite_setPosition(Invaders[i].pointer, Invaders[i].position);
                }
            }
        }
        // Drawing Everything
        // Updates window
        sfRenderWindow_clear(window, sfBlack);
        sfRenderWindow_drawSprite(window, background.pointer, NULL);
        // Updates bullet
        if (bullet.isAlive == 1) {
            bullet.position.y -= bullet.speed;
            sfSprite_setPosition(bullet.pointer, bullet.position);
            sfRenderWindow_drawSprite(window, bullet.pointer, NULL);
        }
        if (laser.isAlive == 1) {
            laser.position.y += laser.speed;
            sfSprite_setPosition(laser.pointer, laser.position);
            sfRenderWindow_drawSprite(window, laser.pointer, NULL);
        }
        // Checks collision between bullet and each Invader
        for (int i = 0; i < 20; i++) {
            // First checks if invader is alive then checks for if left or right border of the bullet is between two borders of the invader's sprite
//            if (Invaders[i].isAlive == 1 && bullet.isAlive == 1 &&
//                ((Invaders[i].position.x <= bullet.position.x && bullet.position.x <= Invaders[i].position.x + 35) ||
//                 (Invaders[i].position.x <= bullet.position.x + 20 &&
//                  bullet.position.x + 20 <= Invaders[i].position.x + 35))) {
//                // If x position matches then it checks if bullet's Y is less than Invader's bottom border Y
//                if (bullet.position.y <= Invaders[i].position.y + 32) {
//                    // If bullet hits, invader and bullet dies
//                    Invaders[i].isAlive = 0;
//                    bullet.isAlive = 0;
//                    bullet.position.y = 950;
//                    sfSound_play(invaderExplosionSound);
//                    sfSprite_setPosition(bullet.pointer, bullet.position);
//                }
//            }
            if (Invaders[i].isAlive == 1 && bullet.isAlive == 1){
                if(checkCollision(Invaders[i], bullet) == True){
                    Invaders[i].isAlive = 0;
                    bullet.isAlive = 0;
                    bullet.position.y = 950;
                    sfSound_play(invaderExplosionSound);
                    sfSprite_setPosition(bullet.pointer, bullet.position);
                }
            }
        }
        // Check if player loses
        for (int i = 0; i < 20; i++) {
            if (Invaders[i].isAlive == 1 && Invaders[i].position.y + 48 >= 700) {
                player.isAlive = 0;
            }
        }
        // Check if player was hit by Invader laser
        if ((laser.position.y >= 714) &&
            ((laser.position.x >= player.position.x && laser.position.x <= player.position.x + 64) ||
             (laser.position.x + 15 >= player.position.x && laser.position.x + 15 <= player.position.x + 64))) {
            player.isAlive = 0;
            gameStatus = GameOver;
        }
        // Destroys bullet if its out of top border
        if (bullet.position.y <= -64) {
            bullet.isAlive = 0;
        }
        if (laser.position.y >= (float) windowSize.height + 30) {
            laser.isAlive = 0;
        }
        // Draws all alive invaders
        for (int i = 0; i < 20; ++i) {
            if (Invaders[i].isAlive != 0) {
                sfSprite_setPosition(Invaders[i].pointer, Invaders[i].position);
                sfRenderWindow_drawSprite(window, Invaders[i].pointer, NULL);
            }
        }
        if (player.isAlive == 1) {
            sfRenderWindow_drawSprite(window, player.pointer, NULL);
        }

        sfRenderWindow_display(window);
    }

    // Cleanup
    sfSprite_destroy(player.pointer);
    sfTexture_destroy(player.texture);
    sfSprite_destroy(bullet.pointer);
    sfTexture_destroy(bullet.texture);
    sfTexture_destroy(background.texture);
    sfSound_destroy(laserShootSound);
    sfSound_destroy(invaderExplosionSound);
    sfSound_destroy(shootSound);
    // Cleaning Invaders
    for (int i = 0; i < 20; i++) {
        sfSprite_destroy(Invaders[i].pointer);
        sfTexture_destroy(Invaders[i].texture);
    };
    sfRenderWindow_destroy(window);
    return 0;
}
