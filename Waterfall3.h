// Waterfall.h
#pragma once

#ifndef WATERFALL_H
#define WATERFALL_H

#include <gl/glut.h>
#include <stdio.h>
#include <time.h>
#include <cmath>
#include <list>

const double PI = acos(-1.0);
// 粒子系统
typedef struct {
	float life;  // last time
	float radius;
	float r, g, b;     // color
	float x, y, z;     // the position
	float xi, yi, zi;  // what direction to move
} WaterfallParticle;

#define DEFAULT_PARTICLE_NUMBER 1000
#define DEFAULT_PARTICLE_LIFESPAN 30

float randFloat01() { return 1.0 * rand() / RAND_MAX; }
float randFloat(float from, float to) {
	return from + (to - from) * randFloat01();
}
int randInt(int from, int to) { return from + rand() % (to - from); }

class Waterfall {
public:
	Waterfall(GLuint particleNumber = DEFAULT_PARTICLE_NUMBER,
		GLuint particleLifespan = DEFAULT_PARTICLE_LIFESPAN)
		: mParticleNumber(particleNumber),
		mParticleLifespan(particleLifespan) {}
	void Update();
	//渲染，普通的渲染函数就是渲染每一粒存在的粒子
	virtual void Render();
	int getSize() { return ls.size(); }

private:
	GLuint mParticleNumber;
	GLuint mParticleLifespan;
	std::list<WaterfallParticle> ls;
	void RenderParticle(const WaterfallParticle& p);
};

//粒子的状态更新，可以尽情发挥自己的创意编写代码
void Waterfall::Render() {
	auto p = ls.begin();
	while (p != ls.end()) {
		p->x += p->xi * 0.1;
		p->y += p->yi * 0.1;
		p->z += p->zi * 0.1;
		p->life--;
		if (p->life == 0) {
			ls.erase(p++);
		}
		else {
			p++;
		}
	}
	glPointSize(5);//设置点的大小为10
	glBegin(GL_POINTS);
	for (auto& item : ls) RenderParticle(item);
	glEnd();
}

void Waterfall::Update() {
	//新粒子的创建
	WaterfallParticle particle;
	int newParticleNumber = mParticleNumber;
	for (int i = 0; i < newParticleNumber; ++i) {
		auto rate = randFloat(-0, 1);
		double sqrt2 = sqrt(2);
		double sqrt6 = sqrt(6);
		particle.x = cos(2 * PI * rate) / sqrt2 + sin(2 * PI * rate) / sqrt6;
		particle.y = -1.0 * cos(2 * PI * rate) / sqrt2 + sin(2 * PI * rate) / sqrt6;
		particle.z = -2.0 * sin(2 * PI * rate) / sqrt6;
		/*
		particle.x = cos(2 * PI * rate) * 1;
		particle.y = sin(2 * PI * rate) * 1;
		particle.z = 0;
		*/
		particle.r = randFloat(0, 1);
		particle.g = randFloat(0, 1);
		particle.b = randFloat(0, 1);
		particle.xi = randFloat(-0.1f, 0.1f);
		particle.yi = randFloat(-0.1f, 0.1f);
		particle.zi = randFloat(-0.1f, 0.1f);
		particle.life = mParticleLifespan;
		ls.push_back(particle);
	}
}

void Waterfall::RenderParticle(const WaterfallParticle& p) {
	glColor4f(p.r, p.g, p.b, p.life);
	glVertex3f(p.x, p.y, p.z);
}
#endif