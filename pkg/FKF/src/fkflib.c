#include <R.h>
#include <Rdefines.h>
#include <Rinternals.h>
#include <Rmath.h>
#include <R_ext/BLAS.h>
#include <R_ext/Lapack.h>

/* Macro to transform an index of a 2-dimensional array into an index
   of a vector */
#define IDX(i,j,dim0) (i) + (j) * (dim0)

/* If DEBUG_PRINT is defined all inputs and outputs will be printed */
/* #define DEBUG_PRINT  */

/* Function to print a 2-dimensional array */
void print_array(double * data, int i, int j, const char * lab)
{
    int icnt, jcnt;
    Rprintf("\n'%s':\n", lab);
    for(icnt = 0; icnt < i; icnt++){
	for(jcnt = 0; jcnt < j; jcnt++){
/* 	    Rprintf("\nIDX(icnt, jcnt, j) = %d\n", IDX(icnt, jcnt, i)); */
	    Rprintf("%3.5f   ", data[IDX(icnt, jcnt, i)]);
	}
	Rprintf("\n");
    }

}

void FKFmirrorLU(double * data, int dim, char * uplo)
{
    int i, j;
    char * ref = "U";
    if(strcmp(uplo, ref) == 0){
	for(j = 1; j < dim; j++){
	    for(i = 0; i < j; i++){
		data[IDX(j, i, dim)] = data[IDX(i, j, dim)];
	    }
	}
/* 	Rprintf("FKFmirrorLU '%s'\n", uplo); */
    }else{
/* 	Rprintf("FKFmirrorLU must not be 'U' '%s'\n", uplo); */
	for(j = 1; j < dim; j++){
	    for(i = 0; i < j; i++){
		data[IDX(i, j, dim)] = data[IDX(j, i, dim)];
	    }
	}
    }
}

void cfkf(/* inputs */
	  int m, int d, int n,
	  double * a0, double * P0,
	  double * dt, int incdt,
	  double * ct, int incct,
	  double * Tt, int incTt,
	  double * Zt, int incZt,
	  double * HHt, int incHHt,
	  double * GGt, int incGGt,
	  double * yt,
	  /* outputs */
	  double * at, double * att,
	  double * Pt, double * Ptt,
	  double * vt, double * Ft,
	  double * Kt,
	  double * loglik, int * status)

