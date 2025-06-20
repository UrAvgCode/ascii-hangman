#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include <ncurses.h>

#include <hangman.h>
#include <menu.h>
#include <sprites.h>

typedef enum { GAME_INTRO, GAME_MENU, GAME_LOOP, GAME_OVER } game_state_t;

static char key;

static game_state_t game_state = GAME_INTRO;
static language_t language = ENGLISH;

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
            draw_menu(tx - 13, ty - 13, language);
            break;
        case GAME_LOOP:
        case GAME_OVER:
            draw_hangman(tx - 9, ty, get_mistakes());
            draw_bird(tx - 12, ty - 5);

            draw_cactus(tx - 35, ty + 4, 0);
            draw_cactus(tx + 27, ty + 9, 1);
            draw_floor(0, ty + 17, max_x);

            draw_hint_letters(10, 7);

            if (game_state == GAME_LOOP) {
                draw_hint_word(10, 5);
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
            if (is_game_over()) {
                game_state = GAME_OVER;
            }
            break;
        case GAME_OVER:
            break;
    }

    if (key == '\x1B') {
        game_state = GAME_MENU;
    }

    return true;
}

int main() {
    srand(time(NULL));
    load_word_lists();

    initscr();
    curs_set(false);
    keypad(stdscr, true);
    noecho();

    render_frame();
    usleep(2'000'000);

    while (update_state()) {
        render_frame();
        key = getch();
    }

    return 0;
}
