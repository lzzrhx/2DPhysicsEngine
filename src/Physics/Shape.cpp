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
    this->vertices = vertices;
}

PolygonShape::~PolygonShape() {
    std::cout << "PolygonShape destructor called!" << std::endl;
}

ShapeType PolygonShape::GetType() const {
    return POLYGON;
}

Shape* PolygonShape::Clone() const {
    return new PolygonShape(vertices);
}

float PolygonShape::GetMomentOfInertia() const {
    return 0.0;
}

BoxShape::BoxShape(float width, float height) {
    std::cout << "BoxShape constructor called!" << std::endl;
    this->width = width;
    this->height = height;
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
