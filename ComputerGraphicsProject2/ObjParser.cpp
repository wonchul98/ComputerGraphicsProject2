#include <GL/glut.h>
#include <GL/freeglut.h>
#include "ObjParser.h"
#include "bmpfuncs.h"

#pragma warning(disable:4996)

#define M_PI 3.1415926535897
using std::cos;
using std::sin;
using std::sqrt;

// global variable for counting fps
volatile int frame = 0, time1, timebase = 0;
volatile float fps;

/* texture mapping set variable */
GLuint textureMonkey;
GLuint texturefront;
GLuint textureback;
GLuint texturefrontboard;
GLuint texturebackboard;
GLuint texturescreen;
GLuint textureabutton;
GLuint texturebbutton;
GLuint textureselectbutton;
GLuint texturestartbutton;
GLuint texturepack;
GLuint texturebattery;
GLuint texturedirect;
GLuint texName[6];

GLfloat x;
GLfloat dx = 0.2;

bool antialiase_on = true;
double radius = 10;
double theta = 45, phi = 45;
double cam[3];
double center[3] = { 0, 0, 0 };
double up[3] = { 0, 1, 0 };
int draw_number = 0;
int move_mode = 0;
int click = 0;
bool pack_motion = 0;

// object var
ObjParser *monkey;
ObjParser* body_back;
ObjParser* body_front;
ObjParser* front_board;
ObjParser* back_board;
ObjParser* abutton;
ObjParser* bbutton;
ObjParser* selectbutton;
ObjParser* startbutton;
ObjParser* pack;
ObjParser* battery;
ObjParser *screen;
ObjParser* direct;

// user-defined function
void init(void);
void light_default();
void add_menu();
void get_resource(const char* str);
void mouse(int, int, int, int);
void mouseWheel(int, int, int, int);
void motion(int, int);
void passiveMotion(int, int);
void keyboard(unsigned char, int, int);
void special_keyboard(int, int, int);
void draw(void);
void resize(int, int);
void idle();
//...

GLUquadric *quadric;

/* Main method */
int main(int argc, char** argv)
{
	// glut initialize
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(250, 250);
	glutCreateWindow("GLUT Test");	// 위의 순서 무조건 고정

	// 사용자 초기화 함수
	init();
	quadric = gluNewQuadric();


	// pop-up 메뉴 등록 함수
	add_menu();

	// 리소스 로드
	body_front = new ObjParser("obj/frontbody.obj");
	body_back = new ObjParser("obj/real_body_back_rec.obj");
	front_board = new ObjParser("obj/front_board.obj");
	back_board = new ObjParser("obj/back_board.obj");
	battery = new ObjParser("obj/battery.obj");
	screen = new ObjParser("obj/screen.obj");
	abutton = new ObjParser("obj/button.obj");
	bbutton = new ObjParser("obj/button.obj");
	startbutton = new ObjParser("obj/select.obj");
	selectbutton = new ObjParser("obj/select.obj");
	pack = new ObjParser("obj/pack.obj");
	direct = new ObjParser("obj/direct.obj");

	body_back->write("body_back_test.obj");
	body_front->write("body_front_test.obj");
	/* Create a single window with a keyboard and display callback */
	glutMouseFunc(&mouse);
	glutMouseWheelFunc(&mouseWheel);
	glutMotionFunc(&motion);
	glutPassiveMotionFunc(&passiveMotion);
	glutKeyboardFunc(&keyboard);
	glutSpecialFunc(&special_keyboard);
	glutDisplayFunc(&draw);
	glutReshapeFunc(&resize);

	glutIdleFunc(&idle);

	/* Run the GLUT event loop */
	glutMainLoop();

	return EXIT_SUCCESS;
}

