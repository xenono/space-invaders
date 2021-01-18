#include <stdio.h>
#include <math.h>
#include <SFML/Graphics.h>
#include <SFML/System.h>
#include <SFML/Audio.h>
#include <SFML/Window.h>
#include <time.h>
#include <stdlib.h>

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
int generateNumberInRange(int lower, int upper);

int main() {
    char windowTitle[20] = "Space Invaders";
    sfVideoMode windowSize = {1000, 800, 32};
    sfRenderWindow *window;
    sfEvent event;

    // Setting up window
    window = sfRenderWindow_create(windowSize, windowTitle, sfClose, NULL);
    sfTexture* backgroundTexture = sfTexture_createFromFile("background.jpg",NULL);
    sfSprite* backgroundSprite = sfSprite_create();
    sfVector2f backgroundPos = {0,0};
    sfSprite_setTexture(backgroundSprite, backgroundTexture, sfTrue);
    sfSprite_setPosition(backgroundSprite, backgroundPos);

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
    sfVector2f playerStartingPosition = {windowSize.width - 32, 700};

    // Moves spaceship to the window border if it tries to go outside
    sfVector2f spaceshipSticksToLeftBorder = {0, 700};
    sfVector2f spaceshipSticksToRightBorder = {736, 700};
    Sprite player;
    player = createSprite("player.png", 1, playerStartingPosition, 0.2);
    sfSprite_setPosition(player.pointer, playerStartingPosition);

    // Setting up spaceship's bullet
    int randomInvader;
    Sprite bullet;
    bullet = createSprite("bullet.png", 0, playerStartingPosition, 0.1);
    sfSprite_setPosition(bullet.pointer, playerStartingPosition);

    // Setting up Earth Invaders x
    Sprite Invaders[20];
    int invaderPositioner = 50;
    for (int i = 0; i < 20; ++i) {
        sfVector2f invaderStartingPosition = {10 + (i % 5 * (invaderPositioner + 30)), 35};
        Invaders[i] = createSprite("ufo.png", 1, invaderStartingPosition, 0.025);
        if (i >= 5) {
            invaderStartingPosition.y = (i / 5) * 70 + invaderStartingPosition.y;
        }
        Invaders[i].position = invaderStartingPosition;
        sfSprite_setPosition(Invaders[i].pointer, invaderStartingPosition);
    }
    // Setting up Invaders' lasers
    Sprite laser;
    laser = createSprite("beams.png", 0, Invaders[0].position, 0.4);
    sfSprite_setPosition(laser.pointer, laser.position);


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


        time(&end);
        timeDifference = difftime(end, start);
        if (timeDifference >= timeInterval) {
            time(&start);
            timeDifference = 0;
            timeInterval = generateNumberInRange(lowerRange, upperRange);
            printf("%d", timeInterval);
        }

        if(laser.isAlive == 0){
            randomInvader = rand() % 20;
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
                if (Invaders[i].position.x + 48 >= windowSize.width) {
                    Invaders[i].position.x = 10;
                    Invaders[i].position.y += 72;
                    sfSprite_setPosition(Invaders[i].pointer, Invaders[i].position);
                }
            }
        }
        // Drawing Everything
        // Updates window
        sfRenderWindow_clear(window, sfBlack);
        sfRenderWindow_drawSprite(window, backgroundSprite, NULL);
        // Updates bullet
        if (bullet.isAlive == 1) {
            bullet.position.y -= bullet.speed;
            sfSprite_setPosition(bullet.pointer, bullet.position);
            sfRenderWindow_drawSprite(window, bullet.pointer, NULL);
        }
        if(laser.isAlive == 1){
            laser.position.y += laser.speed;
            sfSprite_setPosition(laser.pointer, laser.position);
            sfRenderWindow_drawSprite(window, laser.pointer, NULL);
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
        // Check if player loses
        for(int i = 0; i< 20; i++){
            if(Invaders[i].isAlive == 1 && Invaders[i].position.y + 48 >= 700){
                printf("END?");
            }
        }
        // Destroys bullet if its out of top border
        if (bullet.position.y <= -64) {
            bullet.isAlive = 0;
        }
        if(laser.position.y >= windowSize.height + 30){
            laser.isAlive = 0;
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

int generateNumberInRange(int lower, int upper){
    int number;
    number = (rand() % (upper - lower)) + lower;
    return number;
}
