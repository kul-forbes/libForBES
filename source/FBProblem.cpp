/* 
 * File:   SeparableSum.h
 * Author: Lorenzo Stella
 * Author: Pantelis Sopasakis
 *
 * ForBES is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *  
 * ForBES is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with ForBES. If not, see <http://www.gnu.org/licenses/>.
 */

#include "FBProblem.h"

void FBProblem::init() {
    m_f1 = NULL;
    m_f2 = NULL;
    m_g = NULL;
    m_L1 = NULL;
    m_L2 = NULL;
    m_d1 = NULL;
    m_d2 = NULL;
    m_lin = NULL;
}
FBProblem::FBProblem(
        Function& fun_f1,
        LinearOperator& L_1,
        Matrix& d_1,
        Function& fun_f2,
        LinearOperator& L_2,
        Matrix& d_2,
        Matrix& linear,
        Function& fun_g) {
    this->m_f1 = &fun_f1;
    this->m_L1 = &L_1;
    this->m_d1 = &d_1;
    this->m_f2 = &fun_f2;
    this->m_L2 = &L_2;
    this->m_d2 = &d_2;
    this->m_lin = &linear;
    this->m_g = &fun_g;
}

FBProblem::FBProblem(
        Function& fun_f1,
        LinearOperator& L_1,
        Matrix& d_1,
        Function& fun_g) {
    init();
    this->m_f1 = &fun_f1;
    this->m_L1 = &L_1;
    this->m_d1 = &d_1;
    this->m_g = &fun_g;
}

FBProblem::FBProblem(
        Function& fun_f1,
        Function& fun_g) {
    init();
    this->m_f1 = &fun_f1;
    this->m_g = &fun_g;
}

LinearOperator * FBProblem::L1() {
    return m_L1;
}

LinearOperator * FBProblem::L2() {
    return m_L2;
}

Matrix* FBProblem::d1() {
    return m_d1;
}

Matrix* FBProblem::d2() {
    return m_d2;
}

Function* FBProblem::f1() {
    return m_f1;
}

Function* FBProblem::f2() {
    return m_f2;
}

Function* FBProblem::g() {
    return m_g;
}

Matrix* FBProblem::lin() {
    return m_lin;
}

FBProblem::~FBProblem() {
    // nothing to delete    
}
