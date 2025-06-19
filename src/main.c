#include <game.h>

#include <stdlib.h>
#include <time.h>

int main() {
    srand(time(NULL));
    game_loop();
    return 0;
}
