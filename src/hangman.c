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

void pick_guess_word(const language_t language) {
    const char *filename = NULL;
    switch (language) {
        case ENGLISH:
            filename = "wordlist-en";
            break;
        case GERMAN:
            filename = "wordlist-de";
            break;
    }

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("error opening wordlist");
        exit(EXIT_FAILURE);
    }

    fseek(file, 0, SEEK_END);
    const long file_size = ftell(file);
    rewind(file);

    char *word_list = malloc(file_size);
    if (word_list == NULL) {
        perror("memory allocation failed");
        exit(EXIT_FAILURE);
    }

    fread(word_list, sizeof(char), file_size, file);

    int word_count = 0;
    for (int i = 0; i < file_size; ++i) {
        if (word_list[i] == '\n') {
            ++word_count;
        }
    }

    const int random_index = rand() % word_count;
    int current_word = 0;
    int current_char = 0;
    for (int i = 0; i < file_size; ++i) {
        if (current_word == random_index) {
            guess_word[current_char] = word_list[i];
            ++current_char;
        }

        if (word_list[i] == '\n') {
            ++current_word;
        }

        if (current_word > random_index) {
            break;
        }
    }

    guess_word[current_char] = '\0';
    free(word_list);
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
