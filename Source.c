#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#define width 75
#define height 25

char mas[height][width + 1];
char racket[2][9] = { 0,0,0,0,0,0,0,0,0, -4, -3, -2, -1, 0, 1, 2, 3, 4 };
int ball_x, ball_y, ball_x_new, ball_y_new;
BOOL start = FALSE;
int wall;

int up_down = -1;
int left_rigth = 1; 
int alpha = 1;

void init()
{
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			if (i == 0)
				mas[i][j] = '#';
			else if ((j == 0) || (j == width - 1))
				mas[i][j] = '#';
			else if (((i > 4) && (i < 9)) && ((j > 5) && (j < 69)))
				mas[i][j] = '=';
			else
				mas[i][j] = ' ';
		}
	}

	for (int x = 0; x < 9; x++) {
		racket[0][x] = ((width - 9) / 2) + x;
		mas[height - 1][racket[0][x]] = '-';
	}

	ball_x_new = racket[0][4];
	ball_y_new = height - 2;
	ball_x = ball_x_new;
	ball_y = ball_y_new;
	mas[ball_y_new][ball_x_new] = 'o';

}
void moveRacket(int x)
{

	if ((racket[0][0] + x > 0) && (racket[0][0] + x < width - 9))
	{
		if (x == -1)
			for (int i = 0; i < 9; i++)
			{
				mas[height - 1][racket[0][i]] = ' ';
				racket[0][i] += x;
				mas[height - 1][racket[0][i]] = '-';
			}
		else
			for (int i = 8; i >= 0; i--)
			{
				mas[height - 1][racket[0][i]] = ' ';
				racket[0][i] += x;
				mas[height - 1][racket[0][i]] = '-';
			}
	}
}

void moveBall()
{
	if (start == FALSE)
	{
		mas[ball_y_new][ball_x_new] = ' ';
		ball_x_new = racket[0][4];
		ball_x = ball_x_new;
		mas[ball_y_new][ball_x_new] = 'o';
	}
	else
	{
		if (alpha == 0)
		{
			ball_y += up_down;
			alpha = abs(left_rigth);
			mas[ball_y_new][ball_x_new] = ' ';
			ball_y_new = ball_y;
			ball_x_new = ball_x;
			mas[ball_y_new][ball_x_new] = 'o';
		}
		else if (left_rigth > 0)
		{
			ball_x++;
			alpha--;
		}
		else if (left_rigth < 0)
		{
			ball_x--;
			alpha--;
		}
	}
}

int searchWall()
{
	//0 - нет касания, 1 - отскок от стены, 2 - отскок от пола\потолка, 3 - разворот от двух, 4 - разворот от одной
	int flag = 0;

	if ((left_rigth > 0) && (up_down < 0)) //вправо и вверх
	{
		if (mas[ball_y][ball_x + 1] != ' ') flag = 1;
		if (mas[ball_y - 1][ball_x] != ' ') flag = 2;
		if ((mas[ball_y][ball_x + 1] != ' ') && (flag == 2)) flag = 3;
		if ((mas[ball_y - 1][ball_x + 1] != ' ') && (flag == 0)) flag = 4;
	}

	if ((left_rigth < 0) && (up_down < 0)) //влево и вверх
	{
		if (mas[ball_y][ball_x - 1] != ' ') flag = 1;
		if (mas[ball_y - 1][ball_x] != ' ') flag = 2;
		if ((mas[ball_y][ball_x - 1] != ' ') && (flag == 2)) flag = 3;
		if ((mas[ball_y - 1][ball_x - 1] != ' ') && (flag == 0)) flag = 4;
	}


	if ((left_rigth > 0) && (up_down > 0)) //вправо и вниз
	{
		if (mas[ball_y][ball_x + 1] != ' ') flag = 1;
		if (mas[ball_y + 1][ball_x] != ' ') flag = 2;
		if ((mas[ball_y][ball_x - 1] != ' ') && (flag == 2)) flag = 3;
		if ((mas[ball_y + 1][ball_x + 1] != ' ') && (flag == 0)) flag = 4;
	}

	if ((left_rigth < 0) && (up_down > 0)) //влево и вниз
	{
		if (mas[ball_y][ball_x - 1] != ' ') flag = 1;
		if (mas[ball_y + 1][ball_x] != ' ') flag = 2;
		if ((mas[ball_y][ball_x - 1] != ' ') && (flag == 2)) flag = 3;
		if ((mas[ball_y + 1][ball_x - 1] != ' ') && (flag == 0)) flag = 4;
	}

	if ((left_rigth == 0) && (up_down < 0)) // вверх
	{
		if (mas[ball_y - 1][ball_x] != ' ') flag = 2;
	}

	if ((left_rigth == 0) && (up_down > 0)) // вниз
	{
		if (mas[ball_y + 1][ball_x] != ' ') flag = 2;
	}
	return flag;
}

