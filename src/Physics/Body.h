#ifndef BODY_H
#define BODY_H

#include "Vec2.h"
#include "Shape.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

struct Body {
    bool isColliding = false;

    // Linear motion
    Vec2 position;
    Vec2 velocity;
    Vec2 acceleration;

    // Angular motion
    float rotation;
    float angularVelocity;
    float angularAcceleration;
    
    // Forces and torque
    Vec2 sumForces;
    float sumTorque;

    // Mass and moment of inertia
    float mass;
    float invMass;
    float I;
    float invI;

    // Coefficient of restitution (elasticity)
    float restitution;

    // Coefficient of friction
    float friction;

    // Pointer to the shape/geometry of this rigid body
    Shape* shape = nullptr;

    // Pointer to an SDL texture
    SDL_Texture* texture = nullptr;

    // Constructor / destructor
    Body (const Shape& shape, float x, float y, float mass);
    ~Body();

    void SetTexture(const char* textureFileName);

    bool IsStatic() const;

    void ApplyImpulse(const Vec2& j);
    void ApplyImpulse(const Vec2& j, const Vec2& r);

    void AddForce(const Vec2& force);
    void AddTorque(float torque);
    void ClearForces();
    void ClearTorque();

    void IntegrateLinear(float dt);
    void IntegrateAngular(float dt);

    void Update(float dt);
};

#endif
