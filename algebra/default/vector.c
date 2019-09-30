#include "osqp_api_types.h"
#include "algebra_vector.h"
#include "algebra_types.h"


/*******************************************************************************
 *                       External CUDA Functions                               *
 *******************************************************************************/

/* cuda_malloc.h */
extern void cuda_malloc(void** devPtr, size_t size);
extern void cuda_calloc(void** devPtr, size_t size);
extern void cuda_free(void** devPtr);

/* cuda_lin_alg.h */
extern void cuda_vec_copy_d2d(c_float *d_y, const c_float *d_x, c_int n);
extern void cuda_vec_copy_h2d(c_float *d_y, const c_float *h_x, c_int n);
extern void cuda_vec_copy_d2h(c_float *h_y, const c_float *d_x, c_int n);
extern void cuda_vec_set_sc(c_float *d_a, c_float sc, c_int n);
extern void cuda_vec_set_sc_cond(c_float *d_a, const c_int *d_test, c_float sc_if_neg, c_float sc_if_zero, c_float sc_if_pos, c_float n);
extern void cuda_vec_mult_sc(c_float *d_a, c_float sc, c_int n);
extern void cuda_vec_add_scaled(c_float *d_x, const c_float *d_a, const c_float *d_b, c_float sca, c_float scb, c_int n);
extern void cuda_vec_add_scaled3(c_float *d_x, const c_float *d_a, const c_float *d_b, const c_float *d_c, c_float sca, c_float scb, c_float scc, c_int n);
extern void cuda_vec_norm_inf(const c_float *d_x, c_int n, c_float *h_res);
extern void cuda_vec_norm_1(const c_float *d_x, c_int n, c_float *h_res);
extern void cuda_vec_scaled_norm_inf(const c_float *d_S, const c_float *d_v, c_int n, c_float *h_res);
extern void cuda_vec_diff_norm_inf(const c_float *d_a, const c_float *d_b, c_int n, c_float *h_res);
extern void cuda_vec_mean(const c_float *d_x, c_int n, c_float *h_res);
extern void cuda_vec_prod(const c_float *d_a, const c_float *d_b, c_int n, c_float *h_res);
extern void cuda_vec_prod_signed(const c_float *d_a, const c_float *d_b, c_int sign, c_int n, c_float *h_res);
extern void cuda_vec_ew_prod(c_float *d_c, const c_float *d_a, const c_float *d_b, c_int n);
extern void cuda_vec_leq(const c_float *d_l, const c_float *d_u, c_int n, c_int *h_res);
extern void cuda_vec_bound(c_float *d_x, const c_float *d_z, const c_float *d_l, const c_float *d_u, c_int n);
extern void cuda_vec_project_polar_reccone(c_float *d_y, const c_float *d_l, const c_float *d_u, c_float infval, c_int n);
extern void cuda_vec_in_reccone(const c_float *d_y, const c_float *d_l, const c_float *d_u, c_float infval, c_float tol, c_int n, c_int *h_res);
extern void cuda_vec_reciprocal(c_float *d_b, const c_float *d_a, c_int n);
extern void cuda_vec_sqrt(c_float *d_a, c_int n);
extern void cuda_vec_max(c_float *d_c, const c_float *d_a, const c_float *d_b, c_int n);
extern void cuda_vec_min(c_float *d_c, const c_float *d_a, const c_float *d_b, c_int n);
extern void cuda_vec_bounds_type(c_int *d_iseq, const c_float *d_l, const c_float *d_u, c_float infval, c_float tol, c_int n, c_int *h_has_changed);
extern void cuda_vec_set_sc_if_lt(c_float *d_x, const c_float *d_z, c_float testval, c_float newval, c_int n);
extern void cuda_vec_set_sc_if_gt(c_float *d_x, const c_float *d_z, c_float testval, c_float newval, c_int n);


/*******************************************************************************
 *                           API Functions                                     *
 *******************************************************************************/

OSQPVectorf* OSQPVectorf_new(const c_float *a,
                             c_int          length) {

  OSQPVectorf* out = OSQPVectorf_malloc(length);
  if (!out) return OSQP_NULL;

  if (length > 0) OSQPVectorf_from_raw(out, a);
  
  return out;
}