void light_default() {
	glClearColor(0.5f, 0.5f, 0.5f, 0.3f);

	/* Light0 조명 관련 설정 */
	GLfloat ambientLight[] = { 0.3f, 0.3f, 0.3f, 1.0f };
	GLfloat diffuseLight[] = { 0.9f, 0.9f, 0.9f, 1.0f };
	GLfloat specularLight[] = { 0.5f, 0.5f, 0.9f, 1.0f };
	GLfloat light_position[] = { 0.0, 0.0, 0.0, 1.0 };

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);

	/********* light point position setting **********/
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	/* Light1 조명 관련 설정 */
	GLfloat ambientLight1[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat diffuseLight1[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	GLfloat specularLight1[] = { 0.9f, 0.9f, 0.9f, 1.0f };
	GLfloat light_position1[] = { 0.0, 0.0, 0.0, 1.0 };

	glLightfv(GL_LIGHT1, GL_AMBIENT, ambientLight1);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuseLight1);
	glLightfv(GL_LIGHT1, GL_SPECULAR, specularLight1);

	/********* light point position setting **********/
	glLightfv(GL_LIGHT1, GL_POSITION, light_position1);

	/************* spot position setting *************/
	/*GLfloat spot_direction[] = { 0.0, 0.0, 0.0, 1.0 };
	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 45.0);
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spot_direction);
	glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 2.0);*/

	GLfloat specularMaterial[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	GLfloat diffuseMaterial[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat ambientMaterial[] = { 0.2f, 0.2f, 0.2f, 1.0f };

	/************* Material  setting *************/
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambientMaterial);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuseMaterial);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specularMaterial);
	glMaterialf(GL_FRONT, GL_SHININESS, 30);


	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glDisable(GL_COLOR_MATERIAL);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);

	/* DEPTH TEST ENABLE */
	glFrontFace(GL_CW);	// CW CCW바꿔보면서 front face 변경해보기!
}

void setTextureMapping() {
	int imgWidth, imgHeight, channels;
	
	//gluBuild2DMipmaps(GL_TEXTURE_2D, 3, imgWidth, imgHeight, GL_RGB, GL_UNSIGNED_BYTE, img);

	int texNum = 1;
	uchar* img = readImageData("bmp/bodyfront.bmp", &imgWidth, &imgHeight, &channels);
	glGenTextures(texNum, &texturefront);
	glBindTexture(GL_TEXTURE_2D, texturefront);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, imgWidth, imgHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	//GL_REPEAT 둘중 하나 선택
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//texNum = 2;
	img = readImageData("bmp/front_back_rec_uv.bmp", &imgWidth, &imgHeight, &channels);
	glGenTextures(texNum, &textureback);
	glBindTexture(GL_TEXTURE_2D, textureback);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, imgWidth, imgHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	//GL_REPEAT 둘중 하나 선택
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//texNum = 3;
	img = readImageData("bmp/front_board_uv.bmp", &imgWidth, &imgHeight, &channels);
	glGenTextures(texNum, &texturefrontboard);
	glBindTexture(GL_TEXTURE_2D, texturefrontboard);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, imgWidth, imgHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	//GL_REPEAT 둘중 하나 선택
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//texNum = 4;
	img = readImageData("bmp/back_board_uv.bmp", &imgWidth, &imgHeight, &channels);
	glGenTextures(texNum, &texturebackboard);
	glBindTexture(GL_TEXTURE_2D, texturebackboard);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, imgWidth, imgHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	//GL_REPEAT 둘중 하나 선택
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//texNum = 5;
	img = readImageData("bmp/4battery.bmp", &imgWidth, &imgHeight, &channels);
	glGenTextures(texNum, &texturebattery);
	glBindTexture(GL_TEXTURE_2D, texturebattery);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, imgWidth, imgHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	//GL_REPEAT 둘중 하나 선택
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//texNum = 6;
	img = readImageData("bmp/screen.bmp", &imgWidth, &imgHeight, &channels);
	glGenTextures(texNum, &texturescreen);
	glBindTexture(GL_TEXTURE_2D, texturescreen);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, imgWidth, imgHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	//GL_REPEAT 둘중 하나 선택
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//texNum = 7;
	img = readImageData("bmp/button.bmp", &imgWidth, &imgHeight, &channels);
	glGenTextures(texNum, &textureabutton);
	glBindTexture(GL_TEXTURE_2D, textureabutton);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, imgWidth, imgHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	//GL_REPEAT 둘중 하나 선택
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//texNum = 8;
	img = readImageData("bmp/button.bmp", &imgWidth, &imgHeight, &channels);
	glGenTextures(texNum, &texturebbutton);
	glBindTexture(GL_TEXTURE_2D, texturebbutton);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, imgWidth, imgHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	//GL_REPEAT 둘중 하나 선택
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//texNum = 9;
	img = readImageData("bmp/select.bmp", &imgWidth, &imgHeight, &channels);
	glGenTextures(texNum, &textureselectbutton);
	glBindTexture(GL_TEXTURE_2D, textureselectbutton);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, imgWidth, imgHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	//GL_REPEAT 둘중 하나 선택
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	img = readImageData("bmp/select.bmp", &imgWidth, &imgHeight, &channels);
	glGenTextures(texNum, &texturestartbutton);
	glBindTexture(GL_TEXTURE_2D, texturestartbutton);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, imgWidth, imgHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	//GL_REPEAT 둘중 하나 선택
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	img = readImageData("bmp/pack.bmp", &imgWidth, &imgHeight, &channels);
	glGenTextures(texNum, &texturepack);
	glBindTexture(GL_TEXTURE_2D, texturepack);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, imgWidth, imgHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	//GL_REPEAT 둘중 하나 선택
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	img = readImageData("bmp/dirkey.bmp", &imgWidth, &imgHeight, &channels);
	glGenTextures(texNum, &texturedirect);
	glBindTexture(GL_TEXTURE_2D, texturedirect);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, imgWidth, imgHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	//GL_REPEAT 둘중 하나 선택
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

}

