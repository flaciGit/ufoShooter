#include <GL/glut.h>
#include <bevgrafmath2017.h>
#include <stdlib.h>
#include <time.h>
#include <list>
#define _USE_MATH_DEFINES
#include <math.h>  

GLfloat		windowWidth = 200;
GLfloat		windowHeight = 300;

GLfloat		turretWidth = 4;
GLint		turretLength = 20;
vec2		turretCoords[2] = { {windowWidth/2,6},{windowWidth/2,26}};
GLint		dotDistance = 13;
GLfloat		spin = 0.0;
GLfloat		angle = 0.0;

GLint		misslesLeftScreen = 0;

vec2		ufoCenter[2] = { {windowWidth/2,255}, {windowWidth/2,248} };
GLfloat		ufoSpeed = 2.0;
GLfloat		ufoMaxSpeed = 6.0;

GLfloat		ufoAcceleration = 0.1;
GLint		ufoTopWidth = 5;
GLint		ufoBottomWidth = 15;
GLint		ufoBottomHeight = 7;
GLfloat		ufoRedLevel = 0.0;
GLint		ufoLife = 10;
GLint		maxUfoLife = 10;

GLint		ammo = 56; //2 x 28
GLint		maxAmmo = 56;

GLint		turretSquareWidth = 20;
vec2		square[2] = { { windowWidth / 2 - turretSquareWidth / 2,0 },{ windowWidth / 2 + turretSquareWidth / 2,6 } };
vec2		turretlauncher = { 0,0 };

GLint		keyStates[256];

bool		freezeState = false;
GLint		goal = 0;

GLint		novX = 1;
GLint		novY = 1;

GLint		glutTimerFuncValue = 30;

GLint		pointSize = 5;

class Missle {
	GLfloat x1;
	GLfloat y1;
	GLfloat x2;
	GLfloat y2;

	public:
	Missle(GLfloat x, GLfloat y, GLfloat mx, GLfloat my);
	GLfloat getX1() {
		return x1;
	}
	GLfloat getY1() {
		return y1;
	}
	GLfloat getX2() {
		return x2;
	}
	GLfloat getY2() {
		return y2;
	}
	~Missle();
	void setCoords(GLfloat x, GLfloat y) {
		x1 = x;
		y1 = y;
	}
};

Missle::~Missle() {}
Missle::Missle(GLfloat x, GLfloat y, GLfloat mx, GLfloat my) {
	x1 = x;
	y1 = y;
	x2 = mx;
	y2 = my;
}

std::list<Missle> missleList;

void destroyAllMisslesInTheAir() {
	
	missleList.clear();
	printf("Missles in the air have been destroyed.\n");
}

void restartGame() {

	destroyAllMisslesInTheAir();
	turretLength =		20;
	turretCoords[0] =	{ windowWidth/2,6 };
	turretCoords[1] =	{ windowWidth/2, 26 };
	dotDistance =		13;
	spin =				0.0;
	angle =				0.0;
	misslesLeftScreen = 0;
	ufoCenter[0] =		{ windowWidth/2,255 };
	ufoCenter[1] =		{ windowWidth/2, 248 };

	ufoSpeed =			2;
	ufoTopWidth =		5;
	ufoBottomWidth =	15;
	ufoBottomHeight =	7;
	ufoRedLevel =		0.0;
	ufoLife =			maxUfoLife;

	ammo =				56;
	maxAmmo =			56;

	square[0] =			{ windowWidth / 2 - turretSquareWidth / 2,0 };
	square[1] =			{ windowWidth / 2 + turretSquareWidth / 2,6 };

	turretlauncher =	{ 0,0 };

	goal =				0;


	glutTimerFuncValue = 30;

	pointSize =			5;
	freezeState =		false;
}

void keyPressed(unsigned char key, int x, int y)
{
	keyStates[key] = 1;
}

void keyUp(unsigned char key, int x, int y)
{
	keyStates[key] = 0;
}

void keyOperations(int value)
{
	if (keyStates['w']) {
		freezeState = true;
		
		keyStates['w'] = 0;
	}

	glutPostRedisplay();

	glutTimerFunc(glutTimerFuncValue, keyOperations, 0);
}

void init()
{

	srand(time(NULL));
	glEnable(GL_POINT_SMOOTH);
	glPointSize(pointSize);

	glClearColor(1.0, 1.0, 1.0, 0.0);
	//glMatrixMode(GL_PROJECTION);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluOrtho2D(0.0, windowWidth, 0.0, windowHeight);
	glShadeModel(GL_FLAT);
}

