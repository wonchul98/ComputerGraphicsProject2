#include"ObjLoader.h"
#include<GL/glut.h>
//#include<stdio.h>
#include<windows.h>
//#include<string.h>
#include<math.h>
//#include <GLAUX.H>

#define PI 3.1415926

GLfloat emission[] = { 0.0f, 0.0f, 0.0f , 1.0f };
GLfloat cube_emission[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat cylinder_emission[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat earth_emission[] = { 1.0, 1.0, 1.0, 1.0 };

GLuint texName[6];
GLuint texName2[3];
GLuint* texName3 = new GLuint();
GLuint tmp;

GLUquadricObj* qobj = gluNewQuadric();

void init(void);
void draw(void);

//void draw_triangle(void)
//void draw_polygon(void);
int mode = 0;
bool anti = false;

void draw_string(void* font, const char* str, float x, float y, float red, float green, float blue);
void idle(void);
void sub_menu_function(int option);
void main_menu_function(int option);
void mouse(int button, int state, int x, int y);
void keyboard(unsigned char key, int x, int y);
void motion(int x, int y);
void special_keyboard(int key, int x, int y);
void resize(int width, int height);

void draw_text();
void draw_axis(void);
void light_setting();

void anti_aliasing();


void print_instruction();
//bool spin_state = false;
//bool rotation = false;
//float spin = 0;
//int draw_mode = 0;
//float x_trans = 0;
//float y_trans = 0;

double radius = 20;
double theta = 45;
double phi = 45;
double cam[3] = { 0,0,0 };
double center[3] = { 0,0,0 };
double up[3] = { 0,1,0 };
float current_width = 0;
float current_height = 0;

float sun_angle = 0;
float earth_angle = 0;
float moon_angle = 0;
float mars_angle = 0;
float mars_moon_angle = 0;

float sun_angle_1 = 0;
float earth_angle_1 = 0;
float moon_angle_1 = 0;
float mars_angle_1 = 0;
float mars_moon_angle_1 = 0;

int main(int argc, char** argv) {

    int submenu1;
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(300, 300);
    glutCreateWindow("12171797_신원철");
    init(); //사용자 초기화 함수

    submenu1 = glutCreateMenu(sub_menu_function);
    glutAddMenuEntry("Draw", 1);
    glutCreateMenu(main_menu_function);
    glutAddMenuEntry("Quit", 999);
    glutAddMenuEntry("clear", 11);
    glutAddSubMenu("Shape", submenu1);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    gluQuadricDrawStyle(qobj, GLU_FILL); //수정
    gluQuadricNormals(qobj, GLU_FLAT); //수정
    glutDisplayFunc(&draw);
    glutMouseFunc(&mouse);
    glutMotionFunc(&motion);
    glutKeyboardFunc(&keyboard);
    glutSpecialFunc(&special_keyboard);
    glutIdleFunc(&idle);
    glutReshapeFunc(&resize);

    glutMainLoop();

    return 0;
}

void init(void)
{
    glutReshapeFunc(resize);
    glClearColor(0.5, 0.5, 0.5, 0.3);
    //glClear(GL_COLOR_BUFFER_BIT);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);

    light_setting();

    gluQuadricTexture(qobj, GL_TRUE);

    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glEnable(GL_TEXTURE_2D);
    //glClear(GL_COLOR_BUFFER_BIT);
    print_instruction();
}

void draw(void)
{
    //glColor3f(1.0f, 1.0f, 0.0f);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    //glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    cam[0] = radius * sin(phi * PI / 180) * sin(theta * PI / 180);
    cam[1] = radius * cos(theta * PI / 180);
    cam[2] = radius * sin(theta * PI / 180) * cos(phi * PI / 180);
    //up[1] = cos(phi * PI / 180);
    gluLookAt(cam[0], cam[1], cam[2], center[0], center[1], center[2], up[0], up[1], up[2]);
    anti_aliasing();
    //draw_text();
    if (mode == 4) {
        GLfloat light_position[] = { cam[0], cam[1], cam[2], 1.0 };
        glLightfv(GL_LIGHT0, GL_POSITION, light_position);
        glMaterialfv(GL_FRONT, GL_EMISSION, cube_emission);
    }

    //draw_axis();
    //draw_string(GLUT_BITMAP_TIMES_ROMAN_24, "graphics", 20, 50, 1, 1, 0);

    draw_text();
    glutSwapBuffers();
    if (mode == 4) {
        glMaterialfv(GL_FRONT, GL_EMISSION, emission);
    }
    //draw_text();

    // class 생성
    //model myLoader;

    // model load
    char a[12] = "gameboy.obj";
    char b[12] = "gameboy.mtl";
    //myLoader.Load(a, b);

    // draw
    //myLoader.draw();

    glFlush();
}

void idle(void)
{
    glutPostRedisplay();
}

void sub_menu_function(int option) {
    printf("Submenu %d has been selected\n", option);
    switch (option) {
    case 1:
        draw_axis();
        glFlush();
        glutPostRedisplay();
    }

}

void main_menu_function(int option) {
    printf("Main menu %d has been selected\n", option);
    if (option == 999) {
        exit(0);
    }
    if (option == 11) {
        glClear(GL_COLOR_BUFFER_BIT);
        glFlush();
    }
    glutPostRedisplay();
}

void mouse(int button, int state, int x, int y) {
    printf("mouse is clicked! (%d, %d, %d, %d)\n", button, state, x, y);

}

void keyboard(unsigned char key, int x, int y) {
    printf("you pressed %c\n", key);
    if (key == 'a') {
        anti = !anti;
    }
    if (key == 'o') {
        radius -= 1;
    }
    if (key == 'p') {
        radius += 1;
    }
    if (key == '0') {
        mode = 0;
    }
    if (key == '1') {
        mode = 1;
    }
    if (key == '2') {
        mode = 2;
    }
    if (key == '3') {
        mode = 3;
    }
    if (key == '4') {
        mode = 4;
    }

}

void special_keyboard(int key, int x, int y) {
    if (key == GLUT_KEY_UP) {
        printf("you clicked ↑\n");
        theta += 1;
        if (int(theta) % 180 == 0) {
            up[1] *= -1;
        }
        printf("theta: %f, phi : %f\n", theta, phi);
        glFlush();
    }
    else if (key == GLUT_KEY_DOWN) {
        printf("you clicked ↓\n");
        theta -= 1;
        if (int(theta) % 180 == 0) {
            up[1] *= -1;
        }
        printf("theta: %f, phi : %f\n", theta, phi);
        glFlush();
    }
    else if (key == GLUT_KEY_LEFT) {
        printf("you clicked ←\n");
        phi += 2;
        printf("theta: %f, phi : %f\n", theta, phi);
        glFlush();
    }
    else if (key == GLUT_KEY_RIGHT) {
        printf("you clicked →\n");
        phi -= 2;
        printf("theta: %f, phi : %f\n", theta, phi);
        glFlush();
    }
    glutPostRedisplay();
}

void resize(int width, int height)
{
    printf("resize\n");
    current_width = width;
    current_height = height;
    glViewport(0, 0, width, height);
    //glOrtho(width, width, height, height, width, width);
    //glFrustum(width, width, height, height, width, width);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, (float)width / (float)height, 1, 500);
    glMatrixMode(GL_MODELVIEW);
}

void draw_axis(void)
{

    glLineWidth(3); // 좌표축의 두
    //glBegin(GL_LINES);
    glColor3f(1, 0, 0); // X축은 red
    glBegin(GL_LINES);
    glVertex3f(0, 0, 0);
    glVertex3f(4, 0, 0);

    glColor3f(0, 1, 0); // Y축은 green
    glBegin(GL_LINES);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 4, 0);

    glColor3f(0, 0, 1); // Z축은 blue
    glBegin(GL_LINES);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, 4);

    glEnd();
    glLineWidth(1); // 두께 다시 환원
    //glClear(GL_COLOR_BUFFER_BIT);
}

