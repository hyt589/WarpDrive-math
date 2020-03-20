#pragma once
#include <type_traits>
#include "Vector.hpp"

#define MAT_FUN(FUN_NAME, ...)                                                      \
    template <typename Arithmetic, size_t rows, size_t cols, typename isArithmetic> \
    auto Matrix<Arithmetic, rows, cols, isArithmetic>::FUN_NAME __VA_ARGS__

#define MAT_TMP_FUN(FUN_NAME, TMP, ...)                                             \
    template <typename Arithmetic, size_t rows, size_t cols, typename isArithmetic> \
    TMP auto Matrix<Arithmetic, rows, cols, isArithmetic>::FUN_NAME __VA_ARGS__

#define VEC(dim) Vector<Arithmetic, dim>

#define TEMP_matrix template <typename Arithmetic, size_t rows, size_t cols>
#define TEMP_mat_mul template <size_t newCols>

#define MAT_T Matrix<Arithmetic, rows, cols>

/*
TODO:
* scalar operation
* [done] element-wise operations; 
* [done] matrix multiplication; 
* matrix vector multiplication; 
* determinant;
* transpose;
* inverse;
* translation, rotation, and projection matrix generation functions;
*/

namespace WarpDrive
{
namespace Math
{

template <typename Arithmetic, size_t rows, size_t cols,
          typename = typename std::enable_if<std::is_arithmetic<Arithmetic>::value, Arithmetic>::type>
struct ProtoMatrix
{

    union {
        Vector<Arithmetic, cols> rowVectors[rows];
        Arithmetic arr[rows][cols];
    };

    ProtoMatrix(){};
};

template <typename Arithmetic, size_t rows, size_t cols, typename isArithmetic = Arithmetic>
struct Matrix : public ProtoMatrix<Arithmetic, rows, cols, isArithmetic>
{
    static_assert(rows > 1 || cols > 1, "1x1 matrices are not allowed");
    Matrix(){};
    auto colVectors();
    auto toString();

    auto operator+=(MAT_T &rhs); //return reference to this
    auto operator-=(MAT_T &rhs); //return reference to this
    auto operator*=(MAT_T &rhs); //return reference to this
    auto operator/=(MAT_T &rhs); //return reference to this

    TEMP_mat_mul auto mul(Matrix<Arithmetic, cols, newCols> &rhs);
};

template <typename Arithmetic, size_t rows, size_t cols = rows,
          typename isArithmetic = Arithmetic>
struct SquareMatrix : public Matrix<Arithmetic, rows, cols, isArithmetic>
{
    static_assert(rows == cols, "SquareMatrix must have equal rows and cols!");
};

TEMP_matrix auto operator+(MAT_T lhs, MAT_T &rhs);

TEMP_matrix auto operator-(MAT_T lhs, MAT_T &rhs);

TEMP_matrix auto operator*(MAT_T lhs, MAT_T &rhs);

TEMP_matrix auto operator/(MAT_T lhs, MAT_T &rhs);

} // namespace Math

/**************************************************************************************************************************/
/**************************************************************************************************************************/

namespace Math
{

MAT_FUN(colVectors, ())
{
    VEC(rows) *colVecArr = new VEC(rows)[cols];
    for (size_t i = 0; i < cols; i++)
    {
        VEC(rows) *vec = new VEC(rows)();
        colVecArr[i] = *vec;
    }

    for (size_t row = 0; row < rows; row++)
    {
        for (size_t col = 0; col < cols; col++)
        {
            colVecArr[col].data[row] = this->arr[row][col];
        }
    }
    return colVecArr;
}

MAT_FUN(toString, ())
{
    std::string s = "[";
    for (size_t row = 0; row < rows; row++)
    {
        if (row > 0)
        {
            s += " ";
        }
        s += "[";
        for (size_t col = 0; col < cols; col++)
        {
            s += std::to_string(this->arr[row][col]);
            if (col < cols - 1)
            {
                s += ", ";
            }
        }
        s += "]";
        if (row < rows - 1)
        {
            s += "\n";
        }
    }
    s += "]";
    return s;
}

MAT_FUN(operator+=,(MAT_T &rhs))
{
    for (size_t row = 0; row < rows; row++)
    {
        for (size_t col = 0; col < cols; col++)
        {
            this->arr[row][col] += rhs.arr[row][col];
        }
    }
    return *this;
}

MAT_FUN(operator-=,(MAT_T &rhs))
{
    for (size_t row = 0; row < rows; row++)
    {
        for (size_t col = 0; col < cols; col++)
        {
            this->arr[row][col] -= rhs.arr[row][col];
        }
    }
    return *this;
}

MAT_FUN(operator/=,(MAT_T &rhs))
{
    for (size_t row = 0; row < rows; row++)
    {
        for (size_t col = 0; col < cols; col++)
        {
            this->arr[row][col] /= rhs.arr[row][col];
        }
    }
    return *this;
}

MAT_FUN(operator*=,(MAT_T &rhs))
{
    for (size_t row = 0; row < rows; row++)
    {
        for (size_t col = 0; col < cols; col++)
        {
            this->arr[row][col] *= rhs.arr[row][col];
        }
    }
    return *this;
}

MAT_TMP_FUN(mul, TEMP_mat_mul, (Matrix<Arithmetic, cols, newCols> & rhs))
{
    Matrix<Arithmetic, rows, newCols> result;
    for (size_t row = 0; row < rows; row++)
    {
        for (size_t col = 0; col < newCols; col++)
        {
            result.arr[row][col] = ((this->rowVectors[row]) * (rhs.colVectors()[col])).sum();
        }
    }
    return result;
}

TEMP_matrix auto operator+(MAT_T lhs, MAT_T &rhs)
{
    return lhs += rhs;
}

TEMP_matrix auto
operator-(MAT_T lhs, MAT_T &rhs)
{
    return lhs -= rhs;
}

TEMP_matrix auto
operator*(MAT_T lhs, MAT_T &rhs)
{
    return lhs *= rhs;
}

TEMP_matrix auto
operator/(MAT_T lhs, MAT_T &rhs)
{
    return lhs /= rhs;
}

template <typename Arithmetic, size_t dim, typename isArithmetic>
template <size_t dim2>
auto Vector<Arithmetic, dim, isArithmetic>::cross(Vector<Arithmetic, dim2> &vec)
{
    Matrix<Arithmetic, dim, 1> lhs;
    Matrix<Arithmetic, 1, dim2> rhs;
    for (size_t row = 0; row < dim; row++)
    {
        lhs.arr[row][0] = this->data[row];
    }
    for (size_t col = 0; col < dim2; col++)
    {
        rhs.arr[0][col] = vec.data[col];
    }
    return lhs.mul(rhs);
}

} // namespace Math

} // namespace WarpDrive

#undef MAT_FUN
#undef MAT_TMP_FUN
#undef VEC
#undef TEMP_matrix
#undef TEMP_mat_mul
#undef MAT_T