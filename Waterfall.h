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
    // bool bounced;  // bounced or not
    float life;  // last time
    float radius;
    float r, g, b;     // color
    float x, y, z;     // the position
    float xi, yi, zi;  // what direction to move
} WaterfallParticle;

#define DEFAULT_PARTICLE_NUMBER 1000
#define DEFAULT_PARTICLE_LIFESPAN 10000

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
    void Update(GLfloat deltaTime);
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
    for (auto& item : ls) RenderParticle(item);
}

void Waterfall::Update(GLfloat deltaTime) {
    //新粒子的创建
    WaterfallParticle particle;
    int newParticleNumber = 0;
    if (mParticleNumber > ls.size()) {
        newParticleNumber = randInt(100, 200);
    }
    for (int i = 0; i < newParticleNumber; ++i) {
        particle.x = randFloat(-1.0f, 1.0f);
        particle.y = randFloat(-1.0f, 1.0f);
        particle.z = randFloat(-1.0f, 1.0f);
        particle.r = particle.g = particle.b = 1.0f;
        particle.xi = randFloat(-0.1f, 0.1f);
        particle.yi = randFloat(-0.1f, 0.1f);
        particle.zi = randFloat(-0.1f, 0.1f);
        particle.life = mParticleLifespan;
        ls.push_back(particle);
    }

    for (auto& item : ls) {
        WaterfallParticle* p = &item;
        p->x += p->xi * deltaTime;
        p->y += p->yi * deltaTime;
        p->z += p->zi * deltaTime;
        if (p->x < -1.0f || p->x > 1.0f) p->xi = -p->xi;
        if (p->y < -1.0f || p->y > 1.0f) p->yi = -p->yi;
        if (p->z < -1.0f || p->z > 1.0f) p->zi = -p->zi;
        p->life--;
        if (p->life == 0) p->life = mParticleLifespan;
    }
}

void Waterfall::RenderParticle(const WaterfallParticle& p) {
    glColor4f(1, 1, 1, p.life);
    glTranslatef(p.x, p.y, p.z);  //平移
    glutSolidSphere(0.03, 2, 2);  //绘制月亮
}
#endif