void motion(int x, int y) {
    printf("Mouse is moving!(%d, %d)\n", x, y);
}


void light_setting() {
    GLfloat ambientLight[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    GLfloat diffuseLight[] = { 0.7f, 0.7f, 0.7f, 1.0f };
    GLfloat specularLight[] = { 0.9f, 0.9f, 0.9f, 1.0f };
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);

    GLfloat lmodel_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
    //glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
    //glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
    GLfloat ambient_sun[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    GLfloat diffuse_sun[] = { 0.0f, 0.0f, 0.0f, 0.0f };
    GLfloat specular_sun[] = { 0.9f, 0.9f, 0.9f, 1.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_sun);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_sun);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular_sun);
    glMaterialf(GL_FRONT, GL_SHININESS, 16);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);

}

void anti_aliasing() {
    if (anti) {
        glEnable(GL_POINT_SMOOTH);
        glEnable(GL_LINE_SMOOTH);
        glEnable(GL_POLYGON_SMOOTH);
    }

    else {
        glDisable(GL_POINT_SMOOTH);
        glDisable(GL_LINE_SMOOTH);
        glDisable(GL_POLYGON_SMOOTH);
    }
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
}

void print_instruction() {
    printf("-------instruction---------\n");
    printf("확대/축소: o/p\n");
    printf("시점 변경: 방향키\n");
    printf("광원 위치 변경: 0~4\n");
}

void draw_string(void* font, const char* str, float x, float y, float red, float green, float blue)
{
    glPushAttrib(GL_LIGHTING_BIT);
    glDisable(GL_LIGHTING);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(-10, 10, -10, 10);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glColor3f(red, green, blue);
    glRasterPos3f(x, y, 0);
    for (unsigned int i = 0; i < strlen(str); i++) {
        glutBitmapCharacter(font, str[i]);
    }
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopAttrib();
    //glFlush();
}

void draw_text() {
    glViewport(0, current_width / 2, current_width / 2, current_height / 2);
    glLoadIdentity();
    //glColor3f(1, 1, 0);
    draw_string(GLUT_BITMAP_TIMES_ROMAN_24, "graphics", 0, 0, 1, 1, 0);
    glFlush();
}
