#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "config.h"

// functions and procedures
void clean_gen(char** gen, int rows, int colums, char death);
char** init_gen(int rows, int colums);
void random_gen(char** gen, int rows, int colums);
void read_from_file(FILE* file, char** gen, int rows, int colums);
void input_gen(char** gen, int rows, int colums, char life, char death);
void run(char** new_gen, char** old_gen, int rows, int colums);
void init_ncurses();
void output_gen(char** gen, int rows, int colums, int num_gen);
void get_next_gen(char** new_gen, char** old_gen, int rows, int colums, char life, char death);
int count_neighbors(char** gen, int row, int colum);

int main(int argc, char** argv) {
    char** new_gen = init_gen(CONSOLE_HEIGHT, CONSOLE_WIDTH);
    char** old_gen = init_gen(CONSOLE_HEIGHT, CONSOLE_WIDTH);
    FILE* file = NULL;

    if (new_gen != NULL && old_gen != NULL) {
        if (argc > 1 && argv[1] != NULL) {
            if (argv[1][0] == 'r' && argv[1][1] == 'a' && argv[1][2] == 'n' && argv[1][3] == 'd' &&
                argv[1][4] == '\0') {
                random_gen(new_gen, CONSOLE_HEIGHT, CONSOLE_WIDTH);
                run(new_gen, old_gen, CONSOLE_HEIGHT, CONSOLE_WIDTH);
            } else if ((file = fopen(argv[1], "r")) != NULL) {
                read_from_file(file, new_gen, CONSOLE_HEIGHT, CONSOLE_WIDTH);
                run(new_gen, old_gen, CONSOLE_HEIGHT, CONSOLE_WIDTH);
                fclose(file);
            } else {
                printf("ERROR File %s not found\n", argv[1]);
            }

        } else {
            input_gen(new_gen, CONSOLE_HEIGHT, CONSOLE_WIDTH, CHAR_OF_LIFE, CHAR_OF_DEATH);
            if (freopen("/dev/tty", "r", stdin) != NULL) {
                run(new_gen, old_gen, CONSOLE_HEIGHT, CONSOLE_WIDTH);
            }
        }
    }

    free(new_gen);
    free(old_gen);

    return 0;
}

// replace all character in gen on death char
void clean_gen(char** gen, int rows, int colums, char death) {
    for (int row = 0; row < rows; row++) {
        for (int colum = 0; colum < colums; colum++) {
            gen[row][colum] = death;
        }
    }
}

// init gen if file null init clean
char** init_gen(int rows, int colums) {
    char** output = malloc(sizeof(char) * rows * colums + sizeof(char*) * rows);

    if (output != NULL) {
        char* ptr_data = (char*)(output + rows);

        for (int i = 0; i < rows; i++) {
            output[i] = ptr_data + colums * i;
        }

        clean_gen(output, rows, colums, CHAR_OF_DEATH);
    }

    return output;
}

// random iniе state of game
void random_gen(char** gen, int rows, int colums) {
    srand(time(NULL));

    if ((Y_POS_LIFE + Y_SIZE_LIFE / 2) <= rows && (X_POS_LIFE + X_SIZE_LIFE / 2) <= colums &&
        (Y_POS_LIFE - Y_SIZE_LIFE / 2) >= 0 && (X_POS_LIFE - X_SIZE_LIFE / 2) >= 0) {
        for (int row = 0; row < Y_SIZE_LIFE; row++) {
            for (int colum = 0; colum < X_SIZE_LIFE; colum++) {
                gen[row + Y_POS_LIFE - Y_SIZE_LIFE / 2 - 1][colum + X_POS_LIFE - X_SIZE_LIFE / 2 - 1] =
                    (((rand() % 100) + 1) <= CHANCE_OF_LIFE) ? CHAR_OF_LIFE : CHAR_OF_DEATH;
            }
        }
    }
}

