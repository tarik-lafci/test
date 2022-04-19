/*********
   CTIS164 - Template Source Program
----------
STUDENT : TARIK LAFÇI
SECTION : 1
HOMEWORK: 1
----------
PROBLEMS:
----------
ADDITIONAL FEATURES:
*press p to pause/unpause
*press spacebar to turn on/off the tv
*LClick into screen in order to change the color of wheels
*RClick into sky in order to change the position of the sun (if you click edge of the sky it won't change its position in order to display the sun fully)
*********/

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>

#define WINDOW_WIDTH  1400
#define WINDOW_HEIGHT 800
#define TRAIN_LENGHT 575

int ON = 0;
int xR = -475, yR = -175;
int r = 0, g = 0, b = 0;

int sunX = -300, sunY = 200;


#define TIMER_PERIOD  15 // Period for the timer.
#define TIMER_ON 1 // 0:disable timer, 1:enable timer


#define D2R 0.0174532

/* Global Variables for Template File */
bool activeTimer = true;
bool up = false, down = false, right = false, left = false;
int  winWidth, winHeight; // current Window width and height

//
// to draw circle, center at (x,y)
// radius r
//
void circle(int x, int y, int r)
{
#define PI 3.1415
	float angle;
	glBegin(GL_POLYGON);
	for (int i = 0; i < 100; i++)
	{
		angle = 2 * PI*i / 100;
		glVertex2f(x + r * cos(angle), y + r * sin(angle));
	}
	glEnd();
}

void circle_wire(int x, int y, int r)
{
#define PI 3.1415
	float angle;

	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < 100; i++)
	{
		angle = 2 * PI*i / 100;
		glVertex2f(x + r * cos(angle), y + r * sin(angle));
	}
	glEnd();
}

void print(int x, int y, const char *string, void *font)
{
	int len, i;

	glRasterPos2f(x, y);
	len = (int)strlen(string);
	for (i = 0; i < len; i++)
	{
		glutBitmapCharacter(font, string[i]);
	}
}

// display text with variables.
// vprint(-winWidth / 2 + 10, winHeight / 2 - 20, GLUT_BITMAP_8_BY_13, "ERROR: %d", numClicks);
void vprint(int x, int y, void *font, const char *string, ...)
{
	va_list ap;
	va_start(ap, string);
	char str[1024];
	vsprintf_s(str, string, ap);
	va_end(ap);

	int len, i;
	glRasterPos2f(x, y);
	len = (int)strlen(str);
	for (i = 0; i < len; i++)
	{
		glutBitmapCharacter(font, str[i]);
	}
}

// vprint2(-50, 0, 0.35, "00:%02d", timeCounter);
void vprint2(int x, int y, float size, const char *string, ...) {
	va_list ap;
	va_start(ap, string);
	char str[1024];
	vsprintf_s(str, string, ap);
	va_end(ap);
	glPushMatrix();
	glTranslatef(x, y, 0);
	glScalef(size, size, 1);

	int len, i;
	len = (int)strlen(str);
	for (i = 0; i < len; i++)
	{
		glutStrokeCharacter(GLUT_STROKE_ROMAN, str[i]);
	}
	glPopMatrix();
}

//
// To display onto window using OpenGL commands
//