OSQPVectorf* OSQPVectorf_malloc(c_int length) {

  OSQPVectorf *b = (OSQPVectorf*) c_malloc(sizeof(OSQPVectorf));
  if (!b) return OSQP_NULL;

  b->length = length;
  if (length) {
    b->values = (c_float*) c_malloc(length * sizeof(c_float));
    cuda_malloc((void **) &b->d_val, length * sizeof(c_float));
    if (!(b->values) || !(b->d_val)) {
      c_free(b->values);
      cuda_free((void **) &b->d_val);
      c_free(b);
      b = OSQP_NULL;
    }
  }
  else {
    b->values = OSQP_NULL;
    b->d_val  = OSQP_NULL;
  }
  return b;
}

OSQPVectori* OSQPVectori_malloc(c_int length) {

  OSQPVectori *b = (OSQPVectori*) c_malloc(sizeof(OSQPVectori));
  if (!b) return OSQP_NULL;

  b->length = length;
  if (length) {
    b->values = (c_int*) c_malloc(length * sizeof(c_int));
    cuda_malloc((void **) &b->d_val, length * sizeof(c_int));
    if (!(b->values)) {
      c_free(b->values);
      cuda_free((void **) &b->d_val);
      c_free(b);
      b = OSQP_NULL;
    }
  }
  else {
    b->values = OSQP_NULL;
    b->d_val  = OSQP_NULL;
  }
  return b;
}

OSQPVectorf* OSQPVectorf_calloc(c_int length) {

  OSQPVectorf *b = (OSQPVectorf*) c_malloc(sizeof(OSQPVectorf));
  if (!b) return OSQP_NULL;

  b->length = length;
  if (length) {
    b->values = (c_float*) c_calloc(length, sizeof(c_float));
    cuda_calloc((void **) &b->d_val, length * sizeof(c_float));
    if (!(b->values)) {
      c_free(b->values);
      cuda_free((void **) &b->d_val);
      c_free(b);
      b = OSQP_NULL;
    }
  }
  else {
    b->values = OSQP_NULL;
    b->d_val  = OSQP_NULL;
  }
  return b;
}

OSQPVectori* OSQPVectori_calloc(c_int length) {

  OSQPVectori *b = (OSQPVectori*) c_malloc(sizeof(OSQPVectori));
  if (!b) return OSQP_NULL;
  
  b->length = length;
  if (length) {
    b->values = (c_int*) c_calloc(length, sizeof(c_int));
    cuda_calloc((void **) &b->d_val, length * sizeof(c_int));
    if (!(b->values)) {
      c_free(b->values);
      cuda_free((void **) &b->d_val);
      c_free(b);
      b = OSQP_NULL;
    }
  }
  else {
    b->values = OSQP_NULL;
    b->d_val  = OSQP_NULL;
  }
  return b;
}

OSQPVectorf* OSQPVectorf_copy_new(const OSQPVectorf *a) {

  OSQPVectorf* b = OSQPVectorf_malloc(a->length);
  if (b) OSQPVectorf_copy(b, a);
  if (b) cuda_vec_copy_d2d(b->d_val, a->d_val, a->length);

  return b;
}

void OSQPVectorf_free(OSQPVectorf *a) {
  if (a) {
    c_free(a->values);
    cuda_free((void **) &a->d_val);
  }
  c_free(a);
}

void OSQPVectori_free(OSQPVectori *a) {
  if (a) {
    c_free(a->values);
    cuda_free((void **) &a->d_val);
  }
  c_free(a);
}

OSQPVectorf* OSQPVectorf_view(const OSQPVectorf *a,
                              c_int              head,
                              c_int              length) {

  OSQPVectorf* view = (OSQPVectorf*) c_malloc(sizeof(OSQPVectorf));
  if (view) {
    view->length = length;
    view->values = a->values + head;
    view->d_val  = a->d_val  + head;
  }
  return view;
}

void OSQPVectorf_view_free(OSQPVectorf *a) {
  c_free(a);
}

c_int OSQPVectorf_length(const OSQPVectorf *a) {return a->length;}
c_int OSQPVectori_length(const OSQPVectori *a) {return a->length;}

/* Pointer to vector data (floats) */
c_float* OSQPVectorf_data(const OSQPVectorf *a){return a->values;}
c_int*   OSQPVectori_data(const OSQPVectori *a){return a->values;}

void OSQPVectorf_copy(OSQPVectorf       *b,
                      const OSQPVectorf *a) {

  OSQPVectorf_from_raw(b, a->values);
}

