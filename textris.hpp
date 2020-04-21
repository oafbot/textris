#ifndef __TEXTRIS_H__
#define __TEXTRIS_H__

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <wchar.h>
#include <ncurses.h>
#include <curses.h>
#include <unistd.h>

#include <string>
#include <vector>
#include <array>

using namespace std;

#define DELAY 50000
#define COLS  20
#define ROWS  20
#define UP    0
#define RIGHT 1
#define DOWN  2
#define LEFT  3

class Game;
class Block;
class Controller;

class Controller{
    public:
        int charat;
        Controller();
        void input();
};

class Game{
    public:
        bool PAUSED;
        bool WAIT;
        bool RESET;
        bool COMPLETE;
        bool GAME_OVER;
        bool START;

        int level;
        int delay;
        int key;
        int max_y;
        int max_x;
        int next_y;
        int direction;
        int counter;
        int count;
        int lines;
        int score;
        bool cleared;

        array<int, 200> field;
        vector<array<int, 10>> rows;
        vector<int> completed;

        Controller controls;

        Game();
        void pause();
        void loop();
        void update();
        void render();
        void clearRows();
        void updateScore();
        void reset();
};

class Block{
    public:
        int col;
        int row;
        int type;
        int state;
        bool active;
        int index;
        int countdown;
        bool locked;
        bool lockdown;

        array<array<array<bool, 16>,4>,7> config;

        Block();
        void generate();
        void render();
        void rotate();
        void lock();
        // void lockdown();
        void changeState();
        bool traverse(int direction);
        int  convert(int i);
};

extern Game game;
extern Block block;
extern int x, y;
#endif