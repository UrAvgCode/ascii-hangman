#include <hangman.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <ncurses.h>

#define LENGTH 20

int mistakes = 0;
bool has_won = false;

char guess_word[LENGTH];
char hint_word[LENGTH];

char used_letters[6];
int used_letters_counter = 0;

void pick_guess_word(const bool lang) {
    /* Open words file */
    FILE *fp;
    if (lang)
        fp = fopen("wordlist-en", "r");
    else
        fp = fopen("wordlist-de", "r");

    if (fp == NULL) {
        perror("Unable to locate word list");
        exit(EXIT_FAILURE);
    }

    /* Count words in file */
    char word[LENGTH];
    long wc = 0;
    while (fgets(word, sizeof word, fp) != NULL) {
        ++wc;
    }

    /* Choose random word */
    char randwords[LENGTH];
    srand((unsigned) time(NULL));

    rewind(fp);
    const int sel = rand() % wc + 1;
    for (int j = 0; j < sel; j++) {
        if (fgets(word, sizeof word, fp) == NULL) {
            perror("Error in fgets()");
        }
    }
    strcpy(randwords, word);

    if (fclose(fp) != 0) {
        perror("Unable to close file");
    }

    for (int i = 0; i < LENGTH; i++)
        if (randwords[i] == '\n')
            randwords[i] = '\0';

    strcpy(guess_word, randwords);
}

void create_hint_word() {
    for (int i = 0; i < LENGTH; i++) {
        if (is_letter(guess_word[i])) {
            hint_word[i] = '-';
        }
    }
}

void update_hint_word(char chr) {
    if (!is_letter(chr)) {
        return;
    }

    for (int i = 0; i <= used_letters_counter; i++) {
        if (equals_letter(used_letters[i], chr)) {
            return;
        }
    }

    bool contains = false;
    for (int i = 0; i < LENGTH; i++) {
        if (equals_letter(guess_word[i], chr)) {
            hint_word[i] = guess_word[i];
            contains = true;
        }
    }

    if (!contains) {
        mistakes += 1;
        used_letters[used_letters_counter] = to_uppercase(chr);
        used_letters_counter++;
    }
}

void draw_hint_word(const int x, const int y) { mvprintw(y, x, "HintWord: %s", hint_word); }

void draw_hint_letters(const int x, const int y) {
    mvprintw(y, x, "Wrong Letters:");

    for (int i = 0; i < used_letters_counter; i++) {
        mvprintw(y + 1, x + 2 * i, "%c", used_letters[i]);
        if (i != used_letters_counter - 1)
            mvprintw(y + 1, x + 1 + 2 * i, ",");
    }
}

void draw_guess_word(int x, int y) { mvprintw(y, x, "GuessWord: %s", guess_word); }

bool is_game_over() {
    if (mistakes >= 6)
        return true;

    if (strcmp(guess_word, hint_word) == 0) {
        has_won = true;
        return true;
    }

    return false;
}

void reset() {
    mistakes = 0;
    used_letters_counter = 0;
    has_won = false;

    for (int i = 0; i < LENGTH; i++) {
        guess_word[i] = '\0';
        hint_word[i] = '\0';
    }

    for (int i = 0; i < 6; i++)
        used_letters[i] = '\0';
}

bool is_letter(const char chr) {
    if ((chr >= 'a' && chr <= 'z') || (chr >= 'A' && chr <= 'Z')) {
        return true;
    }
    return false;
}

bool equals_letter(const char l1, const char l2) {
    if (l1 == l2 || l1 - 32 == l2 || l1 + 32 == l2) {
        return true;
    }
    return false;
}

char to_uppercase(char c) {
    if (c > 90) {
        return c - 32;
    }
    return c;
}

int get_mistakes() { return mistakes; }

bool get_has_won() { return has_won; }