void init()
{
	printf("init func called\n");
	// clear background color
	glClearColor(0.5f, 0.5f, 0.0f, 0.3f);

	//glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();
	//gluOrtho2D(0, 500, 500, 0);

	// set blend
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	// set antialiasing
	glHint(GL_POLYGON_SMOOTH_HINT, GL_FASTEST);
	glEnable(GL_POLYGON_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_FASTEST);
	glEnable(GL_LINE_SMOOTH);
		
	light_default();

	/* TEXTURE MAPPING SET */

	glEnable(GL_TEXTURE_2D);
	setTextureMapping();
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); //GL_REPLACE : polygon의 원래 색상은 무시하고 texture로 덮음
}

void special_keyboard(int key, int x, int y)
{
	if (key == GLUT_KEY_LEFT) {
		phi -= 5;
		if (phi < 0) phi = 355;
	}
	else if (key == GLUT_KEY_RIGHT) {
		phi += 5;
		if (phi >= 360) phi = 0;
	}
	else if (key == GLUT_KEY_UP) {
		if(theta > 10) theta -= 5;
	}
	else if (key == GLUT_KEY_DOWN) {
		if(theta < 170) theta += 5;
	}

	std::cout << "theta : " << theta << ", phi : " << phi << "\n";
	glutPostRedisplay();
}

/* Keyboard callback function */
void keyboard(unsigned char key, int x, int y)
{
	printf("you pressed %c\n", key);
	switch (key)
	{
		/* Exit on escape key press */
	case '\x1B':
	{
		exit(EXIT_SUCCESS);
		break;
	}
	case 'q':
	{
		if (antialiase_on) {
			antialiase_on = false;
			glDisable(GL_POLYGON_SMOOTH);
		}
		else
		{
			antialiase_on = true;
			glEnable(GL_POLYGON_SMOOTH);
		}
		break;
	}
	case '0':
	{
		move_mode = 0;
		printf("reset move mode\n");
		break;
	}
	case '1':
	{
		move_mode = 1;
		printf("show front body\n");
		break;
	}
	case '2':
	{
		move_mode = 2;
		printf("show back body\n");
		break;
	}
	case '3':
	{
		move_mode = 3;
		printf("show front board\n");
		break;
	}
	case '4':
	{
		move_mode = 4;
		printf("show back board\n");
		break;
	}
	case '5':
	{
		move_mode = 5;
		printf("show battery\n");
		break;
	}
	case '6':
	{
		move_mode = 6;
		printf("show screen\n");
		break;
	}
	case '7':
	{
		move_mode = 7;
		printf("show A, B button\n");
		break;
	}
	case '8':
	{
		move_mode = 8;
		printf("show start, select button\n");
		break;
	}
	case '9':
	{
		move_mode = 9;
		printf("show pack\n");
		break;
	}
	case 'w':
	{
		click = 1;
		break;
	}
	case 'a':
	{
		click = 2;
		break;
	}
	case 's':
	{
		click = 3;
		break;
	}
	case 'd':
	{
		click = 4;
		break;
	}
	case 'z':
	{
		click = 0;
		break;
	}
	case 'k':
	{
		click = 5;
		break;
	}
	case 'l':
	{
		click = 6;
		break;
	}
	case 'n':
	{
		click = 7;
		break;
	}
	case 'm':
	{
		click = 8;
		break;
	}
	case 'p':
	{
		x = 1;
		pack_motion = !pack_motion;
	}

	
	}
	glutPostRedisplay();
}

