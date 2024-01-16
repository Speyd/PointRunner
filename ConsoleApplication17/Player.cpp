#include "Player.h"
#include "World.h"
#include <iostream>
#include <future>
#include <vector>
#include <conio.h>
#include <mutex>

mutex inputMutex1;

using namespace std;

void Player::ChangeLocation(const int height, const int length)
{
    heightCordinate = rand() % ((height - 1) - 0 + 1) + 0;
    lengthCordinate = rand() % ((length - 1) - 0 + 1) + 0;
}

void Player::FillCells(World& worldP)
{
    if (worldP.world[heightCordinate][lengthCordinate] == worldP.symbolPoint)
    {
        if (worldP.doublePoint > 0)
        {
            score += 2;
            worldP.amountPoint++;
        }
        else score++;

        if (rand() % (100 - 1 + 1) + 1 <= 15)//Спавн бомб
        {
            worldP.bombs.push_back(World::Bomb(heightCordinate, lengthCordinate));
            worldP.world[heightCordinate][lengthCordinate] = worldP.symbolBomb;
            if(rand() % (100 - 1 + 1) + 1 <= 20)stunTime = 2;
        }
        else if(rand() % (100 - 1 + 1) + 1 <= 10)//Доб монеты
        {
            int randScore{ rand() % (2 - 1 + 1) + 1 };
            worldP.amountPoint += randScore;
            score += randScore;
            worldP.world[heightCordinate][lengthCordinate] = ' ';
        }
        else if(doubleStep <= 0 && rand() % (100 - 1 + 1) + 1 <= 8)//Двойной шаг
        {
            doubleStep = 10;
            worldP.world[heightCordinate][lengthCordinate] = ' ';
        }
        else  worldP.world[heightCordinate][lengthCordinate] = ' ';
    }
}

void Player::PressButtonPlayer(World& worldP)
{
    char keyPress = _getch();

    if (keyPress == 'w' || keyPress == 'W' || keyPress == 72) //Вверх
    {
        for (size_t i{ 0 }; i < (doubleStep > 0 ? 2 : 1); i++)
        {
            if (heightCordinate > 0)
            {
                heightCordinate--;
                FillCells(worldP);

                if (worldP.world[heightCordinate][lengthCordinate] == ' ')
                {
                    swap(worldP.world[heightCordinate][lengthCordinate], worldP.world[heightCordinate + 1][lengthCordinate]);
                }
                else heightCordinate++;
            }
        }
    }
    else if (keyPress == 's' || keyPress == 'S' || keyPress == 80)//Вниз
    {
        for (size_t i{ 0 }; i < (doubleStep > 0 ? 2 : 1); i++)
        {
            if (heightCordinate < worldP.heightWorld - 1)
            {
                heightCordinate++;
                FillCells(worldP);

                if (worldP.world[heightCordinate][lengthCordinate] == ' ')
                {
                    swap(worldP.world[heightCordinate][lengthCordinate], worldP.world[heightCordinate - 1][lengthCordinate]);
                }
                else heightCordinate--;
            }
        }
    }
    else if (keyPress == 'a' || keyPress == 'A' || keyPress == 75)//На лево
    {
        for (size_t i{ 0 }; i < (doubleStep > 0 ? 2 : 1); i++)
        {
            if (lengthCordinate > 0)
            {
                lengthCordinate--;
                FillCells(worldP);

                if (worldP.world[heightCordinate][lengthCordinate] == ' ')
                {
                    swap(worldP.world[heightCordinate][lengthCordinate], worldP.world[heightCordinate][lengthCordinate + 1]);
                }
                else lengthCordinate++;
            }
        }
    }
    else if (keyPress == 'd' || keyPress == 'D' || keyPress == 77)//На право
    {
        for (size_t i{ 0 }; i < (doubleStep > 0 ? 2 : 1); i++)
        {
            if (lengthCordinate < worldP.lenghtWorld - 1)
            {
                lengthCordinate++;
                FillCells(worldP);

                if (worldP.world[heightCordinate][lengthCordinate] == ' ')
                {
                    swap(worldP.world[heightCordinate][lengthCordinate], worldP.world[heightCordinate][lengthCordinate - 1]);
                }
                else lengthCordinate--;
            }
        }
    }
}