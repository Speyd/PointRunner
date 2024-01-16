#include "Player.h"
#include "World.h"
#include <iostream>
#include <vector>
#include <string>
#include <conio.h>
#define NULLNUM 48

using namespace std;

int World::AmountNonLivingPlayer(const vector<Player>& players, bool& exitPress)
{
    int amountNonPlayer{ 0 };
    for (size_t i{ 0 }; i < amountPlayer; i++)
    {
        if (players[i].alive == false)amountNonPlayer++;
    }

    return amountNonPlayer;
}

void World::BoomBombs(vector<Player>& players, bool& exitPress)
{
    for(size_t i{ 0 }; i < bombs.size(); i++)
    {
        if (bombs[i].GetTimeBoom() <= 0)
        {
            for (int line{ bombs[i].GetLine() - 2 }, maxLine{ bombs[i].GetLine() + 2 }; line <= maxLine; line++)
            {
                for (int column{ bombs[i].GetColumn() - 2 }, maxColumn{ bombs[i].GetColumn() + 2 }; column <= maxColumn; column++)
                {
                    for (Player& player : players)
                    {
                        if (player.alive == true && player.heightCordinate == line && player.lengthCordinate == column)
                        {
                            player.alive = false;
                            if(AmountNonLivingPlayer(players, exitPress) == amountPlayer) exitPress = true;

                            world[player.heightCordinate][player.lengthCordinate] = ' ';
                            break;
                        }
                    }
                }
            }

            world[bombs[i].GetLine()][bombs[i].GetColumn()] = ' ';
            bombs.erase(bombs.begin() + i);

            i--;
        }
        else bombs[i].GetTimeBoom() -= 1;
    }

}

string World::PrintEvent()
{
    if (generalStun > 0)return "General stun(" + to_string(generalStun) + ')';
    else if (reverseControl > 0)return "Reverse control(" + to_string(reverseControl) + ')';
    else if (doublePoint > 0)return "Double point(" + to_string(doublePoint) + ')';
    else if (changeSymbolPlayer > 0)return "Change symbol player(" + to_string(changeSymbolPlayer) + ')';
    return "None";
}

void World::AdditionalPrint(vector<Player>& players, const std::chrono::seconds& duration)
{
    if (amountPlayer > heightWorld)
    {
        for (int i{ heightWorld }; i < amountPlayer; i++)
        {
            cout << string(lenghtWorld, ' ') << "\tScore " << i + 1 << "-th player: " << players[i].GetScore() << endl;
        }
    }
    if (amountPlayer + 1 > heightWorld)
    {
        cout << string(lenghtWorld, ' ') << "\tTime: " << duration.count() << endl;
    }
    if (amountPlayer + 2 > heightWorld && workEvents == true)
    {
        cout << string(lenghtWorld, ' ') << "\tEvent: " << PrintEvent() << endl;
    }
}

void World::RandomEvent(vector<Player>& players)
{
    int num{ rand() % (100 - 1 + 1) + 1 };
    workEvents = true;

    if (num <= 25)
    {
        generalStun = 3;
        for (size_t i{ 0 }; i < amountPlayer; i++)players[i].stunTime = 3;
    }
    else if (num > 25 && num <= 50)reverseControl = 5;
    else if (num > 50 && num <= 75)doublePoint = 5;
    else if (num > 75 && num <= 100)
    {
        changeSymbolPlayer = 10;
        for (size_t i{ 0 }; i < amountPlayer; i++)
        {
            if (players[i].alive == true) {
                players[i].GetSymbol() = char(rand() % (126 - 33 + 1) + 33);
                world[players[i].heightCordinate][players[i].lengthCordinate] = players[i].symbolPlayer;
            }
        }
    }
}

void World::PassageTimeEvent(vector<Player>& players)
{
    if (generalStun > 0)generalStun--;
    if (reverseControl > 0)reverseControl--;
    else if (doublePoint > 0)doublePoint--;
    else if (changeSymbolPlayer > 0)
    {
        changeSymbolPlayer--;
        if(changeSymbolPlayer == 0)
        {
            for (int i{ 0 }; i < amountPlayer; i++)
            {
                if (players[i].alive == true) {
                    players[i].GetSymbol() = char('0' + i);
                    world[players[i].heightCordinate][players[i].lengthCordinate] = players[i].symbolPlayer;
                }
            }
        }
    }
}

void World::CheckEndEvent()
{
    if (generalStun <= 0 && reverseControl <= 0 &&
        doublePoint <= 0 && changeSymbolPlayer <= 0)
    {
        workEvents = false;
        system("cls");
    }
}

void World::PrintWorld(vector<Player>& players, const std::chrono::seconds& duration, bool& exitPress)
{

    static std::chrono::seconds previousDuration = duration;
    auto secondsCount = duration.count();

    if (bombs.size() != 0 && previousDuration < duration)BoomBombs(players, exitPress);
    if (previousDuration < duration)
    {
        PassageTimeEvent(players);
        for(size_t i{ 0 }; i < amountPlayer; i++)
        {
            if (players[i].doubleStep > 0)players[i].doubleStep--;
            if (players[i].stunTime > 0)players[i].stunTime--;
        }
    }
    if (workEvents == false && secondsCount % 15 == 0 && secondsCount != 0)RandomEvent(players);
    if (workEvents == true)CheckEndEvent();

    previousDuration = duration;

    for (size_t i{ 0 }; i < heightWorld; i++)
    {
        for (size_t l{ 0 }; l < lenghtWorld; l++)
        {
            cout << world[i][l];
        }
        if (i < amountPlayer) cout << "\tScore " << i + 1 << "-th player: " << players[i].GetScore();
        else if (i == amountPlayer) cout << "\tTime: " << duration.count();
        else if (workEvents == true && i == amountPlayer + 1)cout << "\tEvent: " << PrintEvent();

        cout << endl;

    }
    AdditionalPrint(players, duration);
}

void World::FillWorld(const vector<Player>& players)
{

    if (amountPlayer >= heightWorld * lenghtWorld)
    {
        throw exception("Amount players more or equel size world!");
    }

    for (size_t i{ 0 }; i < heightWorld; i++)
    {
        for (size_t l{ 0 }; l < lenghtWorld; l++)
        {
            if (rand() % (1 - 0 + 1) + 0)
            {
                world[i][l] = symbolPoint;
                amountPoint++;
            }
            else world[i][l] = ' ';
        }
    }

    for (size_t i{ 0 }; i < amountPlayer; i++)
    {
        if (world[players[i].heightCordinate][players[i].lengthCordinate] == symbolPoint)
        {
            amountPoint--;
        }

        world[players[i].heightCordinate][players[i].lengthCordinate] = players[i].symbolPlayer;
    }
}