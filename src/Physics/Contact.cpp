#include "Contact.h"

void Contact::ResolvePenetration() {
    if (a->IsStatic() && b->IsStatic()) {
        return;
    }

    float da = depth / (a->invMass + b->invMass) * a->invMass;
    float db = depth / (a->invMass + b->invMass) * b->invMass;

    a->position -= normal * da;
    b->position += normal * db;
    a->shape->UpdateVertices(a->rotation, a->position);
    b->shape->UpdateVertices(b->rotation, b->position);
}

void Contact::ResolveCollision() {
    ResolvePenetration();
    float e = std::min(a->restitution, b->restitution);
    float f = std::min(a->friction, b->friction);
    Vec2 ra = end - a->position;
    Vec2 rb = start - b->position;
    Vec2 va = a->velocity + Vec2(-a->angularVelocity * ra.y, a->angularVelocity * ra.x);
    Vec2 vb = b->velocity + Vec2(-b->angularVelocity * rb.y, b->angularVelocity * rb.x);
    const Vec2 vrel = va - vb;

    float vrelDotNormal = vrel.Dot(normal);
    Vec2 impulseDirection = normal;
    float impulseMagnitude = -(1 + e) * vrelDotNormal / ((a->invMass + b->invMass) + ra.Cross(normal) * ra.Cross(normal) * a->invI + rb.Cross(normal) * rb.Cross(normal) * b->invI);
    Vec2 jN = impulseDirection * impulseMagnitude;

    Vec2 tangent = normal.Normal();
    float vrelDotTangent = vrel.Dot(tangent);
    const Vec2 impulseDirectionT = tangent;
    const float impulseMagnitudeT = f * -(1 + e) * vrelDotTangent / ((a->invMass + b->invMass) + ra.Cross(tangent) * ra.Cross(tangent) * a->invI + rb.Cross(tangent) * rb.Cross(tangent) * b->invI);
    Vec2 jT = impulseDirectionT * impulseMagnitudeT;

    Vec2 j = jN + jT;

    a->ApplyImpulseAtPoint(j, ra);
    b->ApplyImpulseAtPoint(-j, rb);
}