void displayOn() {

	glClearColor(180 / 255., 180 / 255., 160 / 255., 0);
	glClear(GL_COLOR_BUFFER_BIT);



	// outside of the tv
	glColor3f(160 / 255.0, 160 / 255.0, 160 / 255.0);
	glRectf(-420, 270, 420, -270);

	// inside of the tv
	glColor3f(0, 128 / 255.0, 255 / 255.0);
	glRectf(-400, 250, 400, -230);


	//sun
	glColor3f(255, 255, 0);
	circle(sunX, sunY, 30);



	// power button
	glColor3f(0.3, 0.3, 1);
	glBegin(GL_LINE_STRIP);
	glVertex2f(-400, -235);
	glVertex2f(-340, -235);
	glVertex2f(-340, -265);
	glVertex2f(-400, -265);
	glVertex2f(-400, -235);
	glEnd();

	glColor3f(1, 0, 0);
	circle(-380, -250, 10);

	glColor3f(1, 1, 1);
	circle_wire(-380, -250, 7);
	circle_wire(-380, -250, 8);


	glBegin(GL_LINES);
	glColor3ub(255, 255, 255);
	glVertex2f(-380, -250);
	glVertex2f(-380, -240);
	glEnd();

	// restart button
	glColor3f(0.3, 0.3, 1);
	glBegin(GL_LINE_STRIP);
	glVertex2f(-200, -235);
	glVertex2f(-60, -235);
	glVertex2f(-60, -265);
	glVertex2f(-200, -265);
	glVertex2f(-200, -235);
	glEnd();
	vprint(-187, -253, GLUT_BITMAP_8_BY_13, "PAUSE / UNPAUSE");



	// outline of the tv
	glColor3ub(128 / 255., 128 / 255., 128 / 255.);
	glBegin(GL_LINES);
	glVertex2f(-420, 270);
	glVertex2f(420, 270);

	glVertex2f(420, 270);
	glVertex2f(420, -270);

	glVertex2f(420, -270);
	glVertex2f(-420, -270);

	glVertex2f(-420, -270);
	glVertex2f(-420, 270);
	glEnd();

	// ground
	glColor3f(102 / 255., 51 / 255., 0);
	glRectf(-400, -100, 400, -230);

	//ground of rails

	glColor3f(255 / 255., 204 / 255., 153 / 255.);
	glRectf(-400, -200, 400, -150);

	//rails

	glColor3ub(1, 1, 1);
	glBegin(GL_LINES);

	glVertex2f(-400, -200);
	glVertex2f(400, -200);

	glVertex2f(-400, -150);
	glVertex2f(400, -150);

	for (int i = -390; i < 400; i += 30)
	{
		glVertex2f(i, -200);
		glVertex2f(i, -150);
	}
	glEnd();



	//end of rails

	glColor3f(1, 1, 1);
	glRectf(-400, -85, 400, -100);

	//waiting area

	glColor3f(128 / 255., 128 / 255., 128 / 255.);
	glRectf(-400, -30, 400, -85);

	//benchs

	for (int i = -330; i <= 330; i += 190)
	{
		glColor3f(51 / 255.0, 25 / 255., 0);
		glRectf(i, -50, i + 60, -60);

		glColor3f(0, 0, 0);
		glRectf(i + 10, -60, i + 20, -80);
		glRectf(i + 40, -60, i + 50, -80);
	}


	//First Train Tires
	glColor3f(r / 255., g / 255., b / 255.);
	circle(xR, yR, 25);
	circle(xR - 120, yR, 25);

	//First lower body

	glColor3f(153 / 255.0, 153 / 255., 0);
	glRectf(xR - 160, yR + 30, xR + 40, yR);

	//nose of lower body

	glColor3f(153 / 255.0, 153 / 255., 0);
	glBegin(GL_TRIANGLES);
	glVertex2f(xR + 40, yR);
	glVertex2f(xR + 40, yR + 30);
	glVertex2f(xR + 70, yR);
	glEnd();

	//First Cabinet
	glColor3f(255 / 255.0, 130 / 255.0, 4 / 255.);
	glRectf(xR - 150, yR + 140, xR - 30, yR + 30);

	//First window
	glColor3f(1, 1, 1);
	glRectf(xR - 140, yR + 125, xR - 40, yR + 55);

	//top of cabinet
	glColor3f(102 / 255., 51 / 255., 0 / 255.);
	glRectf(xR - 155, yR + 145, xR - 25, yR + 130);



	//first engine
	glColor3f(25 / 255., 51 / 255., 0);
	glRectf(xR - 30, yR + 90, xR + 40, yR + 30);

	//tren egzozu
	glColor3f(0, 76 / 255., 153 / 255.);
	glRectf(xR, yR + 125, xR + 30, yR + 90);

	//tren egzozunun ucu
	glColor3f(0 / 255., 102 / 255., 102 / 255.);
	glRectf(xR - 10, yR + 125, xR + 40, yR + 110);

	//stripes
	glColor3ub(0, 0, 0);
	glBegin(GL_LINES);
	glVertex2f(xR - 10, yR + 90);
	glVertex2f(xR - 10, yR + 30);

	glVertex2f(xR - 5, yR + 90);
	glVertex2f(xR - 5, yR + 30);

	glVertex2f(xR, yR + 90);
	glVertex2f(xR, yR + 30);

	glEnd();

	//connection rope

	glColor3ub(0, 0, 0);
	glBegin(GL_LINES);
	glVertex2f(xR - 150, yR + 85);
	glVertex2f(xR - 250, yR + 85);
	glVertex2f(xR - 250, yR + 65);
	glVertex2f(xR - 150, yR + 65);
	glEnd();

	/*SECOND			TRAIN*/

	//tires
	glColor3f(r / 255., g / 255., b / 255.);
	circle(xR - 470, yR, 25);
	circle(xR - 250, yR, 25);

	//train
	glColor3f(255, 102 / 255., 102 / 255.);
	glRectf(xR - 500, yR + 130, xR - 200, yR);

	//top of train
	glColor3f(102 / 255., 51 / 255., 0 / 255.);
	glRectf(xR - 505, yR + 135, xR - 195, yR + 115);

	//windows
	glColor3f(1, 1, 1);
	glRectf(xR - 495, yR + 110, xR - 365, yR + 30);

	glColor3f(1, 1, 1);
	glRectf(xR - 350, yR + 110, xR - 205, yR + 30);




	//to disappear

	//right side of the tv
	glColor3f(160 / 255.0, 160 / 255.0, 160 / 255.0);
	glRectf(400, 250, 420, -268);

	//right outline of the tv
	glColor3ub(128 / 255., 128 / 255., 128 / 255.);
	glBegin(GL_LINES);
	glVertex2f(420, 270);
	glVertex2f(420, -270);
	glEnd();

	//right outside of the tv
	glColor3f(180 / 255., 180 / 255., 160 / 255.);
	glRectf(420, 250, 700, -400);


	//left side of the tv
	glColor3f(160 / 255.0, 160 / 255.0, 160 / 255.0);
	glRectf(-420, 270, -400, -230);

	//left outside of the tv
	glColor3f(180 / 255., 180 / 255., 160 / 255.);
	glRectf(-700, 400, -420, -800);

	//left outline of the tv
	glColor3ub(128 / 255., 128 / 255., 128 / 255.);
	glBegin(GL_LINES);
	glVertex2f(-420, -270);
	glVertex2f(-420, 270);
	glEnd();

	//printing informations
	vprint(425, -250, GLUT_BITMAP_8_BY_13, "*Press 'p' to pause/unpause.");
	vprint(425, -270, GLUT_BITMAP_8_BY_13, "*Press spacebar to turn on/off the");
	vprint(425, -290, GLUT_BITMAP_8_BY_13, "screen.");
	vprint(425, -310, GLUT_BITMAP_8_BY_13, "*LClick inside of the screen to");
	vprint(425, -330, GLUT_BITMAP_8_BY_13, "change color of the wheels");
	vprint(425, -350, GLUT_BITMAP_8_BY_13, "*RClick into the sky to change");
	vprint(425, -370, GLUT_BITMAP_8_BY_13, "the position of the sun");



}
void displayOff() {
	glClearColor(180 / 255., 180 / 255., 160 / 255., 0);
	glClear(GL_COLOR_BUFFER_BIT);

	// outline of the tv
	glColor3f(160 / 255.0, 160 / 255.0, 160 / 255.0);
	glRectf(-420, 270, 420, -270);

	// inside of the tv
	glColor3f(0, 0, 0);
	glRectf(-400, 250, 400, -230);



	// power button
	glColor3f(0.3, 0.3, 1);
	glBegin(GL_LINE_STRIP);
	glVertex2f(-400, -235);
	glVertex2f(-340, -235);
	glVertex2f(-340, -265);
	glVertex2f(-400, -265);
	glVertex2f(-400, -235);
	glEnd();

	glColor3f(1, 0, 0);
	circle(-380, -250, 10);

	glColor3f(1, 1, 1);
	circle_wire(-380, -250, 7);
	circle_wire(-380, -250, 8);


	glBegin(GL_LINES);
	glColor3ub(255, 255, 255);
	glVertex2f(-380, -250);
	glVertex2f(-380, -240);
	glEnd();

	// restart button
	glColor3f(0.3, 0.3, 1);
	glBegin(GL_LINE_STRIP);
	glVertex2f(-200, -235);
	glVertex2f(-60, -235);
	glVertex2f(-60, -265);
	glVertex2f(-200, -265);
	glVertex2f(-200, -235);
	glEnd();
	vprint(-187, -253, GLUT_BITMAP_8_BY_13, "PAUSE / UNPAUSE");


	// outside line of the tv
	glColor3ub(128 / 255., 128 / 255., 128 / 255.);
	glBegin(GL_LINES);
	glVertex2f(-420, 270);
	glVertex2f(420, 270);

	glVertex2f(420, 270);
	glVertex2f(420, -270);

	glVertex2f(420, -270);
	glVertex2f(-420, -270);

	glVertex2f(-420, -270);
	glVertex2f(-420, 270);
	glEnd();


	//printing informations
	vprint(425, -250, GLUT_BITMAP_8_BY_13, "*Press 'p' to pause/unpause.");
	vprint(425, -270, GLUT_BITMAP_8_BY_13, "*Press spacebar to turn on/off the");
	vprint(425, -290, GLUT_BITMAP_8_BY_13, "screen.");
	vprint(425, -310, GLUT_BITMAP_8_BY_13, "*LClick inside of the screen to");
	vprint(425, -330, GLUT_BITMAP_8_BY_13, "change color of the wheels");
	vprint(425, -350, GLUT_BITMAP_8_BY_13, "*RClick into the sky to change");
	vprint(425, -370, GLUT_BITMAP_8_BY_13, "the position of the sun");

}