void OSQPVectorf_from_raw(OSQPVectorf   *b,
                          const c_float *av) {

  c_int i;
  c_int length = b->length;
  c_float* bv  = b->values;

  for (i = 0; i < length; i++) {
    bv[i] = av[i];
  }
  cuda_vec_copy_h2d(b->d_val, av, b->length);
}

void OSQPVectorf_to_raw(c_float           *bv,
                        const OSQPVectorf *a) {

  c_int i;
  c_int length = a->length;
  c_float* av = a->values;

  for (i = 0; i < length; i++) {
    bv[i] = av[i];
  }
  // cuda_vec_copy_d2h(bv, a->d_val, a->length);
}

void OSQPVectorf_set_scalar(OSQPVectorf *a,
                            c_float      sc) {

  c_int i;
  c_int length = a->length;
  c_float* av  = a->values;

  for (i = 0; i < length; i++) {
    av[i] = sc;
  }
  cuda_vec_set_sc(a->d_val, sc, a->length);
}


void OSQPVectorf_set_scalar_conditional(OSQPVectorf       *a,
                                        const OSQPVectori *test,
                                        c_float            sc_if_neg,
                                        c_float            sc_if_zero,
                                        c_float            sc_if_pos) {

  c_int     i;
  c_int     length = a->length;
  c_float*  av     = a->values;
  c_int*    testv  = test->values;

  for (i = 0; i < length; i++) {
      if (testv[i] == 0)      av[i] = sc_if_zero;
      else if (testv[i] > 0)  av[i] = sc_if_pos;
      else                    av[i] = sc_if_neg;
  }
  cuda_vec_set_sc_cond(a->d_val, test->d_val, sc_if_neg, sc_if_zero, sc_if_pos, a->length);
}


void OSQPVectorf_mult_scalar(OSQPVectorf *a,
                             c_float      sc) {

  c_int i;
  c_int length = a->length;
  c_float*  av = a->values;

  for (i = 0; i < length; i++) {
    av[i] *= sc;
  }

  if (sc == 1.0 || !a->d_val) return;
  cuda_vec_mult_sc(a->d_val, sc, a->length);
}

void OSQPVectorf_plus(OSQPVectorf      *x,
                     const OSQPVectorf *a,
                     const OSQPVectorf *b) {

  c_int i;
  c_int length = a->length;
  c_float*  av = a->values;
  c_float*  bv = b->values;
  c_float*  xv = x->values;

  if (x == a) {
    for (i = 0; i < length; i++) {
      xv[i] += bv[i];
    }
  }
  else {
    for (i = 0; i < length; i++) {
      xv[i] = av[i] + bv[i];
    }
  }
  cuda_vec_add_scaled(x->d_val, a->d_val, b->d_val, 1.0, 1.0, a->length);
}

void OSQPVectorf_minus(OSQPVectorf       *x,
                       const OSQPVectorf *a,
                       const OSQPVectorf *b) {

  c_int i;
  c_int length = a->length;
  c_float*  av = a->values;
  c_float*  bv = b->values;
  c_float*  xv = x->values;

  if (x == a) {
    for (i = 0; i < length; i++) {
      xv[i] -= bv[i];
    }
  }
  else {
    for (i = 0; i < length; i++) {
      xv[i] = av[i] - bv[i];
    }
  }
  cuda_vec_add_scaled(x->d_val, a->d_val, b->d_val, 1.0, -1.0, a->length);
}


void OSQPVectorf_add_scaled(OSQPVectorf       *x,
                            c_float            sca,
                            const OSQPVectorf *a,
                            c_float            scb,
                            const OSQPVectorf *b) {

  c_int i;
  c_int length = x->length;
  c_float*  av = a->values;
  c_float*  bv = b->values;
  c_float*  xv = x->values;

  /* shorter version when incrementing */
  if (x == a && sca == 1.){
    for (i = 0; i < length; i++) {
      xv[i] += scb * bv[i];
    }
  }
  else {
    for (i = 0; i < length; i++) {
      xv[i] = sca * av[i] + scb * bv[i];
    }
  }
  cuda_vec_add_scaled(x->d_val, a->d_val, b->d_val, sca, scb, x->length);
}