// for read file gets from args
void read_from_file(FILE* file, char** gen, int rows, int colums) {
    if (file != NULL) {
        char buff[colums + 10];

        for (int row = 0; row < rows; row++) {
            fgets(buff, sizeof(buff), file);

            for (int colum = 0; colum < colums; colum++) {
                if (buff[colum] == CHAR_OF_LIFE) {
                    gen[row][colum] = CHAR_OF_LIFE;
                    buff[colum] = ' ';
                }
            }
        }
    }
}

// scaninig stdin for input gen
void input_gen(char** gen, int rows, int colums, char life, char death) {
    int row = 0;
    int colum = 0;
    char buff;

    while (row < rows && ((buff = getchar()) != EOF)) {
        if (buff == '\n') {
            row++;
            colum = 0;
        } else {
            if (colum < colums) {
                if (buff == life)
                    gen[row][colum] = life;
                else
                    gen[row][colum] = death;
                colum++;
            }
        }
    }
}

// main game method
void run(char** new_gen, char** old_gen, int rows, int colums) {
    int speed = INIT_SPEED, run_game = 1;
    char** buff_gen;
    int num_gen = 0;

    init_ncurses();

    while (run_game) {
        char input = getch();

        if (input == BTN_QUIT) {
            run_game = 0;
            clear();
        }
        if ((input == BTN_SPEED_UP || input == BTN_SPEED_UP + 32) && speed > MAX_SPEED) {
            speed -= DELTA_SPEED;
        }
        if ((input == BTN_SPEED_DOWN || input == BTN_SPEED_DOWN + 32) && speed < MIN_SPEED) {
            speed += DELTA_SPEED;
        }

        flushinp();  // clean input buffer

        output_gen(new_gen, rows, colums, num_gen);

        buff_gen = old_gen;
        old_gen = new_gen;
        new_gen = buff_gen;

        get_next_gen(new_gen, old_gen, rows, colums, CHAR_OF_LIFE, CHAR_OF_DEATH);
        num_gen++;
        napms(speed);
    }

    endwin();
}

// init ncurses
void init_ncurses() {
    initscr();
    noecho();
    curs_set(0);
    nodelay(stdscr, TRUE);
}

// prints gen in console
void output_gen(char** gen, int rows, int colums, int num_gen) {
    move(0, 0);

    for (int row = 0; row < rows + 2; row++) {
        for (int colum = 0; colum < colums + 2; colum++) {
            if (colum == 0 || colum == (colums + 1)) {
                addch(CHAR_FRAME_H);
            } else if (row == 0 || row == (rows + 1)) {
                addch(CHAR_FRAME_W);
            } else {
                addch(gen[row - 1][colum - 1]);
            }
        }
        printw("\n");
    }

    printw("%*cPress %c - to speed up,  %c - to slow down, SPACE BAR - to exit   Gen - %d\n",
           (CONSOLE_WIDTH - 72) / 2, ' ', BTN_SPEED_UP, BTN_SPEED_DOWN, num_gen);
}

// main logic of game create new gen base on old gen
void get_next_gen(char** new_gen, char** old_gen, int rows, int colums, char life, char death) {
    int neighbors;

    for (int row = 0; row < rows; row++) {
        for (int colum = 0; colum < colums; colum++) {
            neighbors = count_neighbors(old_gen, row, colum);

            if (old_gen[row][colum] == life && (neighbors == 2 || neighbors == 3)) {
                new_gen[row][colum] = life;
            } else if (old_gen[row][colum] == death && neighbors == 3) {
                new_gen[row][colum] = life;
            } else {
                new_gen[row][colum] = death;
            }
        }
    }
}

// count life neighbors of cell
int count_neighbors(char** gen, int row, int colum) {
    int count = 0;

    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (i != 0 || j != 0) {
                int row_neigh = (row + i + CONSOLE_HEIGHT) % CONSOLE_HEIGHT;
                int colum_neigh = (colum + j + CONSOLE_WIDTH) % CONSOLE_WIDTH;
                if (gen[row_neigh][colum_neigh] == CHAR_OF_LIFE) {
                    count++;
                }
            }
        }
    }

    return count;
}