void semicircle(GLfloat x, GLfloat y, GLdouble r, GLfloat red, GLfloat g, GLfloat b)
{
	glColor3f(red, g, b);
	glBegin(GL_TRIANGLE_FAN);
	for (GLdouble t = 0; t <= pi(); t += 0.01)
		glVertex2d(x + r * cos(t), y + r * sin(t));
	glEnd();
	glColor3f(0, 0, 0);
	glBegin(GL_LINE_STRIP);
	for (GLdouble t = 0; t <= pi(); t += 0.01)
		glVertex2d(x + r * cos(t), y + r * sin(t));
	glEnd();
}
void ellipse(GLfloat x, GLfloat y, GLdouble a, GLdouble b, GLfloat red, GLfloat g, GLfloat blue)
{
	glColor3f(red, g, blue);
	glBegin(GL_TRIANGLE_FAN);
	for (GLdouble t = 0; t <= 2 * pi(); t += 0.01)
		glVertex2d(x + a * cos(t), y + b * sin(t));
	glEnd();
	glColor3f(0, 0, 0);
	glBegin(GL_LINE_LOOP);
	for (GLdouble t = 0; t <= 2 * pi(); t += 0.01)
		glVertex2d(x + a * cos(t), y + b * sin(t));
	glEnd();
}
void circle(GLfloat x, GLfloat y, GLdouble r,GLfloat red, GLfloat g, GLfloat b) {

	glColor3f(red, g, b);
	glBegin(GL_TRIANGLE_FAN);
	for (GLdouble t = 0; t <= 2 * pi(); t += 0.01)
		glVertex2d(x + r * cos(t), y + r * sin(t));
	glEnd();
	glColor3f(0, 0, 0);
	glBegin(GL_LINE_LOOP);
	for (GLdouble t = 0; t <= 2 * pi(); t += 0.01)
		glVertex2d(x + r * cos(t), y + r * sin(t));

	glEnd();

}

void drawCannon() {
	
	glColor3f(0.0, 1.0, 0.0);
	glLineWidth(turretWidth);

	glBegin(GL_LINES);
	glVertex2d(turretCoords[0].x, turretCoords[0].y);
	glVertex2d(turretCoords[1].x, turretCoords[1].y);

	glEnd();
	glLineWidth(1);
}

bool damageUFO(GLfloat x, GLfloat y) {

	if (
		((x - ufoCenter[1].x)* (x - ufoCenter[1].x) / (ufoBottomWidth * ufoBottomWidth)) +
		((y - ufoCenter[1].y)* (y - ufoCenter[1].y) / (ufoBottomHeight * ufoBottomHeight)) - 1 <= 0

		|| (x - ufoCenter[0].x)*(x - ufoCenter[0].x) + (y - ufoCenter[0].y)* (y - ufoCenter[0].y) - ufoTopWidth * ufoTopWidth <= 0

		)
	{
		ufoLife--;
		printf("ufolife: %d \n", ufoLife);
		ufoRedLevel += 0.1;
		return true;
	}

	return false;
}

void flyTheMissles() {

	std::list<Missle>::iterator it = missleList.begin();
	if(!freezeState)
		while ( it != missleList.end()){
			if (freezeState)
				break;

			if (ufoLife > 0 && misslesLeftScreen < maxAmmo)
				it->setCoords(it->getX1() + it->getX2(), it->getY1() + it->getY2());

			circle(it->getX1(), it->getY1(), 2, 1, 0, 0);

			if (it->getX1() < 0 || it->getX1() > windowWidth || it->getY1() < 0 || it->getY1() > windowHeight
				|| damageUFO(it->getX1(), it->getY1())

				) {
				misslesLeftScreen++;
				it = missleList.erase(it);
				printf("misslesLeftScreen: %d \n", misslesLeftScreen);
			}
			else {
				if (freezeState == false) {
					it++;
				}
			}
		}
}


