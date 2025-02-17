#include "./World.h"
#include "../Graphics.h"
#include "./Constants.h"
#include "./Contact.h"
#include "./CollisionDetection.h"
#include <iostream>
#include <vector>

World::World(float gravity) {
    G = -gravity;
    std::cout << "World constructor called!" << std::endl;
}

World::~World() {
    for (auto body: bodies) {
        delete body;
    }
    for (auto constraint: constraints) {
        delete constraint;
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
    std::vector<PenetrationConstraint> penetrations;

    for (auto& body: bodies) {
        Vec2 weight = Vec2(0.0, body->mass * G * PIXELS_PER_METER);
        body->AddForce(weight);
        for (auto force: forces) 
            body->AddForce(force);
        for (auto torque: torques) 
            body->AddTorque(torque);
        //Vec2 drag = Force::GenerateDragForce(*body, 0.01);
        // body->AddForce(pushForce);
    }
    
    for (auto& body: bodies) {
        body->IntegrateForces(dt);
    }
   
    // TODO: Broad-phase collision check
    // TODO: Narrow-phase collision check

    for (size_t i = 0; i < bodies.size() - 1; i++) {
        for (size_t j = i + 1; j < bodies.size(); j++) {
            Body* a = bodies[i];
            Body* b = bodies[j];
            std::vector<Contact> contacts;
            if (CollisionDetection::IsColliding(a, b, contacts)) {
                for (auto contact: contacts) {
                    Graphics::DrawCircle(contact.start.x, contact.start.y, 5, 0.0, 0xFF00FFFF);
                    Graphics::DrawCircle(contact.end.x, contact.end.y, 2, 0.0, 0xFF00FFFF);
                    PenetrationConstraint penetration(contact.a, contact.b, contact.start, contact.end, contact.normal);
                    penetrations.push_back(penetration);
                }
            }
        }
    }

    for (auto& constraint: constraints) {
        constraint->PreSolve(dt);
    }
    
    for (auto& penetration: penetrations) {
        penetration.PreSolve(dt);
    }

    for (int i = 0; i < NUM_CONSTRAINT_ITERATIONS; i++) {
        for (auto& constraint: constraints) {
            constraint->Solve();
        }
        for (auto& penetration: penetrations) {
            penetration.Solve();
        }
    }
    
    for (auto& constraint: constraints) {
        constraint->PostSolve();
    }
    
    for (auto& penetration: penetrations) {
        penetration.PostSolve();
    }

    for (auto& body: bodies) {
        body->IntegrateVelocities(dt);
    }
}
