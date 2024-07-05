#include <ncurses.h>
#include <unistd.h>

#include "game.h"
#include "hangman.h"
#include "menu.h"
#include "sprites.h"

typedef enum { GAME_INTRO, GAME_MENU, GAME_LOOP, GAME_OVER } GAME_STATE;

const static float fps = 60;

static int frame = 0;
static double elapsed = 0;
static char key;
static GAME_STATE game_state = GAME_INTRO;
LANGUAGE language = ENGLISH;

void render_frame() {
    int max_x, max_y;
    getmaxyx(stdscr, max_y, max_x);
    const int tx = max_x / 2;
    const int ty = max_y / 2;

    clear();

    switch (game_state) {
        case GAME_INTRO:
            intro(tx - 23, ty - 13);
            break;
        case GAME_MENU:
            drawMenu(tx - 13, ty - 13);
            break;
        case GAME_LOOP:
        case GAME_OVER:
            drawHangman(tx - 9, ty, getMistakes());
            drawCactus(tx - 35, ty + 4, 0);
            drawCactus(tx + 27, ty + 9, 1);
            drawFloor(0, ty + 17, max_x);
            drawHintLetters(10, 7);

            if (game_state == GAME_LOOP) {
                drawHintWord(10, 5);
                drawBird((frame / 2) % max_x, 12);
                updateHintWord(key);
            } else {
                drawGuessWord(10, 5);
                drawGameOver(tx - 20, 10, getHasWon());
            }
            break;
    }

    refresh();
}

bool update_state() {
    switch (game_state) {
        case GAME_INTRO:
            if (elapsed > 3)
                game_state = GAME_MENU;
            break;
        case GAME_MENU:
            switch (updateMenuState(key)) {
                case START_GAME:
                    reset();
                    pickGuessWord(language);
                    createHintWord();
                    game_state = GAME_LOOP;
                    break;
                case CHANGE_LANGUAGE:
                    language = !language;
                    break;
                case QUIT_GAME:
                    return false;
            }
            break;
        case GAME_LOOP:
            if (isGameOver())
                game_state = GAME_OVER;
            break;
        case GAME_OVER:
            break;
    }

    if (key == 27) {
        game_state = GAME_MENU;
    }

    return true;
}

void game_loop() {
    const float delta_time = 1.0f / fps;
    const float millis = delta_time * 1000.0f;

    initscr();
    curs_set(false);
    timeout(0);
    keypad(stdscr, true);
    noecho();

    while (update_state()) {
        key = getch();

        render_frame();
        frame++;
        elapsed += delta_time;

        usleep(millis * 1000.0f);
    }
}
