#include <iostream>
#include <chrono>
#include <vector>
#include <conio.h>

using namespace std;
using namespace chrono;

class Player;

class World
{
private:

    const int heightWorld;
    const int lenghtWorld;
    int generalStun;
    int reverseControl;
    int doublePoint;
    int changeSymbolPlayer;
    int amountPoint;
    const int amountPlayer;
    const char symbolPoint;
    const char symbolBomb;
    bool workEvents;
    char** world;

    class Bomb
    {

    private:

        int line;
        int column;
        int timeBoom;

    public:

        Bomb(const int lineP, const int columnP)
            :line{ lineP }, column{ columnP }, timeBoom{ 10 } {}


        Bomb& operator=(const Bomb& bomb)//Для работы erase
        { 
            this->line = bomb.line;
            this->column = bomb.column;
            this->timeBoom = bomb.timeBoom;
            return *this; 
        }


        inline int GetLine() const { return line; }

        inline int GetColumn() const { return column; }

        inline int GetTimeBoom() const { return timeBoom; }

        inline int& GetTimeBoom() { return timeBoom; }


    };

public:

    vector<Bomb> bombs;

    World(const char symbolPointP, const char symbolBombP, const int height, const int lenght, const int amountPlayerP)
        :heightWorld{ height < 0 ? height * -1 : height == 0 ? 1 : height },
        lenghtWorld{ lenght < 0 ? lenght * -1 : lenght == 0 ? 1 : lenght },
        amountPoint{ 0 }, symbolPoint{ symbolPointP != ' ' ? symbolPointP : '.' },
        symbolBomb{ symbolBombP != ' ' ? symbolBombP : 'B' }, generalStun{ 0 },
        amountPlayer{ amountPlayerP < 0 ? amountPlayerP * -1 : amountPlayerP == 0 ? 1 : amountPlayerP },
        reverseControl { 0 }, doublePoint { 0 }, changeSymbolPlayer { 0 }, workEvents{ false }
    {
        world = new char* [heightWorld];

        for (size_t i{ 0 }; i < heightWorld; i++)
        {
            world[i] = new char[lenghtWorld];
        }
    }

    World() : World{ '.', 'B', 4, 4, 1} {}

    ~World()
    {
        for(size_t i{0}; i < lenghtWorld; i++)
        {
            delete[] world[i];
        }
        delete[] world;
    }

    void FillWorld(const vector<Player>& players);

    void PrintWorld(vector<Player>& players, const std::chrono::seconds& duration, bool& exitPress);

    int AmountNonLivingPlayer(const vector<Player>& players, bool& exitPress);

    void BoomBombs(vector<Player>& players, bool& exitPress);

    string PrintEvent();

    void AdditionalPrint(vector<Player>& players, const std::chrono::seconds& duration);

    void RandomEvent(vector<Player>& players);

    void PassageTimeEvent(vector<Player>& players);

    void CheckEndEvent();


    inline int GetAmountPoint() const { return amountPoint; }

    inline int GetAmountPlayer() const { return amountPlayer; }


    friend void RepeatedPlayerCoordinates(vector<Player>& players, const World& world, bool& exitPress);

    friend void Player::FillCells(World& worldP);

    friend void Player::PressButtonPlayer(World& worldP);

    friend void PressButtons(vector<Player>& players, World& world, bool& exitPress);
};