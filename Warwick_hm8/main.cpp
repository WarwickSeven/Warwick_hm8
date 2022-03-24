#include <iostream>
#include <random>
#include <stdlib.h>
#include <cstdlib>
#include <ctime>

using namespace std;
    
enum GameDifficulty { DUMB, NORMAL, NIGHTMARE };
enum GameProgress { IN_PROGRESS, USER_WON, AI_WON, DRAW };
enum FieldCell : char { CROSS = 'X', ZERO = 'O', EMPTY = '_' };

#pragma pack(push, 1)
struct tttGame {
    FieldCell **ppField = nullptr;
    const size_t SIZE = 3;
    FieldCell user = EMPTY;
    FieldCell ai = EMPTY;
    GameProgress progress = IN_PROGRESS;
    GameDifficulty difficulty;
    size_t turn = 0U;
};
#pragma pack (pop)

inline void clearScreen()           // не работает в Xcode
{
    cout << "\x1B[2J\x1B[H";
}
struct TCoord {
    size_t y = 0U;
    size_t x = 0U;
};

int getRandomNum()
{
    srand(static_cast<unsigned int>(time(0)));
    return rand();
}

//======================================================================================================

void initGame(tttGame &g)
{
    g.ppField = new FieldCell * [g.SIZE];
    for (size_t i = 0; i < g.SIZE; i++)
    {
        g.ppField[i] = new FieldCell [g.SIZE];
    }
    
    for (size_t y = 0; y < g.SIZE; y++)
    {
        for (size_t x = 0; x < g.SIZE; x++)
        {
            g.ppField[y][x] = EMPTY;
        }
    }
    if (getRandomNum()%10 > 5)
    {
        g.user = CROSS;
        g.ai = ZERO;
        g.turn = 0U;
    }
    else
    {
        g.user = ZERO;
        g.ai = CROSS;
        g.turn = 1U;
    }
    cout << "Welcome to the tic-tac-toe game!" << endl << endl << "Please select difficulty:" << endl;
    cout << "1 - Dumb" << endl;
    cout << "2 - Normal" << endl;
    cout << "3 - Nightmare" << endl << endl;
    cout << "Press 1, 2 or 3: ";
    int diffChoise;
    cin >> diffChoise;
    switch (diffChoise)
    {
        case 1: g.difficulty = DUMB; break;
        case 2: g.difficulty = NORMAL; break;
        case 3: g.difficulty = NIGHTMARE; break;
    }
}

void printGame(const tttGame &g)
{
    switch (g.difficulty)
    {
        case DUMB: cout << "Your opponent is a Dumb!" << endl << endl; break;
        case NORMAL: cout << "Your opponent is Normal." << endl << endl; break;
        case NIGHTMARE: cout << "Your opponent is a Nightmare!" << endl << endl; break;
    }
    cout << "    ";
    for (size_t x = 0; x < g.SIZE; x++)
    {
        cout << x + 1 << "   ";
    }
    cout << endl;
        
    for (size_t y = 0; y < g.SIZE; y++)
    {
        cout << y+1 << " | ";
        for (size_t x = 0; x < g.SIZE; x++)
        {
            cout << g.ppField[y][x] << " | ";
        }
        cout << endl;
    }
    cout << endl << " User: " << g.user << endl << " Computer: " << g.ai << endl << endl;
}

void deinitGame(tttGame &g)
{
    for (size_t i = 0; i < g.SIZE; i++)
    {
        delete[] g.ppField[i];
    }
    delete[] g.ppField;
    g.ppField = nullptr;
}

void results(const tttGame &g)
{
    if (g.progress == USER_WON)
        cout << "You won! :D" << endl;
    if (g.progress == AI_WON)
        cout << "Computer won! :(" << endl;
    if (g.progress == DRAW)
        cout << "It's a draw." << endl;
}

GameProgress getWon(const tttGame &g)
{
    for (size_t y = 0; y < g.SIZE; y++)
    {
        if ((g.ppField[y][0] == g.ppField[y][1]) && (g.ppField[y][0] == g.ppField[y][2]))
        {
            if (g.ppField[y][0] == g.user)
                return USER_WON;
            if (g.ppField[y][0] == g.ai)
                return AI_WON;
        }
    }
    for (size_t x = 0; x < g.SIZE; x++)
    {
        if ((g.ppField[0][x] == g.ppField[1][x]) && (g.ppField[0][x] == g.ppField[2][x]))
        {
            if (g.ppField[0][x] == g.user)
                return USER_WON;
            if (g.ppField[0][x] == g.ai)
                return AI_WON;
        }
    }
    if (g.ppField[0][0] == g.ppField[1][1] && g.ppField[0][0] == g.ppField[2][2])
    {
        if (g.ppField[0][0] == g.user)
            return USER_WON;
        if (g.ppField[0][0] == g.ai)
            return AI_WON;
    }
    
    if (g.ppField[0][2] == g.ppField[1][1] && g.ppField[0][2] == g.ppField[2][0])
    {
        if (g.ppField[0][2] == g.user)
            return USER_WON;
        if (g.ppField[0][2] == g.ai)
            return AI_WON;
    }
        
    bool draw = true;
    for (size_t y = 0; y < g.SIZE; y++)
    {
        for (size_t x = 0; x < g.SIZE; x++)
        {
            if (g.ppField[y][x] == EMPTY)
            {
                draw = false;
                break;
            }
        }
        if (!draw)
            break;
    }
    if (draw)
    {
        return DRAW;
    }
    return IN_PROGRESS;
}

