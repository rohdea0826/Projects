#include "Model.h"
#include <Windows.h>
#include <stdio.h>

static void setCurPos(int X, int Y)
{
	HANDLE hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	// ��׼����/��׼���/��׼�������
	COORD coord = { X, Y };
	SetConsoleCursorPosition(hStdOutput, coord);
}

void initView(int width, int height)
{
	HANDLE hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO info;
	GetConsoleCursorInfo(hStdOutput, &info);
	info.bVisible = 0;
	SetConsoleCursorInfo(hStdOutput, &info);
}

void displayWall(int width, int height)
{
	// �ϱߵ�ǽ
	setCurPos(0, 0);
	for (int i = 0; i < width + 2; i++) {
		printf("��");
	}

	// �±ߵ�
	setCurPos(0, height + 1);
	for (int i = 0; i < width + 2; i++) {
		printf("��");
	}

	// ��ߵ�
	for (int i = 0; i < height + 2; i++) {
		setCurPos(0, i);
		printf("��");
	}

	// �ұߵ�
	for (int i = 0; i < height + 2; i++) {
		setCurPos(2 * (1 + width), i);
		printf("��");
	}
}

void displaySnake(const Snake *pSnake)
{
	for (Node *cur = pSnake->head; cur != NULL; cur = cur->next) {
		setCurPos(2 * (cur->pos.x + 1), cur->pos.y + 1);
		printf("��");
	}
}

void displaySnakeBlock(int x, int y)
{
	setCurPos(2 * (x + 1), y + 1);
	printf("��");
}

void displayFood(int x, int y)
{
	setCurPos(2 * (x + 1), y + 1);
	printf("��");
}

void cleanSnakeBlock(int x, int y)
{
	setCurPos(2 * (x + 1), y + 1);
	printf("  ");
}

void displayScore(int score)
{
	setCurPos(30 * 2 + 10, 10);
	printf("�÷�: %d", score);
}