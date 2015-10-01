/* 
 * File:   OpComposition.h
 * Author: Pantelis Sopasakis
 *
 * Created on September 14, 2015, 9:26 PM
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
 * 
 */

#ifndef OPCOMPOSITION_H
#define	OPCOMPOSITION_H

#include "LinearOperator.h"

/**
 * \class OpComposition
 * \brief The composition of two linear operators <code>T(x) = A(B(x))</code>
 * \version 0.0-tentative
 * \author Pantelis Sopasakis
 * \date Created on September 14, 2015, 9:26 PM
 * 
 * \ingroup LinOp
 */
class OpComposition : public LinearOperator {
public:        

    OpComposition(LinearOperator& A, LinearOperator& B) : LinearOperator(), A(A), B(B) {
        // check dimensions
        if (A.dimensionIn() != B.dimensionOut()) {
            throw std::invalid_argument("A and B have incompatible dimensions; AoB is not well defined.");
        }
    }

    virtual ~OpComposition();

    virtual Matrix call(Matrix& x);

    virtual size_t dimensionIn();

    virtual size_t dimensionOut();

    virtual bool isSelfAdjoint();

    virtual Matrix callAdjoint(Matrix& x);

private:

    LinearOperator& A;
    LinearOperator& B;
};

#endif	/* OPCOMPOSITION_H */