{
  /*  Description: */

  /*  In what follows, m denotes the dimension of the state vector, */
  /*  d denotes the dimension of the observations, and n denotes the */
  /*  number of observations. */

  /*  The state space model consists of the transition equation */
  /*  and the measurement equation: */

  /*  Transition equation: */
  /*   alpha(t + 1) = d(t) + T(t) * alpha(t) + e(t) */

  /*  Measurement equation: */
  /*   y(t) = c(t) + Z(t) * a(t) + u(t) */

  /*  e(t) and u(t) are independent innovations with zero */
  /*  expectation and variance HH(t) and GG(t), respectively. */
  /*  Covariance between e(t) and u(t) is not supported. */

  /*  The deterministic parameters admit the following dimensions: */
  /*   alpha(t) in R^(m) */
  /*   d(t) in R^(m) */
  /*   T(t) in R^(m x m) */
  /*   HH(t) in R^(m x m)     */

  /*   y(t) in R^(d) */
  /*   c(t) in R^(d) */
  /*   Z(t) in R^(d x m) */
  /*   GG(t) in R^(d x d) */

  /*  If the parameters are constant, i.e. x(i) = x(j) for i not equal to j, */
  /*  incx should be zero. Otherwise incx must be one, which means */
  /*  if x(t) in R^(i x j), x is an array of dimension R^(i x j x n), */
  /*  where n is the number of observations. */

  /*  The outputs at, att, Pt, Ptt, vt, Ft, Kt, ans status admit */
  /*  the following dimensions and interpretations: */

  /*  at in R^(m x (n + 1)), at(i) = E(alpha(i) | y(1), ..., y(i - 1)), at(0) = a0 */
  /*  att in R^(m x n), att(i) = E(alpha(i) | y(1), ..., y(i))  */
  /*  Pt in R^(m x m x (n + 1)), Pt(i) = var(alpha(i) | y(1), ..., y(i - 1)), Pt(0) = P0 */
  /*  Ptt in R^(m x m x n), Ptt(i) = var(alpha(i) | y(1), ..., y(i)) */
  /*  vt in R^(d x n), measurement equation error term v(i) = y(i) - c(i) - Z(i) * a(i) */
  /*  Ft in R^(d x d x n) */
  /*  Kt in R^(m x d x n) */
  /*  status in Z^2 */

  /*  loglik denotes the log-likelihood */

  /*  cfkf uses 'dcopy', 'dgemm' and 'daxpy' from BLAS and 'dpotri' and */
  /*  'dpotrf' from LAPACK. */
  /*  cfkf stops if dpotri or dpotrf return with a non-zero exit status */
  /*  for the computation of the inverse of F(t). potrf is also used to */
  /*  compute the determinant of F(t). However, if the determinant can not be */
  /*  calculated, the loop wont break, but the log-likelihood 'loglik' will be NA. */

  int m_x_m = m * m;
  int d_x_d = d * d;
  int m_x_d = m * d;

  int j;
  int i = 0;
  int potri_info = 0;
  int potrf_info = 0;
  int det_potrf_info = 0;

  /* integers and reals used in dcopy and dgemm */
  int intone = 1;
  double dblone = 1.0, dblminusone = -1.0, dblzero = 0.0;

  double mahalanobis, det, logDet;

  char  *transpose = "T", *dont_transpose = "N";
  char *upper_triangle = "U";
  char dpotri_uplo[1];

  strcpy(dpotri_uplo, "U");

  /* Temporary arrays */
  double *tmpdxm = (double *) Calloc(m_x_d, double);
  double *tmpmxm = (double *) Calloc(m_x_m, double);
  double *tmptmpmxm = (double *) Calloc(m_x_m, double);
  double *tmpdxd = (double *) Calloc(d_x_d, double);
  double *tmpFt_inv = (double *) Calloc(d_x_d, double);

  /* at = a0 */
  F77_NAME(dcopy)(&m, a0, &intone, at, &intone);

#ifdef DEBUG_PRINT
  print_array(at, 1, m, "a0:");
#endif

  /* Pt = P0 */
  F77_NAME(dcopy)(&m_x_m, P0, &intone, Pt, &intone);

#ifdef DEBUG_PRINT
  print_array(Pt, m, m, "P0:");
#endif

  /* Initialize the logLikelihood: loglik = - n * d * log(sqrt(2 * pi)) */
  *loglik = - (double)(n * d) *  M_LN_SQRT_2PI;

  /* ======================================================================  */
  /* Start filtering */
  /* ======================================================================  */
  while(i < n && potrf_info == 0 && potri_info == 0){
    /* potri_info and potrf_info are 0 as long as matrix inversion and
       and Cholesky factorization exit successfully */

#ifdef DEBUG_PRINT
    Rprintf("\n\nLoop Nr.:  %d\n", i);
#endif

    /* ---------------------------------------------------------------------- */
    /* Compute vt[,i] = yt[,i] - ct[,i * incct] - Zt[,,i * incZt] %*% at[,i] */
    /* ---------------------------------------------------------------------- */

    /* vt[,i] = yt[,i] */
    F77_NAME(dcopy)(&d, &yt[d * i], &intone, &vt[d * i], &intone);

#ifdef DEBUG_PRINT
    print_array(&vt[d * i], 1, d, "yt[,i]:");
#endif

    /* vt[,i] = vt[,i] - ct[i * incct,] */
    F77_NAME(daxpy)(&d, &dblminusone, &ct[d * i * incct], &intone, &vt[d * i], &intone);

#ifdef DEBUG_PRINT
    print_array(&ct[d * i * incct], 1, d, "ct:");
#endif


    /* vt[,i] = vt[,i] - Zt[,,i * incZt] %*% at[,i] */
#ifdef DEBUG_PRINT
    print_array(&Zt[m_x_d * i * incZt], d, m, "Zti:");
#endif
    F77_NAME(dgemm)(dont_transpose, dont_transpose, &d,
		    &intone, &m, &dblminusone,
		    &Zt[m_x_d * i * incZt], &d,
		    &at[m * i], &m,
		    &dblone, &vt[d * i], &d);

    /* ---------------------------------------------------------------------- */
    /* Compute Ft[,,i] = Zt[,,i * incZt] %*% Pt[,,i] %*% t(Zt[,,i * incZt]) + GGt[,,i * incGt] */
    /* ---------------------------------------------------------------------- */
    /* Zt[,,i] = Zt[,,i] %*% Pt[,,i] */

    /* tmpdxm = Zt[,,i] %*% Pt[,,i]*/
    F77_NAME(dgemm)(dont_transpose, dont_transpose, &d,
		    &m, &m, &dblone,
		    &Zt[m_x_d * i * incZt], &d,
		    &Pt[m_x_m * i], &m,
		    &dblzero, tmpdxm, &d);
    /* Ft[,,i] = Gt[,,i * incGGt] */
    F77_NAME(dcopy)(&d_x_d, &GGt[d_x_d * i * incGGt], &intone, &Ft[d_x_d * i], &intone);

#ifdef DEBUG_PRINT
    print_array(&Ft[d_x_d * i], d, d, "GGt:");
#endif

    /* Ft[,,i] = tmpdxm %*% t(Zt[,,i * incZt]) + Ft[,,i] */

    F77_NAME(dgemm)(dont_transpose, transpose, &d,
		    &d, &m, &dblone,
		    tmpdxm, &d,
		    &Zt[m_x_d * i * incZt], &d,
		    &dblone, &Ft[d_x_d * i], &d);

    /* ---------------------------------------------------------------------- */
    /* Invert Ft[,,i] */
    /* ---------------------------------------------------------------------- */
    if(d == 1){
      tmpFt_inv[0] = 1/Ft[i];
    }else{
      /* tmpFt_inv = Ft[,,i] */
      F77_NAME(dcopy)(&d_x_d, &Ft[d_x_d * i], &intone, tmpFt_inv, &intone);

      /* Cholesky factorization */
      F77_NAME(dpotrf)(upper_triangle, &d,
		       tmpFt_inv, &d, &potrf_info);
      if(potrf_info != 0)
	Rprintf("Warning: Cholesky factorization 'dpotrf' exited with status: %d\nVariance of the prediction error can not be computed.\n", potrf_info);

      strcpy(dpotri_uplo, "U");
      /* Computes the inverse using the Cholesky factorization */
      F77_NAME(dpotri)(dpotri_uplo, &d,
		       tmpFt_inv, &d, &potri_info);

      /* As 'dpotri' returns a lower or upper tringle -> mirror the matrix */
      FKFmirrorLU(tmpFt_inv, d, dpotri_uplo);

      if(potri_info != 0)
	Rprintf("Warning: 'dpotri' exited with status: %d\nVariance of the prediction error can not be computed.\n", potri_info);
    }


    /* ---------------------------------------------------------------------- */
    /* Kt[,,i] = Pt[,,i] %*% t(Zt[,,i * incZt]) %*% tmpFt_inv */
    /* ---------------------------------------------------------------------- */

    /* tmpdxm = Pt[,,i] %*% t(Zt[,,i * incZt]) */
    F77_NAME(dgemm)(dont_transpose, transpose, &m,
		    &d, &m, &dblone,
		    &Pt[m_x_m * i], &m,
		    &Zt[m_x_d * i * incZt], &d,
		    &dblzero, tmpdxm, &m);

    /* Kt[,,i] = tmpdxm %*% tmpFt_inv */
    F77_NAME(dgemm)(dont_transpose, dont_transpose, &m,
		    &d, &d, &dblone,
		    tmpdxm, &m,
		    tmpFt_inv, &d,
		    &dblzero, &Kt[m_x_d * i], &m);

    /* ---------------------------------------------------------------------- */
    /* att[,i] = at[,i] + Kt[,,i] %*% vt[,i] */
    /* ---------------------------------------------------------------------- */

    /* att[,i] = at[,i] */
    F77_NAME(dcopy)(&m, &at[m * i], &intone, &att[m * i], &intone);

    /* att[,i] = Kt[,,i] %*% vt[,i] + att[,i] */
    F77_NAME(dgemm)(dont_transpose, dont_transpose, &m,
		    &intone, &d, &dblone,
		    &Kt[m_x_d * i], &m,
		    &vt[d * i], &d,
		    &dblone, &att[m * i], &m);

    /* ---------------------------------------------------------------------- */
    /* Ptt[,,i] = Pt[,,i] - Pt[,,i] %*% t(Zt[,,i * incZt]) %*% t(Kt[,,i]) */
    /* ---------------------------------------------------------------------- */

    /* tmpmxm = t(Zt[,,i * incZt]) %*% t(Kt[,,i]) */
    F77_NAME(dgemm)(transpose, transpose, &m,
		    &m, &d, &dblone,
		    &Zt[m_x_d * i * incZt], &d,
		    &Kt[m_x_d * i], &m,
		    &dblzero, tmpmxm, &m);

    /* Ptt[,i] = Pt[,i] */
    F77_NAME(dcopy)(&m_x_m, &Pt[m_x_m * i], &intone, &Ptt[m_x_m * i], &intone);

    /* tmptmpmxm = Pt[,i] */
    F77_NAME(dcopy)(&m_x_m, &Pt[m_x_m * i], &intone, tmptmpmxm, &intone);

    /* Ptt[,,i] = - Ptt[,,i]  %*% tmptmpmxm + tmpmxm */
    F77_NAME(dgemm)(dont_transpose, dont_transpose, &m,
		    &m, &m, &dblminusone,
		    tmptmpmxm, &m,
		    tmpmxm, &m,
		    &dblone, &Ptt[m_x_m * i], &m);


    /* ---------------------------------------------------------------------- */
    /* a[,i] = dt[,i * incdt] + Tt[,,i * incTt] %*% att[,i] */
    /* ---------------------------------------------------------------------- */

    /* at[,i] = dt[,i] */
   F77_NAME(dcopy)(&m, &dt[m * i * incdt], &intone, &at[m * (i + 1)], &intone);
#ifdef DEBUG_PRINT
    print_array(&at[m * (i + 1)], 1, m, "dt:");
#endif

#ifdef DEBUG_PRINT
    print_array(&Tt[m_x_m * i * incTt], m, m, "Tt:");
#endif

    /* at[,i] = Tt[,,i * incTt] %*% att[,i] + at[,i] */
    F77_NAME(dgemm)(dont_transpose, dont_transpose, &m,
		    &intone, &m, &dblone,
		    &Tt[m_x_m * i * incTt], &m,
		    &att[m * i], &m,
		    &dblone, &at[m * (i + 1)], &m);

    /* ---------------------------------------------------------------------- */
    /* Pt[,,i] = Tt[,,i * incTt] %*% Ptt[,,i] %*% t(Tt[,,i * incTt]) + HHt[,,i * incHHt] */
    /* ---------------------------------------------------------------------- */

    /* tmpmxm = Ptt[,,i] %*% t(Tt[,,i * incTt]) */
    F77_NAME(dgemm)(dont_transpose, transpose, &m,
		    &m, &m, &dblone,
		    &Ptt[m_x_m * i], &m,
		    &Tt[m_x_m * i * incTt], &m,
		    &dblzero, tmpmxm, &m);

    /* Pt[,,i] = HHt[,,i * incHHt] */
    F77_NAME(dcopy)(&m_x_m, &HHt[m_x_m * i * incHHt], &intone, &Pt[m_x_m * (i + 1)], &intone);

#ifdef DEBUG_PRINT
    print_array(&HHt[m_x_m * i * incHHt], m, m, "HHt:");
#endif

    /* Pt[,,i] = Tt[,,i * incTt] %*% tmpmxm + Pt[,,i] */
    F77_NAME(dgemm)(dont_transpose, dont_transpose, &m,
		    &m, &m, &dblone,
		    &Tt[m_x_m * i * incTt], &m,
		    tmpmxm, &m,
		    &dblone, &Pt[m_x_m * (i + 1)], &m);

    /* ====================================================================== */
    /* logLik = logLik - 0.5 * log(det(Ft[,,i])) - 0.5 t(vt[,i]) %*% tmpFt_inv %*% vt[,i] */
    /* ====================================================================== */
    /* ---------------------------------------------------------------------- */
    /* Compute the mahalanobis distance first: t(vt) %*% tmpFt_inv %*% vt */
    /* ---------------------------------------------------------------------- */
    /* tmpdxd = tmpFt_inv %*% vt[,i] */
    F77_NAME(dgemm)(dont_transpose, dont_transpose, &d,
		    &intone, &d, &dblone,
		    tmpFt_inv, &d,
		    &vt[d * i], &d,
		    &dblzero, tmpdxd, &d);

    /* mahalanobis = t(vt[,i]) %*% tmpdxd */
    mahalanobis = 0;
    F77_NAME(dgemm)(transpose, dont_transpose, &intone,
		    &intone, &d, &dblone,
		    &vt[d * i], &d,
		    tmpdxd, &d,
		    &dblone, &mahalanobis, &intone);

    /* ---------------------------------------------------------------------- */
    /* Compute the determinant of Fti employing a Cholesky decomposition */
    /* ---------------------------------------------------------------------- */
    if(d == 1){
      det = Ft[i];
    }else{
      /* tmpdxd = Ft[,,i] */
      F77_NAME(dcopy)(&d_x_d, &Ft[d_x_d * i], &intone, tmpdxd, &intone);

      /* Compute the Cholesky factorization */
      F77_NAME(dpotrf)(upper_triangle, &d,
		       tmpdxd, &d, &det_potrf_info);

      if(det_potrf_info != 0)
	Rprintf("Warning: Cholesky factorization 'dpotrf' exited with status: %d\nDeterminant of the variance of the prediction error can not be computed.\n", det_potrf_info);

      /* Take the product of the squared diagonal elements to get the determinant*/
      det = tmpdxd[0];
      for(j = 1; j < d; j++)
	det *= tmpdxd[IDX(j, j, d)];

      det = det * det;
    }

    /* ---------------------------------------------------------------------- */
    /* Sum it up: */
    /* log(density) = -d / 2 * log(2 * pi) - 0.5 * log(det) - 0.5 * mahalanbois */
    /* ---------------------------------------------------------------------- */
    logDet = log(det);
    if(R_finite(logDet) && R_finite(mahalanobis)){
      *loglik -= 0.5 * (mahalanobis + logDet);
    }else{
      *loglik = NA_REAL;
#ifdef DEBUG_PRINT
      Rprintf("logDet or mahalanobis is NaN\n");
#endif
    }

#ifdef DEBUG_PRINT
    Rprintf("\n\nlogDet:   %3.9f;   maha:   %3.9f\n", logDet, mahalanobis);
#endif

#ifdef DEBUG_PRINT
    print_array(&vt[d * i], 1, d, "Vt:");
    print_array(&Ft[d_x_d * i], d, d, "Ft:");
    print_array(&Kt[m_x_d * i], m, d, "Kt:");
    print_array(&at[m * (i + 1)], 1, m, "at:");
    print_array(&att[m * i], 1, m, "att:");
    print_array(&Pt[m_x_m * (i + 1)], m, m, "Pt:");
    print_array(&Ptt[m_x_m * i], m, m, "Ptt:");
#endif

    i++;
  }
  /* ======================================================================  */
  /* End of filtering loop */
  /* ======================================================================  */

  status[0] = potri_info;
  status[1] = potrf_info;

  Free(tmpdxm);
  Free(tmpmxm);
  Free(tmptmpmxm);
  Free(tmpdxd);
  Free(tmpFt_inv);


}