void display() {

	switch (ON) {
	case 1: displayOn(); break;
	case 0: displayOff(); break;
	}

	glutSwapBuffers();


}

//
// key function for ASCII charachters like ESC, a,b,c..,A,B,..Z
//
void onKeyDown(unsigned char key, int x, int y)
{
	// exit when ESC is pressed.
	if (key == 27)
		exit(0);

	// Start Game when the user presses spacebar in Menu.
	if (key == ' ' && ON == 1) {
		ON = 0;
	}
	else if (key == ' ' && ON == 0) {
		ON = 1;
	}


	if (key == 'p') {
		activeTimer = !activeTimer;
	}
	else if (key == 'p') {
		activeTimer = !activeTimer;
	}

	// to refresh the window it calls display() function
	glutPostRedisplay();

}

void onKeyUp(unsigned char key, int x, int y)
{
	// exit when ESC is pressed.
	if (key == 27)
		exit(0);

	// to refresh the window it calls display() function
	glutPostRedisplay();
}

//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyDown(int key, int x, int y)
{
	// Write your codes here.
	switch (key) {
	case GLUT_KEY_UP: up = true; break;
	case GLUT_KEY_DOWN: down = true; break;
	case GLUT_KEY_LEFT: left = true; break;
	case GLUT_KEY_RIGHT: right = true; break;
	}

	// to refresh the window it calls display() function
	glutPostRedisplay();
}

