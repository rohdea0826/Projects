#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <Windows.h>

//方向，上下左右
typedef enum Direction
{
	UP,DOWN,LEFT,RIGHT
}Direction;

//位置坐标
typedef struct Position
{
	int x;
	int y;
}Position;

//
typedef struct Node
{
	Position pos;
	struct Node *next;
}Node;

typedef struct Snake
{
	Node *head;//蛇头，链表尾
	Node *tail;//蛇尾，链表头
	Direction direction;
}Snake;

typedef struct Game
{
	Snake snake;
	Position food;

	int width;
	int height;
	int speed;
}Game;

void initSnake(Snake *psnake);
bool FoodonSnake(Position pos, const Snake *psnake);
Position initFood(int w, int h, const Snake *psnake);
void initGame(Game *pGame);
void addHead(Snake *psnake, Position next);
void removeTail(Snake *psnake);
