/* 
 * File:   ConjugateFunction.cpp
 * Author: chung
 * 
 * Created on November 7, 2015, 3:15 AM
 */

#include "ConjugateFunction.h"

ConjugateFunction::ConjugateFunction(Function& funct) :
Function(), m_function(funct) {
    std::cout << "\n conjugate of " << funct.category().getName();
}

ConjugateFunction::~ConjugateFunction() {
}

int ConjugateFunction::call(Matrix& x, double& f) {
    return m_function.callConj(x, f);
}

int ConjugateFunction::call(Matrix& x, double& f, Matrix& grad) {
    return m_function.callConj(x, f, grad);
}

int ConjugateFunction::callConj(Matrix& x, double& f_star, Matrix& grad) {
    return m_function.call(x, f_star, grad);
}

int ConjugateFunction::callConj(Matrix& x, double& f_star) {
    return m_function.call(x, f_star);
}

int ConjugateFunction::callProx(Matrix& x, double gamma, Matrix& prox) {
    int status = m_function.callProx(x, gamma, prox);
    for (size_t i = 0; i < x.getNrows(); i++) {
        prox.set(i, 0, x.get(i, 0) - prox.get(i, 0));
    }
    return status;
}

int ConjugateFunction::callProx(Matrix& x, double gamma, Matrix& prox, double& f_at_prox) {
    int status = callProx(x, gamma, prox);
    if (ForBESUtils::STATUS_OK != status){
        return status;
    }
    return call(prox, f_at_prox);
}

FunctionOntologicalClass ConjugateFunction::category() {
    FunctionOntologicalClass meta("Conjugate");
    FunctionOntologicalClass orig_meta = m_function.category();
    meta.set_defines_f(orig_meta.defines_conjugate());
    meta.set_defines_grad(orig_meta.defines_conjugate_grad());
    meta.set_defines_conjugate(orig_meta.defines_f());
    meta.set_defines_conjugate_grad(orig_meta.defines_grad());
    
    return meta;
}




