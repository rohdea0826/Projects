#include "Controller.h"

//获取蛇前进方向的下一个坐标
Position getNextPos(const Snake *psnake)
{
	Position pos_tmp;
	pos_tmp.x = 0; pos_tmp.y = 0;
	switch (psnake->direction)
	{
	case UP:
		pos_tmp.x = psnake->head->pos.x;
		pos_tmp.y = psnake->head->pos.y - 1;
		break;
	case DOWN:
		pos_tmp.x = psnake->head->pos.x;
		pos_tmp.y = psnake->head->pos.y + 1;
		break;
	case LEFT:
		pos_tmp.x = psnake->head->pos.x - 1;
		pos_tmp.y = psnake->head->pos.y;
		break;
	case RIGHT:
		pos_tmp.x = psnake->head->pos.x + 1;
		pos_tmp.y = psnake->head->pos.y;
		break;
	default:
		break;
	}
	return pos_tmp;
}

//判断是否吃到食物
bool eatFood(Position food, Position next)
{
	return food.x == next.x && food.y == next.y;
}

bool beyondMap(Game game,Position next)
{
	if (next.x >= game.width || next.x < 0)	return 1;
	if (next.y >= game.height || next.y < 0)	return 1;
	return 0;
}

void pauseGame()
{
	while (1)
	{
		Sleep(300);
		if (GetAsyncKeyState(VK_SPACE))
			break;
	}
}

void runGame()
{
	//游戏启动
	Game game;
	initGame(&game);
	initView(game.height, game.width);
	displayWall(game.width, game.height);
	displaySnake(&game.snake);
	initFood(game.width, game.height, &game.snake);
	
	while (1)
	{
		if (GetAsyncKeyState(VK_UP) && game.snake.direction != DOWN) {
			game.snake.direction = UP;
		}
		else if (GetAsyncKeyState(VK_DOWN) && game.snake.direction != UP) {
			game.snake.direction = DOWN;
		}
		else if (GetAsyncKeyState(VK_LEFT) && game.snake.direction != RIGHT) {
			game.snake.direction = LEFT;
		}
		else if (GetAsyncKeyState(VK_RIGHT) && game.snake.direction != LEFT) {
			game.snake.direction = RIGHT;
		}
		else if (GetAsyncKeyState(VK_SPACE)) {
			pauseGame();
		}
		else if (GetAsyncKeyState(VK_ESCAPE)) {
			break;
		}
		else if (GetAsyncKeyState(VK_F1)) {
			game.speed = 100;
		}

		Position nextPos = getNextPos(&game.snake);
		if (eatFood(game.food,nextPos))
		{
			addHead(&game.snake, nextPos);
			game.food = initFood(game.width, game.height, &game.snake);
		}
		else
		{
			removeTail(&game.snake);
			addHead(&game.snake, nextPos);
		}

		if (beyondMap(game, nextPos)) break;
		
		Sleep(game.speed);
	}
}


int main()
{
	srand((unsigned)time(NULL));
	runGame();
	return 0;
}