void draw_axis(void)
{
	glLineWidth(1.5f);
	glBegin(GL_LINES);

	glColor4f(1.f, 0.f, 0.f, 1.f);
	glVertex3f(0.f, 0.f, 0.f);
	glVertex3f(4.f, 0.f, 0.f);

	glColor4f(0.f, 1.f, 0.f, 1.f);
	glVertex3f(0.f, 0.f, 0.f);
	glVertex3f(0.f, 4.f, 0.f);

	glColor4f(0.f, 0.f, 1.f, 1.f);
	glVertex3f(0.f, 0.f, 0.f);
	glVertex3f(0.f, 0.f, 4.f);

	glEnd();
	glLineWidth(1);
}

void draw_obj(ObjParser *objParser)
{
	
	glBegin(GL_TRIANGLES);
	for (unsigned int n = 0; n < objParser->getFaceSize(); n += 3) {
		glNormal3f(objParser->normal[objParser->normalIdx[n] - 1].x,
			objParser->normal[objParser->normalIdx[n] - 1].y,
			objParser->normal[objParser->normalIdx[n] - 1].z);
		glVertex3f(objParser->vertices[objParser->vertexIdx[n] - 1].x,
			objParser->vertices[objParser->vertexIdx[n] - 1].y,
			objParser->vertices[objParser->vertexIdx[n] - 1].z);

			glNormal3f(objParser->normal[objParser->normalIdx[n + 1] - 1].x,
			objParser->normal[objParser->normalIdx[n + 1] - 1].y,
			objParser->normal[objParser->normalIdx[n + 1] - 1].z);
		glVertex3f(objParser->vertices[objParser->vertexIdx[n + 1] - 1].x,
			objParser->vertices[objParser->vertexIdx[n + 1] - 1].y,
			objParser->vertices[objParser->vertexIdx[n + 1] - 1].z);

		glNormal3f(objParser->normal[objParser->normalIdx[n + 2] - 1].x,
			objParser->normal[objParser->normalIdx[n + 2] - 1].y,
			objParser->normal[objParser->normalIdx[n + 2] - 1].z);
		glVertex3f(objParser->vertices[objParser->vertexIdx[n + 2] - 1].x,
			objParser->vertices[objParser->vertexIdx[n + 2] - 1].y,
			objParser->vertices[objParser->vertexIdx[n + 2] - 1].z);
	}
	
	glEnd();
}

void draw_obj_with_texture(ObjParser *objParser)
{
	glDisable(GL_BLEND);
	// glEnable(GL_TEXTURE_2D);	// texture 색 보존을 위한 enable
	if (draw_number == 0) {
		glBindTexture(GL_TEXTURE_2D, texturefront);
	}
	if (draw_number == 1){
		glBindTexture(GL_TEXTURE_2D, textureback);
	}
	if (draw_number == 2) {
		glBindTexture(GL_TEXTURE_2D, texturefrontboard);
	}
	if (draw_number == 3) {
		glBindTexture(GL_TEXTURE_2D, texturebackboard);
	}
	if (draw_number == 4) {
		glBindTexture(GL_TEXTURE_2D, texturebattery);
	}
	if (draw_number == 5) {
		glBindTexture(GL_TEXTURE_2D, texturescreen);
	}
	if (draw_number == 6) {
		glBindTexture(GL_TEXTURE_2D, textureabutton);
	}
	if (draw_number == 7) {
		glBindTexture(GL_TEXTURE_2D, texturebbutton);
	}
	if (draw_number == 8) {
		glBindTexture(GL_TEXTURE_2D, textureselectbutton);
	}
	if (draw_number == 9) {
		glBindTexture(GL_TEXTURE_2D, texturestartbutton);
	}
	if (draw_number == 10) {
		glBindTexture(GL_TEXTURE_2D, texturepack);
	}
	if (draw_number == 11) {
		glBindTexture(GL_TEXTURE_2D, texturedirect);
	}
	
	glBegin(GL_TRIANGLES);
	for (unsigned int n = 0; n < objParser->getFaceSize(); n += 3) {
		glTexCoord2f(objParser->textures[objParser->textureIdx[n] - 1].x,
			objParser->textures[objParser->textureIdx[n] - 1].y);
		glNormal3f(objParser->normal[objParser->normalIdx[n] - 1].x,
			objParser->normal[objParser->normalIdx[n] - 1].y,
			objParser->normal[objParser->normalIdx[n] - 1].z);
		glVertex3f(objParser->vertices[objParser->vertexIdx[n] - 1].x,
			objParser->vertices[objParser->vertexIdx[n] - 1].y,
			objParser->vertices[objParser->vertexIdx[n] - 1].z);

		glTexCoord2f(objParser->textures[objParser->textureIdx[n + 1] - 1].x,
			objParser->textures[objParser->textureIdx[n + 1] - 1].y);
		glNormal3f(objParser->normal[objParser->normalIdx[n + 1] - 1].x,
			objParser->normal[objParser->normalIdx[n + 1] - 1].y,
			objParser->normal[objParser->normalIdx[n + 1] - 1].z);
		glVertex3f(objParser->vertices[objParser->vertexIdx[n + 1] - 1].x,
			objParser->vertices[objParser->vertexIdx[n + 1] - 1].y,
			objParser->vertices[objParser->vertexIdx[n + 1] - 1].z);

		glTexCoord2f(objParser->textures[objParser->textureIdx[n + 2] - 1].x,
			objParser->textures[objParser->textureIdx[n + 2] - 1].y);
		glNormal3f(objParser->normal[objParser->normalIdx[n + 2] - 1].x,
			objParser->normal[objParser->normalIdx[n + 2] - 1].y,
			objParser->normal[objParser->normalIdx[n + 2] - 1].z);
		glVertex3f(objParser->vertices[objParser->vertexIdx[n + 2] - 1].x,
			objParser->vertices[objParser->vertexIdx[n + 2] - 1].y,
			objParser->vertices[objParser->vertexIdx[n + 2] - 1].z);
	}
	glEnd();
	
	glEnable(GL_BLEND);
}

