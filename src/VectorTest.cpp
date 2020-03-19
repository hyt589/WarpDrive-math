#include <string>
#define CATCH_CONFIG_MAIN // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
#include "WarpMath.hpp"

using namespace WarpMath;

TEST_CASE("Vector initialization and templeate specialization", "[Vector]")
{

    vec4f vec = Vector<float, 4>(new float[4]{1.0f, 2.0f, 3.0f, 4.0f});
    vec3f vec_ijk = pvToVector(vec.ijk);
    vec3f ijk = Vec3f(2.0f, 3.0f, 4.0f);
    REQUIRE(vec.x == vec[0]);
    REQUIRE(vec[0] == 1.0f);
    REQUIRE(vec.xyz.b == 3.0f);
    REQUIRE(vec.w == 4.0f);
    REQUIRE(vec_ijk == ijk);
}

TEST_CASE("Vector-Vector operations", "[Vector]")
{

    vec4f vecL = Vec4f(1.2f, 3.4f, 2.6f, 4.3f);
    vec4f vecR = Vec4f(10.0f, 0.1f, 0.5f, 2.0f);
    SECTION("Addition - communitive")
    {
        vec4f result = vecL + vecR;
        vec4f correctResult = Vec4f(11.2f, 3.5f, 3.1f, 6.3f);
        REQUIRE(result == correctResult);
        result = vecR + vecL;
        REQUIRE(result == correctResult);
    }

    SECTION("Subtraction - not communitive"){
        vec4f result = vecL - vecR;
        vec4f correctResult = Vec4f(1.2f-10.0f, 3.4f-0.1f, 2.6f-0.5f,4.3f-2.0f);
        REQUIRE(result == correctResult);
        result = vecR - vecL;
        correctResult = -correctResult;
        REQUIRE(result == correctResult);
    }

    SECTION("Multiplication - communitive")
    {
        vec4f result = vecL * vecR;
        vec4f correctResult = Vec4f(12.0f, 0.34f, 1.3f, 8.6f);
        REQUIRE(result == correctResult);
        result = vecR * vecL;
        REQUIRE(result == correctResult);
    }

    SECTION("Division - Not communitive"){
        vec4f result = vecL / vecR;
        vec4f correctResult = Vec4f(1.2f/10.0f,3.4f/0.1f, 2.6f/0.5f, 4.3f/2.0f);
        REQUIRE(result == correctResult);
        result = vecR / vecL;
        correctResult = 1.0f / correctResult;
        REQUIRE(result == correctResult);
    }

    SECTION("Increment"){
        vec4f result;
        vec4f correctResult = Vec4f(11.2f, 3.5f, 3.1f, 6.3f);
        for (size_t i = 0; i < 4; i++)
        {
            result.data[i] = vecL.data[i];
        }
        result += vecR;
        REQUIRE(result == correctResult);
    }
}

TEST_CASE("Vector-scalar operation"){
    int scalar = 3;
    vec3i v1 = Vec3i(5,2,7);

    SECTION("Addition commutative"){
        vec3i result = scalar + v1;
        vec3i bench = Vec3i(5+3,2+3,7+3);
        REQUIRE(result == bench);
        result = v1 + scalar;
        REQUIRE(result == bench);
    }

    SECTION("Subtraction not commutative"){
        vec3i result = scalar - v1;
        vec3i bench = Vec3i(3-5,3-2,3-7);
        REQUIRE(result == bench);
        result = v1 - scalar;
        bench = -bench;
        REQUIRE(result == bench);
    }

    SECTION("Multiplication commutative"){
        vec3i result = scalar * v1;
        vec3i bench = Vec3i(3*5,3*2,3*7);
        REQUIRE(result == bench);
        result = v1 * scalar;
        REQUIRE(result == bench);
    }

    SECTION("Division not commutative"){
        vec3i result = scalar / v1;
        vec3i bench = Vec3i(3/5,3/2,3/7);
        REQUIRE(result == bench);
        result = v1 / scalar;
        bench = Vec3i(5/3, 2/3, 7/3);
        REQUIRE(result == bench);
    }
}

TEST_CASE("Vector type casting"){
    //do stuff
    vec4f floatVec = Vec4f(1.2f,3.4f,5.6f,7.8f);
    vec4i intVec = Vec4i(1,3,5,7);
    vec4f floatIntVec = Vec4f(1.0f,3.0f,5.0f,7.0f);
    vec4i convertedIntVec = floatVec.toType<int>();
    vec4f convertedFloatVec = intVec.toType<float>();
    REQUIRE(convertedIntVec == intVec);
    REQUIRE(convertedFloatVec == floatIntVec);
}

TEST_CASE("Vector dot product"){
    //do stuff
}

TEST_CASE("Vector cross product"){
    //do stuff
}