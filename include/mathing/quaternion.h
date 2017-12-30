#ifndef MATHING_QUATERNION_H
#define MATHING_QUATERNION_H

#include <iostream>

#include "scalar.h"
//#include "impl/matrix_impl.h"

namespace mathing
{

class Matrix;

/// Stores a 3D rotation, free of gimbal lock
/// Mathematical structure that you shouldn't even try to visualize. These are
/// good for interpolating between two rotations and applying successive
/// rotations.
class Quaternion
{
public:
	Scalar x, y, z, w;

	/// Initialize with no rotation
	Quaternion();
	/// Initialize with the rotation of another quaternion
	Quaternion(const Quaternion &q);
	/// Initialize with \p x,\p y,\p z,\p w
	Quaternion(Scalar x, Scalar y, Scalar z, Scalar w);
	/// Initialize from Euler rotations
	Quaternion(Scalar rotX, Scalar rotY, Scalar rotZ);
	/// Set to \p x,\p y,\p z,\p w
	void Set(Scalar x, Scalar y, Scalar z, Scalar w);
	/// Set from matrix \p mat

	void FromMatrix(const Matrix &mat);
	/// Set to \p x,\p y,\p z,\p w
	void FromAxisAndAngle(Scalar x, Scalar y, Scalar z, Scalar theta);
	/// Set from Euler angles \p yaw, \p pitch, and \p roll
	void FromEuler(Scalar yaw, Scalar pitch, Scalar roll);
	/// Get the Euler angles from the quaternion
	void GetEuler(Scalar &yaw, Scalar &pitch, Scalar &roll);
	/// Assign to the value of another quaternion
	Quaternion &operator=(const Quaternion &q);

	/// Rotate this quaternion by another
	Quaternion &operator*=(const Quaternion &q);
	/// Return the rotation of this quaternion then another, \p q

	// TODO: ORDER BACKWARDS FROM MATS
	Quaternion operator*(const Quaternion &q);

	/// Smoothly interpolates between two UNIT quaternions
	static Quaternion Slerp(const Quaternion &from, const Quaternion &to, Scalar t);
	/// Linearly interpolates between two UNIT quaternions
	static Quaternion Lerp(const Quaternion &from, const Quaternion &to, Scalar t);
};

std::ostream &operator<<(std::ostream &os, const Quaternion &q);

}  // namespace mathing


#endif  // MATHING_QUATERNION_H