void draw_cube_textures()
{
	int size = 2;
	glBindTexture(GL_TEXTURE_2D, textureMonkey);
	glBegin(GL_QUADS);
	//Quad 1
	glNormal3f(1.0f, 0.0f, 0.0f);   //N1
	glTexCoord2f(0.0f, 1.0f); glVertex3f(size / 2, size / 2, size / 2);   //V2
	glTexCoord2f(0.0f, 0.0f); glVertex3f(size / 2, -size / 2, size / 2);   //V1
	glTexCoord2f(1.0f, 0.0f); glVertex3f(size / 2, -size / 2, -size / 2);   //V3
	glTexCoord2f(1.0f, 1.0f); glVertex3f(size / 2, size / 2, -size / 2);   //V4
  //Quad 2
	glNormal3f(0.0f, 0.0f, -1.0f);  //N2
	glTexCoord2f(0.0f, 1.0f); glVertex3f(size / 2, size / 2, -size / 2);   //V4
	glTexCoord2f(0.0f, 0.0f); glVertex3f(size / 2, -size / 2, -size / 2);   //V3
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-size / 2, -size / 2, -size / 2);   //V5
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-size / 2, size / 2, -size / 2);   //V6
  //Quad 3
	glNormal3f(-1.0f, 0.0f, 0.0f);  //N3
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-size / 2, size / 2, -size / 2);   //V6
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-size / 2, -size / 2, -size / 2);   //V5
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-size / 2, -size / 2, size / 2);   //V7
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-size / 2, size / 2, size / 2);   //V8
  //Quad 4
	glNormal3f(0.0f, 0.0f, 1.0f);   //N4
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-size / 2, size / 2, size / 2);   //V8
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-size / 2, -size / 2, size / 2);   //V7
	glTexCoord2f(1.0f, 0.0f); glVertex3f(size / 2, -size / 2, size / 2);   //V1
	glTexCoord2f(1.0f, 1.0f); glVertex3f(size / 2, size / 2, size / 2);   //V2
  //Quad 5
	glNormal3f(0.0f, 1.0f, 0.0f);   //N5
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-size / 2, size / 2, -size / 2);   //V6
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-size / 2, size / 2, size / 2);   //V8
	glTexCoord2f(1.0f, 0.0f); glVertex3f(size / 2, size / 2, size / 2);   //V2
	glTexCoord2f(1.0f, 1.0f); glVertex3f(size / 2, size / 2, -size / 2);   //V4
  //Quad 6
	glNormal3f(1.0f, -1.0f, 0.0f);  //N6
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-size / 2, -size / 2, size / 2);   //V7
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-size / 2, -size / 2, -size / 2);   //V5
	glTexCoord2f(1.0f, 0.0f); glVertex3f(size / 2, -size / 2, -size / 2);   //V3
	glTexCoord2f(1.0f, 1.0f); glVertex3f(size / 2, -size / 2, size / 2);   //V1
	glEnd();
}

