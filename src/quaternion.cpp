#include "mathing/quaternion.h"
#include "mathing/matrix.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>

#define DELTA 1e-10     // error tolerance used by quaternions

using namespace std;

namespace mathing
{

Quaternion::Quaternion()
: x(0), y(0), z(0), w(1)
{
}

Quaternion::Quaternion(const Quaternion &q)
: x(q.x), y(q.y), z(q.z), w(q.w)
{
}

Quaternion::Quaternion(Scalar x, Scalar y, Scalar z, Scalar w)
: x(x), y(y), z(z), w(w)
{
}

Quaternion::Quaternion(Scalar yaw, Scalar pitch, Scalar roll)
{
	Scalar cr = cos(yaw/2);
	Scalar cp = cos(pitch/2);
	Scalar cy = cos(roll/2);

	Scalar sr = sin(yaw/2);
	Scalar sp = sin(pitch/2);
	Scalar sy = sin(roll/2);

	Scalar cpcy = cp * cy;
	Scalar spsy = sp * sy;
	Scalar cpsy = cp * sy;
	Scalar spcy = sp * cy;

	w = cr * cpcy + sr * spsy;
	x = sr * cpcy - cr * spsy;
	y = cr * spcy + sr * cpsy;
	z = cr * cpsy - sr * spcy;
}

void Quaternion::Set(Scalar x_arg, Scalar y_arg, Scalar z_arg, Scalar w_arg)
{
	x=x_arg;
	y=y_arg;
	z=z_arg;
	w=w_arg;
}

void Quaternion::FromMatrix(const Matrix &mat)
{
	Scalar trace = mat.AxisX().x + mat.AxisY().y + mat.AxisZ().z + 1.0;
	if( trace > DELTA )
	{
		Scalar s = 0.5 / sqrt(trace);
		w = 0.25 / s;
		x = ( mat.AxisY().z - mat.AxisZ().y ) * s;
		y = ( mat.AxisZ().x - mat.AxisX().z ) * s;
		z = ( mat.AxisX().y - mat.AxisY().x ) * s;
	}
	else
	{
		if ( mat.AxisX().x > mat.AxisY().y && mat.AxisX().x > mat.AxisZ().z )
		{
			Scalar s = 2.0 * sqrt( 1.0 + mat.AxisX().x - mat.AxisY().y - mat.AxisZ().z);
			w = (mat.AxisZ().y - mat.AxisY().z ) / s;
			x = 0.25 * s;
			y = (mat.AxisY().x + mat.AxisX().y ) / s;
			z = (mat.AxisZ().x + mat.AxisX().z ) / s;
		}
		else if (mat.AxisY().y > mat.AxisZ().z)
		{
			Scalar s = 2.0 * sqrt( 1.0f + mat.AxisY().y - mat.AxisX().x - mat.AxisZ().z);
			w = (mat.AxisZ().x - mat.AxisX().z ) / s;
			x = (mat.AxisY().x + mat.AxisX().y ) / s;
			y = 0.25 * s;
			z = (mat.AxisZ().y + mat.AxisY().z ) / s;
		}
		else
		{
			Scalar s = 2.0 * sqrt( 1.0f + mat.AxisZ().z - mat.AxisX().x - mat.AxisY().y );
			w = (mat.AxisY().x - mat.AxisX().y ) / s;
			x = (mat.AxisZ().x + mat.AxisX().z ) / s;
			y = (mat.AxisZ().y + mat.AxisY().z ) / s;
			z = 0.25 * s;
		}
	}
}

void Quaternion::FromAxisAndAngle(Scalar xarg, Scalar yarg, Scalar zarg, Scalar theta)
{
	x=xarg * sin(theta/2);
	y=yarg * sin(theta/2);
	z=zarg * sin(theta/2);
	w=cos(theta/2);
}

void Quaternion::FromEuler(Scalar yaw, Scalar pitch, Scalar roll)
{
// Assuming the angles are in radians.
	Scalar c1 = cos(yaw/2);
	Scalar s1 = sin(yaw/2);
	Scalar c2 = cos(roll/2);
	Scalar s2 = sin(roll/2);
	Scalar c3 = cos(pitch/2);
	Scalar s3 = sin(pitch/2);

    Scalar c1c2 = c1*c2;
	Scalar c1s2 = c1*s2;
	Scalar s1c2 = s1*c2;
	Scalar s1s2 = s1*s2;

  	x = c1c2*s3 + s1s2*c3;
	y = s1c2*c3 + c1s2*s3;
	z = c1s2*c3 - s1c2*s3;
	w = c1c2*c3 - s1s2*s3;
}

/** assumes q1 is a normalised quaternion */
void Quaternion::GetEuler(Scalar &yaw, Scalar &pitch, Scalar &roll)
{
	Scalar test = x*y + z*w;
	if (test > 0.499)
	{
		// singularity at north pole
		yaw = 2 * atan2(x,w);
		pitch = (Scalar)M_PI_2;
		roll = 0;
		return;
	}
	else if (test < -0.499)
	{
		// singularity at south pole
		yaw = -2 * atan2(x,w);
		pitch = -(Scalar)M_PI_2;
		roll = 0;
		return;
	}
	
	Scalar sqx = x*x;
	Scalar sqy = y*y;
	Scalar sqz = z*z;
	yaw = atan2(2.0f*y*w - 2.0f*x*z, 1.0f - 2.0f*sqy - 2.0f*sqz);
	roll = asin(2.0f*test);
	pitch = atan2(2*x*w - 2*y*z , 1 - 2*sqx - 2*sqz);
}



	Quaternion &Quaternion::operator=(const Quaternion &q)
{
	x=q.x;
	y=q.y;
	z=q.z;
	w=q.w;
	return *this;
}

