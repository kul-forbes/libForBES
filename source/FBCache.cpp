#include "FBCache.h"
#include "LinearOperator.h"

void FBCache::init() {
    m_flag_evalFBE = 0;
    m_flag_evalf = 0;
    m_flag_gradFBE = 0;
    m_flag_gradstep = 0;
    m_flag_proxgradstep = 0;
}

FBCache::FBCache(FBProblem & p, Matrix & x, double gamma) : m_prob(p), m_x(x), m_gamma(gamma) {
    init();
    // store pointers to problem and all relevant details
    m_f1 = p.f1();
    m_L1 = p.L1();
    m_d1 = p.d1();
    m_f2 = p.f2();
    m_L2 = p.L2();
    m_d2 = p.d2();
    m_lin = p.lin();
    m_g = p.g();

    // get dimensions of things
    x_rows = m_x.getNrows();
    x_cols = m_x.getNcols();

    if (m_d1 != NULL) {
        res1_rows = m_d1->getNrows();
        res1_cols = m_d1->getNcols();
    } else if (m_L1 != NULL) {
        std::pair<size_t, size_t> res1_size = m_L1->dimensionOut();
        res1_rows = res1_size.first;
        res1_cols = res1_size.second;
    } else {
        res1_rows = x_rows;
        res1_cols = x_cols;
    }
    if (m_d2 != NULL) {
        res2_rows = m_d2->getNrows();
        res2_cols = m_d2->getNcols();
    } else if (m_L2 != NULL) {
        std::pair<size_t, size_t> res2_size = m_L2->dimensionOut();
        res2_rows = res2_size.first;
        res2_cols = res2_size.second;
    } else {
        res2_rows = x_rows;
        res2_cols = x_cols;
    }

    // allocate memory for residuals and gradients (where needed)
    if (m_f1 != NULL) {
        m_res1x = new Matrix(res1_rows, res1_cols);
        m_gradf1x = new Matrix(res1_rows, res1_cols);
    } else {
        m_res1x = NULL;
        m_gradf1x = NULL;
    }
    if (m_f2 != NULL) {
        m_res2x = new Matrix(res2_rows, res2_cols);
        m_gradf2x = new Matrix(res2_rows, res2_cols);
    } else {
        m_res2x = NULL;
        m_gradf2x = NULL;
    }

    m_gradfx = new Matrix(x_rows, x_cols);
    m_z = new Matrix(x_rows, x_cols);
    m_y = new Matrix(x_rows, x_cols);
    m_FPR = new Matrix(x_rows, x_cols);

    m_flag_evalf = 0;
    m_flag_gradstep = 0;
    m_flag_proxgradstep = 0;

    m_f1x = 0.0;
    m_f2x = 0.0;
    m_linx = 0.0;
    m_fx = 0.0;
    m_gz = 0.0;
}

FBCache::~FBCache() {
    if (m_z != NULL) {
        delete m_z;
        m_z = NULL;
    }
    if (m_y != NULL) {
        delete m_y;
        m_y = NULL;
    }
    if (m_res2x != NULL) {
        delete m_res2x;
        m_res2x = NULL;
    }
    if (m_gradf2x != NULL) {
        delete m_gradf2x;
        m_gradf2x = NULL;
    }
    if (m_res1x != NULL) {
        delete m_res1x;
        m_res1x = NULL;
    }
    if (m_gradf1x != NULL) {
        delete m_gradf1x;
        m_gradf1x = NULL;
    }
    if (m_gradfx != NULL) {
        delete m_gradfx;
        m_gradfx = NULL;
    }

}

int FBCache::update_eval_f() {

    if (m_flag_evalf == 1) {
        return ForBESUtils::STATUS_OK;
    }

    if (m_f1 != NULL) {
        if (m_L1 != NULL) {
            *m_res1x = m_L1->call(m_x);
        } else {
            *m_res1x = m_x;
        }
        if (m_d1 != NULL) {
            *m_res1x += *m_d1;
        }
        int status = m_f1->call(*m_res1x, m_f1x, *m_gradf1x);
        if (ForBESUtils::STATUS_OK != status) {
            return status;
        }
    }

    if (m_f2 != NULL) {
        if (m_L2 != NULL) *m_res2x = m_L2->call(m_x);
        else *m_res2x = m_x;
        if (m_d2 != NULL) *m_res2x += *m_d2;
        int status = m_f2->call(*m_res2x, m_f2x);
        if (ForBESUtils::STATUS_OK != status) {
            return status;
        }
    }

    if (m_lin != NULL) {
        m_linx = ((*m_lin) * m_x)[0];
    }

    m_fx = m_f1x + m_f2x + m_linx;
    m_flag_evalf = 1;
    return ForBESUtils::STATUS_OK;
}