void draw_textureCube() {
	glColor3f(1.0, 1.0, 1.0);	// white로 color를 set해주어야 texture색상이 제대로 적용 됨!
	//glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
			// polygon의 원래 색상은 무시하고 texture로 덮음
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	// polygon의 원래 색상과 texture 색상을 곱하여 덮음, texture가 입혀진 표면에 광원 효과 설정 가능
//glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
		// texture의 색상으로 덮어 씌운다

	glBindTexture(GL_TEXTURE_2D, textureMonkey);
	glBegin(GL_QUADS);
	glNormal3f(-1.0, 0, 0);	// -x axis
	glTexCoord2f(0, 0); glVertex3f(-1.0, 1.0, -1.0);
	glTexCoord2f(1, 0); glVertex3f(-1.0, -1.0, -1.0);
	glTexCoord2f(1, 1); glVertex3f(-1.0, -1.0, 1.0);
	glTexCoord2f(0, 1); glVertex3f(-1.0, 1.0, 1.0);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, textureMonkey);
	glBegin(GL_QUADS);
	glNormal3f(1.0, 0, 0);	//x axis
	glTexCoord2f(0, 0); glVertex3f(1.0, 1.0, 1.0);
	glTexCoord2f(1, 0); glVertex3f(1.0, -1.0, 1.0);
	glTexCoord2f(1, 1); glVertex3f(1.0, -1.0, -1.0);
	glTexCoord2f(0, 1); glVertex3f(1.0, 1.0, -1.0);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, textureMonkey);
	glBegin(GL_QUADS);
	glNormal3f(0, -1.0, 0);	// -y axis
	glTexCoord2f(0, 0); glVertex3f(-1.0, -1.0, -1.0);
	glTexCoord2f(1, 0); glVertex3f(1.0, -1.0, -1.0);
	glTexCoord2f(1, 1); glVertex3f(1.0, -1.0, 1.0);
	glTexCoord2f(0, 1); glVertex3f(-1.0, -1.0, 1.0);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, textureMonkey);
	glBegin(GL_QUADS);
	glNormal3f(0, 1.0, 0);	// y axis
	glTexCoord2f(0, 0); glVertex3f(-1.0, 1.0, 1.0);
	glTexCoord2f(1, 0); glVertex3f(1.0, 1.0, 1.0);
	glTexCoord2f(1, 1); glVertex3f(1.0, 1.0, -1.0);
	glTexCoord2f(0, 1); glVertex3f(-1.0, 1.0, -1.0);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, textureMonkey);
	glBegin(GL_QUADS);
	glNormal3f(0, 0, 1.0);	//z axis
	glTexCoord2f(0, 0); glVertex3f(1.0, 1.0, 1.0);
	glTexCoord2f(1, 0); glVertex3f(-1.0, 1.0, 1.0);
	glTexCoord2f(1, 1); glVertex3f(-1.0, -1.0, 1.0);
	glTexCoord2f(0, 1); glVertex3f(1.0, -1.0, 1.0);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, textureMonkey);
	glBegin(GL_QUADS);
	glNormal3f(0, 0, -1.0);	//-z축
	glTexCoord2f(0, 0); glVertex3f(1.0, 1.0, -1.0);
	glTexCoord2f(1, 0); glVertex3f(-1.0, 1.0, -1.0);
	glTexCoord2f(1, 1); glVertex3f(-1.0, -1.0, -1.0);
	glTexCoord2f(0, 1); glVertex3f(1.0, -1.0, -1.0);
	glEnd();

	draw_axis();
}

