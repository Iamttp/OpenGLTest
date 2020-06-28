#include <GL/glut.h>
#include <cstdio>
#include <ctime>
#include <cmath>
#include "Waterfall4.h"

static int my_angle = 360;
static float angle = 0.0;
static float x = 0.0f, y = 0.0f, z = 5.0f;
static float lx = 0.0f, ly = 0.0f, lz = -1.0f;

// ----------------- 满天星粒子 ---------------------------
static const float xBound = 2.0f, yBound = 2.0f, zBound = 2.0f;

WaterfallParticle func() {
    WaterfallParticle particle;
    particle.x = randFloat(-xBound, xBound);
    particle.y = randFloat(-yBound, yBound);
    particle.z = randFloat(-zBound, zBound);
    particle.r = particle.g = particle.b = randFloat(0, 1);
    particle.xi = randFloat(-0.1f, 0.1f);
    particle.yi = randFloat(-0.1f, 0.1f);
    particle.zi = randFloat(-0.1f, 0.1f);
    particle.life = randInt(0, 100);
    return particle;
}

Waterfall star(func);
// ------------------------------------------------------

// ------------------ 流星粒子 ---------------------------
WaterfallParticle func2() {
    WaterfallParticle particle;
    particle.x = particle.y = particle.z = 0;
    particle.r = randFloat(0, 1);
    particle.g = randFloat(0, 1);
    particle.b = randFloat(0, 1);
    particle.xi = randFloat(-0.1f, 0.1f);
    particle.yi = 1;
    particle.zi = randFloat(-0.1f, 0.1f);
    particle.life = randInt(0, 100);
    return particle;
}

struct star2 {
    Waterfall *w2{};
    float xx{}, yy{}, zz{}, rr{};
    float yyd{};

    void init() {
        w2 = new Waterfall(func2);
        rr = randInt(-90, 90);
        xx = randFloat(-1, 1);
        yy = randFloat(3, 4);
        zz = randFloat(-1, 1);
        yyd = randFloat(0, 0.1);
    }

    star2() {
        init();
    }
} starv[3];
// ------------------------------------------------------

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
    timegap = (current - last) / (double) CLK_TCK;
    save = 50.0 / timegap;
    return save;
}

/**
 * 视野漫游函数
 */
void orientMe(float ang) {
    lx = std::sin(ang);
    lz = -std::cos(ang);
    glLoadIdentity();
    gluLookAt(x, y, z, x + lx, y + ly, z + lz, 0.0f, 1.0f, 0.0f);
}

void moveMeFlat(int direction) {
    x = x + direction * (lx) * 0.1;
    z = z + direction * (lz) * 0.1;
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
        default:
            break;
    }
}

bool mouseDown = false;
float xrot = 0.0f, yrot = 0.0f;
float xdiff = 0.0f, ydiff = 0.0f;
float myratio;  // angle绕y轴的旋转角，ratio窗口高宽比

// 鼠标事件
void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        mouseDown = true;
        xdiff = x - yrot;
        ydiff = -y + xrot;
    } else
        mouseDown = false;
}

// 鼠标移动事件
void mouseMotion(int x, int y) {
    if (mouseDown) {
        yrot = x - xdiff;
        xrot = y + ydiff;
        glutPostRedisplay();
    }
}

/**
 * 定义观察方式
 */
void changeSize(int w, int h) {
    //除以0的情况
    if (h == 0) h = 1;
    myratio = 1.0f * w / h;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //设置视口为整个窗口大小
    glViewport(0, 0, w, h);
    //设置可视空间
    gluPerspective(45, myratio, 1, 1000);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(x, y, z, x + lx, y + ly, z + lz, 0.0f, 1.0f, 0.0f);
}

bool flagInit = true;

void myDisplay() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(x, y, z, x + lx, y + ly, z + lz, 0.0f, 1.0f, 0.0f);

    // 实现鼠标旋转的核心
    glRotatef(xrot, 1.0f, 0.0f, 0.0f);
    glRotatef(yrot, 0.0f, 1.0f, 0.0f);

    // 太阳
    glPushMatrix();
    glColor3f(1.0, 1.0, 0.0);
    glutSolidSphere(0.15, 200, 200);
    glPopMatrix();

    // 地球
    glPushMatrix();
    glColor3f(0.0, 0.0, 1.0);
    glRotated(my_angle, 1.0, 1.0, 1.0);  //公转
    glTranslatef(0.5, 0.5, -0.5);        //平移
    glutSolidSphere(0.1, 200, 200);
    glPopMatrix();

    // 月亮
    glPushMatrix();
    glColor3f(1.0, 1.0, 1.0);
    glRotated(my_angle, 1.0, 1.0, 1.0);  //然后移动到地球旁边旋转
    glTranslatef(0.5, 0.5, -0.5);        //平移
    glRotated(my_angle, 1.0, 1.0, 1.0);  //先假设原点为地球旋转
    glTranslatef(-0.15, -0.15, 0.15);    //平移
    glutSolidSphere(0.05, 200, 200);     //绘制月亮
    glPopMatrix();

    glPushMatrix();
    star.Render();
    glPopMatrix();

    for (auto &item : starv) {
        item.yy = item.yy - item.yyd;
        if (item.yy <= -2) item.init();
        glPushMatrix();
        glRotatef(item.rr, 0, 0, 1);
        glTranslatef(item.xx, item.yy, item.zz);    //平移
        item.w2->Render();
        glPopMatrix();
    }

    glutSwapBuffers();
}

const int msDelay = 10;

/**
 * 计时增加角度
 */
void myIdle(int i) {
    double FPS = CalFrequency();
    printf("ls.size=%d\tFPS = %f\n", star.getSize(), FPS);

    if (++my_angle >= 360) my_angle = 0;
    star.Update(msDelay / 1000.0);

    for (auto &item : starv) {
        item.w2->Update(msDelay / 1000.0);
    }
    myDisplay();
    glutTimerFunc(msDelay, myIdle, 1);
}

int main(int argc, char *argv[]) {
    srand((unsigned int) time(0));
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(1000, 1000);
    glutCreateWindow("Demo");  // 改了窗口标题

    glutDisplayFunc(myDisplay);
    //    glutIdleFunc(myIdle);  // 表示在CPU空闲的时间调用某一函数
    glutTimerFunc(msDelay, myIdle, 1);
    glutSpecialFunc(processSpecialKeys);  // 按键
    glutReshapeFunc(changeSize);
    glutMouseFunc(mouse);
    glutMotionFunc(mouseMotion);
    glutMainLoop();
    return 0;
}