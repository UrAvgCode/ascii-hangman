#include <hangman.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <ncurses.h>

#define LENGTH 20

typedef struct {
    const char *word_list;
    long file_size;
    int word_count;
} word_list_t;

static word_list_t word_lists[2];

static const char *guess_word;
static char hint_word[LENGTH];

static char used_letters[6];

int mistakes = 0;
bool has_won = false;

void load_word_lists() {
    for (int i = 0; i < 2; ++i) {
        const char *filenames[] = {"wordlist-en", "wordlist-de"};
        FILE *file = fopen(filenames[i], "r");
        if (file == NULL) {
            perror("error opening wordlist");
            exit(EXIT_FAILURE);
        }

        fseek(file, 0, SEEK_END);
        const long file_size = ftell(file);
        rewind(file);

        char *word_list = malloc(file_size + 1);
        if (word_list == NULL) {
            perror("memory allocation failed");
            exit(EXIT_FAILURE);
        }

        fread(word_list, sizeof(char), file_size, file);
        word_list[file_size] = '\0';

        int word_count = 0;
        for (int j = 0; j < file_size; ++j) {
            if (word_list[j] == '\n') {
                word_list[j] = '\0';

                if (j != file_size - 1) {
                    ++word_count;
                }
            }
        }

        word_lists[i].word_list = word_list;
        word_lists[i].file_size = file_size;
        word_lists[i].word_count = word_count;
    }
}

void pick_guess_word(const language_t language) {
    const char *word_list = word_lists[language].word_list;
    const long file_size = word_lists[language].file_size;
    const int word_count = word_lists[language].word_count;

    const int random_index = rand() % word_count;

    int current_index = 0;
    for (int i = 0; i < file_size; ++i) {
        if (current_index == random_index) {
            guess_word = word_list + i;
            break;
        }

        if (word_list[i] == '\0') {
            ++current_index;
        }
    }
}

void create_hint_word() {
    for (int i = 0; i < LENGTH; ++i) {
        if (is_letter(guess_word[i])) {
            hint_word[i] = '-';
        } else {
            hint_word[i] = guess_word[i];
        }

        if (guess_word[i] == '\0') {
            break;
        }
    }
}

void update_hint_word(const char chr) {
    if (!is_letter(chr)) {
        return;
    }

    for (int i = 0; i <= mistakes; ++i) {
        if (equals_letter(used_letters[i], chr)) {
            return;
        }
    }

    bool contains = false;
    for (int i = 0; i < LENGTH; ++i) {
        if (guess_word[i] == '\0') {
            break;
        }

        if (equals_letter(guess_word[i], chr)) {
            hint_word[i] = guess_word[i];
            contains = true;
        }
    }

    if (!contains) {
        used_letters[mistakes++] = to_uppercase(chr);
    }
}

void draw_hint_word(const int x, const int y) { mvprintw(y, x, "HintWord: %s", hint_word); }

void draw_hint_letters(const int x, const int y) {
    mvprintw(y, x, "Wrong Letters:");

    for (int i = 0; i < mistakes; ++i) {
        mvprintw(y + 1, x + 2 * i, "%c", used_letters[i]);
        if (i != mistakes - 1) {
            mvprintw(y + 1, x + 1 + 2 * i, ",");
        }
    }
}

void draw_guess_word(const int x, const int y) { mvprintw(y, x, "GuessWord: %s", guess_word); }

bool is_game_over() {
    if (mistakes >= 6) {
        return true;
    }

    if (strcmp(guess_word, hint_word) == 0) {
        has_won = true;
        return true;
    }

    return false;
}

void reset() {
    mistakes = 0;
    has_won = false;

    for (int i = 0; i < 6; i++) {
        used_letters[i] = '\0';
    }
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

char to_uppercase(const char c) {
    if (c >= 'a' && c <= 'z') {
        return (char) (c - 32);
    }
    return c;
}

int get_mistakes() { return mistakes; }

bool get_has_won() { return has_won; }