/* Display callback function */
void draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glEnable(GL_DEPTH_TEST);

	cam[0] = radius * sin(theta*M_PI / 180)*sin(phi*M_PI / 180);
	cam[1] = radius * cos(theta*M_PI / 180);
	cam[2] = radius * sin(theta*M_PI / 180)*cos(phi*M_PI / 180);

	gluLookAt(cam[0], cam[1], cam[2], center[0], center[1], center[2], up[0], up[1], up[2]);

	//glDisable(GL_LIGHT1);
	GLfloat light_position[] = { cam[0],cam[1],cam[2],1.0 };
	GLfloat light_position2[] = { 0,0,0,1.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	draw_axis();
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);

	glPushMatrix();
	glColor3f(0.5f, 0.5f, 0.5f);
	
	GLfloat tmp[16];
	/*DRAW BODY_FRONT*/
	glPushMatrix();
	glLightfv(GL_LIGHT1, GL_POSITION, light_position2);
	glTranslatef(2, 0.4, 4.75); //고정
	if (move_mode == 1) {
		glTranslatef(1,1,1); //유동
	}
	draw_number = 0;
	draw_obj_with_texture(body_front);
	glPopMatrix();

	/*DRAW BODY_BACK*/
	glPushMatrix();
	glRotatef(180, 0, 1, 0);
	draw_number = 1;
	if (move_mode == 2) {
		glTranslatef(-1, 1, -1); //유동
	}
	draw_obj_with_texture(body_back);
	glPopMatrix();

	/*DRAW FRONT_BOARD*/
	glPushMatrix();
	glLightfv(GL_LIGHT1, GL_POSITION, light_position2);
	glTranslatef(1.7, 2.5, 2.75); //고정
	if (move_mode == 3) {
		glTranslatef(1, 1, 1); //유동
	}
	draw_number = 2;
	draw_obj_with_texture(front_board);
	glPopMatrix();

	/*DRAW BACK_BOARD*/
	glPushMatrix();
	//glLightfv(GL_LIGHT0, GL_POSITION, light_position2);
	glTranslatef(0.35, -0.25, 0.3);//고정
	if (move_mode == 4) {
		glTranslatef(1, 1, 1); //유동
	}
	draw_number = 3;
	draw_obj_with_texture(back_board);
	glPopMatrix();

	/*DRAW BATTERY*/
	glPushMatrix();
	//glLightfv(GL_LIGHT0, GL_POSITION, light_position2);
	draw_number = 4;
	glTranslatef(0.20, -0.15, 0);
	if (move_mode == 5) {
		glTranslatef(1, 1, 1); //유동
	}
	draw_obj_with_texture(battery);
	glPopMatrix();

	/*DRAW SCREEN*/
	glPushMatrix();
	draw_number = 5;
	glTranslatef(2.83, 3.52, -1.1);//고정
	if (move_mode == 6) {
		glTranslatef(1, 1, 1); //유동
	}
	draw_obj_with_texture(screen);
	glPopMatrix();

	/*DRAW ABUTTON*/
	glPushMatrix();
	draw_number = 6;
	glTranslatef(0.77, 0.77, 0.35);//고정
	if (click == 5) {
		glTranslatef(0, 0, -0.05);
	}
	if (move_mode == 7) {
		glTranslatef(1, 1, 1); //유동
	}
	draw_obj_with_texture(abutton);
	glPopMatrix();

	/*DRAW BBUTTON*/
	glPushMatrix();
	draw_number = 7;
	glTranslatef(0.925, 0.84, 0.35);//고정
	if (click == 6) {
		glTranslatef(0, 0, -0.05);
	}
	if (move_mode == 7) {
		glTranslatef(1, 1, 1); //유동
	}
	draw_obj_with_texture(bbutton);
	glPopMatrix();

	/*DRAW SELECTBUTTON*/
	glPushMatrix();
	draw_number = 8;
	glTranslatef(0.1, 1.1, 0.55);
	if (click == 8) {
		glTranslatef(0, 0, -0.05);
	}
	if (move_mode == 8) {
		glTranslatef(1, 1, 1); //유동
	}
	draw_obj_with_texture(selectbutton);
	glPopMatrix();

	/*DRAW STARTBUTTON*/
	glPushMatrix();
	draw_number = 9;
	glTranslatef(-0.05, 1.1, 0.55);
	if (click == 7) {
		glTranslatef(0, 0, -0.05);
	}
	if (move_mode == 8) {
		glTranslatef(1, 1, 1); //유동
	}
	draw_obj_with_texture(startbutton);
	glPopMatrix();
	/*DRAW PACK*/
	glPushMatrix();
	draw_number = 10;
	glRotatef(180, 0, 1, 0);
	glTranslatef(1.5, 0.05, 4.3);//고정
	if (pack_motion) {
		glTranslatef(0, x, 0);
	}
	if (move_mode == 9) {
		glTranslatef(-1, 1, -1); //유동
	}
	draw_obj_with_texture(pack);
	glPopMatrix();

	/*DRAW DIRECT*/
	glPushMatrix();
	draw_number = 11;
	//glRotatef(180, 0, 1, 0);
	glTranslatef(-0.25, -0.185, 0.28);
	if (move_mode == 7) {
		glTranslatef(1, 1, 1); //유동
	}
	if (click == 1) {
		glRotatef(-15, 1, 0, 0);
	}
	if (click == 2) {
		glRotatef(-15, 0, 1, 0);
	}
	if (click == 3) {
		glRotatef(15, 1, 0, 0);
	}
	if (click == 4) {
		glRotatef(15, 0, 1, 0);
	}
	
	draw_obj_with_texture(direct);
	glPopMatrix();
	
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-3.0f, 0.f, 0.f);
	glGetFloatv(GL_MODELVIEW_MATRIX, tmp);
	glPopMatrix();

	glPushMatrix();
	glLoadMatrixf(tmp);
	glPopMatrix();
	
	glutSwapBuffers();
	glFlush();
}

