#include "textris.hpp"
#include "game.cpp"
#include "controller.cpp"
#include "block.cpp"

Game game;
Block block;
int x, y;

int main(int argc, char *argv[]) {
    initscr();
    keypad(stdscr, TRUE); // Global var `stdscr` is created by the call to `initscr()`
    cbreak();
    noecho();
    timeout(0);
    curs_set(0);

    game = * new Game();
    block = * new Block();
    x = 8,
    y = 0;

    game.loop();

    endwin();
    exit(0);
}