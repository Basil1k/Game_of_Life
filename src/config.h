#ifndef CONFIG_H
#define CONFIG_H

// Define grafics of the game
#define CONSOLE_HEIGHT 25  // -2 for frame
#define CONSOLE_WIDTH 80
#define CHAR_OF_LIFE '@'
#define CHAR_OF_DEATH ' '
#define CHAR_FRAME_H '|'
#define CHAR_FRAME_W '*'

// Define speed params of the game
#define MAX_SPEED 50    // milisecond 0.1 second
#define MIN_SPEED 1000  // milisecond 2 second
#define DELTA_SPEED 50  // milisecond 0.05 second
#define INIT_SPEED 500  // milisecond 1 second

// Define controls
#define BTN_SPEED_UP 'A'
#define BTN_SPEED_DOWN 'Z'
#define BTN_QUIT ' '

// Define params of random state of game
#define X_POS_LIFE CONSOLE_WIDTH / 2 + CONSOLE_WIDTH % 2
#define Y_POS_LIFE CONSOLE_HEIGHT / 2 + CONSOLE_HEIGHT % 2
#define X_SIZE_LIFE 30
#define Y_SIZE_LIFE 15
#define CHANCE_OF_LIFE 30  // procents

#endif