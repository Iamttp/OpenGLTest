// Waterfall.h
#pragma once

#ifndef WATERFALL_H
#define WATERFALL_H

#include <gl/glut.h>
#include <stdio.h>
#include <time.h>
#include <cmath>
#include <list>

// 粒子系统
typedef struct {
    float life;  // last time
    float radius;
    float r, g, b;     // color
    float x, y, z;     // the position
    float xi, yi, zi;  // what direction to move
} WaterfallParticle;

#define DEFAULT_PARTICLE_NUMBER 100
#define DEFAULT_PARTICLE_LIFESPAN 10

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
    void Update(double x, double y, double z);
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
        p->x += p->xi * 0.5;
        p->y += p->yi * 0.5;
        p->z += p->zi * 0.5;
        p->life--;
        if (p->life == 0) {
            ls.erase(p++);
        } else {
            p++;
        }
    }
    glBegin(GL_TRIANGLES);
    for (auto& item : ls) RenderParticle(item);
    glEnd();
}

void Waterfall::Update(double x, double y, double z) {
    //新粒子的创建
    WaterfallParticle particle;
    for (int i = 0; i < mParticleNumber; ++i) {
        particle.x = x;
        particle.y = y;
        particle.z = z;
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
    glColor3f(p.r, p.g, p.b);
    glVertex3f(p.x, p.y, p.z);
}
#endif