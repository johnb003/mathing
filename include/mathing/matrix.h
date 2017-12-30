#ifndef MATHING_MATRIX_H
#define MATHING_MATRIX_H

/** Mathematical structure for 3D transformations. Good for storing a
	stack of transformations and can transform points efficiently.

	Since matrices are layed out in a 2D array, the indexing can be done in two ways:
	row-major, and column-major.

	Column-major indexing (like OpenGL)...
		| 0  4  8 12 |
		| 1  5  9 13 |
		| 2  6 10 14 |
		| 3  7 11 15 |

 	Row-major indexing (like DirectX)...
		| 0  1  2  3 |
		| 4  5  6  7 |
		| 8  9 10 11 |
		|12 13 14 15 |

	Regardless of how the values are layed out in memory, the math always works the same. (Row * Column)
		| - - - - |   | - - # - |   | - - - - |
		| # # # # | * | - - # - | = | - - # - |
		| - - - - |   | - - # - |   | - - - - |
		| - - - - |   | - - # - |   | - - - - |

	So, if our vectors are considered row-major, the only valid way to multiply the vector by the matrix is on the left, v * M
	and conversely if our vectors were column-major, the only valid way to multiply would be if the vector was on the right M * v

		Row-Major: ( v * M )						Column-Major: ( M * v )
					| - - # - |						| - - - - |   | x |   | - |
		[x y z w] * | - - # - | = [- - # -]			| # # # # | * | y | = | # |
					| - - # - |						| - - - - |   | z |   | - |
					| - - # - |						| - - - - |   | w |   | - |

	So when it comes to matrix multiplication order, how can you tell which way to multiply?
	Well if you want to transform a vector by a series of matrices, the first matrix it is transformed by is the most local.
	So, to transform by the chain of bones A,B,C (where A is the root):
		In column-major, it's left to right A*B*C*v.
		In row-major, it's right to left v*C*B*A


	A 2D array in C/C++ like this:
	Scalar [][4] matrix = {
		{ a, b, c, d },
		{ e, f, g, h },
		{ i, j, k, l },
		{ m, n, o, p }};

	Will be stored in memory like this: a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p.

	This library uses row major format.

	\sa Vec4,
		Quaternion
*/

// TODO:
// * Support shear, reflection, and projection
// * Orthonormalize
// * Remember tricks like, if you have a shear matrix you can still rotate vectors but you have to do
//   extra work, like 


#include <iostream>

#include "quaternion.h"
#include "vector.h"

#include "impl/matrix_impl.h"

// the design idea here is:
// great, simple API
// with inline calls to a simple C++ implementation.
// If anyone wants to port this library to work with their own math... the interface will be the same
// they just have to replace the inline calls to call their own impl, and swap out the whole library.

// It could be fancier, without having to have the user recompile the lbirary using some template meta
// magic, but this sounds kind of pointless "elegance".
// I imagine it like:
// template <typename Impl>
// typedef Matrix<Impl> Matrix;
// Then there can be an instance like:
//
// Impl _impl;
//
// and assumed calls like _impl.Whatever();
//
// However, I feel like this either requires a second layer of abstraction that has to be implemeneted to
// wrap a client library, or a lot of assumptions made. So in the end, why bother? It's meant to be
// light weight which means, so simple devs should feel ok diving in and copying the core library to
// wrap their own.

namespace mathing
{

// The goal of this class is to provide the front end API as a completely transparent
// wrapper around an implementaiton. The default implementation is provided as
// a simple c++ implementation.
class Matrix
{
	// in case some of the wrapper logic needs to build one from the thing it's wrapping.
	Matrix(MatrixCppImpl4x4 impl) : _impl(impl) {}
public:

	MatrixCppImpl4x4 _impl;

