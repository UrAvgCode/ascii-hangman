#include <game.h>

#include <unistd.h>

#include <ncurses.h>

#include <hangman.h>
#include <menu.h>
#include <sprites.h>

typedef enum { GAME_INTRO, GAME_MENU, GAME_LOOP, GAME_OVER } game_state_t;

const static float fps = 60;

static int frame = 0;
static double elapsed = 0;
static char key;
static game_state_t game_state = GAME_INTRO;

language_t language = ENGLISH;

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
            draw_menu(tx - 13, ty - 13);
            break;
        case GAME_LOOP:
        case GAME_OVER:
            draw_hangman(tx - 9, ty, get_mistakes());
            draw_cactus(tx - 35, ty + 4, 0);
            draw_cactus(tx + 27, ty + 9, 1);
            draw_floor(0, ty + 17, max_x);
            draw_hint_letters(10, 7);

            if (game_state == GAME_LOOP) {
                draw_hint_word(10, 5);
                draw_bird((frame / 2) % max_x, 12);
                update_hint_word(key);
            } else {
                draw_guess_word(10, 5);
                draw_game_over(tx - 20, 10, get_has_won());
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
            switch (update_menu_state(key)) {
                case START_GAME:
                    reset();
                    pick_guess_word(language);
                    create_hint_word();
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
            if (is_game_over())
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
