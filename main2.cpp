/**
 * 首先进行程序的主体设计，程序应包括绘制模块、逻辑控制模块、回调函数模块等。
 * 绘制模块主要实现图形的绘制等
 * 逻辑控制模块主要实现位置判定以及碰撞检测等
 * 回调函数模块主要实现利用键盘完成视角转换以及漫游场景等操作
 */

#include <gl/glut.h>
#include <stdio.h>
#include <cmath>

static float angle = 0.0, ratio;  // angle绕y轴的旋转角，ratio窗口高宽比
static float x = 0.0f, y = 1.75f, z = 5.0f;  //相机位置
static float lx = 0.0f, ly = 0.0f,
	     lz = -1.0f;  //视线方向，初始设为沿着Z轴负方向
static GLint snowman_display_list;  //雪人显示列表索引

/**
 * 定义观察方式
 */
void changeSize(int w, int h) {
    //除以0的情况
    if (h == 0) h = 1;
    ratio = 1.0f * w / h;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //设置视口为整个窗口大小
    glViewport(0, 0, w, h);
    //设置可视空间
    gluPerspective(45, ratio, 1, 1000);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(x, y, z, x + lx, y + ly, z + lz, 0.0f, 1.0f, 0.0f);
}

/**
 * 视野漫游函数
 */
void orientMe(float ang) {
    lx = sin(ang);
    lz = -cos(ang);
    glLoadIdentity();
    gluLookAt(x, y, z, x + lx, y + ly, z + lz, 0.0f, 1.0f, 0.0f);
}
void moveMeFlat(int direction) {
    x = x + direction * (lx)*0.1;
    z = z + direction * (lz)*0.1;
    glLoadIdentity();
    gluLookAt(x, y, z, x + lx, y + ly, z + lz, 0.0f, 1.0f, 0.0f);
}

/**
 * 加入按键控制
 */
void processSpecialKeys(int key, int x, int y) {
    switch (key) {
	case GLUT_KEY_LEFT:
	    angle -= 0.01f;
	    orientMe(angle);
	    break;
	case GLUT_KEY_RIGHT:
	    angle += 0.01f;
	    orientMe(angle);
	    break;
	case GLUT_KEY_UP:
	    moveMeFlat(1);
	    break;
	case GLUT_KEY_DOWN:
	    moveMeFlat(-1);
	    break;
    }
}

//定义显示列表，绘制雪人，初始化场景，渲染场景
void drawSnowMan() {
    glColor3f(1.0f, 1.0f, 1.0f);

    //画身体
    glTranslatef(0.0f, 0.75f, 0.0f);
    glutSolidSphere(0.75f, 20, 20);

    //画头
    glTranslatef(0.0f, 1.0f, 0.0f);
    glutSolidSphere(0.25f, 20, 20);

    //画眼睛           
    glPushMatrix();
    glColor3f(0.0f, 0.0f, 0.0f);
    glTranslatef(0.05f, 0.10f, 0.18f);
    glutSolidSphere(0.05f, 10, 10);
    glTranslatef(-0.1f, 0.0f, 0.0f);
    glutSolidSphere(0.05f, 10, 10);
    glPopMatrix();

    //画鼻子
    glColor3f(1.0f, 0.5f, 0.5f);
    glRotatef(0.0f, 1.0f, 0.0f, 0.0f);
    glutSolidCone(0.08f, 0.5f, 10, 2);
}

GLuint createDL() {
    GLuint snowManDL;
    //生成一个显示列表号
    snowManDL = glGenLists(1);
    //开始显示列表
    glNewList(snowManDL, GL_COMPILE);
    drawSnowMan();
    glEndList();
    return (snowManDL);
}

void myDisplay() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //画地面
    glBegin(GL_QUADS);
    glVertex3f(-100.0f, 0.0f, -100.0f);
    glVertex3f(-100.0f, 0.0f, 100.0f);
    glVertex3f(100.0f, 0.0f, 100.0f);
    glVertex3f(100.0f, 0.0f, -100.0f);
    glEnd();

    //画36个雪人
    for (int i = -3; i < 3; i++)
        for (int j = -3; j < 3; j++) {
            glPushMatrix();
            glTranslatef(i * 10.0, 0, j * 10.0);
            glCallList(snowman_display_list);
            glPopMatrix();
        }

    glutSwapBuffers();
}

int main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(1000, 1000);
    glutCreateWindow("雪人");  // 改了窗口标题

    glEnable(GL_DEPTH_TEST);
    snowman_display_list = createDL();
    glutDisplayFunc(&myDisplay);
    glutIdleFunc(&myDisplay);

    glutSpecialFunc(&processSpecialKeys);  // 按键
    glutReshapeFunc(changeSize);

    glutMainLoop();
    return 0;
}