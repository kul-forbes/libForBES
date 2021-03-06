/* 
 * File:   QuadraticLossOverAffine.cpp
 * Author: Pantelis Sopasakis
 * 
 * Created on November 3, 2015, 3:58 PM
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

#include "QuadraticLossOverAffine.h"
#include <cmath>

QuadraticLossOverAffine::QuadraticLossOverAffine(Matrix& A, Matrix& b, Matrix& w, Matrix& p) {
    m_A = &A;
    m_b = &b;
    m_w = &w;
    m_p = &p;
    m_F = NULL;
    m_solver = NULL;
    if (!w.isColumnVector()) {
        throw std::invalid_argument("w is not a column vector");
    }
    if (!p.isColumnVector()) {
        throw std::invalid_argument("p is not a column vector");
    }
    if (A.getNrows() != b.getNrows()) {
        throw std::invalid_argument("A and b have incompatible dimensions");
    }
    /* F = A * diag(1/sqrt(w_i))_i */
    m_F = new Matrix();
    Matrix W_inv_sqrt(w);
    for (size_t i = 0; i < w.getNrows(); ++i) {
        W_inv_sqrt[i] = 1 / std::sqrt(W_inv_sqrt[i]);
    }
    W_inv_sqrt.toggle_diagonal();
    *m_F = A * W_inv_sqrt;
    m_solver = new S_LDLFactorization(*m_F, __QUADLOSS_AFFINE_EPSILON);
    int status = m_solver -> factorize();
    if (ForBESUtils::is_status_error(status)) {
        throw std::invalid_argument("Matrix FF'+eI cannot be LDL-factorized");
    }
}

QuadraticLossOverAffine::~QuadraticLossOverAffine() {
    if (m_F != NULL) {
        delete m_F;
        m_F = NULL;
    }
    if (m_solver != NULL) {
        delete m_solver;
        m_solver = NULL;
    }
}

int QuadraticLossOverAffine::callConj(Matrix& y, double& f_star, Matrix& grad) {
    size_t ny = y.getNrows();
    Matrix sigma(ny, 1);
    for (size_t i = 0; i < ny; i++) {
        sigma[i] =  y[i] / m_w->get(i) + m_p->get(i);
    }
    Matrix h = (*m_A) * sigma - (*m_b);
    Matrix q;
    int status = m_solver -> solve(h, q);
    if (ForBESUtils::is_status_error(status)) {
        return status;
    }
    m_A->transpose();
    Matrix c = (*m_A) * q;
    m_A->transpose();
    for (size_t i = 0; i < ny; i++) {
        grad[i] = sigma[i] - c[i] / m_w->get(i);
    }
    f_star = (grad *  (y))[0];
    return ForBESUtils::STATUS_OK;
}

int QuadraticLossOverAffine::callConj(Matrix& y, double& f_star) {
    size_t ny = y.getNrows();
    Matrix grad_temp(ny, 1);
    return callConj(y, f_star, grad_temp);
}

FunctionOntologicalClass QuadraticLossOverAffine::category() {
    FunctionOntologicalClass meta("QuadraticLossOverAffine");
    meta.set_defines_conjugate(true);
    meta.set_defines_conjugate_grad(true);
    meta.getSuperclasses().push_back(FunctionOntologyRegistry::conj_quadratic());
    return meta;
}





