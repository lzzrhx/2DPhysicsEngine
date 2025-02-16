#ifndef CONSTRAINT_H
#define CONSTRAINT_H

#include "Body.h"
#include "MatMN.h"
#include "VecN.h"

class Constraint {
    public:
        Body* a;
        Body* b;

        Vec2 aPoint;
        Vec2 bPoint;
        
        virtual ~Constraint() = default;
        
        MatMN GetInvM() const;
        VecN GetVelocities() const;
        virtual void Solve() {}
        virtual void PreSolve(const float dt) {}
        virtual void PostSolve() {}
};

class JointConstraint: public Constraint {
    private:
        MatMN jacobian;
        VecN cachedLambda;
        float bias;
    public:
        JointConstraint();
        JointConstraint(Body* a, Body* b, const Vec2& anchorPoint);
        void Solve() override;
        void PreSolve(const float dt) override;
};

class PenetrationConstraint: public Constraint {
    MatMN jacobian;
    //void Solve() override;
};

#endif
