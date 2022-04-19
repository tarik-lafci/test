/***
   CTIS164 - Template Source Program
----------
STUDENT : Tarik Lafci
SECTION : 1
HOMEWORK: 2
----------
PROBLEMS:
----------
ADDITIONAL FEATURES:
***/

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

#define WINDOW_WIDTH  1300
#define WINDOW_HEIGHT 700

#define TIMER_PERIOD  16 // Period for the timer.
#define TIMER_ON         1 // 0:disable timer, 1:enable timer

#define D2R 0.0174532

int weX = -600, weY = 100;
int buX = weX + 115, buY = weY + 55.0 / 2;
int nonRanY = -300;
int timer = 10;

/* Global Variables for Template File */
bool up = false, down = false, right = false, left = false;
int  winWidth, winHeight; // current Window width and height

typedef struct {
	double x, y;
	int r, g, b;
	bool display;
} object_t;

#define CIRCLES_MAX 5
object_t circles[CIRCLES_MAX];

bool activeTimer = false;
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
		angle = 2 * PI * i / 100;
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
		angle = 2 * PI * i / 100;
		glVertex2f(x + r * cos(angle), y + r * sin(angle));
	}
	glEnd();
}

void print(int x, int y, const char* string, void* font)
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
void vprint(int x, int y, void* font, const char* string, ...)
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
void vprint2(int x, int y, float size, const char* string, ...) {
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

void display_circle(object_t c) {
	if (c.display) {
		glColor3ub(c.r, c.g, c.b);
		circle(c.x, c.y, 25);
	}
}

//
// To display onto window using OpenGL commands
//
void display() {
	//
	// clear window to black
	//
	glClearColor(0.5, 0.5, 1, 0);
	glClear(GL_COLOR_BUFFER_BIT);

	//displaying a text
	vprint(-600, 300, GLUT_BITMAP_8_BY_13, "Tarik Lafci, “HOMEWORK #2");
	//body of a cannon

	glColor3f(0, 0, 102. / 255);
	glBegin(GL_QUADS);
	glVertex2f(weX - 20, weY + 50);
	glVertex2f(weX + 100, weY + 40);
	glVertex2f(weX + 100, weY + 15);
	glVertex2f(weX - 20, weY + 5);
	glEnd();

	//stripes of a body

	glColor3f(0, 0, 0);
	glLineWidth(2);
	glBegin(GL_LINES);

	glVertex2f(weX + 82, weY + 42);
	glVertex2f(weX + 82, weY + 14);

	glVertex2f(weX + 89, weY + 41);
	glVertex2f(weX + 89, weY + 15);

	glVertex2f(weX, weY - 25);
	glVertex2f(weX, weY + 25);
	glEnd();

	//wheel of a cannon
	glColor3f(102. / 255, 0, 51.0 / 255);
	circle(weX, weY, 25);
	glColor3f(0.3, 0.3, 1);

	//stripes of a wheel
	glColor3f(0, 0, 0);
	glLineWidth(2);
	glBegin(GL_LINES);

	glVertex2f(weX, weY - 25);
	glVertex2f(weX, weY + 25);

	glVertex2f(weX - 25, weY);
	glVertex2f(weX + 25, weY);

	glVertex2f(weX, weY - 25);
	glVertex2f(weX, weY + 25);
	glEnd();

	//cannon muzzle
	glColor3f(0, 0, 0);
	glBegin(GL_QUADS);
	glVertex2f(weX + 100, weY + 40);
	glVertex2f(weX + 110, weY + 45);
	glVertex2f(weX + 110, weY + 10);
	glVertex2f(weX + 100, weY + 15);
	glEnd();

	//cannon ball
	glColor3f(0, 0, 0);
	circle(buX, buY, 15);

	//target creation
	for (int i = 0; i < CIRCLES_MAX; i++) {
		display_circle(circles[i]);
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

	else if (key == ' ' && buX == weX + 115)
	{
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

	if (key == GLUT_KEY_DOWN && weY > -100 && !activeTimer)
		weY -= 10, buY -= 10;

	else if (key == GLUT_KEY_UP && weY < 200 && !activeTimer)
		weY += 10, buY += 10;
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

		//   x2 = x1 - winWidth / 2
//   y2 = winHeight / 2 - y1
		printf("Event : CLK %d %d\n", x - WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 - y);
	}


	// to refresh the window it calls display() function
	glutPostRedisplay();
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

bool collis(object_t c) {
	return (sqrt(powf(buX - c.x, 2) + powf(buY - c.y, 2)) < 40);
}

#if TIMER_ON == 1
void onTimer(int v) {

	glutTimerFunc(TIMER_PERIOD, onTimer, 0);
	// Write your codes here.
	if (activeTimer)
	{
		buX += 10, buY -= 1;
	}

	if (buX > 700)
	{
		buX = weX + 115, buY = weY + 55.0 / 2;
		activeTimer = !activeTimer;
	}

	for (int i = 0; i < CIRCLES_MAX; i++) {
		if (circles[i].display) {
			circles[i].y += 2;
		}
		if (circles[i].y > winHeight / 2. + 50)
			circles[i].y = -winHeight / 2. - 50;
		if (circles[i].display && collis(circles[i]))
			circles[i].display = false;
	}

	nonRanY++;
	timer--;



	// to refresh the window it calls display() function
	glutPostRedisplay(); // display()

}
#endif

void Init() {

	// Smoothing shapes
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	srand(time(NULL));
	for (int i = 0; i < CIRCLES_MAX; i++) {
		circles[i] = { WINDOW_WIDTH / 2. - 250 + rand() % 50, -WINDOW_HEIGHT / 2. - 150. * i, rand() % 255 , rand() % 255 , rand() % 255, true };
	}

}

void main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	//glutInitWindowPosition(100, 100);
	glutCreateWindow("Shooting Cannon");

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
