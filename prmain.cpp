#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include "util.h"
#include "projectile.h"
#include <windows.h>
#include <math.h>

int main(){
	system("cls");
	system("mode con cols=184 lines=80");

	
	PR a;
	Boundary b;
	b.generate();
	int trial=0; 
	double pitch = 262;
	
	double dt(0.005);
	VECTOR r0 = { 42., 28. }, v;
	
	int test(0);
	TILE_STATE s(EMPTY);

	do {
		system("cls");
		b.print();

		v = Input_Initial_velocity();
	
		a.SetData(r0, v, dt);
		int xpos = 42; 
		int ypos= 28;
		int end(0);
		do {
			VECTOR force = { (100*(2 * (42 - xpos)) / pow((pow(42 - xpos,2) + pow(19 - ypos,2)),1.5))+(100 * (1 * (80 - xpos)) / pow((pow(80 - xpos,2) + pow(19 - ypos,2)),1.5)), (100* (2 * (19 - ypos)) / pow((pow(42 - xpos,2) + pow(19 - ypos,2)),1.5))+ (100 * (1 * (19 - ypos)) / pow((pow(80 - xpos,2) + pow(19 - ypos,2)),1.5)) };

			a.Update(force);
			a.SECONDUpdate();

			xpos = a.Update(force);
			ypos = a.SECONDUpdate();
							
			s = b.check_hit(a);
			switch (s) {
			case TARGET:
				trial += 1;
				gotoxy(1, 42);
				printf("number of trial:%d",trial);
				gotoxy(1, 43); 
				printf("The target is destroyed!!!");
				Beep(262*2, 200);
				Beep(330*2, 200);
				Beep(392*2, 200);
				Beep(523*2, 500);
				end = 1;
				b.generate();
				trial = 0;
				pitch = 262;
				break;

			case BORDER_H:
				a.ReflectLeftRight(); 
				Beep(pitch, 50);
				pitch = pitch * pow(2, 1 / 6.);
				break;

			case BORDER_V:
				a.ReflectUpDown();
				Beep(pitch, 50);
				pitch = pitch * pow(2., 1 / 6.);
				break;

			case BORDER_CORNER:
				a.ReflectUpDown();
				Beep(pitch, 50);
				pitch = pitch * pow(2, 1 / 6.);
				break;
			case BORDER_Earth:
				a.ReflectEarth(); //Earth에 닿았을 때 중지 
				Beep(392*2, 50);
				Beep(349*2, 50);
				Beep(330*2, 50);
				Beep(294*2, 50);
				Beep(262*2, 50);
				trial += 1;
				gotoxy(1, 42);
				printf("number of trial:%d", trial);
				end = 1;
				pitch = 262;
				break;
			case BORDER_Moon:
				a.ReflectMoon(); //Moon에 닿았을 때 중지 
				Beep(392*2, 50);
				Beep(349*2, 50);
				Beep(330*2, 50);
				Beep(294*2, 50);
				Beep(262*2, 50);
				trial += 1;
				gotoxy(1, 42);
				printf("number of trial:%d", trial);
				end = 1;
				pitch = 262;
				break;
			}

			if (s==EMPTY) a.draw();

		} while (!end);

		gotoxy(1, 44); 	
		printf("Quit? ('q' to quit)");				
		test = _getch();
		
	} while (test != 'q');
}
