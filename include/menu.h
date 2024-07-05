#ifndef MENU_H
#define MENU_H

typedef enum { START_GAME, CHANGE_LANGUAGE, QUIT_GAME } MENU_STATE;

MENU_STATE updateMenuState(char key);

void drawMenu(int x, int y);

#endif // MENU_H
