#pragma once

#pragma once
#include "Model.h"

void SnakeInit(Snake *psnake);

Position FoodInit(int w, int h, const Snake *psnake);

bool KilledByWall(const Snake *pSnake, int w, int h);

bool KilledBySelf(const Snake *pSnake);

bool GameOver(const Game *game, int w, int h);

void GameInit(Game *pGame);