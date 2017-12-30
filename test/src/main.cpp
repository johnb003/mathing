#include "gtest/gtest.h"
#include "mathing/matrix.h"
#include "mathing/util.h"

#define MAT_EPSILON 1.0e-15

#define EXPECT_MATRIX_EQ(a, b) \
  for (int i = 0; i < 16; ++i) { \
    EXPECT_NEAR(a.Buff()[i], b.Buff()[i], MAT_EPSILON) << a << " at index " << i; \
  }

#define EXPECT_MATRIX_ARY_EQ(a, ary) \
  for (int i = 0; i < 16; ++i) { \
    EXPECT_NEAR(a.Buff()[i], ary[i], MAT_EPSILON) << a << " at index " << i; \
  }

using namespace mathing;

static Scalar g_ident[16] = {
  1,0,0,0,
  0,1,0,0,
  0,0,1,0,
  0,0,0,1,
};

static Scalar g_ary9090[16] = {
  0,1,0,0,
  0,0,1,0,
  1,0,0,0,
  0,0,0,1,
};

static Scalar g_ary_z90[16] = {
  0,1,0,0,
  -1,0,0,0,
  0,0,1,0,
  0,0,0,1,
};

TEST(MatrixConstruct, Default) {
  Matrix m;
  EXPECT_MATRIX_ARY_EQ(m, g_ident);
}

TEST(MatrixConstruct, Copy) {
  Matrix m(Matrix::Identity());
  EXPECT_MATRIX_ARY_EQ(m, g_ident);
}

TEST(MatrixConstruct, Array) {
  Matrix m(g_ary9090);
  EXPECT_MATRIX_ARY_EQ(m, g_ary9090);
}

TEST(MatrixConstruct, From4Vecs) {
  Matrix m(Vec4::m_UnitY, Vec4::m_UnitZ, Vec4::m_UnitX, Vec4::m_UnitW);
  EXPECT_MATRIX_ARY_EQ(m, g_ary9090);
}

TEST(MatrixConstruct, From3Vecs) {
  Matrix m(Vec4::m_UnitY, Vec4::m_UnitZ, Vec4::m_UnitX);
  EXPECT_MATRIX_ARY_EQ(m, g_ary9090);
}

TEST(MatrixConstruct, FromQuatIdent) {
  // default construct quaternion is identity.
  // default translation, constructor for quats is 0
  Matrix m((Quaternion()));
  EXPECT_MATRIX_ARY_EQ(m, g_ident);
}

TEST(MatrixConstruct, FromQuat90AxisAngle) {
  Quaternion q_z90;
  q_z90.FromAxisAndAngle(0, 0, 1, Degree(90.0));
  Matrix m(q_z90);
  EXPECT_MATRIX_ARY_EQ(m, g_ary_z90);
}


#if 0
  // Logical messing around.
  // with goal of:
  // 0, 1, 0
  // 0, 0, 1
  // 1, 0, 0

  Quaternion yaw90;
  yaw90.FromEuler(PI_2, 0, 0);
  std::cout << "Yaw90: " << std::endl << yaw90 << std::endl;
  std::cout << "Yaw90 as Matrix: " << std::endl << Matrix(yaw90) << std::endl;

  Quaternion roll90;
  roll90.FromEuler(0, 0, PI_2);
  std::cout << "Roll90: " << std::endl << roll90 << std::endl;
  std::cout << "Roll90 as Matrix: " << std::endl << Matrix(roll90) << std::endl;

  Quaternion pitch90;
  pitch90.FromEuler(0, PI_2, 0);
  std::cout << "Pitch90: " << std::endl << pitch90 << std::endl;
  std::cout << "Pitch90 as Matrix: " << std::endl << Matrix(pitch90) << std::endl;

  // z, +90;  z is like roll
  // x, +90;  x is like pitch

  // Take something from global space:
  // v = {0, 0, 1}

  // transform it into
  // v = {1, 0, 0}

  // NOTE: matrix multiplication is backwards from the quat multiplication

  // CORRECT multiply is:
  // 

  // from global to local
  // v * pitch * roll

  // After x pitching 90, local yaw 90
  //                      or global roll 90


  // roll 90, pitch 90 == pitch 90, yaw 90



  Quaternion yawroll9090;
  roll90.FromEuler(-PI_2, 0, PI_2);
  std::cout << "yawroll9090: " << std::endl << yawroll9090 << std::endl;
  std::cout << "yawroll9090 as Matrix: " << std::endl << Matrix(yawroll9090) << std::endl;


  std::cout << std::endl;

  // GOAL:
  // x -> +y
  // y -> +z
  // z -> +x


  // think of quat order
  Quaternion q = roll90 * yaw90;
  std::cout << "q: " << std::endl << q << std::endl;
  std::cout << "q as Matrix: " << std::endl << Matrix(q) << std::endl;
  std::cout << "Matrix from same rots: " << std::endl << Matrix(yaw90) * Matrix(roll90) << std::endl;

  // Yaw 90
  // Roll 90
//  q.FromEuler(PI_2, 0, 0);
//  q.FromAxisAndAngle()
  Matrix m(Vec4::m_UnitY, Vec4::m_UnitZ, -Vec4::m_UnitX);
  EXPECT_MATRIX_ARY_EQ(m, g_ary9090)
#endif

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    int ret = RUN_ALL_TESTS();
    return ret;
}