SEXP FKF(SEXP a0, SEXP P0, SEXP dt, SEXP ct, SEXP Tt,
	 SEXP Zt, SEXP HHt, SEXP GGt, SEXP yt)

{
  int m = length(a0);
  int d = INTEGER(GET_DIM(yt))[0];
  int n = INTEGER(GET_DIM(yt))[1];

  int m_x_n = m * n;
  int d_x_n = d * n;

  int m_x_d_x_n = m * d * n;
  int m_x_m_x_n = m * m * n;
  int d_x_d_x_n = d * d * n;

  int intzero = 0, intone = 1;

  double dbl_NA = NA_REAL;

  SEXP att, at, Ptt, Pt, vt, Ft, Kt, loglik,
      status, ans, ans_names, class_name;

  SEXP dim_att, dim_at, dim_Ptt, dim_Pt, dim_vt, dim_Kt, dim_Ft;

  /* Allocate memory for objects to be returned and set values to NA. */
  /* In case Ft can not be inverted, the loop breaks and the values of */
  /* the return variables will be NA. */
  PROTECT(att = NEW_NUMERIC(m * n));
  F77_NAME(dcopy)(&m_x_n, &dbl_NA, &intzero, NUMERIC_POINTER(att), &intone);

  PROTECT(at = NEW_NUMERIC(m * (n + 1)));
  F77_NAME(dcopy)(&m_x_n, &dbl_NA, &intzero, NUMERIC_POINTER(at), &intone);

  PROTECT(Ptt = NEW_NUMERIC(m * m * n));
  F77_NAME(dcopy)(&m_x_m_x_n, &dbl_NA, &intzero, NUMERIC_POINTER(Ptt), &intone);

  PROTECT(Pt = NEW_NUMERIC(m * m * (n + 1)));
  F77_NAME(dcopy)(&m_x_m_x_n, &dbl_NA, &intzero, NUMERIC_POINTER(Pt), &intone);

  PROTECT(vt = NEW_NUMERIC(d * n));
  F77_NAME(dcopy)(&d_x_n, &dbl_NA, &intzero, NUMERIC_POINTER(vt), &intone);

  PROTECT(Ft = NEW_NUMERIC(d * d * n));
  F77_NAME(dcopy)(&d_x_d_x_n, &dbl_NA, &intzero, NUMERIC_POINTER(Ft), &intone);

  PROTECT(Kt = NEW_NUMERIC(m * d * n));
  F77_NAME(dcopy)(&m_x_d_x_n, &dbl_NA, &intzero, NUMERIC_POINTER(Kt), &intone);

  PROTECT(loglik = NEW_NUMERIC(1));

  PROTECT(status = NEW_INTEGER(2));

  cfkf(m, d, n,
       NUMERIC_POINTER(a0), NUMERIC_POINTER(P0),
       NUMERIC_POINTER(dt), INTEGER(GET_DIM(dt))[1] == n,
       NUMERIC_POINTER(ct), INTEGER(GET_DIM(ct))[1] == n,
       NUMERIC_POINTER(Tt), INTEGER(GET_DIM(Tt))[2] == n,
       NUMERIC_POINTER(Zt), INTEGER(GET_DIM(Zt))[2] == n,
       NUMERIC_POINTER(HHt), INTEGER(GET_DIM(HHt))[2] == n,
       NUMERIC_POINTER(GGt), INTEGER(GET_DIM(GGt))[2] == n,
       NUMERIC_POINTER(yt),
       NUMERIC_POINTER(at), NUMERIC_POINTER(att),
       NUMERIC_POINTER(Pt), NUMERIC_POINTER(Ptt),
       NUMERIC_POINTER(vt), NUMERIC_POINTER(Ft),
       NUMERIC_POINTER(Kt),
       NUMERIC_POINTER(loglik), INTEGER_POINTER(status));

  /* Produce named return list */
  PROTECT(ans = NEW_LIST(9));
  PROTECT(ans_names = NEW_CHARACTER(9));
  SET_STRING_ELT(ans_names, 0, mkChar("att"));
  SET_STRING_ELT(ans_names, 1, mkChar("at"));
  SET_STRING_ELT(ans_names, 2, mkChar("Ptt"));
  SET_STRING_ELT(ans_names, 3, mkChar("Pt"));
  SET_STRING_ELT(ans_names, 4, mkChar("vt"));
  SET_STRING_ELT(ans_names, 5, mkChar("Ft"));
  SET_STRING_ELT(ans_names, 6, mkChar("Kt"));
  SET_STRING_ELT(ans_names, 7, mkChar("logLik"));
  SET_STRING_ELT(ans_names, 8, mkChar("status"));

  setAttrib(ans, R_NamesSymbol, ans_names);

  /* Coerce vectors to matrices and arrays */

  /* Set matrix dimensions */
  PROTECT(dim_at = NEW_INTEGER(2));
  PROTECT(dim_att = NEW_INTEGER(2));
  PROTECT(dim_vt = NEW_INTEGER(2));

  INTEGER(dim_at)[0] = m;
  INTEGER(dim_at)[1] = n + 1;

  INTEGER(dim_att)[0] = m;
  INTEGER(dim_att)[1] = n;

  INTEGER(dim_vt)[0] = d;
  INTEGER(dim_vt)[1] = n;

  setAttrib(at, R_DimSymbol, dim_at);
  setAttrib(att, R_DimSymbol, dim_att);
  setAttrib(vt, R_DimSymbol, dim_vt);

  /* Set array dimensions */
  PROTECT(dim_Pt = NEW_INTEGER(3));
  PROTECT(dim_Ptt = NEW_INTEGER(3));
  PROTECT(dim_Ft = NEW_INTEGER(3));
  PROTECT(dim_Kt = NEW_INTEGER(3));

  INTEGER(dim_Pt)[0] = m;
  INTEGER(dim_Pt)[1] = m;
  INTEGER(dim_Pt)[2] = n + 1;

  INTEGER(dim_Ptt)[0] = m;
  INTEGER(dim_Ptt)[1] = m;
  INTEGER(dim_Ptt)[2] = n;

  INTEGER(dim_Ft)[0] = d;
  INTEGER(dim_Ft)[1] = d;
  INTEGER(dim_Ft)[2] = n;

  INTEGER(dim_Kt)[0] = m;
  INTEGER(dim_Kt)[1] = d;
  INTEGER(dim_Kt)[2] = n;

  setAttrib(Pt, R_DimSymbol, dim_Pt);
  setAttrib(Ptt, R_DimSymbol, dim_Ptt);
  setAttrib(Ft, R_DimSymbol, dim_Ft);
  setAttrib(Kt, R_DimSymbol, dim_Kt);

  /* Fill the list */
  SET_VECTOR_ELT(ans, 0, att);
  SET_VECTOR_ELT(ans, 1, at);
  SET_VECTOR_ELT(ans, 2, Ptt);
  SET_VECTOR_ELT(ans, 3, Pt);
  SET_VECTOR_ELT(ans, 4, vt);
  SET_VECTOR_ELT(ans, 5, Ft);
  SET_VECTOR_ELT(ans, 6, Kt);
  SET_VECTOR_ELT(ans, 7, loglik);
  SET_VECTOR_ELT(ans, 8, status);

  /* Set the class to 'fkf' */
  PROTECT(class_name = NEW_CHARACTER(1));
  SET_STRING_ELT(class_name, 0, mkChar("fkf"));
  classgets(ans, class_name);

  UNPROTECT(19);
  return(ans);
}
