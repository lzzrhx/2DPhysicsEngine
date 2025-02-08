#ifndef MATMN_H
#define MATMN_H

#include "VecN.h"

struct MatMN {
    int M;
    int N;
    VecN* rows;

    MatMN();
    MatMN(int M, int N);
    MatMN(const MatMN* m);
    ~MatMN();

    void Zero();
    MatMN Transpose() const;

    const MatMN operator = (const MatMN& m);
    VecN operator * (const VecN& v) const;
    MatMN operator * (const MatMN& m) const;
};

#endif
