//初始化 Game
#include "Controller.h"

void SnakeInit(Snake *psnake)
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
}

bool IsOverlap(Position pos, const Snake *psnake)
{
	Node *newNode;
	for (newNode = psnake->tail; newNode != NULL; newNode = newNode->next)
	{
		if (pos.x == newNode->pos.x &&pos.y == newNode->pos.y) return 1;
	}
	return 0;
}

//食物规则
//1.随机  2.范围内  3.不在蛇身上
Position FoodInit(int w,int h,const Snake *psnake)
{
	Position pos;
	do
	{
		pos.x = rand() % w;
	pos.y = rand() % h; 
	}
	while (IsOverlap(pos, psnake));
	return pos;
}

void GameInit(Game *pGame)
{
	pGame->width = 28;
	pGame->height = 27;

	SnakeInit(&pGame->snake);
	pGame->food = FoodInit(pGame->width,pGame->height,&pGame->snake);

}

void AddHead(Snake *psnake, Position next)
{
	Node *newNode = (Node *)malloc(sizeof(Node));
	newNode->pos = next;
	newNode->next = NULL;
	psnake->head->next = newNode;
	psnake->head = newNode;
}

Position getNextPos(const Snake *psnake)
{
	
}

bool isEat(Position food, Position next)
{
	return food.x == next.x && food.y == next.y;
}

void GameRun()
{
	Game game;
	GameInit(&game);

	while (1)
	{
		Position nextPos = getNextPos(&game.snake);
		if (isEat(game.food,nextPos))
		{
			AddHead(&game.snake, nextPos);
			game.food = FoodInit(game.width, game.height, &game.snake);
		}
		else
		{
			//removeTail(&game.snake);
			
		}
		
	}
}


int main()
{
	while (1)
	{
		if (GetAsyncKeyState(VK_UP))
			printf("%d\n", VK_UP);
		else
			printf("%d\n", 0);
		Sleep(500);
	}


	//Game a;
	//GameInit(&a);
	return 0;
}