void OSQPVectorf_add_scaled3(OSQPVectorf       *x,
                             c_float            sca,
                             const OSQPVectorf *a,
                             c_float            scb,
                             const OSQPVectorf *b,
                             c_float            scc,
                             const OSQPVectorf *c) {

  c_int i;
  c_int length = x->length;
  c_float*  av = a->values;
  c_float*  bv = b->values;
  c_float*  cv = c->values;
  c_float*  xv = x->values;

  /* shorter version when incrementing */
  if (x == a && sca == 1.){
    for (i = 0; i < length; i++) {
      xv[i] += scb * bv[i] + scc * cv[i];
    }
  }
  else {
    for (i = 0; i < length; i++) {
      xv[i] =  sca * av[i] + scb * bv[i] + scc * cv[i];
    }
  }
  cuda_vec_add_scaled3(x->d_val, a->d_val, b->d_val, c->d_val, sca, scb, scc, x->length);
}


c_float OSQPVectorf_norm_inf(const OSQPVectorf *v) {

  c_int   i;
  c_int length  = v->length;
  c_float*  vv  = v->values;
  c_float normval = 0.0;
  c_float absval, cuda_normval;

  if (v->length) cuda_vec_norm_inf(v->d_val, v->length, &cuda_normval);
  else           cuda_normval = 0.0;

  for (i = 0; i < length; i++) {
    absval = c_absval(vv[i]);
    if (absval > normval) normval = absval;
  }
  return normval;
}

c_float OSQPVectorf_norm_1(const OSQPVectorf *v) {

  c_int   i;
  c_int length  = v->length;
  c_float*  vv  = v->values;
  c_float normval = 0.0;
  c_float cuda_normval;

  if (v->length) cuda_vec_norm_1(v->d_val, v->length, &cuda_normval);
  else           cuda_normval = 0.0;

  for (i = 0; i < length; i++) {
    normval += c_absval(vv[i]);
  }
  return normval;
}

c_float OSQPVectorf_scaled_norm_inf(const OSQPVectorf *S,
                                    const OSQPVectorf *v) {

  c_int   i;
  c_int length  = v->length;
  c_float*  vv  = v->values;
  c_float*  Sv  = S->values;
  c_float absval;
  c_float normval = 0.0;
  c_float cuda_normval;

  if (v->length) cuda_vec_scaled_norm_inf(S->d_val, v->d_val, v->length, &cuda_normval);
  else           cuda_normval = 0.0;

  for (i = 0; i < length; i++) {
    absval = c_absval(Sv[i] * vv[i]);
    if (absval > normval) normval = absval;
  }
  return normval;
}

c_float OSQPVectorf_norm_inf_diff(const OSQPVectorf *a,
                                  const OSQPVectorf *b) {

  c_int   i;
  c_int   length = a->length;
  c_float*  av   = a->values;
  c_float*  bv   = b->values;
  c_float absval;
  c_float normDiff = 0.0;
  c_float cuda_normDiff;

  if (a->length) cuda_vec_diff_norm_inf(a->d_val, b->d_val, a->length, &cuda_normDiff);
  else           cuda_normDiff = 0.0;

  for (i = 0; i < length; i++) {
    absval = c_absval(av[i] - bv[i]);
    if (absval > normDiff) normDiff = absval;
  }
  return normDiff;
}

// REMOVE
c_float OSQPVectorf_sum(const OSQPVectorf *a) {

  c_int   i;
  c_int   length = a->length;
  c_float*  av   = a->values;
  c_float val = 0.0;

  for (i = 0; i < length; i++) {
    val += av[i];
  }

  return val;
}

c_float OSQPVectorf_mean(const OSQPVectorf *a) {

  c_float cuda_val;

  if (a->length) cuda_vec_mean(a->d_val, a->length, &cuda_val);
  else           cuda_val = 0.0;

  if (a->length) {
    return OSQPVectorf_sum(a) / (a->length);
  }
  else {
    return 0;
  }
}

c_float OSQPVectorf_dot_prod(const OSQPVectorf *a,
                             const OSQPVectorf *b) {

  c_int   i;
  c_int   length = a->length;
  c_float*  av   = a->values;
  c_float*  bv   = b->values;
  c_float dotprod = 0.0;
  c_float cuda_res;

  if (a->length) cuda_vec_prod(a->d_val, b->d_val, a->length, &cuda_res);
  else           cuda_res = 0.0;

  for (i = 0; i < length; i++) {
    dotprod += av[i] * bv[i];
  }
  return dotprod;
}

