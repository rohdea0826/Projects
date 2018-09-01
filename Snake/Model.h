#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <Windows.h>

typedef enum Direction
{
	UP,DOWN,LEFT,RIGHT
}Direction;

typedef struct Position
{
	int x;
	int y;
}Position;

typedef struct Node
{
	Position pos;
	struct Node *next;
}Node;

typedef struct Snake
{
	Node *head;//��ͷ������β
	Node *tail;//��β������ͷ
	Direction direction;
}Snake;

typedef struct Game
{
	Snake snake;
	Position food;

	int width;
	int height;
}Game;

