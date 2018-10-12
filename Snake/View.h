#pragma once

void initView(int width, int height);
void displayWall(int width, int height);
void displaySnake(const Snake *pSnake);
void displaySnakeBlock(int x, int y);
void cleanSnakeBlock(int x, int y);
void displayFood(int x, int y);
void displayScore(int score);