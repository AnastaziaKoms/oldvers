#include <iostream>
#include <ctime>
#include <ncurses.h>
#include <vector>

using namespace std;

struct Point {
    size_t x;
    size_t y;

    Point(size_t x, size_t y) : x(x), y(y) {}

    Point() {}
};

class Map {
public:
    Point map_size;
    vector<vector<char>> matrix;
    int count;

    Map(size_t a, size_t b) {
        map_size.x = a;
        map_size.y = b;
        count = a * 2 + 1;
        for (int i = 0; i < map_size.x; ++i) {
            matrix.push_back(vector<char>(b, ' '));
        }
    }

    char &operator()(Point coord) {
        return matrix[coord.y][coord.x];
    }

    char operator()(Point coord) const {
        return matrix[coord.y][coord.x];
    }

};


class GameManager {
public:
    Map map;
    bool flag;
    char first = 'X';
    char second = '0';
    int size;
    int win_block;

    char currentGamer;


    Point cordpoint;

    GameManager(int n, int w, Point start_coord) : win_block(w), map(n, n), flag(true), currentGamer(first),
                                                   cordpoint(start_coord), size(n) {
    }

    void play() {
        while (true) {
            HandleInput(getch());
            WindowRender();
        }
    }

    void HandleInput(chtype c) {
        switch (c) {
            case KEY_UP:
                if (cordpoint.y != 0)
                    cordpoint.y -= 1;
                break;
            case KEY_DOWN:
                if (cordpoint.y != size - 1)
                    cordpoint.y += 1;
                break;
            case KEY_LEFT:
                if (cordpoint.x != 0)
                    cordpoint.x -= 1;
                break;
            case KEY_RIGHT:
                if (cordpoint.x != size - 1)
                    cordpoint.x += 1;
                break;
            case 32:
                TakeMove(cordpoint);
                break;
        }


    }

    void TakeMove(Point cord) {
        if (map.matrix[cord.y][cord.x] == ' ') {
            map.matrix[cord.y][cord.x] = currentGamer;
            if (Checkwin(currentGamer)) {
                clear();
                mvprintw(getmaxy(stdscr) / 2, (getmaxx(stdscr) - 10) / 2, "%c WINER!!!", currentGamer);
                getch();
                exit(0);
            }
            if (isMapFull()) {
                clear();
                mvprintw(getmaxy(stdscr) / 2, (getmaxx(stdscr) - 9) / 2, "LOSERS !.");
                getch();
                exit(0);
            }
            SwapGamers();
        }

    }

    void SwapGamers() {
        if (currentGamer == first) currentGamer = second;
        else currentGamer = first;
    }

    bool Checkwin(char sym) {
        for (size_t col = 0; col < size - win_block + 1; col++) {
            for (size_t row = 0; row < size - win_block + 1; row++) {
                if (checkDiagonal(sym, col, row) || checkLanes(sym, col, row)) return true;
            }

        }
        return false;
    }

    bool checkDiagonal(char symb, int offsetX, int offsetY) {
        bool toright, toleft;
        toright = true;
        toleft = true;
        for (int i = 0; i < win_block; i++) {
            toright &= (map.matrix[i + offsetX][i + offsetY] == symb);
            toleft &= (map.matrix[win_block - i - 1 + offsetX][i + offsetY] == symb);
        }

        if (toright || toleft) return true;

        return false;
    }

    bool checkLanes(char symb, int offsetX, int offsetY) {
        bool cols, rows;
        for (int col = offsetX; col < win_block + offsetX; col++) {
            cols = true;
            rows = true;
            for (int row = offsetY; row < win_block + offsetY; row++) {
                cols &= (map.matrix[col][row] == symb);
                rows &= (map.matrix[row][col] == symb);
            }

            if (cols || rows) return true;
        }

        return false;
    }

    bool isMapFull() {
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                if (map.matrix[i][j] == ' ') return false;
            }
        }
        return true;
    }


    void WindowRender() {

        clear();
        int coeff = (getmaxx(stdscr) - map.count) / 2;
        size_t x, y;
        chtype c;
        for (y = 0; y < map.map_size.y * 2; y++) {

            if (y % 2 == 0) {
                for (x = 0; x < map.count; x++) {
                    mvprintw(y + 4, coeff + x, "%c", '-');
                }
            } else {
                for (x = 0; x < map.map_size.x * 2; x += 2) {
                    mvprintw(y + 4, coeff + x, "%c", '|');
                    c = map.matrix[(y / 2)][x / 2];
                    if (c == '0') {
                        move(y + 4, coeff + x + 1);
                        addch(c | COLOR_PAIR(2));
                        continue;
                    }
                    mvprintw(y + 4, coeff + x + 1, "%c", map.matrix[(y / 2)][x / 2]);

                }
                mvprintw(y + 4, coeff + x, "%c", '|');
            }

        }
        for (x = 0; x < map.count; x++) {
            mvprintw(y + 4, coeff + x, "%c", '-');
        }
        mvprintw(cordpoint.y * 2 + 5, cordpoint.x * 2 + coeff + 1, "%c", '*');
    }
};

void Menu() {
    const char items[3][8] = {
            "Play   ",
            "Credits",
            "Exit   "
    };
    int p = 0;
    while (true) {
        clear();
        box(stdscr, 0, 0);

        for (int i = 0; i < 3; i++) {
            move((getmaxy(stdscr)) / 2 + i, getmaxx(stdscr) / 2 - 5);
            if (i == p)
                addch('-');
            else
                addch(' ');

            printw("%s\n", items[i]);
        }

        switch (getch()) {
            case KEY_UP:
                if (p)
                    p--;
                break;
            case KEY_DOWN:
                if (p != 2)
                    p++;
                break;
            case 10:
                if (p == 0) return;
                if (p == 1) {
                    clear();
                    mvprintw(getmaxy(stdscr) / 2, (getmaxx(stdscr) - 30) / 2, "Nastya aka TurboCoder was here.");
                    getch();

                }
                if (p == 2) {
                    clear();
                    exit(0);
                }
                break;
        }
    }
}

int main() {
    int n;
    int w;
    initscr();
    cbreak();
    keypad(stdscr, TRUE);
    curs_set(0);
    clear();
    start_color();
    init_pair(1, COLOR_BLACK, COLOR_YELLOW);
    init_pair(2, COLOR_RED, COLOR_YELLOW);

    attron(COLOR_PAIR(1));
    bkgd(COLOR_PAIR(1));


    Menu();

    while (true) {
        clear();
        mvprintw(getmaxy(stdscr) / 2, getmaxx(stdscr) / 2 - 9, "Input field:");
        scanw("%d", &n);
        if (n >= 3 && n <= 10) break;
    }

    while (true) {
        clear();
        mvprintw(getmaxy(stdscr) / 2, getmaxx(stdscr) / 2 - 9, "Input win number:");
        scanw("%d", &w);
        if (w >= 3 && w <= n) break;
    }
    GameManager Game(n, w, Point(n / 2, n / 2));
    Game.WindowRender();
    Game.play();
    return 0;
}