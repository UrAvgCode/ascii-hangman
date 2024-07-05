#ifndef GAME_H
#define GAME_H

#include <stdbool.h>

typedef enum { ENGLISH, GERMAN } LANGUAGE;
extern LANGUAGE language;

void render_frame();

bool update_state();

void game_loop();

#endif // GAME_H