void userTurn(const tttGame &g)
{
    size_t x = 0U, y = 0U;
    do {
        cout << " Enter x: ";
        cin >> x;
        cout << " Enter y: ";
        cin >> y;
        x--;
        y--;
    } while (x > 2 || y > 2 || g.ppField[y][x] != EMPTY);
    g.ppField[y][x] = g.user;
}

TCoord aiDumbTurn(tttGame &g)
{
    TCoord buf[9];
    size_t num = 0;
    for (size_t y = 0; y < g.SIZE; y++)
    {
        for (size_t x = 0; x < g.SIZE; x++)
        {
            if (g.ppField[y][x] == EMPTY)
            {
                buf[num] = { y, x };
                num++;
            }
        }
    }
    const size_t index = getRandomNum() % num;
    return buf[index];
}

TCoord aiNormalTurn(tttGame &g)
{
    //центр
    if (g.ppField[1][1] == EMPTY)
    {
        return { 1, 1 };
    }
    
    //углы
    TCoord buf[4];
    size_t num = 0;
    if (g.ppField[0][0] == EMPTY)
    {
        buf[num] = { 0, 0 };
        num++;
    }
    if (g.ppField[0][2] == EMPTY)
    {
        buf[num] = { 0, 2 };
        num++;
    }
    if (g.ppField[2][0] == EMPTY)
    {
        buf[num] = { 2, 0 };
        num++;
    }
    if (g.ppField[2][2] == EMPTY)
    {
        buf[num] = { 2, 2 };
        num++;
    }
    if (num > 0)
    {
        const size_t index = getRandomNum() % num;
        return buf[index];
    }
    
    //не углы
    if (g.ppField[0][1] == EMPTY)
    {
        buf[num] = { 0, 1 };
        num++;
    }
    if (g.ppField[1][0] == EMPTY)
    {
        buf[num] = { 1, 0 };
        num++;
    }
    if (g.ppField[1][2] == EMPTY)
    {
        buf[num] = { 1, 2 };
        num++;
    }
    if (g.ppField[2][1] == EMPTY)
    {
        buf[num] = { 2, 1 };
        num++;
    }
    if (num > 0)
    {
        const size_t index = getRandomNum() % num;
        return buf[index];
    }
    return { 5, 5 };
}

TCoord aiNightmareTurn(tttGame &g)
{
    for (size_t y = 0; y < g.SIZE; y++)
    {
        for (size_t x = 0; x < g.SIZE; x++)
        {
            if (g.ppField[y][x] == EMPTY)
            {
                g.ppField[y][x] = g.ai;
                if (getWon(g) == AI_WON)
                {
                    g.ppField[y][x] = EMPTY;
                    return { y, x };
                }
                g.ppField[y][x] = EMPTY;
            }
        }
    }
    
    for (size_t y = 0; y < g.SIZE; y++)
    {
        for (size_t x = 0; x < g.SIZE; x++)
        {
            if (g.ppField[y][x] == EMPTY)
            {
                g.ppField[y][x] = g.user;
                if (getWon(g) == USER_WON)
                {
                    g.ppField[y][x] = EMPTY;
                    return { y, x };
                }
                g.ppField[y][x] = EMPTY;
            }
        }
    }
    
    //центр
    
    if (g.ppField[1][1] == EMPTY)
    {
        return { 1, 1 };
    }
    
    //углы
    
    TCoord buf[4];
    size_t num = 0;
    if (g.ppField[0][0] == EMPTY)
    {
        buf[num] = { 0, 0 };
        num++;
    }
    if (g.ppField[0][2] == EMPTY)
    {
        buf[num] = { 0, 2 };
        num++;
    }
    if (g.ppField[2][0] == EMPTY)
    {
        buf[num] = { 2, 0 };
        num++;
    }
    if (g.ppField[2][2] == EMPTY)
    {
        buf[num] = { 2, 2 };
        num++;
    }
    if (num > 0)
    {
        const size_t index = getRandomNum() % num;
        return buf[index];
    }
    
    //не углы
    
    if (g.ppField[0][1] == EMPTY)
    {
        buf[num] = { 0, 1 };
        num++;
    }
    if (g.ppField[1][0] == EMPTY)
    {
        buf[num] = { 1, 0 };
        num++;
    }
    if (g.ppField[1][2] == EMPTY)
    {
        buf[num] = { 1, 2 };
        num++;
    }
    if (g.ppField[2][1] == EMPTY)
    {
        buf[num] = { 2, 1 };
        num++;
    }
    if (num > 0)
    {
        const size_t index = getRandomNum() % num;
        return buf[index];
    }
    return { 5, 5 };
}

//======================================================================================================

int main()
{
    tttGame g;
    initGame(g);
    clearScreen();
    printGame(g);
    do {
        if (g.turn % 2 == 0)
        {
            userTurn(g);
        }
        else
        {
            switch (g.difficulty)
            {
                case DUMB:
                {
                    TCoord c = aiDumbTurn(g);
                    g.ppField[c.y][c.x] = g.ai;
                    break;
                }
                case NORMAL:
                {
                    TCoord c = aiNormalTurn(g);
                    g.ppField[c.y][c.x] = g.ai;
                    break;
                }
                case NIGHTMARE:
                {
                    TCoord c = aiNightmareTurn(g);
                    g.ppField[c.y][c.x] = g.ai;
                    break;
                }
            }
        }
        clearScreen();
        printGame(g);
        g.progress = getWon(g);
        g.turn++;
    } while (g.progress == IN_PROGRESS);
    
    results(g);
    deinitGame(g);
    return 0;
}