void mouse(int button, int state, int x, int y)
{
	if (state) {
		// printf("button(%d), state(%d), x(%d), y(%d)\n", button, state, x, y);
	}
	else {
		// printf("button(%d), state(%d), x(%d), y(%d)\n", button, state, x, y);
	}
	glutPostRedisplay();
}

void mouseWheel(int button, int dir, int x, int y)
{
	if (dir > 0) {
		// printf("button(%d), dir(%d), x(%d), y(%d)\n", button, dir, x, y);
		if (radius > 1) radius -= 0.5;
	}
	else {
		// printf("button(%d), dir(%d), x(%d), y(%d)\n", button, dir, x, y);
		if (radius < 100) radius += 0.5;
	}
	glutPostRedisplay();
}

void passiveMotion(int x, int y)
{
	//printf("Mouse movement x, y = (%d, %d)\n", x, y);
	glutPostRedisplay();
}

void motion(int x, int y)
{
	//printf("Mouse movement x, y = (%d, %d)\n", x, y);
	glutPostRedisplay();
}

void main_menu(int option)
{
	if (option == 99) exit(0);
	else if (option == 1) {
		radius = 10;
		theta = 45; phi = 45;
	}
	glutPostRedisplay();
}

void sub_menu(int option)
{
	printf("Submenu %d has been selected\n", option);
}

void add_menu()
{
	int mainmenu1 = glutCreateMenu(&main_menu);
	glutAddMenuEntry("Init", 1);
	glutAddMenuEntry("Quit", 99);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void idle()
{
	frame++;
	time1 = glutGet(GLUT_ELAPSED_TIME); // unit : ms

	if (time1 - timebase > 1000)
	{
		fps = frame * 1000.f / (time1 - timebase);
		timebase = time1;
		frame = 0;
		x = x - dx;
		if (x <= 0) {
			x = 1;
			if(pack_motion == 1) pack_motion = !pack_motion;
		}
		printf("x: %f\n", x);
		//printf("fps : %.0f\n", fps);
		glutPostRedisplay();
	}
}

void resize(int width, int height)
{
	printf("resize func called\n");
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//gluOrtho2D(0, 500, 0, 500);
	gluPerspective(60, (double)width / (double)height, 1, 1000);
	glMatrixMode(GL_MODELVIEW);
}

void envTextureMapping() {
	//GLuint g_nCubeTex;
	glGenTextures(1, &g_nCubeTex);

	glBindTexture(GL_TEXTURE_CUBE_MAP, g_nCubeTex);
	int sizeX, sizeY, data;
	glBindTexture(GL_TEXTURE_2D, g_nCubeTex);

	uchar* img0 = readImageData("img/256px.bmp", &sizeX, &sizeY, &data);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA, sizeX, sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, img0);

	uchar* img1 = readImageData("img/256nx.bmp", &sizeX, &sizeY, &data);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGBA, sizeX, sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, img1);

	uchar* img2 = readImageData("img/256ny.bmp", &sizeX, &sizeY, &data);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGBA, sizeX, sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, img2);

	uchar* img3 = readImageData("img/256py.bmp", &sizeX, &sizeY, &data);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA, sizeX, sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, img3);

	uchar* img4 = readImageData("img/256nz.bmp", &sizeX, &sizeY, &data);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGBA, sizeX, sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, img4);

	uchar* img5 = readImageData("img/256pz.bmp", &sizeX, &sizeY, &data);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA, sizeX, sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, img5);

	/*텍스처 속성 지정*/
	//glBindTexture(GL_TEXTURE_CUBE_MAP, g_nCubeTex);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

}
