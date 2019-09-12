#include <gl/glut.h>
#include <stdio.h>
#include <time.h>
#include <cmath>
#include "Waterfall.h"

const double PI = acos(-1.0);
static int my_angle = 360;
static float angle = 0.0;
static float x = 0.0f, y = 0.0f, z = 5.0f;
static float lx = 0.0f, ly = 0.0f, lz = -1.0f;
Waterfall w;
/**
 * 帧率计算
 */
double CalFrequency() {
    static int count;
    static double save;
    static clock_t last, current;
    double timegap;

    ++count;
    if (count <= 50) return save;
    count = 0;
    last = current;
    current = clock();
    timegap = (current - last) / (double)CLK_TCK;
    save = 50.0 / timegap;
    return save;
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
 * 加入按键控制，试试F1\F2
 */
void processSpecialKeys(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_F1:
            my_angle += 4;
            break;
        case GLUT_KEY_F2:
            my_angle -= 4;
            break;
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

void myDisplay() {
    double FPS = CalFrequency();
    printf("ls.size=%d\tFPS = %f\tx = %f\ty = %f\tz = %f\n", w.getSize(), FPS,
           x, y, z);
    w.Update(0.1);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //在glMatrixMode的“()”里的选项(参数)有3种模式:
    // GL_PROJECTION 投影, GL_MODELVIEW 模型视图, GL_TEXTURE 纹理.
    // TODO 没有GL_PROJECTION貌似就没有深度测试？？
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(75, 1, 0.001f, 1000000000000.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(x, y, z, x + lx, y + ly, z + lz, 0.0f, 1.0f, 0.0f);

    //中心点白色光源
    {
        GLfloat sun_light_position[] = {0.0f, 0.0f, 0.0f, 1.0f};
        GLfloat sun_light_ambient[] = {1.0f, 1.0f, 1.0f, 1.0f};
        GLfloat sun_light_diffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
        GLfloat sun_light_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};

        glLightfv(GL_LIGHT0, GL_POSITION, sun_light_position);
        glLightfv(GL_LIGHT0, GL_AMBIENT, sun_light_ambient);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, sun_light_diffuse);
        glLightfv(GL_LIGHT0, GL_SPECULAR, sun_light_specular);

        glEnable(GL_LIGHT0);
        glEnable(GL_LIGHTING);
        glEnable(GL_DEPTH_TEST);
    }
    // 太阳
    glPushMatrix();
    {
        GLfloat sun_mat_ambient[] = {0.0f, 0.0f, 0.0f, 1.0f};
        GLfloat sun_mat_diffuse[] = {0.0f, 0.0f, 0.0f, 1.0f};
        GLfloat sun_mat_specular[] = {0.0f, 0.0f, 0.0f, 1.0f};
        GLfloat sun_mat_emission[] = {1.0, 1.0, 0.0f, 1.0f};
        GLfloat sun_mat_shininess = 0.0f;

        glMaterialfv(GL_FRONT, GL_AMBIENT, sun_mat_ambient);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, sun_mat_diffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR, sun_mat_specular);
        glMaterialfv(GL_FRONT, GL_EMISSION, sun_mat_emission);
        glMaterialf(GL_FRONT, GL_SHININESS, sun_mat_shininess);
        glColor3f(1.0, 1.0, 0.0);
        glutSolidSphere(0.15, 200, 200);
    }
    glPopMatrix();

    // 地球
    glPushMatrix();
    {
        GLfloat earth_mat_ambient[] = {0.0f, 0.0f, 0.5f, 1.0f};
        GLfloat earth_mat_diffuse[] = {0.0f, 0.0f, 0.5f, 1.0f};
        GLfloat earth_mat_specular[] = {0.0f, 0.0f, 1.0f, 1.0f};
        GLfloat earth_mat_emission[] = {0.0f, 0.0f, 0.0f, 1.0f};
        GLfloat earth_mat_shininess = 20.0f;

        glMaterialfv(GL_FRONT, GL_AMBIENT, earth_mat_ambient);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, earth_mat_diffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR, earth_mat_specular);
        glMaterialfv(GL_FRONT, GL_EMISSION, earth_mat_emission);
        glMaterialf(GL_FRONT, GL_SHININESS, earth_mat_shininess);

        glColor3f(0.0, 0.0, 1.0);
        glRotated(my_angle, 1.0, 1.0, 1.0);  //公转
        glTranslatef(0.5, 0.5, -0.5);        //平移
        glutSolidSphere(0.1, 200, 200);
    }
    glPopMatrix();

    // 月亮
    glPushMatrix();
    {
        GLfloat earth_mat_ambient[] = {0.5f, 0.5f, 0.5f, 1.0f};
        GLfloat earth_mat_diffuse[] = {0.5f, 0.5f, 0.5f, 1.0f};
        GLfloat earth_mat_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
        GLfloat earth_mat_emission[] = {0.0f, 0.0f, 0.0f, 1.0f};
        GLfloat earth_mat_shininess = 20.0f;

        glMaterialfv(GL_FRONT, GL_AMBIENT, earth_mat_ambient);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, earth_mat_diffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR, earth_mat_specular);
        glMaterialfv(GL_FRONT, GL_EMISSION, earth_mat_emission);
        glMaterialf(GL_FRONT, GL_SHININESS, earth_mat_shininess);

        glColor3f(1.0, 1.0, 1.0);
        glRotated(my_angle, 1.0, 1.0, 1.0);  //然后移动到地球旁边旋转
        glTranslatef(0.5, 0.5, -0.5);        //平移
        glRotated(my_angle, 1.0, 1.0, 1.0);  //先假设原点为地球旋转
        glTranslatef(-0.15, -0.15, 0.15);    //平移
        glutSolidSphere(0.05, 200, 200);     //绘制月亮
    }
    glPopMatrix();

    // glPushMatrix();
    w.Render();
    // glPopMatrix();

    glutSwapBuffers();
}

/**
 * 计时增加角度
 */
void myIdle(void) {
    static int mm = 0;
    mm++;
    if (mm % 300000 == 0) {
        /* 新的函数，在空闲时调用，作用是把日期往后移动一天并重新绘制，达到动画效果
         */
        ++my_angle;
        if (my_angle >= 360) {
            my_angle = 0;
        }
        myDisplay();
    }
}

int main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(1000, 1000);
    glutCreateWindow("太阳，地球和月亮");  // 改了窗口标题

    glutIdleFunc(&myIdle);  // 表示在CPU空闲的时间调用某一函数
    glutSpecialFunc(&processSpecialKeys);  // 按键

    //在OpenGL中，默认是没有开启深度检测的，后绘制的物体覆盖先绘制的物体。
    // GL_DEPTH_TEST 用来开启更新深度缓冲区的功能
    glEnable(GL_DEPTH_TEST);
    glutMainLoop();
    return 0;
}