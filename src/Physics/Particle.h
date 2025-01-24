#ifndef PARTICLE_H
#define PARTICLE_H

#include "Vec2.h"

struct Particle {
    Vec2 position;
    Vec2 velocity;
    Vec2 acceleration;
    Vec2 sumForces;

    int radius;
    float mass;
    float invMass;

    Particle (float x, float y, float mass);
    ~Particle();

    void AddForce(const Vec2& force);
    void ClearForces();
    void Integrate(float dt);
};

#endif
