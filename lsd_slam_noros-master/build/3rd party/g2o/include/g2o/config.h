#ifndef G2O_CONFIG_H
#define G2O_CONFIG_H

#define G2O_HAVE_OPENGL 1
#define G2O_OPENGL_FOUND 1
#define G2O_OPENMP 1
#define G2O_SHARED_LIBS 1
#define G2O_LGPL_SHARED_LIBS 1

// available sparse matrix libraries
/* #undef G2O_HAVE_CHOLMOD */
#define G2O_HAVE_CSPARSE 1

#define G2O_CXX_COMPILER "MSVC C:/Program Files (x86)/Microsoft Visual Studio 12.0/VC/bin/cl.exe"

// give a warning if Eigen defaults to row-major matrices.
// We internally assume column-major matrices throughout the code.
#ifdef EIGEN_DEFAULT_TO_ROW_MAJOR
#  error "g2o requires column major Eigen matrices (see http://eigen.tuxfamily.org/bz/show_bug.cgi?id=422)"
#endif

#if __cplusplus > 199711L
namespace std {
  namespace tr1 = ::std;
}
#endif

#endif
