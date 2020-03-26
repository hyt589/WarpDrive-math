#include <string>
#include <iostream>
// #define CATCH_CONFIG_MAIN // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
#include "WarpMath.hpp"

using namespace WarpDrive::Math;

// TEST_CASE("Vector initialization and templeate specialization", "[Vector]")
// {

//     vec4f vec = Vector<float, 4>(new float[4]{1.0f, 2.0f, 3.0f, 4.0f});
//     vec3f vec_ijk = pvToVector(vec.ijk);
//     vec3f ijk = Vec3f(2.0f, 3.0f, 4.0f);
//     REQUIRE(vec.x == vec[0]);
//     REQUIRE(vec[0] == 1.0f);
//     REQUIRE(vec.xyz.b == 3.0f);
//     REQUIRE(vec.w == 4.0f);
//     REQUIRE(vec_ijk == ijk);
// }

// TEST_CASE("Vector-Vector operations", "[Vector]")
// {

//     vec4f vecL = Vec4f(1.2f, 3.4f, 2.6f, 4.3f);
//     vec4f vecR = Vec4f(10.0f, 0.1f, 0.5f, 2.0f);
//     SECTION("Addition - communitive")
//     {
//         vec4f result = vecL + vecR;
//         vec4f correctResult = Vec4f(11.2f, 3.5f, 3.1f, 6.3f);
//         REQUIRE(result == correctResult);
//         result = vecR + vecL;
//         REQUIRE(result == correctResult);
//     }

//     SECTION("Subtraction - not communitive"){
//         vec4f result = vecL - vecR;
//         vec4f correctResult = Vec4f(1.2f-10.0f, 3.4f-0.1f, 2.6f-0.5f,4.3f-2.0f);
//         REQUIRE(result == correctResult);
//         result = vecR - vecL;
//         correctResult = -correctResult;
//         REQUIRE(result == correctResult);
//     }

//     SECTION("Multiplication - communitive")
//     {
//         vec4f result = vecL * vecR;
//         vec4f correctResult = Vec4f(12.0f, 0.34f, 1.3f, 8.6f);
//         REQUIRE(result == correctResult);
//         result = vecR * vecL;
//         REQUIRE(result == correctResult);
//     }

//     SECTION("Division - Not communitive"){
//         vec4f result = vecL / vecR;
//         vec4f correctResult = Vec4f(1.2f/10.0f,3.4f/0.1f, 2.6f/0.5f, 4.3f/2.0f);
//         REQUIRE(result == correctResult);
//         result = vecR / vecL;
//         correctResult = 1.0f / correctResult;
//         REQUIRE(result == correctResult);
//     }

//     SECTION("Increment"){
//         vec4f result;
//         vec4f correctResult = Vec4f(11.2f, 3.5f, 3.1f, 6.3f);
//         for (size_t i = 0; i < 4; i++)
//         {
//             result.data[i] = vecL.data[i];
//         }
//         result += vecR;
//         REQUIRE(result == correctResult);
//     }
// }

// TEST_CASE("Vector-scalar operation"){
//     int scalar = 3;
//     vec3i v1 = Vec3i(5,2,7);

//     SECTION("Addition commutative"){
//         vec3i result = scalar + v1;
//         vec3i bench = Vec3i(5+3,2+3,7+3);
//         REQUIRE(result == bench);
//         result = v1 + scalar;
//         REQUIRE(result == bench);
//     }

//     SECTION("Subtraction not commutative"){
//         vec3i result = scalar - v1;
//         vec3i bench = Vec3i(3-5,3-2,3-7);
//         REQUIRE(result == bench);
//         result = v1 - scalar;
//         bench = -bench;
//         REQUIRE(result == bench);
//     }

//     SECTION("Multiplication commutative"){
//         vec3i result = scalar * v1;
//         vec3i bench = Vec3i(3*5,3*2,3*7);
//         REQUIRE(result == bench);
//         result = v1 * scalar;
//         REQUIRE(result == bench);
//     }

//     SECTION("Division not commutative"){
//         vec3i result = scalar / v1;
//         vec3i bench = Vec3i(3/5,3/2,3/7);
//         REQUIRE(result == bench);
//         result = v1 / scalar;
//         bench = Vec3i(5/3, 2/3, 7/3);
//         REQUIRE(result == bench);
//     }
// }

// TEST_CASE("Vector type casting"){
//     //do stuff
//     vec4f floatVec = Vec4f(1.2f,3.4f,5.6f,7.8f);
//     vec4i intVec = Vec4i(1,3,5,7);
//     vec4f floatIntVec = Vec4f(1.0f,3.0f,5.0f,7.0f);
//     vec4i convertedIntVec = floatVec.toType<int>();
//     vec4f convertedFloatVec = intVec.toType<float>();
//     REQUIRE(convertedIntVec == intVec);
//     REQUIRE(convertedFloatVec == floatIntVec);
// }

// TEST_CASE("Vector dot product"){
//     //do stuff
//     vec4f vec1 = Vec4f(1.2f,3.4f,5.6f,7.8f);
//     vec4f vec2 = Vec4f(8.7f,6.5f,4.3f,2.1f);
//     float bench = 1.2f*8.7f + 3.4f*6.5f + 5.6f*4.3f + 7.8f*2.1f;
//     float result = vec1.dot(vec2);
//     REQUIRE(result == bench);
//     result = vec2.dot(vec1);
//     REQUIRE(result == bench);
// }

// TEST_CASE("Vector norm"){
//     vec4i vec = Vec4i(1,2,3,4);
//     float bench = sqrt(1.0f*1.0f+2.0f*2.0f+3.0f*3.0f+4.0f*4.0f);
//     REQUIRE(vec.norm() == bench);
// }

// TEST_CASE("Dimensional operation"){
//     vec4f vec = Vec4f(5.2f,3.2f,-4.4f,6.5f);
//     REQUIRE(vec.min() == vec.z);
//     REQUIRE(vec.max() == vec.w);
//     REQUIRE(vec.sum() == (vec.x + vec.y + vec.z + vec.w));
// }

// TEST_CASE("Vector cross product"){
//     //do stuff
//     REQUIRE(true);
// }

// TEST_CASE("matrix playground"){
//     Matrix<int, 2, 2> mat;
//     mat.arr[0][0] = 1;
//     mat.arr[0][1] = 2;
//     mat.arr[1][0] = 3;
//     mat.arr[1][1] = 4;
//     UNSCOPED_INFO("matrix: " << mat.toString());
//     REQUIRE(true);
// }
int main(int argc, char const *argv[])
{
    Matrix<int, 3, 3> mat1;
    mat1.arr[0][0] = 1;
    mat1.arr[0][1] = 2;
    mat1.arr[0][2] = 3;
    mat1.arr[1][0] = 4;
    mat1.arr[1][1] = 5;
    mat1.arr[1][2] = 6;
    mat1.arr[2][0] = 7;
    mat1.arr[2][1] = 8;
    mat1.arr[2][2] = 8;

    // vec3f v1 = Vec3f(1.0f, 0.0f, 0.0f);
    // vec3f v2 = Vec3f(0.0f, 1.0f, 0.0f);
    // vec3i v3 = v1.toInt();
    // auto mat2 = mat1 + v3;
    // std::cout << mat2.toString() << "\n";
    // Matrix<int, 2, 2> mat3((int[2][2]){{1, 2},
    //                                    {3, 4}});
    std::cout << mat1.toString() << "\n";
    std::cout << mat1.toFloat().inverse().toString() << "\n";
    return 0;
}
