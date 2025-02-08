#ifndef CONSTRAINT_H
#define CONSTRAINT_H

#include "Body.h"

class Constraint {
    public:
        Body* a;
        Body* b;
        MatMN GetInvM() const;
        VecN vec;
        void Solve();
};

#endif
