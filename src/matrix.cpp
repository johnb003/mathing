#include "mathing/matrix.h"

#include <iostream>
#include <iomanip>      // std::setprecision

using namespace std;

namespace mathing
{
static Scalar identity[16] = {1, 0, 0, 0,
							  0, 1, 0, 0,
							  0, 0, 1, 0,
							  0, 0, 0, 1};
const MatrixCppImpl4x4 MatrixCppImpl4x4::m_Identity(identity);

//
// Constructors
//

// Matrix::Matrix()
// {
// 	// identity
// 	// | 1 0 0 0 |
// 	// | 0 1 0 0 |
// 	// | 0 0 1 0 |
// 	// | 0 0 0 1 |
// 	AxisX() = Vec4::m_UnitX;
// 	AxisY() = Vec4::m_UnitY;
// 	AxisZ() = Vec4::m_UnitZ;
// 	Pos() = Vec4::m_UnitW;
// }

// Matrix::Matrix(const Matrix &rhs)
// {
// 	// This could be improved on a PC with a fast copy 32 bit aligned, 32 bit packed
// 	memcpy(m, rhs.m, sizeof(Scalar)*16);
// }

// Since we can't really tell what format the Scalar array is in, we assume it's in "row-major"
// Even if it were column-major, you'd probably still write out the vectors together, instead of by groups of components
// Matrix::Matrix(const Scalar farray[16])
// {
// 	Set(farray);
// }

// Matrix::Matrix(const Vec4 &xv, const Vec4 &yv, const Vec4 &zv, const Vec4 &pv)
// {
// 	Set(xv, yv, zv, pv);
// }

// Matrix::Matrix(const Quaternion &q, const Vec4 &pv)
// {
// 	Set(q, pv);
// }

//
// Set operations
//

// void Matrix::Set(const Scalar farray[16])
// {
// 	// This could be improved on a PC with a fast copy 32 bit aligned, 32 bit packed
// 	memcpy(m, farray, sizeof(Scalar)*16);
// }

// void Matrix::Set(const Vec4 &xv, const Vec4 &yv, const Vec4 &zv, const Vec4 &pv)
// {
// 	AxisX() = xv;
// 	AxisY() = yv;
// 	AxisZ() = zv;
// 	Pos() = pv;

// 	// just in case there is garbage in the incoming vector's 4th component
// 	m[ 3] = 0;   
// 	m[ 7] = 0;    
// 	m[11] = 0;      
// 	m[15] = 1;
// }



// void Matrix::Set(const Quaternion &q, const Vec4 &pv)
// {
// 	Scalar x2 = q.x + q.x;
// 	Scalar y2 = q.y + q.y;
// 	Scalar z2 = q.z + q.z;

// 	Scalar wx = q.w*x2;
// 	Scalar wy = q.w*y2;
// 	Scalar wz = q.w*z2;

// 	Scalar xx = q.x*x2;
// 	Scalar xy = q.x*y2;
// 	Scalar xz = q.x*z2;

// 	Scalar yy = q.y*y2;
// 	Scalar yz = q.y*z2;

// 	Scalar zz = q.z*z2;

// 	m[ 0] = 1 - (yy + zz);
// 	m[ 1] = xy + wz;
// 	m[ 2] = xz - wy;
// 	m[ 3] = 0;

// 	m[ 4] = xy - wz;
// 	m[ 5] = 1 - (xx + zz);
// 	m[ 6] = yz + wx;
// 	m[ 7] = 0;

// 	m[ 8] = xz + wy;
// 	m[ 9] = yz - wx;
// 	m[10]= 1 - (xx + yy);
// 	m[11] = 0;

// 	m[12] = pv.x;
// 	m[13] = pv.y;
// 	m[14] = pv.z;
// 	m[15] = 1;
// }

//
// Operator overloads
//

// Matrix &Matrix::operator=(const Matrix &rhs)
// {
// 	memcpy(m, rhs.m, sizeof(Scalar)*16);
// 	return *this;
// }

// Matrix &Matrix::operator+=(const Vec4 &rhs)
// {
// 	m[12] += rhs.x;
// 	m[13] += rhs.y;
// 	m[14] += rhs.z;
// 	return *this;
// }
	 
// 	Matrix &Matrix::operator*=(const Matrix &rhs)
// 	{
// 	// Matrix math is row * column
// 	Matrix tmp;
// 	tmp.m[ 0]=m[ 0]*rhs.m[ 0] + m[ 1]*rhs.m[ 4] + m[ 2]*rhs.m[ 8] + m[ 3]*rhs.m[12];
// 	tmp.m[ 1]=m[ 0]*rhs.m[ 1] + m[ 1]*rhs.m[ 5] + m[ 2]*rhs.m[ 9] + m[ 3]*rhs.m[13];
// 	tmp.m[ 2]=m[ 0]*rhs.m[ 2] + m[ 1]*rhs.m[ 6] + m[ 2]*rhs.m[10] + m[ 3]*rhs.m[14];
// 	tmp.m[ 3]=m[ 0]*rhs.m[ 3] + m[ 1]*rhs.m[ 7] + m[ 2]*rhs.m[12] + m[ 3]*rhs.m[15];

// 	tmp.m[ 4]=m[ 4]*rhs.m[ 0] + m[ 5]*rhs.m[ 4] + m[ 6]*rhs.m[ 8] + m[ 7]*rhs.m[12];
// 	tmp.m[ 5]=m[ 4]*rhs.m[ 1] + m[ 5]*rhs.m[ 5] + m[ 6]*rhs.m[ 9] + m[ 7]*rhs.m[13];
// 	tmp.m[ 6]=m[ 4]*rhs.m[ 2] + m[ 5]*rhs.m[ 6] + m[ 6]*rhs.m[10] + m[ 7]*rhs.m[14];
// 	tmp.m[ 7]=m[ 4]*rhs.m[ 3] + m[ 5]*rhs.m[ 7] + m[ 6]*rhs.m[12] + m[ 7]*rhs.m[15];

// 	tmp.m[ 8]=m[ 8]*rhs.m[ 0] + m[ 9]*rhs.m[ 4] + m[10]*rhs.m[ 8] + m[11]*rhs.m[12];
// 	tmp.m[ 9]=m[ 8]*rhs.m[ 1] + m[ 9]*rhs.m[ 5] + m[10]*rhs.m[ 9] + m[11]*rhs.m[13];
// 	tmp.m[10]=m[ 8]*rhs.m[ 2] + m[ 9]*rhs.m[ 6] + m[10]*rhs.m[10] + m[11]*rhs.m[14];
// 	tmp.m[11]=m[ 8]*rhs.m[ 3] + m[ 9]*rhs.m[ 7] + m[10]*rhs.m[12] + m[11]*rhs.m[15];

// 	tmp.m[12]=m[12]*rhs.m[ 0] + m[13]*rhs.m[ 4] + m[14]*rhs.m[ 8] + m[15]*rhs.m[12];
// 	tmp.m[13]=m[12]*rhs.m[ 1] + m[13]*rhs.m[ 5] + m[14]*rhs.m[ 9] + m[15]*rhs.m[13];
// 	tmp.m[14]=m[12]*rhs.m[ 2] + m[13]*rhs.m[ 6] + m[14]*rhs.m[10] + m[15]*rhs.m[14];
// 	tmp.m[15]=m[12]*rhs.m[ 3] + m[13]*rhs.m[ 7] + m[14]*rhs.m[12] + m[15]*rhs.m[15];

// 	memcpy(m, tmp.m, sizeof(Scalar)*16);
// 	return *this;
// }

// 	Matrix Matrix::operator*(const Matrix &rhs) const
// 	{
// 	Matrix tmp;
// 	tmp.m[ 0]=m[ 0]*rhs.m[ 0] + m[ 1]*rhs.m[ 4] + m[ 2]*rhs.m[ 8] + m[ 3]*rhs.m[12];
// 	tmp.m[ 1]=m[ 0]*rhs.m[ 1] + m[ 1]*rhs.m[ 5] + m[ 2]*rhs.m[ 9] + m[ 3]*rhs.m[13];
// 	tmp.m[ 2]=m[ 0]*rhs.m[ 2] + m[ 1]*rhs.m[ 6] + m[ 2]*rhs.m[10] + m[ 3]*rhs.m[14];
// 	tmp.m[ 3]=m[ 0]*rhs.m[ 3] + m[ 1]*rhs.m[ 7] + m[ 2]*rhs.m[12] + m[ 3]*rhs.m[15];

// 	tmp.m[ 4]=m[ 4]*rhs.m[ 0] + m[ 5]*rhs.m[ 4] + m[ 6]*rhs.m[ 8] + m[ 7]*rhs.m[12];
// 	tmp.m[ 5]=m[ 4]*rhs.m[ 1] + m[ 5]*rhs.m[ 5] + m[ 6]*rhs.m[ 9] + m[ 7]*rhs.m[13];
// 	tmp.m[ 6]=m[ 4]*rhs.m[ 2] + m[ 5]*rhs.m[ 6] + m[ 6]*rhs.m[10] + m[ 7]*rhs.m[14];
// 	tmp.m[ 7]=m[ 4]*rhs.m[ 3] + m[ 5]*rhs.m[ 7] + m[ 6]*rhs.m[12] + m[ 7]*rhs.m[15];

// 	tmp.m[ 8]=m[ 8]*rhs.m[ 0] + m[ 9]*rhs.m[ 4] + m[10]*rhs.m[ 8] + m[11]*rhs.m[12];
// 	tmp.m[ 9]=m[ 8]*rhs.m[ 1] + m[ 9]*rhs.m[ 5] + m[10]*rhs.m[ 9] + m[11]*rhs.m[13];
// 	tmp.m[10]=m[ 8]*rhs.m[ 2] + m[ 9]*rhs.m[ 6] + m[10]*rhs.m[10] + m[11]*rhs.m[14];
// 	tmp.m[11]=m[ 8]*rhs.m[ 3] + m[ 9]*rhs.m[ 7] + m[10]*rhs.m[12] + m[11]*rhs.m[15];

// 	tmp.m[12]=m[12]*rhs.m[ 0] + m[13]*rhs.m[ 4] + m[14]*rhs.m[ 8] + m[15]*rhs.m[12];
// 	tmp.m[13]=m[12]*rhs.m[ 1] + m[13]*rhs.m[ 5] + m[14]*rhs.m[ 9] + m[15]*rhs.m[13];
// 	tmp.m[14]=m[12]*rhs.m[ 2] + m[13]*rhs.m[ 6] + m[14]*rhs.m[10] + m[15]*rhs.m[14];
// 	tmp.m[15]=m[12]*rhs.m[ 3] + m[13]*rhs.m[ 7] + m[14]*rhs.m[12] + m[15]*rhs.m[15];
// 	return tmp;
// }

// Row major, therefore we ONLY get v*M
// This would only work for column major

//	Vec4 Matrix::operator*(const Vec4 &v)
//	{
//	}

// // same as <x,y,z,1> * M
// Vec4 Matrix::Transform(const Vec4 &v) const
// {
// 	Vec4 ret;
// 	ret.x=v.x*m[ 0] + v.y*m[ 4] + v.z*m[ 8] + m[12];
// 	ret.y=v.x*m[ 1] + v.y*m[ 5] + v.z*m[ 9] + m[13];
// 	ret.z=v.x*m[ 2] + v.y*m[ 6] + v.z*m[10] + m[14];
// 	ret.w=v.w; //v.x*m[ 3] + v.y*m[ 7] + v.z*m[11] + m[15];	// usually just 1
// 	return ret;
// }

// // same as <x,y,z,0> * M
// 	Vec4 Matrix::Rotate(const Vec4 &v) const
// 	{
// 	Vec4 ret;
// 	ret.x=v.x*m[ 0] + v.y*m[ 4] + v.z*m[ 8];
// 	ret.y=v.x*m[ 1] + v.y*m[ 5] + v.z*m[ 9];
// 	ret.z=v.x*m[ 2] + v.y*m[ 6] + v.z*m[10];
// 	ret.w=0;
// 	return ret;
// 	}

// Matrix &Matrix::FlipZ(Matrix &outMat) const
// {
// 													// Negate .z
// 	outMat.m[ 0] =  m[ 0];	outMat.m[ 1] =  m[ 1];	outMat.m[ 2] = -m[ 2];	outMat.m[ 3] =  m[ 3];
// 	outMat.m[ 4] =  m[ 4];	outMat.m[ 5] =  m[ 5];	outMat.m[ 6] = -m[ 6];	outMat.m[ 7] =  m[ 7];
// 	outMat.m[ 8] = -m[ 8];	outMat.m[ 9] = -m[ 9];	outMat.m[10] =  m[10];	outMat.m[11] =  m[11];	// Negate AxisZ
// 	outMat.m[12] =  m[12];	outMat.m[13] =  m[13];	outMat.m[14] = -m[14];	outMat.m[15] =  m[15];

// 	return outMat;
// }

// Matrix Matrix::Inverse() const
// {
// 	// Assumes an

// 	Matrix ret;
// 	// Transpose the rotation Matrix
// 	ret.m[ 0] = m[ 0];
// 	ret.m[ 1] = m[ 4];
// 	ret.m[ 2] = m[ 8];
// 	ret.m[ 3] = 0;

// 	ret.m[ 4] = m[ 1];
// 	ret.m[ 5] = m[ 5];
// 	ret.m[ 6] = m[ 9];
// 	ret.m[ 7] = 0;

// 	ret.m[ 8] = m[ 2];
// 	ret.m[ 9] = m[ 6];
// 	ret.m[10] = m[10];
// 	ret.m[11] = 0;

// 	// This is right, but it causes data to be copied for the negate, and we can make it slightly more efficient
// 	// by unrolling it
// 	// ret.Pos() = -ret.Rotate(Pos());

// 	ret.m[12] = -(m[12]*ret.m[ 0] + m[13]*ret.m[ 4] + m[14]*ret.m[ 8]);
// 	ret.m[13] = -(m[12]*ret.m[ 1] + m[13]*ret.m[ 5] + m[14]*ret.m[ 9]);
// 	ret.m[14] = -(m[12]*ret.m[ 2] + m[13]*ret.m[ 6] + m[14]*ret.m[10]);
// 	ret.m[15] = 1;

// 	return ret;
// }

// Matrix Matrix::Transpose() const
// {
// 	Matrix ret;
// 	ret.m[ 0] = m[ 0];
// 	ret.m[ 1] = m[ 4];
// 	ret.m[ 2] = m[ 8];
// 	ret.m[ 3] = m[12];

// 	ret.m[ 4] = m[ 1];
// 	ret.m[ 5] = m[ 5];
// 	ret.m[ 6] = m[ 9];
// 	ret.m[ 7] = m[13];

// 	ret.m[ 8] = m[ 2];
// 	ret.m[ 9] = m[ 6];
// 	ret.m[10] = m[10];
// 	ret.m[11] = m[14];

// 	ret.m[12] = m[ 3];
// 	ret.m[13] = m[ 7];
// 	ret.m[14] = m[11];
// 	ret.m[15] = m[15];
// 	return ret;
// }

ostream &operator<<(ostream &os, const MatrixCppImpl4x4 &m)
{
	os << std::fixed << std::setprecision(2);
    os << "[" << std::setw(5) << m.AxisX().x << ", " << std::setw(5) << m.AxisX().y << ", " << std::setw(5) << m.AxisX().z << ", " << std::setw(5) << m.AxisX().w << "]\n";
    os << "[" << std::setw(5) << m.AxisY().x << ", " << std::setw(5) << m.AxisY().y << ", " << std::setw(5) << m.AxisY().z << ", " << std::setw(5) << m.AxisY().w << "]\n";
    os << "[" << std::setw(5) << m.AxisZ().x << ", " << std::setw(5) << m.AxisZ().y << ", " << std::setw(5) << m.AxisZ().z << ", " << std::setw(5) << m.AxisZ().w << "]\n";
    os << "[" << std::setw(5) << m.Pos().x << ", "   << std::setw(5) << m.Pos().y << ", "   << std::setw(5) << m.Pos().z << ", "   << std::setw(5) << m.Pos().w << "]";
	return os;
}

ostream &operator<<(ostream &os, const Matrix &m) {
	return os << m._impl;
}


}  // namespace mathing
