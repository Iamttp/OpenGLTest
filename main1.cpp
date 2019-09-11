#include <gl/glut.h>
#include <cmath>
#include <time.h>
#include <stdio.h>

const double PI = acos(-1.0);
int angle = 0;

/**
 * 帧率计算
*/
double CalFrequency() {
	static int count;
	static double save;
	static clock_t last, current;
	double timegap;

	++count;
	if (count <= 50)
		return save;
	count = 0;
	last = current;
	current = clock();
	timegap = (current - last) / (double)CLK_TCK;
	save = 50.0 / timegap;
	return save;
}

void myDisplay() {
	double FPS = CalFrequency();
	printf("FPS = %f\n", FPS);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//在glMatrixMode的“()”里的选项(参数)有3种模式: 
	//GL_PROJECTION 投影, GL_MODELVIEW 模型视图, GL_TEXTURE 纹理.
    // TODO 没有GL_PROJECTION貌似就没有深度测试？？
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(75, 1, 0.001f, 1000000000000.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0, 0.0, 1.5, 0.0, 0.0, -1.5, 0.0, 1.0, 0.0);

	// 太阳
	glPushMatrix();
	glColor3f(1.0, 1.0, 0.0);
	glutSolidSphere(0.15, 200, 200);
	glPopMatrix();

	// 地球
	glPushMatrix();
	glColor3f(0.0, 0.0, 1.0);
	glRotated(angle, 1.0, 1.0, 1.0);    //公转
	glTranslatef(0.5, 0.5, -0.5);           //平移
	glutSolidSphere(0.1, 200, 200);
	glPopMatrix();

	// 月亮
	glPushMatrix();
	glColor3f(1.0, 1.0, 1.0);
	glRotated(angle, 1.0, 1.0, 1.0);    //然后移动到地球旁边旋转
	glTranslatef(0.5, 0.5, -0.5);           //平移
	glRotated(angle, 1.0, 1.0, 1.0);    //先假设原点为地球旋转
	glTranslatef(-0.15, -0.15, 0.15);           //平移
	glutSolidSphere(0.05, 200, 200);     //绘制月亮 
	glPopMatrix();

	glutSwapBuffers();
}

/**
* 计时增加角度
*/
int mm = 0;
void myIdle(void) {
	mm++;
	if (mm % 300000 == 0) {
		/* 新的函数，在空闲时调用，作用是把日期往后移动一天并重新绘制，达到动画效果 */
		++angle;
		if (angle >= 360)
			angle = 0;
		myDisplay();
		return;
	}
}

int main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH |GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(1000, 1000);
	glutCreateWindow("太阳，地球和月亮");    // 改了窗口标题
	
	//glutDisplayFunc(&myDisplay);
	glutIdleFunc(&myIdle);                // 表示在CPU空闲的时间调用某一函数
	//在OpenGL中，默认是没有开启深度检测的，后绘制的物体覆盖先绘制的物体。 
	//GL_DEPTH_TEST 用来开启更新深度缓冲区的功能
    glEnable(GL_DEPTH_TEST);
	glutMainLoop();
	return 0;
}