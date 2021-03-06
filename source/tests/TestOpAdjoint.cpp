/*
 * File:   TestOpAdjoint.cpp
 * Author: Pantelis Sopasakis
 *
 * Created on Sep 15, 2015, 3:01:47 PM
 */

#include "TestOpAdjoint.h"
#include "OpAdjoint.h"


CPPUNIT_TEST_SUITE_REGISTRATION(TestOpAdjoint);

TestOpAdjoint::TestOpAdjoint() {
}

TestOpAdjoint::~TestOpAdjoint() {
}

void TestOpAdjoint::setUp() {
}

void TestOpAdjoint::tearDown() {
}

void TestOpAdjoint::testCall() {
    const size_t n = 16;
    const double tol = 1e-10;
    Matrix A = MatrixFactory::MakeRandomMatrix(n, n, 0.0, 1.0, Matrix::MATRIX_DENSE);
    Matrix x = MatrixFactory::MakeRandomMatrix(n, 1, 0.0, 1.0, Matrix::MATRIX_DENSE);

    LinearOperator *T = new MatrixOperator(A);
    LinearOperator *Tstar = new OpAdjoint(*T);

    Matrix Tx_adj = T->callAdjoint(x);  /*     T*(x)    */
    Matrix Tstarx = Tstar -> call(x);   /*     T*(x)    */

    for (size_t i = 0; i < n; i++) {
        _ASSERT_NUM_EQ(Tx_adj.get(i, 0), Tstarx.get(i, 0), tol);
    }


    Matrix T_star_star_x = Tstar->callAdjoint(x);                   /*     T**(x)    */
    Matrix T_x = T->call(x);                                        /*     T(x)      */
    
    _ASSERT_EQ(T_x, T_star_star_x);
    _ASSERT_EQ(T->isSelfAdjoint(), Tstar->isSelfAdjoint());

    delete T;
    delete Tstar;
}

void TestOpAdjoint::testCallAdjoint() {
    const size_t n = 16;
    const size_t m = 11;
    const double tol = 1e-8;
    Matrix A = MatrixFactory::MakeRandomMatrix(m, n, 0.0, 1.0, Matrix::MATRIX_DENSE);
    Matrix x = MatrixFactory::MakeRandomMatrix(m, 1, 0.0, 1.0, Matrix::MATRIX_DENSE);
    
    LinearOperator *T = new MatrixOperator(A);
    LinearOperator *Tstar = new OpAdjoint(*T);

    Matrix Tx = T->callAdjoint(x);
    Matrix T_doubleAdj_x = Tstar->call(x);
    
    _ASSERT_EQ(n, T->dimensionIn().first);
    _ASSERT_EQ(m, T->dimensionOut().first);
    _ASSERT_EQ(m, Tstar->dimensionIn().first);
    _ASSERT_EQ(n, Tstar->dimensionOut().first);

    for (size_t i = 0; i < n; i++) {
        _ASSERT_NUM_EQ(Tx.get(i, 0), T_doubleAdj_x.get(i, 0), tol);
    }
    
    delete T;
    delete Tstar;
}

void TestOpAdjoint::testSelfAdjoint() {
    const size_t n = 5;
    Matrix A = MatrixFactory::MakeRandomMatrix(n, n, 0.0, 1.0, Matrix::MATRIX_SYMMETRIC);
    
    LinearOperator *T = new MatrixOperator(A);
    LinearOperator *Tstar = new OpAdjoint(*T);
    
    _ASSERT(T->isSelfAdjoint());
    _ASSERT(Tstar->isSelfAdjoint());
    
    delete T;
    delete Tstar;
}