int FBCache::update_forward_step(double gamma) {

    if (m_flag_evalf == 0) {
        int status = update_eval_f();
    }

    if (m_flag_gradstep == 1) {
        if (gamma != this->m_gamma) {
            this->m_gamma = gamma;
            // *m_y = m_x - gamma * (*m_gradfx);
            *m_y = m_x;
            Matrix::add(*m_y, -gamma, *m_gradfx, 1.0);
        }
        return ForBESUtils::STATUS_OK;
    }

    m_gamma = gamma;

    if (m_f1 != NULL) {
        if (m_L1) {
            Matrix d_gradfx = m_L1->callAdjoint(*m_gradf1x);
            *m_gradfx += d_gradfx;
        } else {
            *m_gradfx += *m_gradf1x;
        }
    }

    if (m_f2 != NULL) {
        m_f2->call(m_x, m_f2x, *m_gradf2x);
        if (m_L2) {
            Matrix d_gradfx = m_L2->callAdjoint(*m_gradf2x);
            *m_gradfx += d_gradfx;
        } else {
            *m_gradfx += *m_gradf2x;
        }
    }

    if (m_lin) {
        *m_gradfx += (*m_lin);
    }

    // *m_y = m_x - gamma * (*m_gradfx);
    *m_y = m_x;
    Matrix::add(*m_y, -gamma, *m_gradfx, 1.0);
    
    m_flag_gradstep = 1;

    return ForBESUtils::STATUS_OK;
}

int FBCache::update_forward_backward_step(double gamma) {
    double gamma0 = this->m_gamma;

    if (m_flag_gradstep == 0 || gamma != gamma0) {
        int status = update_forward_step(gamma);
    }

    if (m_flag_proxgradstep == 0 || gamma != gamma0) {
        int status = m_g->callProx(*m_y, gamma, *m_z, m_gz);
    }

    *m_FPR = (m_x - *m_z);

    m_sqnormFPRx = 0;
    for (int i = 0; i < m_FPR->length(); i++) {
        m_sqnormFPRx += (*m_FPR)[i]*(*m_FPR)[i];
    }

    this->m_gamma = gamma;
    m_flag_proxgradstep = 1;

    return ForBESUtils::STATUS_OK;
}

int FBCache::update_eval_FBE(double gamma) {
    if (m_flag_evalFBE == 1 && gamma == m_gamma) {
        return ForBESUtils::STATUS_OK;
    }

    if (m_flag_proxgradstep == 0 || gamma != m_gamma) {
        int status = m_g->callProx(*m_y, gamma, *m_z, m_gz);
    }

    double innprod = 0;

    for (int i = 0; i < x_rows; i++) {
        for (int j = 0; j < x_cols; j++) {
            innprod += m_FPR->get(i, j) * m_gradfx->get(i, j);
        }
    }

    m_FBEx = m_fx + m_gz - innprod + 0.5 / m_gamma*m_sqnormFPRx;

    this->m_gamma = gamma;
    m_flag_evalFBE = 1;

    return ForBESUtils::STATUS_OK;
}

int FBCache::update_grad_FBE(double gamma) {
    if (m_flag_gradFBE == 1 && gamma == m_gamma) {
        return ForBESUtils::STATUS_OK;
    }

    if (m_flag_proxgradstep == 0 || gamma != m_gamma) {
        int status = m_g->callProx(*m_y, gamma, *m_z, m_gz);
    }

    /* TODO: fill in here */

    this->m_gamma = gamma;
    m_flag_gradFBE = 1;

    return ForBESUtils::STATUS_UNDEFINED_FUNCTION;
}

double FBCache::get_eval_FBE(double gamma) {
    int status = update_eval_FBE(gamma);
    return m_FBEx;
}

double FBCache::get_eval_f() {
    int status = update_eval_f();
    return m_fx;
}

Matrix* FBCache::get_forward_backward_step(double gamma) {
    update_forward_backward_step(gamma);
    return m_z;
}



