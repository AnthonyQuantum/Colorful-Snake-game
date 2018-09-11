#include <stdlib.h>
#include <conio.h>
#include <iostream>
#include <GL\glut.h>
#include <GL\glaux.h>
#include <time.h>
#pragma comment(lib, "legacy_stdio_definitions.lib")

using namespace std;

int N = 20; int M = 20;
int Scale = 50;
int w = Scale*N; int h = Scale*M;
bool movepossible = true;
int f = 6;
int waitrecolor = 0;
int recolorcounter = 0;
int recolorspeed = 30;
int sncolorcounter = 0;
int curcolor = -1;
int startcolor = -1;
int Score = 0;
int dir, leng = 4; //Направление и длина змейки
bool game = true;
int cursorw = w;

struct
{
	int x;
	int y;
	int sncolor;
} s[100];

class Fruits {
public:
	int x, y, color;
	void NewFruit() {
		x = rand() % (N-1);
		y = rand() % (M-1);
		color = rand() % 7;
	}
	void DrawFruit() {
		if (waitrecolor == recolorspeed) { color = rand() % 7; recolorcounter++; }
		if (color == 0) { glColor3f(1, 0.1, 0.1); }
		if (color == 1) { glColor3f(1, 0.6, 0.1); }
		if (color == 2) { glColor3f(1, 1, 0.1); }
		if (color == 3) { glColor3f(0.1, 1, 0.1); }
		if (color == 4) { glColor3f(0.117, 0.749, 1); }
		if (color == 5) { glColor3f(0.1, 0.1, 1); }
		if (color == 6) { glColor3f(0.664, 0.1, 1); }
		glRectf(x*Scale, y*Scale, (x + 1)*Scale, (y + 1)*Scale);
		if ((waitrecolor == recolorspeed) && (recolorcounter == 10)) { waitrecolor = 0; recolorcounter = 0; }
	}
} Fruit[10];

void DrawDigit(int digit = 0) {
	switch (digit) {
	case 0: { glColor3f(1, 1, 1); glRectf(cursorw - 10, h, cursorw - 40, h - 50); glColor3f(0, 0, 0); glRectf(cursorw - 20, h - 10, cursorw - 30, h - 40); break;}
	case 1: { glColor3f(1, 1, 1); glRectf(cursorw - 10, h, cursorw - 20, h - 50); break;}
	case 2: {glColor3f(1, 1, 1); glRectf(cursorw - 10, h, cursorw - 40, h - 50); glColor3f(0, 0, 0); glRectf(cursorw - 20, h - 10, cursorw - 40, h - 20); glRectf(cursorw - 10, h - 30, cursorw - 30, h - 40); break;}
	case 3: { glColor3f(1, 1, 1); glRectf(cursorw - 10, h, cursorw - 40, h - 50); glColor3f(0, 0, 0); glRectf(cursorw - 20, h - 10, cursorw - 40, h - 20); glRectf(cursorw - 20, h - 30, cursorw - 40, h - 40); break;}
	case 4: { glColor3f(1, 1, 1); glRectf(cursorw - 10, h, cursorw - 40, h - 50); glColor3f(0, 0, 0); glRectf(cursorw - 20, h, cursorw - 30, h - 20); glRectf(cursorw - 20, h - 30, cursorw - 50, h - 50); break;}
	case 5: {glColor3f(1, 1, 1); glRectf(cursorw - 10, h, cursorw - 40, h - 50); glColor3f(0, 0, 0); glRectf(cursorw - 10, h - 10, cursorw - 30, h - 20); glRectf(cursorw - 20, h - 30, cursorw - 40, h - 40); break;}
	case 6: {glColor3f(1, 1, 1); glRectf(cursorw - 10, h, cursorw - 40, h - 50); glColor3f(0, 0, 0); glRectf(cursorw - 10, h - 10, cursorw - 30, h - 20); glRectf(cursorw - 20, h - 30, cursorw - 30, h - 40); break;}
	case 7: {glColor3f(1, 1, 1); glRectf(cursorw - 10, h, cursorw - 40, h - 50); glColor3f(0, 0, 0); glRectf(cursorw - 20, h - 10, cursorw - 40, h - 50); break;}
	case 8: {glColor3f(1, 1, 1); glRectf(cursorw - 10, h, cursorw - 40, h - 50); glColor3f(0, 0, 0); glRectf(cursorw - 20, h - 10, cursorw - 30, h - 20); glRectf(cursorw - 20, h - 30, cursorw - 30, h - 40); break;}
	case 9: {glColor3f(1, 1, 1); glRectf(cursorw - 10, h, cursorw - 40, h - 50); glColor3f(0, 0, 0); glRectf(cursorw - 20, h - 10, cursorw - 30, h - 20); glRectf(cursorw - 20, h - 30, cursorw - 40, h - 40); break;}
	}
}

