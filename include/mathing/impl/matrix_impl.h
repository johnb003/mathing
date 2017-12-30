#ifndef MATHING_IMPL_MATRIX_H
#define MATHING_IMPL_MATRIX_H

#include "../quaternion.h"
#include "../vector.h"
#include "../scalar.h"

namespace mathing
{

// The goal of this class is to provide the front end API as a completely transparent
// wrapper around an implementaiton. The default implementation is provided as
// a simple c++ implementation.
class MatrixCppImpl4x4
{
	/// Holds the data for the matrix
	Scalar m[16];

	// Access is awkward like this, and rarely used.
	// If direct access to the buffer is really needed, it can be an accessor.
	// More commonly, it'll be via AxisX(), Pos() etc.
public:

	MatrixCppImpl4x4() {
		Set(m_Identity.m);		// this style of use a different constructor,
								// can it be more standardized? Or should we call "Set"?
	}
	MatrixCppImpl4x4(const MatrixCppImpl4x4 &rhs)			{ Set(rhs.m); }
	MatrixCppImpl4x4(const Scalar farray[16])				{ Set(farray); }
	MatrixCppImpl4x4(const Vec4 &xv, const Vec4 &yv, const Vec4 &zv,
					 const Vec4 &pv = Vec4())				{ Set(xv, yv, zv, pv); }
	MatrixCppImpl4x4(const Quaternion &q, const Vec4 &pv)	{ Set(q, pv); }
	~MatrixCppImpl4x4() {}

	inline void Set(const Scalar farray[16])
	{
		// This could be improved on a PC with a fast copy 32 bit aligned, 32 bit packed
		memcpy(m, farray, sizeof(Scalar)*16);
	}

	inline void Set(const Vec4 &xv, const Vec4 &yv, const Vec4 &zv, const Vec4 &pv = Vec4())
	{
		// Addres aliases into the m[], used as LHS assignment, does that optimize ok?
		AxisX() = xv;
		AxisY() = yv;
		AxisZ() = zv;
		Pos() = pv;

		// just in case there is garbage in the incoming vector's 4th component
		// That's why people use 4x3's
		m[ 3] = 0;
		m[ 7] = 0;
		m[11] = 0;
		m[15] = 1;
	}

	inline void Set(const Quaternion &q, const Vec4 &pv)
	{
		Scalar x2 = q.x + q.x;
		Scalar y2 = q.y + q.y;
		Scalar z2 = q.z + q.z;

		Scalar wx = q.w*x2;
		Scalar wy = q.w*y2;
		Scalar wz = q.w*z2;

		Scalar xx = q.x*x2;
		Scalar xy = q.x*y2;
		Scalar xz = q.x*z2;

		Scalar yy = q.y*y2;
		Scalar yz = q.y*z2;

		Scalar zz = q.z*z2;

		// TODO: fold this up into cleaner functions?

		m[ 0] = 1 - (yy + zz);
		m[ 1] = xy + wz;
		m[ 2] = xz - wy;
		m[ 3] = 0;

		m[ 4] = xy - wz;
		m[ 5] = 1 - (xx + zz);
		m[ 6] = yz + wx;
		m[ 7] = 0;

		m[ 8] = xz + wy;
		m[ 9] = yz - wx;
		m[10]= 1 - (xx + yy);
		m[11] = 0;

		m[12] = pv.x;
		m[13] = pv.y;
		m[14] = pv.z;
		m[15] = 1;
	}

	inline Vec4 &AxisX() const { return (Vec4 &)m[0]; }
	inline Vec4 &AxisY() const { return (Vec4 &)m[4]; }
	inline Vec4 &AxisZ() const { return (Vec4 &)m[8]; }
	inline Vec4 &Pos() const { return (Vec4 &)m[12]; }

	inline Scalar *Buff() { return m; }
	inline const Scalar *Buff() const { return m; }

	inline MatrixCppImpl4x4 &operator=(const MatrixCppImpl4x4 &rhs) {
		memcpy(m, rhs.m, sizeof(Scalar)*16);
		return *this;
	}

	/// Transforms the current matrix by an offset.
	/// TODO: This is a pretty non-trivial add.
	/// Maybe it should be called: 
	inline MatrixCppImpl4x4 &operator+=(const Vec4 &rhs) {
		m[12] += rhs.x;
		m[13] += rhs.y;
		m[14] += rhs.z;
		return *this;
	}