//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyUp(int key, int x, int y)
{
	// Write your codes here.
	switch (key) {
	case GLUT_KEY_UP: up = false; break;
	case GLUT_KEY_DOWN: down = false; break;
	case GLUT_KEY_LEFT: left = false; break;
	case GLUT_KEY_RIGHT: right = false; break;
	}

	// to refresh the window it calls display() function
	glutPostRedisplay();
}

//
// When a click occurs in the window,
// It provides which button
// buttons : GLUT_LEFT_BUTTON , GLUT_RIGHT_BUTTON
// states  : GLUT_UP , GLUT_DOWN
// x, y is the coordinate of the point that mouse clicked.
//
void onClick(int button, int stat, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && stat == GLUT_DOWN) {

		if ((x > 300 && y > 635) && (x < 360 && y < 665) && ON == 0)
			ON = 1;
		else if ((x > 300 && y > 635) && (x < 360 && y < 665) && ON == 1)
			ON = 0;
	}

	if (button == GLUT_LEFT_BUTTON && stat == GLUT_DOWN) {

		if ((x > 500 && y > 635) && (x < 640 && y < 660) && ON == 0)
			activeTimer = !activeTimer;
		else if ((x > 500 && y > 635) && (x < 640 && y < 665) && ON == 1)
			activeTimer = !activeTimer;
	}

	if (button == GLUT_LEFT_BUTTON && stat == GLUT_DOWN) {

		if ((x > 300 && y > 150) && (x < 1100 && y < 630))
		{
			r = rand() % 256;
			g = rand() % 256;
			b = rand() % 256;
		}
	}

	if (button == GLUT_RIGHT_BUTTON && stat == GLUT_DOWN)
	{
		if ((x > 330 && y > 180) && (x < 1070 && y < 400))
		{
			sunX = x - winWidth / 2;
			sunY = winHeight / 2 - y;
		}
	}

}

