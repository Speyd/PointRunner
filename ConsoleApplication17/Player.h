#include <iostream>
#include <vector>
using namespace std;

class World;

class Player
{
private:

    char symbolPlayer;
    bool alive;
    int heightCordinate;
    int lengthCordinate;
    int doubleStep;
    int stunTime;
    int score;

public:

    Player(const char symbol, const int height, const int length)
        : symbolPlayer{ symbol }, heightCordinate{ rand() % ((height - 1) - 0 + 1) + 0 }, stunTime{ 0 },
        lengthCordinate{ rand() % ((length - 1) - 0 + 1) + 0 }, doubleStep{ 0 }, score{ 0 }, alive{ true } {}

    Player(Player&& player) :
        symbolPlayer{ player.symbolPlayer }, heightCordinate{ player.heightCordinate }, doubleStep{ player.doubleStep },
        lengthCordinate{ player.lengthCordinate }, alive{ player.alive }, score{ player.score }, stunTime{ player.stunTime } {}

    Player() : Player{ '0', 0, 0 } {}


    friend class World;


    void FillCells(World& worldP);

    void PressButtonPlayer(World& worldP);

    void ChangeLocation(const int height, const int length);


    inline int GetScore() const { return score; }

    inline int GetStunTime() const { return stunTime; }

    inline int GetHeight() const { return heightCordinate; }

    inline int& GetHeight() { return heightCordinate; }

    inline int GetLength() const { return lengthCordinate; }

    inline int& GetLength() { return lengthCordinate; }

    inline char GetSymbol() const { return symbolPlayer; }

    inline char& GetSymbol() { return symbolPlayer; }

    inline bool GetAlive() const { return alive; }


    friend void RepeatedPlayerCoordinates(vector<Player>& players, const World& world, bool& exitPress);

    friend void PressButtons(vector<Player>& players, World& world, bool& exitPress);
};