	/// Applies the rhs transformation to the current matrix, stores the result in the current matrix,
	/// and returns the address.	
	inline MatrixCppImpl4x4 &operator*=(const MatrixCppImpl4x4 &rhs)
	{
		// Matrix math is row * column
		// TODO: Address storage container and copies.
		MatrixCppImpl4x4 tmp;
		tmp.m[ 0]=m[ 0]*rhs.m[ 0] + m[ 1]*rhs.m[ 4] + m[ 2]*rhs.m[ 8] + m[ 3]*rhs.m[12];
		tmp.m[ 1]=m[ 0]*rhs.m[ 1] + m[ 1]*rhs.m[ 5] + m[ 2]*rhs.m[ 9] + m[ 3]*rhs.m[13];
		tmp.m[ 2]=m[ 0]*rhs.m[ 2] + m[ 1]*rhs.m[ 6] + m[ 2]*rhs.m[10] + m[ 3]*rhs.m[14];
		tmp.m[ 3]=m[ 0]*rhs.m[ 3] + m[ 1]*rhs.m[ 7] + m[ 2]*rhs.m[12] + m[ 3]*rhs.m[15];

		tmp.m[ 4]=m[ 4]*rhs.m[ 0] + m[ 5]*rhs.m[ 4] + m[ 6]*rhs.m[ 8] + m[ 7]*rhs.m[12];
		tmp.m[ 5]=m[ 4]*rhs.m[ 1] + m[ 5]*rhs.m[ 5] + m[ 6]*rhs.m[ 9] + m[ 7]*rhs.m[13];
		tmp.m[ 6]=m[ 4]*rhs.m[ 2] + m[ 5]*rhs.m[ 6] + m[ 6]*rhs.m[10] + m[ 7]*rhs.m[14];
		tmp.m[ 7]=m[ 4]*rhs.m[ 3] + m[ 5]*rhs.m[ 7] + m[ 6]*rhs.m[12] + m[ 7]*rhs.m[15];

		tmp.m[ 8]=m[ 8]*rhs.m[ 0] + m[ 9]*rhs.m[ 4] + m[10]*rhs.m[ 8] + m[11]*rhs.m[12];
		tmp.m[ 9]=m[ 8]*rhs.m[ 1] + m[ 9]*rhs.m[ 5] + m[10]*rhs.m[ 9] + m[11]*rhs.m[13];
		tmp.m[10]=m[ 8]*rhs.m[ 2] + m[ 9]*rhs.m[ 6] + m[10]*rhs.m[10] + m[11]*rhs.m[14];
		tmp.m[11]=m[ 8]*rhs.m[ 3] + m[ 9]*rhs.m[ 7] + m[10]*rhs.m[12] + m[11]*rhs.m[15];

		tmp.m[12]=m[12]*rhs.m[ 0] + m[13]*rhs.m[ 4] + m[14]*rhs.m[ 8] + m[15]*rhs.m[12];
		tmp.m[13]=m[12]*rhs.m[ 1] + m[13]*rhs.m[ 5] + m[14]*rhs.m[ 9] + m[15]*rhs.m[13];
		tmp.m[14]=m[12]*rhs.m[ 2] + m[13]*rhs.m[ 6] + m[14]*rhs.m[10] + m[15]*rhs.m[14];
		tmp.m[15]=m[12]*rhs.m[ 3] + m[13]*rhs.m[ 7] + m[14]*rhs.m[12] + m[15]*rhs.m[15];

		memcpy(m, tmp.m, sizeof(Scalar)*16);
		return *this;
	}