	/// Initialize the matrix to the identity Matrix.
	Matrix() : _impl() {}
	/// Initialize the matrix from an existing Matrix.
	Matrix(const Matrix &arg) : _impl(arg._impl) {}
	/// Initialize the matrix from an array, every 4 consecutive values define an axis
	Matrix(const Scalar farray[16]) : _impl(farray) {}
	/// Initialize the matrix from 3 vectors and an optional position vector.
	Matrix(const Vec4 &xv, const Vec4 &yv, const Vec4 &zv, const Vec4 &pv = Vec4()) :
		_impl(xv, yv, zv, pv) {}
	/// Initialize the matrix from an orientation Quaternion and a position vector.
	Matrix(const Quaternion &q, const Vec4 &pv = Vec4()) : _impl(q, pv) {}
	~Matrix() {}

	/// Sets the matrix from an array
	// TODO: ADD option to specify the input as either row or col-major;
	inline void Set(const Scalar farray[16]) { _impl.Set(farray); }
	/// Sets the matrix from axis vectors and position (optional).
	inline void Set(const Vec4 &xv, const Vec4 &yv, const Vec4 &zv, const Vec4 &pv = Vec4()) {
		_impl.Set(xv, yv, zv, pv);
	}
	/// Sets the matrix from an orientation Quaternion and a position vector.
	inline void Set(const Quaternion &q, const Vec4 &pv) { _impl.Set(q, pv); }

	inline Vec4 &AxisX() const { return _impl.AxisX(); }
	inline Vec4 &AxisY() const { return _impl.AxisY(); }
	inline Vec4 &AxisZ() const { return _impl.AxisZ(); }
	inline Vec4 &Pos() const { return _impl.Pos(); }

	inline Scalar *Buff() { return _impl.Buff(); }
	inline const Scalar *Buff() const { return _impl.Buff(); }

	/// Assigns the matrix to the value of another matrix, and returns the address to this matrix.
	inline Matrix &operator=(const Matrix &rhs) { _impl = rhs._impl; return *this; }

	/// Transforms the current matrix by an offset.
	inline Matrix &operator+=(const Vec4 &rhs) { _impl += rhs; return *this; }

	/// Applies the rhs transformation to the current matrix, stores the result in the current matrix,
	/// and returns the address.
	inline Matrix &operator*=(const Matrix &rhs) { _impl *= rhs._impl; return *this; }

	/// Transforms the current matrix by another and return the the resulting matrix.
	inline Matrix operator*(const Matrix &rhs) const { return Matrix(_impl * rhs._impl); }

	/// <x,y,z,1> * Matrix, transforms the point by the matrix, and returns the resulting point
	/// This is a convenience similar to the multiplication, but ignores the w of the vec4, and assumes 1.
	inline Vec4 Transform(const Vec4 &rhs) const { return _impl.Transform(rhs); }
	/// <x,y,z,0> * Matrix, transforms the vector, does not apply translation
	/// This is a convenience similar to the multiplication, but ignores the w of the vec4, and assumes 0.
	inline Vec4 Rotate(const Vec4 &v) const { return _impl.Rotate(v); }

	/// Convert handedness.
	/// For example if using right-hand and expecting X to the right, Y up, and Z would be towards you,
	/// then to convert to left hand, we'll invert Z. This negates all z components of all axies,
	/// AND negates the Z axis, thus, the z component of the z axis will remain positive.
	Matrix &FlipZ(Matrix &outMat) const;

	/// The transformation matrix that undoes this transformation.
	Matrix Inverse() const;

	/// Swap the rows and columns (used to get access to a column major version)
	Matrix Transpose() const;

	/// Transforms a vector by a matrix and returns the the resulting vector.
	/// Vec4's with a 1 in the w will be affected by translation part of the matrix.
	/// Vec4's with a 0 in the w will NOT be affected by translation; only be rotated.
	/// Vec4's with a 0 in the w are like directions that get transformed.
	friend Vec4 operator*(const Vec4 &lhs, const Matrix &rhs);

	// TODO: Compare return by value here completely inline vs
	// return by address of a static wrapped ident.
	static const Matrix Identity() { return Matrix(MatrixCppImpl4x4::m_Identity); }
};

//typedef Matrix<MatrixCppImpl4x4> Matrix;

std::ostream &operator<<(std::ostream &os, const Matrix &m);
Vec4 operator*(const Vec4 &lhs, const Matrix &rhs);

}  // namespace mathing

#endif  // MATHING_MATRIX_H
