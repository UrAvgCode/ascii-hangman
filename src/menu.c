#include <ncurses.h>

#include "game.h"
#include "menu.h"

static MENU_STATE state = START_GAME;

MENU_STATE updateMenuState(const char key) {
    if (key == 3) {
        state = (MENU_STATE) ((state + 2) % 3);
    } else if (key == 2) {
        state = (MENU_STATE) ((state + 1) % 3);
    } else if (key == 10) {
        return state;
    }

    return -1;
}

void drawMenu(const int x, const int y) {
    mvprintw(y + 0, x, "   _____ _             _   ");
    mvprintw(y + 1, x, "  / ____| |           | |  ");
    mvprintw(y + 2, x, " | (___ | |_ __ _ _ __| |_");
    mvprintw(y + 3, x, "  \\___ \\| __/ _` | '__| __|");
    mvprintw(y + 4, x, "  ____) | || (_| | |  | |_");
    mvprintw(y + 5, x, " |_____/ \\__\\__,_|_|   \\__|");

    mvprintw(y + 8, x, "  _");
    mvprintw(y + 9, x, " | |");
    mvprintw(y + 10, x, " | |     __ _ _ __   __ _");
    mvprintw(y + 11, x, " | |    / _` | '_ \\ / _` |");
    mvprintw(y + 12, x, " | |___| (_| | | | | (_| |_");
    mvprintw(y + 13, x, " |______\\__,_|_| |_|\\__, (_)");
    mvprintw(y + 14, x, "                     __/ |");
    mvprintw(y + 15, x, "                    |___/");

    mvprintw(y + 18, x, "   ____        _ _");
    mvprintw(y + 19, x, "  / __ \\      (_) |");
    mvprintw(y + 20, x, " | |  | |_   _ _| |_");
    mvprintw(y + 21, x, " | |  | | | | | | __|");
    mvprintw(y + 22, x, " | |__| | |_| | | |_");
    mvprintw(y + 23, x, "  \\___\\_\\\\__,_|_|\\__|");

    switch (language) {
        case ENGLISH:
            mvprintw(y + 8, x + 30, "  ______ _   _");
            mvprintw(y + 9, x + 30, " |  ____| \\ | |");
            mvprintw(y + 10, x + 30, " | |__  |  \\| |");
            mvprintw(y + 11, x + 30, " |  __| | . ` |");
            mvprintw(y + 12, x + 30, " | |____| |\\  |");
            mvprintw(y + 13, x + 30, " |______|_| \\_|");
            break;
        case GERMAN:
            mvprintw(y + 8, x + 30, "  _____  ______ ");
            mvprintw(y + 9, x + 30, " |  __ \\|  ____|");
            mvprintw(y + 10, x + 30, " | |  | | |__");
            mvprintw(y + 11, x + 30, " | |  | |  __|");
            mvprintw(y + 12, x + 30, " | |__| | |____");
            mvprintw(y + 13, x + 30, " |_____/|______|");
            break;
    }

    int y_offset = state * 9;
    if (state == CHANGE_LANGUAGE)
        y_offset--;

    mvprintw(y + y_offset, x - 5, "__");
    mvprintw(y + y_offset + 1, x - 5, "\\ \\");
    mvprintw(y + y_offset + 2, x - 5, " \\ \\");
    mvprintw(y + y_offset + 3, x - 5, "  > >");
    mvprintw(y + y_offset + 4, x - 5, " / /");
    mvprintw(y + y_offset + 5, x - 5, "/_/");
}
