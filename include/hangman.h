#pragma once

typedef enum { ENGLISH = 0, GERMAN = 1 } language_t;

void intro(int x, int y);

int get_mistakes();

bool get_has_won();

bool is_game_over();

void pick_guess_word(language_t);

void create_hint_word();

void update_hint_word(char chr);

void draw_hint_word(int x, int y);

void draw_hint_letters(int x, int y);

void draw_guess_word(int x, int y);

bool is_letter(char chr);

bool equals_letter(char l1, char l2);

char to_uppercase(char c);

void reset();