c_float OSQPVectorf_dot_prod_signed(const OSQPVectorf *a,
                                    const OSQPVectorf *b,
                                    c_int              sign) {

  c_int   i;
  c_int   length = a->length;
  c_float*  av   = a->values;
  c_float*  bv   = b->values;
  c_float dotprod = 0.0;
  c_float cuda_res;

  if (a->length) cuda_vec_prod_signed(a->d_val, b->d_val, sign, a->length, &cuda_res);
  else           cuda_res = 0.0;

  if (sign == 1) {  /* dot with positive part of b */
    for (i = 0; i < length; i++) {
      dotprod += av[i] * c_max(bv[i], 0.);
    }
  }
  else if (sign == -1){  /* dot with negative part of b */
    for (i = 0; i < length; i++) {
      dotprod += av[i] * c_min(bv[i], 0.);
    }
  }
  else{
    /* return the conventional dot product */
    dotprod = OSQPVectorf_dot_prod(a, b);
  }
  return dotprod;
}

void OSQPVectorf_ew_prod(OSQPVectorf       *c,
                         const OSQPVectorf *a,
                         const OSQPVectorf *b) {

  c_int i;
  c_int   length = a->length;
  c_float*  av   = a->values;
  c_float*  bv   = b->values;
  c_float*  cv   = c->values;

  if (c->length) cuda_vec_ew_prod(c->d_val, a->d_val, b->d_val, c->length);

  if (c == a) {
    for (i = 0; i < length; i++) {
      cv[i] *= bv[i];
    }
  }
  else {
    for (i = 0; i < length; i++) {
      cv[i] = av[i] * bv[i];
    }
  }
}

c_int OSQPVectorf_all_leq(OSQPVectorf *l,
                          OSQPVectorf *u) {

  c_int i;
  c_int length = l->length;
  c_float*  lv = l->values;
  c_float*  uv = u->values;
  c_int  cuda_res;

  cuda_vec_leq(l->d_val, u->d_val, l->length, &cuda_res);

  for (i = 0; i < length; i++) {
    if (lv[i] > uv[i]) return 0;
  }
  return 1;
}

void OSQPVectorf_ew_bound_vec(OSQPVectorf       *x,
                              const OSQPVectorf *z,
                              const OSQPVectorf *l,
                              const OSQPVectorf *u) {

  c_int i;
  c_int length = x->length;
  c_float*  xv = x->values;
  c_float*  zv = z->values;
  c_float*  lv = l->values;
  c_float*  uv = u->values;

  cuda_vec_bound(x->d_val, z->d_val, l->d_val, u->d_val, x->length);

  for (i = 0; i < length; i++) {
    xv[i] = c_min(c_max(zv[i], lv[i]), uv[i]);
  }
}

void OSQPVectorf_project_polar_reccone(OSQPVectorf       *y,
                                       const OSQPVectorf *l,
                                       const OSQPVectorf *u,
                                       c_float            infval) {

  c_int i; // Index for loops
  c_int    length = y->length;
  c_float* yv = y->values;
  c_float* lv = l->values;
  c_float* uv = u->values;

  cuda_vec_project_polar_reccone(y->d_val, l->d_val, u->d_val, infval, y->length);

  for (i = 0; i < length; i++) {
    if (uv[i]   > +infval) {       // Infinite upper bound
      if (lv[i] < -infval) {       // Infinite lower bound
        // Both bounds infinite
        yv[i] = 0.0;
      } else {
        // Only upper bound infinite
        yv[i] = c_min(yv[i], 0.0);
      }
    } else if (lv[i] < -infval) {  // Infinite lower bound
      // Only lower bound infinite
      yv[i] = c_max(yv[i], 0.0);
    }
  }
}

c_int OSQPVectorf_in_reccone(const OSQPVectorf *y,
                             const OSQPVectorf *l,
                             const OSQPVectorf *u,
                             c_float            infval,
                             c_float            tol) {

  c_int i; // Index for loops

  c_int    length = y->length;
  c_float* yv     = y->values;
  c_float* lv     = l->values;
  c_float* uv     = u->values;
  c_int cuda_res;

  cuda_vec_in_reccone(y->d_val, l->d_val, u->d_val, infval, tol, y->length, &cuda_res);

  for (i = 0; i < length; i++) {
    if (((uv[i] < +infval) &&
         (yv[i] > +tol)) ||
        ((lv[i] > -infval) &&
         (yv[i] < -tol))) {
      // At least one condition not satisfied -> not dual infeasible
      return 0;
    }
  }
  return 1;
}

