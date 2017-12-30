#ifndef MATHING_UTIL_H
#define MATHING_UTIL_H

#include <iostream>

namespace mathing {

static const Scalar PI = 3.141592653589793;
static const Scalar PI_2 = 3.141592653589793/2.0;
static const Scalar PI_180 = 3.141592653589793/180.0;

template <typename T>
inline T Max(T a, T b)
{
  return a > b ? a : b;
}

template <typename T>
inline T Min(T a, T b)
{
  return a < b ? a : b;
}

// Declare angle in degrees. Converts to radions.
Scalar Degree(Scalar degrees) {
	return degrees * PI_180;
}

std::ostream &operator<<(std::ostream &os, const MatrixCppImpl4x4 &m);
std::ostream &operator<<(std::ostream &os, const Matrix &m);


};

#endif  // MATHING_UTIL_H