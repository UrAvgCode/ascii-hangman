#pragma once

typedef enum { START_GAME, CHANGE_LANGUAGE, QUIT_GAME } menu_state_t;

menu_state_t update_menu_state(char key);

void draw_menu(int x, int y);