void display()
{

	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(1.0, 1.0, 1.0, 0.0);
	if (ufoLife <= 0) {
		glClearColor(0.0, 1.0, 0.0, 0.0);
		ufoSpeed = 0;
	}
	if (misslesLeftScreen >= maxAmmo) {
		glClearColor(1.0, 0.0, 0.0, 0.0);
		ufoSpeed = 0;
	}
	
	glColor3f(0.0, 0.0, 1.0);
	glRecti(square[0].x, square[0].y, square[1].x, square[1].y);

	ufoCenter[0].x += ufoSpeed;
	ufoCenter[1].x += ufoSpeed;

	if (ufoCenter[1].x + ufoBottomWidth + ufoSpeed >= windowWidth)
	{
		ufoSpeed = abs(ufoSpeed) > ufoMaxSpeed ? ufoSpeed : ufoSpeed + ufoAcceleration;
		ufoSpeed *= -1;
		ufoCenter[1].x = windowWidth - ufoBottomWidth - ufoSpeed;
		ufoCenter[0].x = windowWidth - ufoBottomWidth - ufoSpeed;
		printf("ufoSpeed: %g \n", ufoSpeed);

	}
	if (ufoCenter[1].x - ufoBottomWidth - ufoSpeed <= 0)
	{
		ufoSpeed = abs(ufoSpeed) > ufoMaxSpeed ? ufoSpeed : ufoSpeed - ufoAcceleration;
		ufoSpeed *= -1;
		ufoCenter[1].x = ufoBottomWidth + ufoSpeed;
		ufoCenter[0].x = ufoBottomWidth + ufoSpeed;
		printf("ufoSpeed: %g \n",ufoSpeed);
	}

	GLint leftAmmo = (maxAmmo / 2) - (maxAmmo - ammo);
	glColor3f(0.0, 0.0, 0.0);
	GLfloat circleWidth = 5;
	GLfloat y = circleWidth;
	GLint t = 7;
	GLint count = 0;
	for (GLfloat i = 1; i<8; i++) {
		for (GLfloat x = circleWidth; x <= t * 2 * circleWidth - circleWidth; x += 2 * circleWidth) {
			if (count < leftAmmo)
				circle(x, y, circleWidth, 0, 0, 0);
			count++;
		}

		y += 2 * circleWidth;
		t--;

	}

	y = circleWidth;
	t = 7;
	count = 0;
	for (GLfloat i = 1; i<8; i++) {
		for (GLfloat x = windowWidth - circleWidth; x >= windowWidth - (t * 2 * circleWidth - circleWidth); x -= 2 * circleWidth) {
			if (count < ammo)
				circle(x, y, circleWidth, 0, 0, 0);
			count++;
		}

		y += 2 * circleWidth;
		t--;

	}
	semicircle(ufoCenter[0].x, ufoCenter[0].y, ufoTopWidth, 1, 1- ufoRedLevel, 1- ufoRedLevel);
	ellipse(ufoCenter[1].x, ufoCenter[1].y, ufoBottomWidth, ufoBottomHeight, 1, 1- ufoRedLevel, 1- ufoRedLevel);
	drawCannon();

	if (freezeState == false)
		flyTheMissles();
	else
		restartGame();

	glutSwapBuffers();
	
}

void update(int n)
{

	glutPostRedisplay();

	glutTimerFunc(glutTimerFuncValue, update, 0);

}

void processMouse(GLint button, GLint action, GLint xMouse, GLint yMouse) {
	if (button == GLUT_LEFT_BUTTON && action == GLUT_DOWN)
		if (ammo > 0) {

			vec2 mouseVec = { xMouse - turretCoords[0].x,windowHeight - yMouse - turretCoords[0].y };
			GLfloat n = sqrt(mouseVec.x*mouseVec.x + mouseVec.y* mouseVec.y);
			vec2 mouseVecNorm = { mouseVec.x / n, mouseVec.y / n };
			turretCoords[1] = { mouseVecNorm.x * turretLength + turretCoords[0].x , mouseVecNorm.y * turretLength + turretCoords[0].y };

			Missle m(turretCoords[1].x, turretCoords[1].y, mouseVecNorm.x, mouseVecNorm.y);
			missleList.insert(missleList.begin(), m);
			ammo--;
		}
			
}


void processMouseActiveMotion(GLint xMouse, GLint yMouse) {
	vec2 mouseVec = { xMouse- turretCoords[0].x,windowHeight-yMouse- turretCoords[0].y };
	GLfloat n = sqrt(mouseVec.x*mouseVec.x + mouseVec.y* mouseVec.y);
	vec2 mouseVecNorm = { mouseVec.x / n, mouseVec.y / n };
	turretCoords[1] = { mouseVecNorm.x * turretLength + turretCoords[0].x , mouseVecNorm.y * turretLength + turretCoords[0].y };
}


int main(int argc, char** argv)
{

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(windowWidth, windowHeight);
	glutInitWindowPosition(100, 100);
	glutCreateWindow(argv[0]);
	init();
	glutDisplayFunc(display);
	glutTimerFunc(0, update, 0);
	glutMouseFunc(processMouse);
	glutPassiveMotionFunc(processMouseActiveMotion);
	glutTimerFunc(0, keyOperations, 0);
	glutIgnoreKeyRepeat(1);
	glutKeyboardFunc(keyPressed);
	glutKeyboardUpFunc(keyUp);
	glutMainLoop();
	return 0;
}
