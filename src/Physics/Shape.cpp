#include "./Shape.h"
#include <iostream>

CircleShape::CircleShape(const float radius) {
    this->radius = radius;
    std::cout << "CircleShape constructor called!" << std::endl;
}

CircleShape::~CircleShape() {
    std::cout << "CircleShape destructor called!" << std::endl;
}

ShapeType CircleShape::GetType() const {
    return CIRCLE;
}

Shape* CircleShape::Clone() const {
    return new CircleShape(radius);
}

float CircleShape::GetMomentOfInertia() const {
    // 1/2 * r^2
    return 0.5 * (radius * radius);
}

PolygonShape::PolygonShape(const std::vector<Vec2> vertices) {
    std::cout << "PolygonShape constructor called!" << std::endl;
    this->localVertices = vertices;
    this->worldVertices = vertices;
}

PolygonShape::~PolygonShape() {
    std::cout << "PolygonShape destructor called!" << std::endl;
}

ShapeType PolygonShape::GetType() const {
    return POLYGON;
}

Shape* PolygonShape::Clone() const {
    return new PolygonShape(localVertices);
}

float PolygonShape::GetMomentOfInertia() const {
    return 0.0;
}

void PolygonShape::UpdateVertices(float angle, const Vec2& position) {
    for (size_t i = 0; i < localVertices.size(); i++) {
        worldVertices[i] = localVertices[i].Rotate(angle) + position;
    }
}

BoxShape::BoxShape(float width, float height) {
    std::cout << "BoxShape constructor called!" << std::endl;
    this->width = width;
    this->height = height;
    localVertices.push_back(Vec2(-width / 2.0, -height / 2.0));
    localVertices.push_back(Vec2( width / 2.0, -height / 2.0));
    localVertices.push_back(Vec2( width / 2.0,  height / 2.0));
    localVertices.push_back(Vec2(-width / 2.0,  height / 2.0));
    worldVertices.push_back(Vec2(-width / 2.0, -height / 2.0));
    worldVertices.push_back(Vec2( width / 2.0, -height / 2.0));
    worldVertices.push_back(Vec2( width / 2.0,  height / 2.0));
    worldVertices.push_back(Vec2(-width / 2.0,  height / 2.0));
}

BoxShape::~BoxShape() {
    std::cout << "BoxShape destructor called!" << std::endl;
}

ShapeType BoxShape::GetType() const {
    return BOX;
}

Shape* BoxShape::Clone() const {
    return new BoxShape(width, height);
}

float BoxShape::GetMomentOfInertia() const {
    // 1/12 * (w^2 + h^2)
    return 0.083333 * (width * width + height * height);
}