void DrawField() //Постройка фоновой сетки
{
	glColor3f(0.3, 0.3, 0.3);
	glBegin(GL_LINES);
	for (int i = 0; i < w; i += Scale) { glVertex2f(i, 0); glVertex2f(i, h); }
	for (int j = 0; j < h; j += Scale) { glVertex2f(0, j); glVertex2f(w, j); }
	glEnd();
	DrawDigit(Score % 10);
	if (Score / 10 != 0) { cursorw = cursorw - 40; DrawDigit(Score / 10); cursorw = w; }
}

void Tick()
{
	if (game == true) {
	movepossible = true;
	waitrecolor++;
	sncolorcounter++;
	for (int i = leng; i > 0; i -= 1) //Для тела
	{
		s[i].x = s[i - 1].x;
		s[i].y = s[i - 1].y;
	}

	if (s[0].x > (N-1)) game=false;
	if (s[0].x < 0) game = false;
	if (s[0].y > (M-1)) game = false;
	if (s[0].y < 0) game = false;

	if (dir == 0) s[0].y += 1; //Для головы
	if (dir == 1) s[0].x -= 1;
	if (dir == 2) s[0].x += 1;
	if (dir == 3) s[0].y -= 1;

	for (int i = 0;i < 10;i++)
		if ((s[0].x == Fruit[i].x) && (s[0].y == Fruit[i].y) && (s[0].sncolor == Fruit[i].color)) { leng++; Fruit[i].NewFruit(); Score++; system("cls"); cout << "Your score: " << Score << endl; }
	for (int i = 1;i < leng; i++)
		if ((s[0].x == s[i].x) && (s[0].y == s[i].y)) leng = i;
	}
	else { system("cls"); cout << "Game Over :( \nYour score: " << Score << "\n\nPress any key to close the game" << endl; _getch(); exit(0); };
}

void DrawSnake()
{
	if (sncolorcounter == 50) { startcolor = startcolor + 1; sncolorcounter = 0; }
	if (startcolor == 7) { startcolor = -1; }
	for (int i = 0; i < leng; i++)
	{
		if (curcolor == 6) { curcolor = -1; }
		curcolor++;
		s[i].sncolor = curcolor;
	}
	curcolor = startcolor;
	for (int i = 0; i < leng; i++)
	{
		if (s[i].sncolor == 0) { glColor3f(1, 0.1, 0.1); }
		if (s[i].sncolor == 1) { glColor3f(1, 0.6, 0.1); }
		if (s[i].sncolor == 2) { glColor3f(1, 1, 0.1); }
		if (s[i].sncolor == 3) { glColor3f(0.1, 1, 0.1); }
		if (s[i].sncolor == 4) { glColor3f(0.117, 0.749, 1); }
		if (s[i].sncolor == 5) { glColor3f(0.1, 0.1, 1); }
		if (s[i].sncolor == 6) { glColor3f(0.664, 0.1, 1); }
		glRectf(s[i].x*Scale, s[i].y*Scale, (s[i].x + 1)*Scale, (s[i].y + 1)*Scale);
	}
}

void Keyboard(int key, int a, int b)
{
	switch (key) {
	case 101: if (dir != 3) { dir = 0; movepossible = false; break; } //Вверх
	case 102: if (dir != 1) { dir = 2; movepossible = false; break; } //Вправо
	case 100: if (dir != 2) { dir = 1; movepossible = false; break; } //Влево
	case 103: if (dir != 0) { dir = 3; movepossible = false; break; } //Вниз
	}
}

void Display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	for (int i = 0;i < 10;i++) { Fruit[i].DrawFruit(); }
	DrawField();
	DrawSnake();
	glEnd();
	glFlush();
}

void Timer(int = 0)
{
	Display();
	Tick();
	glutTimerFunc(90, Timer, 0);
}

int main(int argc, char** argv)
{
		for (int i = 0;i < 10;i++) { Fruit[i].NewFruit(); }
		s[0].x = 10; //Начальное положение головы
		s[0].y = 10;
		glEnable(GL_TEXTURE_2D);
		glutInit(&argc, argv);
		glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
		glutInitWindowPosition(0, 0);
		glutInitWindowSize(w, h);
		glutCreateWindow("Qolorful Snake");
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluOrtho2D(0, w, 0, h);

		glutDisplayFunc(Display);
		glutTimerFunc(90, Timer, 0);
		glutSpecialFunc(Keyboard);
		if (game == true) {  }
		else { system("cls"); cout << "Game Over :( \n Your score: " << Score << endl; };
		glutMainLoop();
}