#include "util.h"
#include<windows.h>
#include <stdio.h>

void gotoxy(int x, int y)
{
	COORD Pos = { x, y};
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}

void textcolor(int foreground, int background)
{
	int color = foreground + background * 16;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

double RAD(const double& x)
{
	return (x*_PI / 180.);
}

double DEG(const double& x)
{
	return (x*180. / _PI);
}