//
// This function is called when the window size changes.
// w : is the new width of the window in pixels.
// h : is the new height of the window in pixels.
//
void onResize(int w, int h)
{
	winWidth = w;
	winHeight = h;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-w / 2, w / 2, -h / 2, h / 2, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	display(); // refresh window.
}

void onMoveDown(int x, int y) {
	// Write your codes here.



	// to refresh the window it calls display() function   
	glutPostRedisplay();
}

// GLUT to OpenGL coordinate conversion:
//   x2 = x1 - winWidth / 2
//   y2 = winHeight / 2 - y1
void onMove(int x, int y) {
	// Write your codes here.



	// to refresh the window it calls display() function
	glutPostRedisplay();
}

#if TIMER_ON == 1
void onTimer(int v) {
	glutTimerFunc(TIMER_PERIOD, onTimer, 0);

	if (activeTimer) {
		// animate the current shape until it touches the edge of the window.

		if (xR - TRAIN_LENGHT < WINDOW_WIDTH / 2)
		{
			xR++;
		}

		if (xR - TRAIN_LENGHT >= WINDOW_WIDTH / 2)
			xR = -475; //xR's initial value

	}

	//to refresh the window it calls display() function
	glutPostRedisplay(); //display()

}
#endif

void Init() {

	// Smoothing shapes
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}

void main(int argc, char *argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	//glutInitWindowPosition(100, 100);
	glutCreateWindow("Moving Train");

	glutDisplayFunc(display);
	glutReshapeFunc(onResize);

	//
	// keyboard registration
	//
	glutKeyboardFunc(onKeyDown);
	glutSpecialFunc(onSpecialKeyDown);

	glutKeyboardUpFunc(onKeyUp);
	glutSpecialUpFunc(onSpecialKeyUp);

	//
	// mouse registration
	//
	glutMouseFunc(onClick);
	glutMotionFunc(onMoveDown);
	glutPassiveMotionFunc(onMove);

#if  TIMER_ON == 1
	// timer event
	glutTimerFunc(TIMER_PERIOD, onTimer, 0);
#endif

	Init();

	glutMainLoop();
}