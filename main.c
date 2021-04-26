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

#define GameOver 2
#define Starting 3
#define Running 4
#define True 1
#define False 0

int main() {
    char windowTitle[20] = "Space Invaders";
    sfVideoMode windowSize = {1000, 700, 32};
    sfRenderWindow *window;
    sfEvent event;
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
    sfVector2f playButtonPos = {(float) windowSize.width / 2 - 257, 370};
    StaticSprite playButton = createStaticSprite("playButton.png", playButtonPos, 540, 92);
    sfTexture *playButtonActiveTexture = sfTexture_createFromFile("playButtonActive.png", NULL);
    sfTexture *playAgainButtonActiveTexture = sfTexture_createFromFile("playAgainButtonActive.png", NULL);
    sfTexture *playAgainButtonTexture = sfTexture_createFromFile("playAgainButton.png", NULL);

    sfSprite_setPosition(playButton.pointer, playButton.position);

    // Exit button
    sfVector2f exitButtonPos = {(float) windowSize.width / 2 - 257, 570};
    StaticSprite exitButton = createStaticSprite("exitButton.png", exitButtonPos, 540, 92);
    sfTexture *exitButtonActiveTexture = sfTexture_createFromFile("exitButtonActive.png", NULL);


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

    // Counts compute power
    time_t startCounter, endCounter;
    int frameCounter = 0;
    double frameCounterTimeDif;
    time(&startCounter);



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



    // Score text
    int score = 0;

    sfText *scoreText = sfText_create();
    sfVector2f scoreTextPosition = {10, 0};
    char scoreTextString[20] ;
    snprintf(scoreTextString, 20, "Score: %d", score);
    sfText_setString(scoreText, scoreTextString);
    sfText_setFillColor(scoreText, sfWhite);
    sfText_setFont(scoreText, font);
    sfText_setCharacterSize(scoreText, 30);
    sfText_setPosition(scoreText, scoreTextPosition);

    // Moves spaceship to the window border if it tries to go outside
    // Setting up player's spaceship
    sfVector2f playerStartingPosition = {(float) windowSize.width / 2 - 32, 632};
    sfVector2f spaceshipSticksToLeftBorder = {0, 632};
    sfVector2f spaceshipSticksToRightBorder = {736, 632};
    Sprite player;
    player = createSprite("player.png", 1, playerStartingPosition, 0.9, 64, 64);
    sfSprite_setPosition(player.pointer, playerStartingPosition);


    // Setting up spaceship's bullet
    int randomInvader;
    Sprite bullet;
    bullet = createSprite("bullet.png", 0, playerStartingPosition, 1.1, 10, 26);
    sfSprite_setPosition(bullet.pointer, playerStartingPosition);

    // Setting up Earth Invaders x
    Sprite Invaders[20];
    int invaderPositioner = 50;
    int invadersNumber = sizeof(Invaders) / sizeof(*Invaders);
    double invaderSpeed = 0.2;
    for (int i = 0; i < invadersNumber; ++i) {
        sfVector2f invaderStartingPosition = {(float) 10 + (i % 5 * (invaderPositioner + 30)), 5};
        Invaders[i] = createSprite("ufo.png", 1, invaderStartingPosition, invaderSpeed, 35, 32);
        if (i >= 5) {
            invaderStartingPosition.y = (i / 5) * 50 + invaderStartingPosition.y;
        }
        Invaders[i].position = invaderStartingPosition;
        sfSprite_setPosition(Invaders[i].pointer, invaderStartingPosition);
    }
    // Setting up Invaders' lasers
    Sprite laser;
    double laserSpeed = 1.7;
    laser = createSprite("beams.png", 0, Invaders[0].position, laserSpeed, 15, 19);
    sfSprite_setPosition(laser.pointer, laser.position);

    // Setting up defences
    Sprite Defences[30];
    int counter = 0;
    int rowCounter = 0;
    int defenceLine = 510;
    for(int i = 0; i < 24; i++){
        sfVector2f defencePosition = {100 + (counter * 40) + ((counter / 4) * 160), defenceLine + (rowCounter * 40)};
        counter++;
        Defences[i] = createSprite("defense.png",1,defencePosition,0,40,40);
        sfSprite_setPosition(Defences[i].pointer, Defences[i].position);
        if(counter % 12 == 0){
            counter = 0;
            rowCounter++;
        }
    }
    counter = 0;
    for(int i = 24; i <  30; i++){
        sfVector2f defencePosition = {100 + (counter * 40) + ((counter / 4) * 160), defenceLine + (rowCounter * 40)};
        if(i % 2 == 0){
            counter+=3;
        } else {
            counter++;
        }
        Defences[i] = createSprite("defense.png",1,defencePosition,0,40,40);
        sfSprite_setPosition(Defences[i].pointer, Defences[i].position);
    }

    while (sfRenderWindow_isOpen(window)) {

//        time(&endCounter);
//        frameCounterTimeDif = difftime(endCounter,startCounter);
//        if(frameCounterTimeDif <= 1) {
//            frameCounter++;
//        } else {
//            time(&startCounter);
//            printf("player speed: %f\n", (1387.0 / frameCounter));
//            frameCounter = 0;
//        }

        mouseCoordinates.position.x = (float)sfMouse_getPosition(window).x;
        mouseCoordinates.position.y = (float)sfMouse_getPosition(window).y;
        while (sfRenderWindow_pollEvent(window, &event)) {
            if (event.type == sfEvtClosed) {
                sfRenderWindow_close(window);
            }
            if (sfKeyboard_isKeyPressed(sfKeySpace) && bullet.isAlive == 0 && gameStatus == Running) {
                bullet.isAlive = 1;
                bullet.position.x = player.position.x + player.width / 2 - bullet.width / 2;
                bullet.position.y = player.position.y - bullet.height;
                sfSound_play(shootSound);
                sfSprite_setPosition(bullet.pointer, bullet.position);
            };
            if(sfMouse_isButtonPressed(sfMouseLeft) && (gameStatus == Starting || gameStatus == GameOver)){
                if(checkCollisionStatic(mouseCoordinates,exitButton))
                    sfRenderWindow_close(window);
                if(checkCollisionStatic(mouseCoordinates,playButton)) {
                    // Place laser out of window
                    laser.position.x = -64;
                    laser.position.y = -64;
                    player.isAlive = 1;
                    score = 0;
                    snprintf(scoreTextString, 20, "Score: %d", score);
                    sfText_setString(scoreText, scoreTextString);
                    gameStatus = Running;
                }
            }

        }
        if (gameStatus == Starting) {

            mouseCoordinates.position.x = (float)sfMouse_getPosition(window).x;
            mouseCoordinates.position.y = (float)sfMouse_getPosition(window).y;
            if (sfSprite_getTexture(playButton.pointer) == playButtonActiveTexture) {
                sfSprite_setTexture(playButton.pointer, playButton.texture, sfTrue);
            }
            if (sfSprite_getTexture(exitButton.pointer) == exitButtonActiveTexture) {
                sfSprite_setTexture(exitButton.pointer, exitButton.texture, sfTrue);
            }
            sfRenderWindow_clear(window, sfBlack);
            sfRenderWindow_drawSprite(window, background.pointer, NULL);
            sfRenderWindow_drawSprite(window, startPageSprite.pointer, NULL);

            if(checkCollisionStatic(mouseCoordinates,playButton)){
                sfSprite_setTexture(playButton.pointer, playButtonActiveTexture, sfTrue);
            }
            if(checkCollisionStatic(mouseCoordinates,exitButton)){
                sfSprite_setTexture(exitButton.pointer, exitButtonActiveTexture, sfTrue);
            }
            sfRenderWindow_drawSprite(window, playButton.pointer, NULL);
            sfRenderWindow_drawSprite(window, exitButton.pointer, NULL);
            sfRenderWindow_display(window);
            continue;

        } else if (gameStatus == GameOver) {
            // Reset Game Variables to initial
            for(int i = 0; i< invadersNumber; i++){
                sfVector2f invaderStartingPosition = {(float) 10 + (i % 5 * (invaderPositioner + 30)), 5};
                if (i >= 5) {
                    invaderStartingPosition.y = (i / 5) * 70 + invaderStartingPosition.y;
                }
                Invaders[i].position = invaderStartingPosition;
                Invaders[i].isAlive = True;
                sfSprite_setPosition(Invaders[i].pointer, invaderStartingPosition);
            }
            player.position = playerStartingPosition;
            bullet.position = playerStartingPosition;
            bullet.isAlive = False;
            laser.position = Invaders[0].position;
            sfSprite_setPosition(player.pointer, playerStartingPosition);
            sfSprite_setPosition(laser.pointer, laser.position);

            counter = 0;
            rowCounter = 0;
            for(int i = 0; i < 24; i++){
                sfVector2f defencePosition = {100 + (counter * 40) + ((counter / 4) * 160), defenceLine + (rowCounter * 40)};
                counter++;
                Defences[i] = createSprite("defense.png",1,defencePosition,0,40,40);
                sfSprite_setPosition(Defences[i].pointer, Defences[i].position);
                if(counter % 12 == 0){
                    counter = 0;
                    rowCounter++;
                }
            }
            counter = 0;
            for(int i = 24; i <  30; i++){
                sfVector2f defencePosition = {100 + (counter * 40) + ((counter / 4) * 160), defenceLine + (rowCounter * 40)};
                if(i % 2 == 0){
                    counter+=3;
                } else {
                    counter++;
                }
                Defences[i] = createSprite("defense.png",1,defencePosition,0,40,40);
                sfSprite_setPosition(Defences[i].pointer, Defences[i].position);
            }

            // End of reset instructions

            // Score text
            sfText *scoreResultText = sfText_create();
            snprintf(scoreTextString, 20, "Your score: %d", score);
            sfText_setString(scoreResultText, scoreTextString);
            sfText_setFillColor(scoreResultText, sfWhite);
            sfText_setFont(scoreResultText, font);
            sfText_setCharacterSize(scoreResultText, 100);
            sfVector2f scoreResultTextPosition = {windowSize.width / 2 - (sfText_getLocalBounds(scoreResultText).width / 2), (float) 75};
            sfText_setPosition(scoreResultText, scoreResultTextPosition);

            playButton.texture = playAgainButtonTexture;
            sfSprite_setTexture(playButton.pointer, playButton.texture, sfTrue);

            // Click listener
            mouseCoordinates.position.x = (float)sfMouse_getPosition(window).x;
            mouseCoordinates.position.y = (float)sfMouse_getPosition(window).y;
            if (sfSprite_getTexture(playButton.pointer) == playAgainButtonActiveTexture) {
                sfSprite_setTexture(playButton.pointer, playButton.texture, sfTrue);
            }
            if (sfSprite_getTexture(exitButton.pointer) == exitButtonActiveTexture) {
                sfSprite_setTexture(exitButton.pointer, exitButton.texture, sfTrue);
            }

            // Draws GameOver screen
            sfRenderWindow_clear(window, sfBlack);
            sfRenderWindow_drawSprite(window, background.pointer, NULL);


            scoreResultTextPosition.x =
                    ((float) windowSize.width / 2) - (sfText_getLocalBounds(scoreResultText).width / 2);
            sfText_setPosition(scoreResultText, scoreResultTextPosition);


            if(checkCollisionStatic(mouseCoordinates,playButton)){
                sfSprite_setTexture(playButton.pointer, playAgainButtonActiveTexture, sfTrue);
            }
            if(checkCollisionStatic(mouseCoordinates,exitButton)){
                sfSprite_setTexture(exitButton.pointer, exitButtonActiveTexture, sfTrue);
            }
            sfRenderWindow_drawText(window, scoreResultText, NULL);
            sfRenderWindow_drawSprite(window, playButton.pointer, NULL);
            sfRenderWindow_drawSprite(window, exitButton.pointer, NULL);
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
            if (Invaders[i].isAlive == True) {
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
        if (bullet.isAlive == True) {
            bullet.position.y -= bullet.speed;
            sfSprite_setPosition(bullet.pointer, bullet.position);
            sfRenderWindow_drawSprite(window, bullet.pointer, NULL);
        }
        if (laser.isAlive == True) {
            laser.position.y += laser.speed;
            sfSprite_setPosition(laser.pointer, laser.position);
            sfRenderWindow_drawSprite(window, laser.pointer, NULL);
        }
        // Checks collision between bullet and each Invader
        for (int i = 0; i < 20; i++) {
            if (Invaders[i].isAlive == True && bullet.isAlive == True){
                if(checkCollision(Invaders[i], bullet) == True){
                    Invaders[i].isAlive = False;
                    bullet.isAlive = False;
                    bullet.position.y = 950;
                    score++;
                    // Update Score string
                    snprintf(scoreTextString, 20, "Score: %d", score);
                    sfSound_play(invaderExplosionSound);
                    sfText_setString(scoreText, scoreTextString);
                    sfSprite_setPosition(bullet.pointer, bullet.position);
                }
            }
        }
        // Checks collision between bullet or laser with any defenses
        for(int i = 0; i< 30; i++){
            if(Defences[i].isAlive == True && bullet.isAlive == True){
                if(checkCollision(Defences[i],bullet)){
                    Defences[i].isAlive = False;
                    bullet.isAlive = False;
                    bullet.position.y = 950;
                    sfSound_play(invaderExplosionSound);

                }
            }
            if(Defences[i].isAlive == True && laser.isAlive == True){
                if(checkCollision(Defences[i],laser)){
                    Defences[i].isAlive = False;
                    laser.isAlive = False;
                    laser.position.y = 950;
                    sfSound_play(invaderExplosionSound);

                }
            }
        }

        // Check if player loses
        for (int i = 0; i < 20; i++) {
            if (Invaders[i].isAlive == True && Invaders[i].position.y + 48 >= defenceLine) {
                player.isAlive = False;
                gameStatus = GameOver;
            }
        }

        // Check if player was hit by Invader laser
        if (checkCollision(laser,player)) {
            player.isAlive = False;
            gameStatus = GameOver;
        }
        // Check if player wins
        if(score == invadersNumber){
            player.isAlive = False;
            gameStatus = GameOver;

        }
        // Destroys bullet if its out of top border
        if (bullet.position.y <= -64) {
            bullet.isAlive = False;
        }

        if (laser.position.y >= (float) windowSize.height + 30) {
            laser.isAlive = False;
        }

        // Draws all alive invaders
        for (int i = 0; i < invadersNumber ; ++i) {
            if (Invaders[i].isAlive !=False) {
                sfSprite_setPosition(Invaders[i].pointer, Invaders[i].position);
                sfRenderWindow_drawSprite(window, Invaders[i].pointer, NULL);
            }
        }

        // Draws all alive defences
        for (int i = 0; i < 30; ++i) {
            if (Defences[i].isAlive != False) {
                sfRenderWindow_drawSprite(window, Defences[i].pointer, NULL);
            }
        }

        if (player.isAlive == True) {
            sfRenderWindow_drawSprite(window, player.pointer, NULL);
        }
        if(gameStatus == Running){
            sfRenderWindow_drawText(window, scoreText, NULL);
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
    for (int i = 0; i < 30; i++) {
        sfSprite_destroy(Defences[i].pointer);
        sfTexture_destroy(Defences[i].texture);
    };
    sfRenderWindow_destroy(window);
    return 0;
}

