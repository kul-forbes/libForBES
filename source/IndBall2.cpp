/* 
 * File:   IndBall2.cpp
 * Author: Pantelis Sopasakis
 * 
 * Created on November 3, 2015, 4:18 PM
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

#include "IndBall2.h"
#include <cmath>

void check_rho(double rho);

void check_rho(double rho) {
    if (rho < 0) {
        throw std::invalid_argument("rho must be positive");
    }
}

IndBall2::IndBall2() {
    m_rho = 1;
    m_xc = NULL;
    m_is_xc_zero = true;
}

IndBall2::IndBall2(double rho) {
    check_rho(rho);
    m_rho = rho;
    m_xc = NULL;
    m_is_xc_zero = true;
}

IndBall2::IndBall2(double rho, Matrix& c) {
    check_rho(rho);
    m_rho = rho;
    m_xc = &c;
    m_is_xc_zero = false;
}

IndBall2::~IndBall2() {
    // nothing to delete
}

double IndBall2::norm_div(const Matrix& x) {
    double ndiv = 0.0;
    if (m_is_xc_zero) {
        for (size_t i = 0; i < x.getNrows(); i++) {
            ndiv += std::pow(x[i], 2);
        }
    } else {
        for (size_t i = 0; i < x.getNrows(); i++) {
            ndiv += std::pow(x[i] - m_xc->get(i), 2);
        }
    }
    ndiv = std::sqrt(ndiv);
    return ndiv;
}

int IndBall2::callProx(Matrix& x, double gamma, Matrix& prox) {
    if (!x.isColumnVector()) {
        throw std::invalid_argument("x must be a vector");
    }
    double normDiv = norm_div(x);
    if (normDiv <= m_rho) {
        prox = x;
    } else {
        double alpha = m_rho / normDiv;
        for (size_t i = 0; i < x.getNrows(); i++) {
            prox[i] = (m_is_xc_zero ? 0.0 : m_xc->get(i))
                    + alpha * (x[i] - (m_is_xc_zero ? 0.0 : m_xc->get(i)));
        }
    }
    return ForBESUtils::STATUS_OK;
}

int IndBall2::callProx(Matrix& x, double gamma, Matrix& prox, double& f_at_prox) {
    callProx(x, gamma, prox);
    f_at_prox = 0.0;
    return ForBESUtils::STATUS_OK;
}

FunctionOntologicalClass IndBall2::category() {
    FunctionOntologicalClass meta("IndBall2");
    meta.add_superclass(FunctionOntologyRegistry::indicator());
    meta.set_defines_prox(true);
    return meta;
}