	// TODO: Address storage container and copies.
	inline MatrixCppImpl4x4 operator*(const MatrixCppImpl4x4 &rhs) const
	{
		MatrixCppImpl4x4 tmp;
		tmp.m[ 0] = m[ 0] * rhs.m[ 0] + m[ 1]*rhs.m[ 4] + m[ 2]*rhs.m[ 8] + m[ 3]*rhs.m[12];
		tmp.m[ 1] = m[ 0] * rhs.m[ 1] + m[ 1]*rhs.m[ 5] + m[ 2]*rhs.m[ 9] + m[ 3]*rhs.m[13];
		tmp.m[ 2] = m[ 0] * rhs.m[ 2] + m[ 1]*rhs.m[ 6] + m[ 2]*rhs.m[10] + m[ 3]*rhs.m[14];
		tmp.m[ 3] = m[ 0] * rhs.m[ 3] + m[ 1]*rhs.m[ 7] + m[ 2]*rhs.m[12] + m[ 3]*rhs.m[15];

		tmp.m[ 4] = m[ 4] * rhs.m[ 0] + m[ 5]*rhs.m[ 4] + m[ 6]*rhs.m[ 8] + m[ 7]*rhs.m[12];
		tmp.m[ 5] = m[ 4] * rhs.m[ 1] + m[ 5]*rhs.m[ 5] + m[ 6]*rhs.m[ 9] + m[ 7]*rhs.m[13];
		tmp.m[ 6] = m[ 4] * rhs.m[ 2] + m[ 5]*rhs.m[ 6] + m[ 6]*rhs.m[10] + m[ 7]*rhs.m[14];
		tmp.m[ 7] = m[ 4] * rhs.m[ 3] + m[ 5]*rhs.m[ 7] + m[ 6]*rhs.m[12] + m[ 7]*rhs.m[15];

		tmp.m[ 8] = m[ 8] * rhs.m[ 0] + m[ 9]*rhs.m[ 4] + m[10]*rhs.m[ 8] + m[11]*rhs.m[12];
		tmp.m[ 9] = m[ 8] * rhs.m[ 1] + m[ 9]*rhs.m[ 5] + m[10]*rhs.m[ 9] + m[11]*rhs.m[13];
		tmp.m[10] = m[ 8] * rhs.m[ 2] + m[ 9]*rhs.m[ 6] + m[10]*rhs.m[10] + m[11]*rhs.m[14];
		tmp.m[11] = m[ 8] * rhs.m[ 3] + m[ 9]*rhs.m[ 7] + m[10]*rhs.m[12] + m[11]*rhs.m[15];

		tmp.m[12] = m[12] * rhs.m[ 0] + m[13]*rhs.m[ 4] + m[14]*rhs.m[ 8] + m[15]*rhs.m[12];
		tmp.m[13] = m[12] * rhs.m[ 1] + m[13]*rhs.m[ 5] + m[14]*rhs.m[ 9] + m[15]*rhs.m[13];
		tmp.m[14] = m[12] * rhs.m[ 2] + m[13]*rhs.m[ 6] + m[14]*rhs.m[10] + m[15]*rhs.m[14];
		tmp.m[15] = m[12] * rhs.m[ 3] + m[13]*rhs.m[ 7] + m[14]*rhs.m[12] + m[15]*rhs.m[15];
		return tmp;
	}

	/// <x,y,z,1> * MatrixCppImpl4x4, transforms the point by the matrix, and returns the resulting point
	/// This is a convenience similar to the multiplication, but ignores the w of the vec4, and assumes 1.
	inline Vec4 Transform(const Vec4 &v) const
	{
		Vec4 ret;
		ret.x = v.x * m[0] + v.y * m[4] + v.z * m[ 8] + m[12];
		ret.y = v.x * m[1] + v.y * m[5] + v.z * m[ 9] + m[13];
		ret.z = v.x * m[2] + v.y * m[6] + v.z * m[10] + m[14];

		// TODO: Change to 1?
		ret.w=v.w; //v.x*m[ 3] + v.y*m[ 7] + v.z*m[11] + m[15];	// usually just 1
		return ret;
	}

	/// <x,y,z,0> * MatrixCppImpl4x4, transforms the vector, does not apply translation
	/// This is a convenience similar to the multiplication, but ignores the w of the vec4, and assumes 0.
	inline Vec4 Rotate(const Vec4 &v) const
	{
		Vec4 ret;
		ret.x = v.x * m[ 0] + v.y*m[ 4] + v.z*m[ 8];
		ret.y = v.x * m[ 1] + v.y*m[ 5] + v.z*m[ 9];
		ret.z = v.x * m[ 2] + v.y*m[ 6] + v.z*m[10];
		ret.w = 0;
		return ret;
	}

	/// Convert handedness.
	/// For example if using right-hand and expecting X to the right, Y up, and Z would be towards you,
	/// then to convert to left hand, we'll invert Z. This negates all z components of all axies,
	/// AND negates the Z axis, thus, the z component of the z axis will remain positive.
	inline MatrixCppImpl4x4 &FlipZ(MatrixCppImpl4x4 &outMat) const
	{
		outMat.m[ 0] =  m[ 0];	outMat.m[ 1] =  m[ 1];	outMat.m[ 2] = -m[ 2];	outMat.m[ 3] =  m[ 3];
		outMat.m[ 4] =  m[ 4];	outMat.m[ 5] =  m[ 5];	outMat.m[ 6] = -m[ 6];	outMat.m[ 7] =  m[ 7];
		outMat.m[ 8] = -m[ 8];	outMat.m[ 9] = -m[ 9];	outMat.m[10] =  m[10];	outMat.m[11] =  m[11];	// Negate AxisZ
		outMat.m[12] =  m[12];	outMat.m[13] =  m[13];	outMat.m[14] = -m[14];	outMat.m[15] =  m[15];

		return outMat;

	}

