#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
#include "WarpMath.hpp"

using namespace WarpMath;

TEST_CASE("Vector initialization and templeate specialization","[Vector]"){
    
    vec4f vec = Vector<float, 4>(new float[4] {1.0,2.0,3.0,4.0});
    REQUIRE( vec.x == vec[0] );
    REQUIRE( vec[0] == 1.0 );
    REQUIRE( vec.xyz.b == 3.0 );
    REQUIRE( vec.w == 4.0 );
}

TEST_CASE("Vector-Vector operations", "[Vector]"){
    
    vec4f vecL = Vec4f(1.2,3.4,2.6,4.3);
    vec4f vecR = Vec4f(10.0,0.1,0.5, 2.0);
    vec4f result = vecL * vecR;
    REQUIRE(result.x == 12.0f);
    REQUIRE(result.y == 0.34f);
    REQUIRE(result.z == 1.3f);
    REQUIRE(result.w == 8.6f);
}