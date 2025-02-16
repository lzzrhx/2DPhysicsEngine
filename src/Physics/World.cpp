#include "./World.h"
#include "./Constants.h"
#include "./Contact.h"
#include "./CollisionDetection.h"
#include <iostream>

World::World(float gravity) {
    G = -gravity;
    std::cout << "World constructor called!" << std::endl;
}

World::~World() {
    for (auto body: bodies) {
        delete body;
    }
    std::cout << "World destructor called!" << std::endl;
}

void World::AddBody(Body* body) {
    bodies.push_back(body);
}

std::vector<Body*>& World::GetBodies() {
    return bodies;
}

void World::AddConstraint(Constraint* constraint) {
    constraints.push_back(constraint);
}

std::vector<Constraint*>& World::GetConstraints() {
    return constraints;
}

void World::AddForce(const Vec2& force) {
    forces.push_back(force);
}

void World::AddTorque(float torque) {
    torques.push_back(torque);
}

void World::Update(float dt) {
    for (auto body: bodies) {
        Vec2 weight = Vec2(0.0, body->mass * G * PIXELS_PER_METER);
        body->AddForce(weight);
        for (auto force: forces) 
            body->AddForce(force);
        for (auto torque: torques) 
            body->AddTorque(torque);
        //Vec2 drag = Force::GenerateDragForce(*body, 0.01);
        // body->AddForce(pushForce);
        body->isColliding = false;
    }
    
    for (auto body: bodies) {
        body->IntegrateForces(dt);
    }

    for (auto& constraint: constraints) {
        constraint->Solve();
    }

    for (auto body: bodies) {
        body->IntegrateVelocities(dt);
    }
    
    CheckCollisions();
}

void World::CheckCollisions() {
    for (size_t i = 0; i < bodies.size() - 1; i++) {
        for (size_t j = i + 1; j < bodies.size(); j++) {
            Body* a = bodies[i];
            Body* b = bodies[j];
            Contact contact;
            if (CollisionDetection::IsColliding(a, b, contact)) {
                contact.ResolveCollision();
                a->isColliding = true;
                b->isColliding = true;
            }
        }
    }
}
