/* 
 * File:   MatrixOperator.cpp
 * Author: Pantelis Sopasakis
 * 
 * Created on July 24, 2015, 7:31 PM
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

#include "MatrixOperator.h"

Matrix& MatrixOperator::GetMatrix() const {
    return A;
}

bool MatrixOperator::isSelfAdjoint() {
    return m_isSelfAdjoint;
}

void MatrixOperator::SetMatrix(Matrix& A) {
    this->A = A;
    m_isSelfAdjoint = A.isSymmetric();
}

MatrixOperator::MatrixOperator(Matrix& A) :
A(A) {
    if (A.isSymmetric()) {
        this->m_isSelfAdjoint = true;
    }
}

MatrixOperator::~MatrixOperator() {
}

Matrix MatrixOperator::call(Matrix& x) {
    Matrix y;
    y = A*x;
    return y;
}

Matrix MatrixOperator::callAdjoint(Matrix& x) {
    if (isSelfAdjoint()) {
        return call(x);
    }
    Matrix y;
    A.transpose();
    y = A*x;
    A.transpose();
    return y;
}

size_t MatrixOperator::dimensionIn() {
    return A.getNcols();
}

size_t MatrixOperator::dimensionOut() {
    return A.getNrows();
}

