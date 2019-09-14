#include <gl/glut.h>
#include <stdio.h>
#include <time.h>
#include <cmath>
#include "Waterfall3.h"

static float angle = 0.0, ratio;  // angle��y�����ת�ǣ�ratio���ڸ߿���
static float x = 0.0f, y = 0.0f, z = 3.0f;  //���λ��
static float lx = 0.0f, ly = 0.0f, lz = -1.0f;  //���߷��򣬳�ʼ��Ϊ����Z�Ḻ����
static int my_angle = 0; // ��ʾ��ת�ĽǶ�
Waterfall w;
int flag = 0;
/**
 * ����۲췽ʽ
 */
void changeSize(int w, int h) {
	//����0�����
	if (h == 0) h = 1;
	ratio = 1.0f * w / h;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//�����ӿ�Ϊ�������ڴ�С
	glViewport(0, 0, w, h);
	//���ÿ��ӿռ�
	gluPerspective(45, ratio, 1, 1000);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(x, y, z, x + lx, y + ly, z + lz, 0.0f, 1.0f, 0.0f);
}
/**
 * ��Ұ���κ���
 */
void orientMe(float ang) {
	lx = sin(ang);
	lz = -cos(ang);
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
 * ���밴������
 */
void processSpecialKeys(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_F1:
		flag = 1;
		break;
	case GLUT_KEY_F2:
		flag = 2;
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

void myDisplay(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// ̫��
	glPushMatrix();
	glColor3f(1.0, 1.0, 0.0);
	glutSolidSphere(0.15, 200, 200);
	glPopMatrix();

	// ����
	glPushMatrix();
	glColor3f(0.0, 0.0, 1.0);
	glRotated(my_angle, 1.0, 1.0, 1.0);  //��ת
	glTranslatef(0.5, 0.5, -0.5);        //ƽ��
	glutSolidSphere(0.1, 200, 200);
	glPopMatrix();

	// ����
	glPushMatrix();
	glColor3f(1.0, 1.0, 1.0);
	glRotated(my_angle, 1.0, 1.0, 1.0);  //Ȼ���ƶ��������Ա���ת
	glTranslatef(0.5, 0.5, -0.5);        //ƽ��
	glRotated(my_angle, 1.0, 1.0, 1.0);  //�ȼ���ԭ��Ϊ������ת
	glTranslatef(-0.15, -0.15, 0.15);    //ƽ��
	glutSolidSphere(0.05, 200, 200);     //��������
	glPopMatrix();
	if (flag == 1) {
		flag = 0;
		w.Update();
	}
	glPushMatrix();
	w.Render();
	glPopMatrix();

	glutSwapBuffers();
}
/**
 * ��ʱ���ӽǶ�
 */
void myIdle(void) {
	static int mm = 0;
	mm++;
	if (mm % 300000 == 0) {
		++my_angle;
		if (my_angle >= 360) my_angle = 0;
		myDisplay();
	}
}

int main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(1000, 1000);
	glutCreateWindow("̫�������������");  // ���˴��ڱ���

	glutDisplayFunc(&myDisplay);
	glutIdleFunc(&myIdle);  // ��ʾ��CPU���е�ʱ�����ĳһ����
	glutSpecialFunc(&processSpecialKeys);  // ����
	glutReshapeFunc(&changeSize);

	// ��OpenGL�У�Ĭ����û�п�����ȼ��ģ�����Ƶ����帲���Ȼ��Ƶ����塣
	// GL_DEPTH_TEST ��������������Ȼ������Ĺ���
	glEnable(GL_DEPTH_TEST);
	glutMainLoop();
	return 0;
}
