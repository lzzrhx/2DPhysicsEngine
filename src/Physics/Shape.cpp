#include "./Shape.h"
#include <iostream>
#include <limits>

CircleShape::CircleShape(const float radius) {
    this->radius = radius;
    std::cout << "CircleShape constructor called!" << std::endl;
}

CircleShape::~CircleShape() {
    std::cout << "CircleShape destructor called!" << std::endl;
}

void CircleShape::UpdateVertices(float angle, const Vec2& position) {
    return;
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
    for (auto vertex: vertices) {
        localVertices.push_back(vertex);
        worldVertices.push_back(vertex);
    }
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
    return 5000;
}

Vec2 PolygonShape::EdgeAt(int index) const { 
    int currVertex = index;
    int nextVertex = (index + 1) % worldVertices.size();
    return worldVertices[nextVertex] - worldVertices[currVertex];
}

float PolygonShape::FindMinSeparation(const PolygonShape* other, int& indexReferenceEdge, Vec2& supportPoint) const {
    float separation = std::numeric_limits<float>::lowest();
    for (size_t i = 0; i < this->worldVertices.size(); i++) {
        Vec2 va = this->worldVertices[i];
        Vec2 normal = this->EdgeAt(i).Normal();
        float minSep = std::numeric_limits<float>::max();
        Vec2 minVertex;
        for (size_t j = 0; j < other->worldVertices.size(); j++) {
            Vec2 vb = other->worldVertices[j];
            float proj = (vb - va).Dot(normal);
            if (proj < minSep) {
                minSep = proj;
                minVertex = vb;
            }
        }
        if (minSep > separation) {
            separation = minSep;
            indexReferenceEdge = i;
            supportPoint = minVertex;
        }
        separation = std::max(separation, minSep);
    }
    return separation;
}

int PolygonShape::FindIncidentEdge(const Vec2& normal) const {
    int indexIncidentEdge;
    float minProj = std::numeric_limits<float>::max();
    for (size_t i = 0; i < this->worldVertices.size(); ++i) {
        auto edgeNormal = this->EdgeAt(i).Normal();
        auto proj = edgeNormal.Dot(normal);
        if (proj < minProj) {
            minProj = proj;
            indexIncidentEdge = i;
        }
    }
    return indexIncidentEdge;
}

int PolygonShape::ClipSegmentToLine(const std::vector<Vec2>& contactsIn, std::vector<Vec2>& contactsOut, const Vec2& c0, const Vec2& c1) const {
    int numOut = 0;
    Vec2 normal = (c1 - c0).Normalize();
    float dist0 = (contactsIn[0] - c0).Cross(normal);
    float dist1 = (contactsIn[1] - c0).Cross(normal);
    if (dist0 <= 0)
        contactsOut[numOut++] = contactsIn[0];
    if (dist1 <= 0)
        contactsOut[numOut++] = contactsIn[1];

    if (dist0 * dist1 < 0) {
        float totalDist = dist0 - dist1;
        float t = dist0 / (totalDist);
        Vec2 contact = contactsIn[0] + (contactsIn[1] - contactsIn[0]) * t;
        contactsOut[numOut] = contact;
        numOut++;
    }
    return numOut;
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
