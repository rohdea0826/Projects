#include "Model.h"
#include "View.h"
#include <stdlib.h>
#include <stdbool.h>

//��ʼ����
void initSnake(Snake *psnake)
{
	psnake->tail = NULL;
	for (int i = 0; i < 3; i++)
	{
		Node *newNode = (Node *)malloc(sizeof(Node));
		newNode->pos.x = 7 + i;
		newNode->pos.y = 2;

		if (i == 0)
		{
			psnake->head = newNode;
		}

		newNode->next = psnake->tail;
		psnake->tail = newNode;
	}
	psnake->direction = DOWN;
}

//�ж�ʳ�ﲻ��������
bool FoodonSnake(Position pos, const Snake *psnake)
{
	Node *newNode;
	for (newNode = psnake->tail; newNode != NULL; newNode = newNode->next)
	{
		if (pos.x == newNode->pos.x &&pos.y == newNode->pos.y) return 1;
	}
	return 0;
}

//ʳ�����
//1.���  2.��Χ��  3.����������
Position initFood(int w, int h, const Snake *psnake)
{
	Position pos;
	do
	{
		pos.x = rand() % w;
		pos.y = rand() % h;
	} while (FoodonSnake(pos, psnake));
	displayFood(pos.x, pos.y);
	return pos;
}

//��Ϸ��ͼ���߳�ʼ��
void initGame(Game *pGame)
{
	system("cls");
	pGame->width = 28;
	pGame->height = 27;

	initSnake(&pGame->snake);
	pGame->food = initFood(pGame->width, pGame->height, &pGame->snake);
	initView(pGame->width, pGame->height);
	displayWall(pGame->width, pGame->height);
	displaySnake(&pGame->snake);

	pGame->speed = 300;
}

//ͷ�����һ���ڵ�
void addHead(Snake *psnake, Position next)
{
	Node *newNode = (Node *)malloc(sizeof(Node));
	newNode->pos = next;
	newNode->next = NULL;
	psnake->head->next = newNode;
	psnake->head = newNode;
	displaySnakeBlock(next.x, next.y);
}

void removeTail(Snake *psnake)
{
	Node *del = psnake->tail;
	psnake->tail = psnake->tail->next;
	cleanSnakeBlock(del->next->pos.x, del->next->pos.y);
	free(del);
	del = NULL;
}