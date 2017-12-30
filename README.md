This is a basic 3D vector math library written in C++.

It's designed for use in real-time applications, where you need to transform stuff. It's comprised of vector, matrix and quaternion math, that rely heavily on special cases for 3D transforms. In fact, it's only used for translation and rotation operations, so the math isn't even designed to handle scale, projection, or shear deformations. Though these may be added in the future, they complicate some operations, such as inverse transform.


## Motivation

When I started the library there weren't any obvious great 3D math libraries for game dev needs. I started with a simple C++ implementation planning to eventually add intrinsics. I never actually got around to it, but now days it looks like the compilers are doing a damn good job of handling the intrinsics anyway.

I recognize a lot of people still just prefer to have their own math implementation, so this bridges the gap.  This library keeps the interface and the implementation separate. The goal is for the interface to be idiomatic, work with C++98, and just be a simple wrapper around the implementation that's completely transparent (inline), with the intent to make it easy to swap the implementation. This way, other libraries with math needs can just use this lightweight math library, worry-free.


## Version 0.0.1

While the underlying math is fairly mature from using the library on my own projects over the years, the refactoring to meet my stated goals has just begun.


## What in the library?


NOTE: The library does use operator overloads (for things like matrix multiplication, but NOT for things like dot product). This means there are several operations that imply temp storage and copies, and relies on inlining from the compiler to avert the overhead. This keeps the math in the code clean, and simple to read and write.

### A Scalar type

Scalar is just a word for a quantity. Like a real number, except it can store the quantity of an imaginary number too, so we don't say it's a "Real". It's just like a float or a double, and in fact that's exactly how it's defined in the library.

The rest of the library uses Scalar to represent components of vectors, matrices, and quaternions, to be consistent as either single or double precision. You can change the typedef in the scalar.h how you see fit.

#### A note about Scalars and the future of the library

In the future, I may actually actually use a different alias or a template parameter for the precision of the library, as the idea of a Scalar typically in the math library can actually be represented as a 4-component vector storing the value in each component. This can be beneficial when doing scalar-vector operations, as the compiler can use other intrinsic magic.

The idea is, if you're doing vector math to compute something, use Scalars for individual values, as even if it's not memory efficient, necessarily, it's fast. And for typical stored values, use the float or double.

In the future I may change the Scalar to be a vector under the hood.


### Vector Math

The vector class is currently always a 4 element vector, either as double or float (depending on the scalar definition). It's designed this way because it keeps the memory 16 byte aligned, which is important for a lot of intrinsic operations. This also works well with 4x4 matrix math allowing the 4th component (which is w by the way. x,y,z,w -- not w,x,y,z), to be 0 for directions or 1 for positions. For serializing, or networking or compressing the data, you're likely to use a different structure anyway. This all may be a little different on mobile, where the memory bandwidth is so bad.

To keep things simple, there currently is no Vector3 or Vector2 class. But they could easily be made if you so demanded it.

### Matrix Math

The matrices are hard-coded to 4x4 matrices, which can store rotation and translation (Orthonormal Affine Matrices). They are row-major, which implies that vector, matrix multiplication is: v * M, and not M * v. Which also allows you to deduce which way to multiply matrices correctly to combine transforms as the one closest to the vector is the most local transformation to the vector.

### Quaternion Math

Quaternions are an interesting tools in algebra, but we use a tiny subset of that power to solve a problem with 3D rotations. They work well for smoothly interpolating between two orientations and constructing rotations as an axis and angle (because they are closely related to axis and angle).

#### A few notes on how to think about quaternion rotations
Rotation quaternions are unit length (4-component vectors), and comprise of an imaginary vector part (3-component) and real part (1 scalar value). Technically speaking the real-part is kind of just a supplemental value as it's a sort of sin/cos compliment to the magnitude of the imaginary vector part. One way to think about the angle is to imagine you have a slider that goes between the imaginary vector part, and the real part. The position of the slider is related to the angle to rotate around the vector. Interestingly if the slider is all the way to the vector part, then there's no rotation. And if the slider is all the way to the real part, then there's "a lot" of rotation, but no axis to define which way to rotate around; it turns out, this is when the angle works out to be a full loop around the circle -- or in the case of quats, 2 loops, but lets not get lost in the details. In other words, it doesn't matter which way you rotate 360 degrees (or 720) is same as 0.


## History

### Originated From Skeletal Animation
This was a personal library I wrote that began with "Animadead, A Skeletal Animation Library". Over the years I've continued to use it and added on only what I needed as I went. Animadead was already open source, but I stopped contributing to that a long time ago, but wanted to bring this math library up to date and share it at least.  It didn't really make sense to release this independent of the animation library and keep the namespace, so I came up with a cheesy name for it. Some of my colleagues wrote "MathFu" and I'm super envious of that name for a math library! However, I didn't really like the interface in MathFu.

### Intrinsics
Funny enough, throughout the years using this library, the intent was always, "well I only need a basic c++ implementation now, and I'll go back and try to optimize it later with intrinsics when it matters." But, it really never mattered for what I was doing. In fact, at the time of re-releasing this as its own library, I did some disassembly to verify that the inlining worked out how I wanted, and there weren't a lot of unnecessary copies, and I was really quite surprised at how well the compiler managed to use intrinsics for everything.

I come from the game industry, and every math library I've used did not trust the compiler at all. The libraries rarely used operators because of the risk of copying by value, and would result in long form code where you controlled every temporary variable. Also they were hand-written with intrinsics for each platform. I figured it'd be nice to have my own library have that level of stuff, but anyway, in the end it really seems like compilers are good enough, and it leaves a nice gem for squeezing out performance later if it ever really came to that. Ultimately, this is all moot anyway; see the motivation section.



