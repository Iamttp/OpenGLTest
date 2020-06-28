#ifndef OPENGLGAME_WATERFALL_H
#define OPENGLGAME_WATERFALL_H

#include <GL/glut.h>
#include <cstdio>
#include <ctime>
#include <cmath>
#include <list>

// 粒子系统
typedef struct {
    // bool bounced;  // bounced or not
    int life;  // last time
    float radius;
    float r, g, b;     // color
    float x, y, z;     // the position
    float xi, yi, zi;  // what direction to move
} WaterfallParticle;

inline float randFloat01() { return 1.0 * rand() / RAND_MAX; }

inline float randFloat(float from, float to) { return from + (to - from) * randFloat01(); }

inline int randInt(int from, int to) { return from + rand() % (to - from); }

class Waterfall {
public:
    WaterfallParticle(*initParticle)();

    typedef WaterfallParticle(*InitParticle)();  //定义类型

    std::list<WaterfallParticle> ls;

    explicit Waterfall(InitParticle func1 = nullptr) {
        initParticle = func1;
    }

    void Update(GLfloat deltaTime) {
        int newParticleNumber = randInt(0, 100);

        for (int i = 0; i < newParticleNumber; ++i) {
            ls.push_back(initParticle());
        }

        for (auto& item : ls) {
            item.x += item.xi * deltaTime;
            item.y += item.yi * deltaTime;
            item.z += item.zi * deltaTime;
        }

        for (auto p = ls.begin(); p != ls.end();) {
            p->life--;
            if (p->life <= 0) ls.erase(p++);
            else ++p;
        }
    }

    //渲染，普通的渲染函数就是渲染每一粒存在的粒子
    virtual void Render() {
        glBegin(GL_POINTS);
        for (auto& item : ls) {
            glColor4f(item.r, item.g, item.b, item.life);
            glVertex3f(item.x, item.y, item.z);  //平移
        }
        glEnd();
    }

    int getSize() { return ls.size(); }
};

#endif //OPENGLGAME_WATERFALL_H
