#include "projectile.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include<windows.h>
#include "util.h"
#include <math.h>

VECTOR Input_Initial_velocity()
{
	double angle, magnitude;

	gotoxy(1, 40); 
	printf("각도를 입력하시오: ");
	scanf_s("%lf", &angle);

	gotoxy(1, 41); 
	printf("속도를 입력하시오: ");
	scanf_s("%lf", &magnitude);

	VECTOR ans = { magnitude*cos(RAD(angle)), magnitude*sin(RAD(angle)) };	

	return ans;
}

//======================================================

void PR::SetData(const VECTOR& r0, const VECTOR& v0, const double& dt)
{
	m_r = r0;
	m_v = v0;
	m_h1 = dt;
	m_h2 = dt*dt*0.5;

	m_r_int.x = int(m_r.v[0]);
	m_r_int.y = int(m_r.v[1]);

	m_r_int_prev.x = m_r_int_prev.y = 0;
}

int PR::Update(const VECTOR& force)
{
	m_r_int_prev = m_r_int;
	int i;
	for (i = 0; i < 2; i++) {
		m_r.v[i] += m_h1*m_v.v[i] + m_h2*force.v[i];
		m_v.v[i] += m_h1*force.v[i];
	}

	m_r_int.x = int(m_r.v[0]);
	
	return m_r_int.x;
}

int PR::SECONDUpdate()
{
	m_r_int.y = int(m_r.v[1]);

	return m_r_int.y;
}

void PR::ReflectLeftRight()
{
	m_v.v[0] = -m_v.v[0];
}

void PR::ReflectUpDown()
{
	m_v.v[1] = -m_v.v[1];
}

void PR::ReflectCorner()
{
	int i;
	for (i = 0; i < 2; i++) m_v.v[i] = - m_v.v[i];
}
void PR::ReflectEarth()
{
	m_v.v[0] = 0;
	m_v.v[1] = 0; //수정: Earth에 닿았을 때 중지
}
void PR::ReflectMoon()
{
	m_v.v[0] = 0;
	m_v.v[1] = 0; //수정: Moon에 닿았을 때 중지
}
void PR::draw() const
{	
	bool changed = ((abs(m_r_int.x - m_r_int_prev.x) + abs(m_r_int.y - m_r_int_prev.y)) > 0) ? true : false;
	if (!changed) return;

	gotoxy(m_r_int_prev.x, Ny - 1 - m_r_int_prev.y);
//	printf(" "); //이전 위치를 지웁니다.
	gotoxy(m_r_int.x, Ny-1-m_r_int.y);
	printf("*"); //새로운 위치에 문자를 출력해줍니다.

	Sleep(5);
}

//======================================================

TILE_STATE Boundary::check_hit(const PR& r) const
{
	INT_POS p = r.GetIntPosition();	

	return m_tile[p.x][p.y];
}

void Boundary::generate()
{
	int i, j, ran;
	for (i = 1; i < Nx-1; i++) for (j = 1; j < Ny-1; j++) m_tile[i][j] = EMPTY;
	
	for (i = 1; i < Nx-1; i++) m_tile[i][0] = m_tile[i][Ny-1] = BORDER_V;
	for (i = 1; i < Ny-1; i++) m_tile[0][i] = m_tile[Nx-1][i] = BORDER_H;
	for (i = 0; i < 360;i++) m_tile[int(42+8*cos(RAD(i)))][int(19+8*sin(RAD(i)))] = BORDER_Earth; //지구 
	for (i = 0; i < 360;i++) m_tile[int(80 + 4 * cos(RAD(i)))][int(19 + 4 * sin(RAD(i)))] = BORDER_Moon; //달 

	m_tile[0][0] = m_tile[Nx - 1][0] = m_tile[Nx - 1][0] = m_tile[Nx - 1][Ny - 1] = BORDER_CORNER;

	srand(time(NULL));//랜덤한 숫자
		i = (rand() % 102) + 2;
		ran = (rand() % 2)+0; // 행성의 내부에 목표물이 위치하지 않도록 수정
		if (ran == 0)
			j = (rand() % 9) + 28;
		else
			j = (rand() % 9) + 1;
	m_tile[i][j] = TARGET;
}

void Boundary::print() const
{
	int i, j;
	for (i = 0; i < Nx; i++) for (j = 0; j < Ny; j++) {
		gotoxy(i, Ny-1-j);
		switch (m_tile[i][j]) {
		case EMPTY : 
			printf(" ");
			break;

		case BORDER_H:
		case BORDER_V:
		case BORDER_CORNER:
		case BORDER_Earth:
		case BORDER_Moon:
			printf("#");
			break;

		case TARGET:
			printf("$");
			break;
		}
	}
}
