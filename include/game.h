#pragma once

typedef enum { ENGLISH, GERMAN } language_t;
extern language_t language;

void render_frame();

bool update_state();

void game_loop();
