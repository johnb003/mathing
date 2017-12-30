#include "mathing/vector.h"
#include <math.h>

#include <iostream>

using namespace std;

namespace mathing
{

////////////////////////
// Vec4
////////////////////////
Vec4::Vec4()
	: x(0), y(0), z(0), w(0)
{
}

Vec4::Vec4(const Vec4 &v)
	: x(v.x), y(v.y), z(v.z), w(v.w)
{
}

Vec4::Vec4(Scalar x, Scalar y, Scalar z, Scalar w)
	: x(x), y(y), z(z), w(w)
{
}

void Vec4::Set(Scalar x_arg, Scalar y_arg, Scalar z_arg, Scalar w_arg)
{
	x = x_arg;
	y = y_arg;
	z = z_arg;
	w = w_arg;
}

Vec4 &Vec4::operator=(const Vec4 &v)
{
	x = v.x;
	y = v.y;
	z = v.z;
	w = v.w;
	return *this;
}

Vec4 &Vec4::operator+=(const Vec4 &v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	w += v.w;
	return *this;
}

Vec4 &Vec4::operator-=(const Vec4 &v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	w -= v.w;
	return *this;
}

Vec4 &Vec4::operator*=(Scalar f)
{
	x *= f;
	y *= f;
	z *= f;
	w *= f;
	return *this;
}

Vec4 &Vec4::operator/=(Scalar f)
{
	x /= f;
	y /= f;
	z /= f;
	w /= f;
	return *this;
}

Vec4 Vec4::operator+(const Vec4 &v) const
{
	return Vec4(
		x + v.x,
		y + v.y,
		z + v.z,
		w + v.w);
}

Vec4 Vec4::operator-(const Vec4 &v) const
{
	return Vec4(
		x - v.x,
		y - v.y,
		z - v.z,
		w - v.w);
}

Vec4 Vec4::operator-() const
{
	return Vec4(
		-x,
		-y,
		-z,
		-w);
}


Vec4 Vec4::operator*(const Scalar f) const
{
	return Vec4(
		x * f,
		y * f,
		z * f,
		w * f);
}

Vec4 Vec4::operator/(const Scalar f) const
{
	return Vec4(
		x / f,
		y / f,
		z / f,
		w / f);
}

Scalar Vec4::Length3() const
{
	return sqrt(Length3Sqr());
}

Scalar Vec4::Length3Sqr() const
{
	return Dot3(*this, *this);
}

Scalar Vec4::Length4() const
{
	return sqrt(Length4Sqr());
}

Scalar Vec4::Length4Sqr() const
{
	return Dot4(*this, *this);
}

Scalar Vec4::Normalize3()
{
	Scalar dist = Length3();
	x /= dist;
	y /= dist;
	z /= dist;
	return dist;
}

Scalar Vec4::Normalize3Safe(Scalar threshold)
{
	Scalar dist = Length3();
	if (dist < threshold)
	{
		x = m_UnitX.x;
		y = m_UnitX.y;
		z = m_UnitX.z;
		return 0;
	}
	x /= dist;
	y /= dist;
	z /= dist;
	return dist;
}


Scalar Vec4::Normalize4()
{
	Scalar dist = Length4();
	*this /= dist;
	return dist;
}

Vec4 Vec4::Cross(const Vec4 &v1, const Vec4 &v2)
{
	Vec4 ret;
	ret.x = v1.y * v2.z - v2.y * v1.z;
	ret.y = v1.z * v2.x - v2.z * v1.x;
	ret.z = v1.x * v2.y - v2.x * v1.y;
	ret.w = 0.0;
	return ret;
}

Scalar Vec4::Dot3(const Vec4 &v1, const Vec4 &v2)
{
	return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
}

Scalar Vec4::Dot4(const Vec4 &v1, const Vec4 &v2)
{
	return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z + v1.w*v2.w;
}

Vec4 Vec4::Lerp(const Vec4 &from, const Vec4 &to, Scalar t)
{
	Vec4 ret;
	ret = from + (to - from)*t;
	return ret;
}

const Vec4 Vec4::m_UnitX(1, 0, 0, 0);
const Vec4 Vec4::m_UnitY(0, 1, 0, 0);
const Vec4 Vec4::m_UnitZ(0, 0, 1, 0);
const Vec4 Vec4::m_UnitW(0, 0, 0, 1);
const Vec4 Vec4::m_Zero(0, 0, 0, 0);

ostream &operator<<(ostream &os, const Vec4 &v)
{
	os << v.x << "," << v.y << "," << v.z << "," << v.w;
	return os;
}

Vec4 operator*(Scalar f, const Vec4 &rhs)
{
	return rhs * f;
}

}  // namespace mathing