	/// The transformation matrix that undoes this transformation.
	// This is a special case inverse that works for rotation & translation (NOT SCALE, Sheer or projection)
	inline MatrixCppImpl4x4 Inverse() const
	{
		MatrixCppImpl4x4 ret;

		// Transpose the rotation Matrix
		ret.m[ 0] = m[ 0];
		ret.m[ 1] = m[ 4];
		ret.m[ 2] = m[ 8];
		ret.m[ 3] = 0;

		ret.m[ 4] = m[ 1];
		ret.m[ 5] = m[ 5];
		ret.m[ 6] = m[ 9];
		ret.m[ 7] = 0;

		ret.m[ 8] = m[ 2];
		ret.m[ 9] = m[ 6];
		ret.m[10] = m[10];
		ret.m[11] = 0;

		// This is right, but it causes data to be copied for the negate, and we can make it slightly more efficient
		// by unrolling it
		// ret.Pos() = -ret.Rotate(Pos());

		ret.m[12] = -(m[12]*ret.m[ 0] + m[13]*ret.m[ 4] + m[14]*ret.m[ 8]);
		ret.m[13] = -(m[12]*ret.m[ 1] + m[13]*ret.m[ 5] + m[14]*ret.m[ 9]);
		ret.m[14] = -(m[12]*ret.m[ 2] + m[13]*ret.m[ 6] + m[14]*ret.m[10]);
		ret.m[15] = 1;

		return ret;
	}

	/// Swap the rows and columns (used to get access to a column major version)
	inline MatrixCppImpl4x4 Transpose() const
	{
		MatrixCppImpl4x4 ret;
		ret.m[ 0] = m[ 0];
		ret.m[ 1] = m[ 4];
		ret.m[ 2] = m[ 8];
		ret.m[ 3] = m[12];

		ret.m[ 4] = m[ 1];
		ret.m[ 5] = m[ 5];
		ret.m[ 6] = m[ 9];
		ret.m[ 7] = m[13];

		ret.m[ 8] = m[ 2];
		ret.m[ 9] = m[ 6];
		ret.m[10] = m[10];
		ret.m[11] = m[14];

		ret.m[12] = m[ 3];
		ret.m[13] = m[ 7];
		ret.m[14] = m[11];
		ret.m[15] = m[15];
		return ret;
	}

	static const MatrixCppImpl4x4 m_Identity;

	/// Transforms a vector by a matrix and returns the the resulting vector.
	/// Vec4's with a 1 in the w will be affected by translation part of the matrix.
	/// Vec4's with a 0 in the w will NOT be affected by translation; only be rotated.
	/// Vec4's with a 0 in the w are like directions that get transformed.
	friend Vec4 operator*(const Vec4 &lhs, const MatrixCppImpl4x4 &rhs);
//	friend MatrixCppImpl4x4 operator*(const MatrixCppImpl4x4 &lhs, const MatrixCppImpl4x4 &rhs);

};

inline Vec4 operator*(const Vec4 &lhs, const MatrixCppImpl4x4 &rhs)
{
	Vec4 ret;
	ret.x=lhs.x*rhs.m[ 0] + lhs.y*rhs.m[ 4] + lhs.z*rhs.m[ 8] + lhs.w*rhs.m[12];
	ret.y=lhs.x*rhs.m[ 1] + lhs.y*rhs.m[ 5] + lhs.z*rhs.m[ 9] + lhs.w*rhs.m[13];
	ret.z=lhs.x*rhs.m[ 2] + lhs.y*rhs.m[ 6] + lhs.z*rhs.m[10] + lhs.w*rhs.m[14];
	ret.w=lhs.x*rhs.m[ 3] + lhs.y*rhs.m[ 7] + lhs.z*rhs.m[11] + lhs.w*rhs.m[15];	// usually just 1
	return ret;
}


}  // namespace mathing

#endif  // MATHING_IMPL_MATRIX_H
