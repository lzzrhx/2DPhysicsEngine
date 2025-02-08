#include "Constraint.h"

Mat6x6 Constraint::GetInvM() const {
    Mat6x6 invM(6, 6);

    invM.rows[0][0] = a->invMass;
    invM.rows[1][1] = 
    invM.rows[2][2] = 
    invM.rows[3][3] = 
    invM.rows[4][4] = 
    invM.rows[5][5] = 
    invM.rows[6][6] = 
}