	Quaternion &Quaternion::operator*=(const Quaternion &q)
{
	Scalar tw = w;
	Scalar tx = x;
	Scalar ty = y;

	Scalar E, F, G, H;
//		A = (q1->w + q1->x)*(q2->w + q2->x);
//		B = (q1->z - q1->y)*(q2->y - q2->z);
//		C = (q1->w - q1->x)*(q2->y + q2->z); 
//		D = (q1->y + q1->z)*(q2->w - q2->x);
	E = (tx +  z)*(q.x + q.y);
	F = (tx -  z)*(q.x - q.y);
	G = (tw + ty)*(q.w - q.z);
	H = (tw - ty)*(q.w + q.z);

	w = /*B*/( z - ty)*(q.y - q.z) + (-E - F + G + H)/2;
	x = /*A*/(tw + tx)*(q.w + q.x) - ( E + F + G + H)/2; 
	y = /*C*/(tw - tx)*(q.y + q.z) + ( E - F + G - H)/2; 
	z = /*D*/(ty +  z)*(q.w - q.x) + ( E - F - G + H)/2;

	return *this;
}

	Quaternion Quaternion::operator*(const Quaternion &q)
{
	Quaternion ret;

	Scalar E, F, G, H;

//		A = (w + x)*(q.w + q.x);
//		B = (z - y)*(q.y - q.z);
//		C = (w - x)*(q.y + q.z);
//		D = (y + z)*(q.w - q.x);
	E = (x + z)*(q.x + q.y);
	F = (x - z)*(q.x - q.y);
	G = (w + y)*(q.w - q.z);
	H = (w - y)*(q.w + q.z);

	ret.w = /*B*/(z - y)*(q.y - q.z) + (-E - F + G + H)/2;
	ret.x = /*A*/(w + x)*(q.w + q.x) - ( E + F + G + H)/2; 
	ret.y = /*C*/(w - x)*(q.y + q.z) + ( E - F + G - H)/2; 
	ret.z = /*D*/(y + z)*(q.w - q.x) + ( E - F - G + H)/2;

	return ret;
}

/** Generate a quaternion by spherically-linearly interpolating between the poses
\p from	and \p to.
\param from The source \p from quaternion
\param to The source \p to quaternion
\param t In the range of 0 to 1, this value specifies how much to
	interpolate from the \p from quaternion to the \p to quaternion.
*/
Quaternion Quaternion::Slerp(const Quaternion &from, const Quaternion &to, Scalar t)
{
	// Most of this code is optimized for speed and not for readability
	// slerp(p,q,t) = (p*sin((1-t)*omega) + q*sin(t*omega)) / sin(omega)
	Quaternion ret;
	Scalar to1[4];
	Scalar omega, cosom, sinom;
	Scalar scale0, scale1;

	// cheap cosine (quaternion dot product)
	// calc cosine
	cosom = from.x*to.x + from.y*to.y + from.z*to.z + from.w*to.w;
	// adjust signs (if necessary)
	if (cosom < 0.0)
	{
		cosom = -cosom;
		to1[0] = -to.x;
		to1[1] = -to.y;
		to1[2] = -to.z;
		to1[3] = -to.w;
	} else  {
		to1[0] = to.x;
		to1[1] = to.y;
		to1[2] = to.z;
		to1[3] = to.w;
	}

	// calculate coefficients
	if ((1.0 - cosom) > DELTA) 
	{
		// standard case (slerp)
		omega = acos(cosom);
		sinom = sin(omega);
		scale0 = (Scalar)sin((1.0 - t) * omega) / sinom;
		scale1 = (Scalar)sin(t * omega) / sinom;
	} else {        
		// "from" and "to" quaternions are very close 
		//  ... so we can do a linear interpolation
		scale0 = 1.0f - t;
		scale1 = t;
	}

	// calculate final values
	ret.x = scale0*from.x + scale1*to1[0];
	ret.y = scale0*from.y + scale1*to1[1];
	ret.z = scale0*from.z + scale1*to1[2];
	ret.w = scale0*from.w + scale1*to1[3];
	return ret;
}

/** Generate a quaternion by linearly interpolating between the poses
\p from	and \p to.
\param from The source \p from quaternion
\param to The source \p to quaternion
\param t In the range of 0 to 1, this value specifies how much to
	interpolate from the \p from quaternion to the \p to quaternion.
*/
Quaternion Quaternion::Lerp(const Quaternion &from, const Quaternion &to, Scalar t)
{
	// Linearly interpolates between two quaternion positions
	// fast but not as nearly as smooth as Slerp
	Quaternion ret;
	Scalar to1[4];
	Scalar cosom;
	Scalar scale0, scale1;

	// cheap cosine (quaternion dot product)
	// calc cosine
	cosom = from.x*to.x + from.y*to.y + from.z*to.z + from.w*to.w;
    // adjust signs (if necessary)
    if ( cosom < 0.0 )
	{
		to1[0] = - to.x;
		to1[1] = - to.y;
		to1[2] = - to.z;
		to1[3] = - to.w;
    } else  {
		to1[0] = to.x;
		to1[1] = to.y;
		to1[2] = to.z;
		to1[3] = to.w;
    }

	// interpolate linearly
    scale0 = 1.0f - t;
    scale1 = t;

	// calculate final values
	ret.x = scale0*from.x + scale1*to1[0];
	ret.y = scale0*from.y + scale1*to1[1];
	ret.z = scale0*from.z + scale1*to1[2];
	ret.w = scale0*from.w + scale1*to1[3];
	return ret;
}

ostream &operator<<(ostream &os, const Quaternion &q)
{
	os << "(" << q.x << ", " << q.y << ", " << q.z << ", " << q.w << ")";
	return os;
}

}  // namespace mathing