void OSQPVectorf_ew_reciprocal(OSQPVectorf       *b,
                               const OSQPVectorf *a) {

  c_int i;
  c_int length = a->length;
  c_float*  av = a->values;
  c_float*  bv = b->values;

  if (b->length) cuda_vec_reciprocal(b->d_val, a->d_val, b->length);

  for (i = 0; i < length; i++) {
    bv[i] = (c_float)1.0 / av[i];
  }
}

void OSQPVectorf_ew_sqrt(OSQPVectorf *a) {

  c_int i;
  c_int length = a->length;
  c_float*  av = a->values;

  if (a->length) cuda_vec_sqrt(a->d_val, a->length);

  for (i = 0; i < length; i++) {
    av[i] = c_sqrt(av[i]);
  }
}

void OSQPVectorf_ew_max_vec(OSQPVectorf       *c,
                            const OSQPVectorf *a,
                            const OSQPVectorf *b) {

  c_int i;
  c_int length = a->length;
  c_float*  av = a->values;
  c_float*  bv = b->values;
  c_float*  cv = c->values;

  if (c->length) cuda_vec_max(c->d_val, a->d_val, b->d_val, c->length);

  for (i = 0; i < length; i++) {
    cv[i] = c_max(av[i], bv[i]);
  }
}

void OSQPVectorf_ew_min_vec(OSQPVectorf       *c,
                            const OSQPVectorf *a,
                            const OSQPVectorf *b) {

  c_int i;
  c_int length = a->length;
  c_float*  av = a->values;
  c_float*  bv = b->values;
  c_float*  cv = c->values;

  if (c->length) cuda_vec_min(c->d_val, a->d_val, b->d_val, c->length);

  for (i = 0; i < length; i++) {
    cv[i] = c_min(av[i], bv[i]);
  }
}

c_int OSQPVectorf_ew_bounds_type(OSQPVectori      *iseq,
                                const OSQPVectorf *l,
                                const OSQPVectorf *u,
                                c_float            tol,
                                c_float            infval) {

  c_int i;
  c_int length   = iseq->length;
  c_int*   iseqv = iseq->values;
  c_float* lv    = l->values;
  c_float* uv    = u->values;
  c_int old_value;
  c_int has_changed = 0;
  c_int cuda_has_changed;

  cuda_vec_bounds_type(iseq->d_val, l->d_val, u->d_val, infval, tol, iseq->length, &cuda_has_changed);

  for (i = 0; i < length; i++) {

    old_value = iseqv[i];

    if ((lv[i] < -infval) && (uv[i] > infval)) {
      // Loose bounds
      iseqv[i] = -1;
    } else if (uv[i] - lv[i] < tol) {
      // Equality constraints
      iseqv[i] = 1;
    } else {
      // Inequality constraints
      iseqv[i] = 0;
    }

    //has anything changed?
    has_changed = has_changed || (iseqv[i] != old_value);
  }

  return has_changed;
}

void OSQPVectorf_set_scalar_if_lt(OSQPVectorf       *x,
                                  const OSQPVectorf *z,
                                  c_float            testval,
                                  c_float            newval) {

  c_int i;
  c_int length = x->length;
  c_float*  xv = x->values;
  c_float*  zv = z->values;

  cuda_vec_set_sc_if_lt(x->d_val, z->d_val, testval, newval, x->length);

  for (i = 0; i < length; i++) {
    xv[i] = zv[i] < testval ? newval : zv[i];
  }
}

void OSQPVectorf_set_scalar_if_gt(OSQPVectorf       *x,
                                  const OSQPVectorf *z,
                                  c_float            testval,
                                  c_float            newval) {

  c_int i;
  c_int length = x->length;
  c_float*  xv = x->values;
  c_float*  zv = z->values;

  cuda_vec_set_sc_if_gt(x->d_val, z->d_val, testval, newval, x->length);

  for (i = 0; i < length; i++) {
    xv[i] = zv[i] > testval ? newval : zv[i];
  }
}
