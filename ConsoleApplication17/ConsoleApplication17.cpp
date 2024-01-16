#include "Player.h"
#include "World.h"
#include <iostream>
#include <windows.h>
#include <chrono>
#include <thread>
#include <vector>
#include <conio.h>
#include <mutex>
#include <future>
#define NULLNUM 48

using namespace std;
using namespace chrono;

mutex inputMutex;

//mutex inputMutex1;


#pragma region SpecialAbilities

void Gotoxy(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

BOOL ShowConsoleCursor(BOOL bShow)
{
    CONSOLE_CURSOR_INFO cci;
    HANDLE hStdOut;
    hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hStdOut == INVALID_HANDLE_VALUE)
        return FALSE;
    if (!GetConsoleCursorInfo(hStdOut, &cci))
        return FALSE;
    cci.bVisible = bShow;
    if (!SetConsoleCursorInfo(hStdOut, &cci))
        return FALSE;
    return TRUE;
}

void ClearKeyboardBuffer()
{
    while (_kbhit())
    {
        _getch();
    }
}

#pragma endregion

int AmountNonLivingPlayer(const vector<Player>& players, const World& world)
{
    int amountPlayer{ 0 };
    for(size_t i{ 0 }; i < world.GetAmountPlayer(); i++)
    {
        if (players[i].GetAlive() == false)amountPlayer++;
    }

    return amountPlayer;
}

void PressButtons(vector<Player>& players, World& world, bool& exitPress)
{
    if (AmountNonLivingPlayer(players, world) == world.GetAmountPlayer())exitPress = true;
    else 
    {   
        if (_kbhit()) 
        {
            if (players[0].alive && players[0].stunTime <= 0 ||
                world.reverseControl > 0 && players[1].alive && players[1].stunTime <= 0)
            {
                if (world.reverseControl <= 0
                    || world.amountPlayer == 1 && world.reverseControl > 0)
                {
                    players[0].PressButtonPlayer(world);
                }
                else if (world.amountPlayer > 1 && world.reverseControl > 0)
                {
                    players[1].PressButtonPlayer(world);
                }

            }
        }

        if (world.amountPlayer > 1) 
        {
            if (_kbhit()) {
                if (players[1].alive && players[1].stunTime <= 0 ||
                    world.reverseControl > 0 && players[0].alive && players[0].stunTime <= 0)
                {
                    if (world.reverseControl <= 0)
                    {
                        players[1].PressButtonPlayer(world);
                    }
                    else if (world.reverseControl > 0)
                    {
                        players[0].PressButtonPlayer(world);
                    }
                }
            }
        }
    }
}

int SumScore(const vector<Player>& players)
{
    size_t size{ players.size() };
    int sum{ 0 };

    for(size_t i{0}; i < size; i++)
    {
        sum += players[i].GetScore();
    }

    return sum;
}

void RepeatedPlayerCoordinates(vector<Player>& players, const World& world, bool& exitPress)
{
    for (int i{ 0 }; i < world.amountPlayer; i++)
    {
        int tryAddPlayer{ 3 };//Переменная которая дает три попытки на постановку игрока на карту(3 попытки иначе игра закрываеться)
        players.push_back(Player{ char(NULLNUM + i), world.heightWorld, world.lenghtWorld });

        for (size_t l{ 0 }, indexLast{ players.size() - 1 }; l < i; l++)
        {
            if (tryAddPlayer == 0)break;
            else if (players[indexLast].GetHeight() == players[l].GetHeight() &&
                     players[indexLast].GetLength() == players[l].GetLength())
            {
                players[indexLast].ChangeLocation(world.heightWorld, world.lenghtWorld);
                l = -1;
                tryAddPlayer--;
            }
        }

        if (tryAddPlayer == 0)
        {
            exitPress = true;
            break;
        }
    }
}

void CinAmountPlayer(int& amountPlayer)
{
    system("cls");
    cout << "\tPointRunner\nEnter amount player: "; cin >> amountPlayer;
    amountPlayer = amountPlayer < 0 ? amountPlayer * -1 : amountPlayer == 0 ? 1 : amountPlayer;

    system("cls");

    if (amountPlayer > 2)
    {
        throw exception ("Sorry, but in this game don't add three and more players!");
    }
}

void InputResult(const World& world, const vector<Player>& players)
{
    if (world.GetAmountPlayer() == 1 && players[0].GetAlive())
    {
        cout << "You collected all points!" << endl;
    }
    if (world.GetAmountPlayer() == 1 && players[0].GetAlive() == false)
    {
        cout << "You Died! And you collected " << players[0].GetScore() << " points!" << endl;
    }
    else if (world.GetAmountPlayer() > 1)
    {
        if (players[0].GetScore() > players[1].GetScore())
        {
            cout << "First player collected more points!" << endl;
        }
        else if (players[0].GetScore() < players[1].GetScore())
        {
            cout << "Second player collected more points!" << endl;
        }
        else cout << "DRAW!" << endl;
    }
}


int main()
{

    srand(time(0));
    ShowConsoleCursor(TRUE);

    try {

        #pragma region Variables

        bool exitPress{ false };
        int amountPlayer{ 1 };
        CinAmountPlayer(amountPlayer);

        //Мир(заполнение) и игрок(розмещение)
        World world{ '.', 'B', 10, 10, amountPlayer};
        vector<Player> players;
        RepeatedPlayerCoordinates(players, world, exitPress);
        world.FillWorld(players);

        //Время
        using Clock = high_resolution_clock;
        auto start_time = Clock::now();

        #pragma endregion

        while (exitPress != true)
        {

            Gotoxy(0, 0);
            PressButtons(players, world, exitPress);

            if (SumScore(players) == world.GetAmountPoint())exitPress = true;

            auto end_time = Clock::now();
            auto elapsed_time = duration_cast<seconds>(end_time - start_time);

            world.PrintWorld(players, elapsed_time, exitPress);

        }
        InputResult(world, players);
    }
    catch (exception error)
    {
       cout << "Error: " << error.what() << endl;
    }
    
    system("pause");
    return 0;
}