void reactionWall()
{
	if (alpha == abs(left_rigth))
	{
		switch (wall)
		{
		case 1:
			left_rigth = -left_rigth;
			break;
		case 2:
			up_down = -up_down;
			break;
		case 3:
			left_rigth = -left_rigth;
			up_down = -up_down;
			break;
		case 4:
			left_rigth = -left_rigth;
			up_down = -up_down;
			break;
		}
	}

	else if (wall > 0)
	{
		alpha = 0;
	}
}

void reactionRacket()
{
	int u;
	if ((ball_y_new == 23) && (alpha == 0))
	{
		u = racket[1][(ball_x_new - racket[0][0])];
		left_rigth = left_rigth + u;
		if (left_rigth < -4) left_rigth = -4;
		if (left_rigth > 4) left_rigth = 4;
	}
}


void breakBrick()
{
	if ((alpha == abs(left_rigth)) && (wall > 0))
	{
		if ((left_rigth > 0) && (up_down < 0)) //вправо и вверх
		{
			if (mas[ball_y][ball_x + 1] == '=') mas[ball_y][ball_x + 1] = ' ';
			if (mas[ball_y - 1][ball_x] == '=') mas[ball_y - 1][ball_x] = ' ';
			if ((mas[ball_y - 1][ball_x + 1] == '=') && wall == 4) mas[ball_y - 1][ball_x + 1] = ' ';
		}

		if ((left_rigth < 0) && (up_down < 0)) //влево и вверх
		{
			if (mas[ball_y][ball_x - 1] == '=') mas[ball_y][ball_x - 1] = ' ';
			if (mas[ball_y - 1][ball_x] == '=') mas[ball_y - 1][ball_x] = ' ';
			if ((mas[ball_y - 1][ball_x - 1] == '=') && wall == 4) mas[ball_y - 1][ball_x - 1] = ' ';
		}

		if ((left_rigth > 0) && (up_down > 0)) //вправо и вниз
		{
			if (mas[ball_y][ball_x + 1] == '=') mas[ball_y][ball_x + 1] = ' ';
			if (mas[ball_y + 1][ball_x] == '=') mas[ball_y + 1][ball_x] = ' ';
			if ((mas[ball_y + 1][ball_x + 1] == '=') && wall == 4) mas[ball_y + 1][ball_x + 1] = ' ';
		}

		if ((left_rigth < 0) && (up_down > 0)) //влево и вниз
		{
			if (mas[ball_y][ball_x - 1] == '=') mas[ball_y][ball_x - 1] = ' ';
			if (mas[ball_y + 1][ball_x] == '=') mas[ball_y + 1][ball_x] = ' ';
			if ((mas[ball_y + 1][ball_x - 1] == '=') && wall == 4) mas[ball_y + 1][ball_x - 1] = ' ';;
		}

		if ((left_rigth == 0) && (up_down < 0)) // вверх
		{
			if (mas[ball_y - 1][ball_x] == '=') mas[ball_y - 1][ball_x] = ' ';
		}

		if ((left_rigth == 0) && (up_down > 0)) // вниз
		{
			if (mas[ball_y + 1][ball_x] == '=') mas[ball_y + 1][ball_x] = ' ';
		}
	}
}

void gameover()
{
	if (ball_y == 24)
	{
		start = FALSE;
		up_down = -1;
		left_rigth = 1;
		alpha = 1;
		init();
	}
}

int show()
{
	for (int i = 0; i < height; i++)
	{
		printf(mas[i]);
		printf("\n");
	}
}

void setcur(int x, int y)
{
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}


void main()
{

	init();
	int i = 0;

	do
	{
		setcur(0, 0);

		if ((GetKeyState('A') < 0) || (GetKeyState(VK_LEFT) < 0))  moveRacket(-1);
		if ((GetKeyState('D') < 0) || (GetKeyState(VK_RIGHT) < 0))  moveRacket(1);

		if (GetKeyState(VK_SPACE) < 0) start = TRUE;

		if (i == 0)
		{
			wall = searchWall();

			if (wall != 0)
			{
				breakBrick();
				reactionWall();	
			}

			reactionRacket();
			moveBall();
			
			i =(5 - abs(alpha));
		}
		show();
		gameover();
		i--;
		Sleep(10);

	} while (GetKeyState(VK_ESCAPE) >= 0);

	return 0;
}