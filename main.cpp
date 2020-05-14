#include <iostream>
#include <ctime>
#include <ncurses.h>
#include <string>

using namespace std;

struct Point
{
    size_t x;
    size_t y;
    Point(size_t x, size_t y) : x(x), y(y) {}
    Point() {}
};

class Map
{
public:
    Point map_size;
    char matrix[100][100];
    int count;

    Map(size_t a, size_t b)
    {
        map_size.x = a;
        map_size.y = b;
        count = a * 2 + 1;
        for (int i = 0; i < map_size.x; ++i)
        {
            for (int j = 0; j < map_size.y; ++j)
            {
                matrix[i][j] = ' ';
            }
        }
    }

    char &operator()(Point coord)
    {
        return matrix[coord.y][coord.x];
    }
    char operator()(Point coord) const
    {
        return matrix[coord.y][coord.x];
    }
};

class Gamer
{
private:
    char sym;
public:
    char GetChar() {
        return sym;
    }
    explicit Gamer(char s): sym(s) {}
};

class GameManager
{
public:
    Map map;
    bool flag;
    Gamer first;
    Gamer second;
    int size;
    int win_block;

    Gamer* currentGamer;

    GameManager(int n, int w, Point start_coord) : map(n, n), flag(true), first('X'), second('0'), currentGamer(&first), size(n), win_block(w)
    {
    }

    void play()
    {
        while (true)
        {
            HandleInput();
            WindowRender();
        }
    }

    void HandleInput()
    {
        char s[100];
        getstr(s);
        string str(s);
        if(str.length() != 2 || (int)str[0] < 97 || (int)str[1] < 49) {
            return;
        }
        Point p(str[1] - '0' - 1,(int)str[0] - 97);

        if(map(p) != ' ') return;
        map(p) = currentGamer->GetChar();

        if(Checkwin(currentGamer->GetChar()) || isMapFull()) exit(0);
        SwapGamers();
    }

    bool Checkwin(char sym) {
        for (size_t col = 0; col < size-win_block+1; col++)
        {
            for (size_t row = 0; row < size-win_block+1; row++)
            {
                if (checkDiagonal(sym, col, row) || checkLanes(sym, col, row)) return true;
            }

        }
        return false;
    }

    bool checkDiagonal(char symb, int offsetX, int offsetY) {
        bool toright, toleft;
        toright = true;
        toleft = true;
        for (int i=0; i<win_block; i++) {
            toright &= (map.matrix[i+offsetX][i+offsetY] == symb);
            toleft &= (map.matrix[win_block-i-1+offsetX][i+offsetY] == symb);
        }

        if (toright || toleft) return true;

        return false;
    }

    /** Проверяем горизонтальные и вертикальные линии */
    bool checkLanes(char symb, int offsetX, int offsetY) {
        bool cols, rows;
        for (int col=offsetX; col<win_block+offsetX; col++) {
            cols = true;
            rows = true;
            for (int row=offsetY; row<win_block+offsetY; row++) {
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

    void SwapGamers() {
        if(currentGamer == &first)
            currentGamer = &second;
        else if (currentGamer == &second)
            currentGamer = &first;
    }

    void WindowRender()
    {
        clear();
        size_t x, y;
        for (y = 0; y < map.map_size.y * 2; y++)
        {

            if (y % 2 == 0)
            {
                for (x = 0; x < map.count; x++)
                {
                    mvprintw(y, x, "%c", '-');
                }
            }
            else
            {
                for (x = 0; x < map.map_size.x * 2; x += 2)
                {
                    mvprintw(y, x, "%c", '|');
                    mvprintw(y, x+1, "%c", map(Point(x/2, y/2)));
                }
                mvprintw(y, x, "%c", '|');
            }
        }
        for (x = 0; x < map.count; x++)
        {
            mvprintw(y, x, "%c", '-');
        }
        mvprintw(map.count,0,"Input coordinates:");
    }
};

int main()
{
    int n;
    int w;
    initscr();
    //noecho();
    cbreak();
    keypad(stdscr, TRUE);
    while (true)
    {
        clear();
        addstr("Input field:");
        scanw("%d", &n);
        if (n >= 3 && n <= 10) break;
    }

    while (true)
    {
        clear();
        addstr("Input win number:");
        scanw("%d", &w);
        if(w >= 3 && w <= n) break;
    }

    curs_set(0);
    //
    GameManager Game(n, w, Point(n / 2, n / 2));
    Game.WindowRender();
    Game.play();
    return 0;
}