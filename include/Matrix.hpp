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
* [done] determinant;
* [done] transpose;
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
    ProtoMatrix(Arithmetic data[rows][cols])
    {
        for (size_t row = 0; row < rows; row++)
        {
            for (size_t col = 0; col < cols; col++)
            {
                this->arr[row][col] = data[row][col];
            }
        }
    }
};

template <typename Arithmetic, size_t rows, size_t cols, typename isArithmetic = Arithmetic>
struct Matrix : public ProtoMatrix<Arithmetic, rows, cols, isArithmetic>
{
    static_assert(rows >= 1 && cols >= 1, "Matrix size must be larger that 1x1");
    Matrix(){};
    Matrix(Arithmetic data[rows][cols]) : ProtoMatrix<Arithmetic, rows, cols, isArithmetic>(data){};
    auto colVector(int col);
    auto toString();

    auto determinant();
    auto transpose();

    auto operator+=(MAT_T &rhs);     //return reference to this
    auto operator+=(Arithmetic rhs); //return reference to this

    auto operator+=(Vector<Arithmetic, cols> &rhs); //return reference to this

    auto operator-=(MAT_T &rhs);     //return reference to this
    auto operator-=(Arithmetic rhs); //return reference to this

    auto operator-=(Vector<Arithmetic, cols> &rhs); //return reference to this

    auto operator*=(MAT_T &rhs);     //return reference to this
    auto operator*=(Arithmetic rhs); //return reference to this

    auto operator*=(Vector<Arithmetic, cols> &rhs); //return reference to this

    auto operator/=(MAT_T &rhs);     //return reference to this
    auto operator/=(Arithmetic rhs); //return reference to this

    auto operator/=(Vector<Arithmetic, cols> &rhs); //return reference to this

    auto operator[](int i);

    TEMP_mat_mul auto mul(Matrix<Arithmetic, cols, newCols> &rhs);
    auto mul(Vector<Arithmetic, cols> &rhs);

    auto cof(size_t p, size_t q);
    auto adjoint();
    auto inverse();

    auto toFloat();
};

template <typename Arithmetic, size_t rows, size_t cols = rows,
          typename isArithmetic = Arithmetic>
struct SquareMatrix : Matrix<Arithmetic, rows, cols, isArithmetic>
{
    static_assert(rows == cols, "SquareMatrix must have equal rows and cols!");
};

TEMP_matrix auto operator+(MAT_T lhs, MAT_T &rhs);
TEMP_matrix auto operator+(MAT_T lhs, Arithmetic rhs);
TEMP_matrix auto operator+(MAT_T lhs, Vector<Arithmetic, cols> &rhs);

TEMP_matrix auto operator-(MAT_T lhs, MAT_T &rhs);
TEMP_matrix auto operator-(MAT_T lhs, Arithmetic rhs);
TEMP_matrix auto operator-(MAT_T lhs, Vector<Arithmetic, cols> &rhs);

TEMP_matrix auto operator*(MAT_T lhs, MAT_T &rhs);
TEMP_matrix auto operator*(MAT_T lhs, Arithmetic rhs);
TEMP_matrix auto operator*(MAT_T lhs, Vector<Arithmetic, cols> &rhs);

TEMP_matrix auto operator/(MAT_T lhs, MAT_T &rhs);
TEMP_matrix auto operator/(MAT_T lhs, Arithmetic rhs);
TEMP_matrix auto operator/(MAT_T lhs, Vector<Arithmetic, cols> &rhs);

} // namespace Math

/**************************************************************************************************************************/
/**************************************************************************************************************************/

namespace Math
{

MAT_FUN(colVector, (int col))
{
    std::unique_ptr<VEC(rows)> vec = std::make_unique<VEC(rows)>();
    for (size_t row = 0; row < rows; row++)
    {
        vec.get()->data[row] = this->arr[row][col];
    }
    return *(vec.get());
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

//private helper function to perform LU decomposition
template <typename T, size_t n>
static void LUdecomposition(T a[n][n], T l[n][n], T u[n][n])
{
    int i = 0, j = 0, k = 0;
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < n; j++)
        {
            if (j < i)
                l[j][i] = 0;
            else
            {
                l[j][i] = a[j][i];
                for (k = 0; k < i; k++)
                {
                    l[j][i] = l[j][i] - l[j][k] * u[k][i];
                }
            }
        }
        for (j = 0; j < n; j++)
        {
            if (j < i)
                u[i][j] = 0;
            else if (j == i)
                u[i][j] = 1;
            else
            {
                u[i][j] = a[i][j] / l[i][i];
                for (k = 0; k < i; k++)
                {
                    u[i][j] = u[i][j] - ((l[i][k] * u[k][j]) / l[i][i]);
                }
            }
        }
    }
}

MAT_FUN(determinant, ())
{
    static_assert(rows == cols, "Determinant for non square matrices is not defined");
    Arithmetic l[cols][cols];
    Arithmetic u[cols][cols];

    LUdecomposition(this->arr, l, u);
    Arithmetic detL = 1;
    Arithmetic detU = 1;
    for (size_t i = 0; i < cols; i++)
    {
        detL *= l[i][i];
        detU *= u[i][i];
    }
    return detL * detU;
}

MAT_FUN(transpose, ())
{
    Matrix<Arithmetic, cols, rows> result;
    for (size_t row = 0; row < cols; row++)
    {
        result.rowVectors[row] = this->colVector(row);
    }
    return result;
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

MAT_FUN(operator+=,(Arithmetic rhs))
{
    for (size_t row = 0; row < rows; row++)
    {
        for (size_t col = 0; col < cols; col++)
        {
            this->arr[row][col] += rhs;
        }
    }
    return *this;
}

MAT_FUN(operator+=,(Vector<Arithmetic, cols> &rhs))
{
    for (size_t i = 0; i < rows; i++)
    {
        this->rowVectors[i] += rhs;
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

MAT_FUN(operator-=,(Arithmetic rhs))
{
    for (size_t row = 0; row < rows; row++)
    {
        for (size_t col = 0; col < cols; col++)
        {
            this->arr[row][col] -= rhs;
        }
    }
    return *this;
}

MAT_FUN(operator-=,(Vector<Arithmetic, cols> &rhs))
{
    for (size_t i = 0; i < rows; i++)
    {
        this->rowVectors[i] -= rhs;
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

MAT_FUN(operator/=,(Vector<Arithmetic, cols> &rhs))
{
    for (size_t i = 0; i < rows; i++)
    {
        this->rowVectors[i] /= rhs;
    }
    return *this;
}

MAT_FUN(operator/=,(Arithmetic rhs))
{
    for (size_t row = 0; row < rows; row++)
    {
        for (size_t col = 0; col < cols; col++)
        {
            this->arr[row][col] /= rhs;
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

MAT_FUN(operator*=,(Arithmetic rhs))
{
    for (size_t row = 0; row < rows; row++)
    {
        for (size_t col = 0; col < cols; col++)
        {
            this->arr[row][col] *= rhs;
        }
    }
    return *this;
}

MAT_FUN(operator*=,(Vector<Arithmetic, cols> &rhs))
{
    for (size_t i = 0; i < rows; i++)
    {
        this->rowVectors[i] *= rhs;
    }
    return *this;
}

MAT_FUN(operator[],(int i))
{
    return this->rowVectors[i];
}

MAT_TMP_FUN(mul, TEMP_mat_mul, (Matrix<Arithmetic, cols, newCols> & rhs))
{
    Matrix<Arithmetic, rows, newCols> result;
    for (size_t row = 0; row < rows; row++)
    {
        for (size_t col = 0; col < newCols; col++)
        {
            auto rvec = rhs.colVector(col);
            result.arr[row][col] = ((this->rowVectors[row]) * rvec).sum();
        }
    }
    return result;
}

MAT_FUN(mul, (Vector<Arithmetic, cols> & rhs))
{
    Matrix<Arithmetic, cols, 1> rmatrix;
    for (size_t i = 0; i < cols; i++)
    {
        rmatrix.arr[0][i] = rhs.data[i];
    }

    return this->mul(rmatrix);
}

TEMP_matrix auto operator+(MAT_T lhs, MAT_T &rhs)
{
    MAT_T result;
    for (size_t row = 0; row < rows; row++)
    {
        for (size_t col = 0; col < cols; col++)
        {
            result.arr[row][col] = lhs.arr[row][col] + rhs.arr[row][col];
        }
    }
    return result;
}

TEMP_matrix auto operator+(MAT_T lhs, Arithmetic rhs)
{
    MAT_T result;
    for (size_t row = 0; row < rows; row++)
    {
        for (size_t col = 0; col < cols; col++)
        {
            result.arr[row][col] = lhs.arr[row][col] + rhs;
        }
    }
    return result;
}

TEMP_matrix auto operator+(MAT_T lhs, Vector<Arithmetic, cols> &rhs)
{
    MAT_T result;
    for (size_t i = 0; i < rows; i++)
    {
        result.rowVectors[i] = lhs.rowVectors[i] + rhs;
    }
    return result;
}

TEMP_matrix auto
operator-(MAT_T lhs, MAT_T &rhs)
{
    MAT_T result;
    for (size_t row = 0; row < rows; row++)
    {
        for (size_t col = 0; col < cols; col++)
        {
            result.arr[row][col] = lhs.arr[row][col] - rhs.arr[row][col];
        }
    }
    return result;
}

TEMP_matrix auto operator-(MAT_T lhs, Arithmetic rhs)
{
    MAT_T result;
    for (size_t row = 0; row < rows; row++)
    {
        for (size_t col = 0; col < cols; col++)
        {
            result.arr[row][col] = lhs.arr[row][col] - rhs;
        }
    }
    return result;
}

TEMP_matrix auto operator-(MAT_T lhs, Vector<Arithmetic, cols> &rhs)
{
    MAT_T result;
    for (size_t i = 0; i < rows; i++)
    {
        result.rowVectors[i] = lhs.rowVectors[i] - rhs;
    }
    return result;
}

TEMP_matrix auto
operator*(MAT_T lhs, MAT_T &rhs)
{
    MAT_T result;
    for (size_t row = 0; row < rows; row++)
    {
        for (size_t col = 0; col < cols; col++)
        {
            result.arr[row][col] = lhs.arr[row][col] * rhs.arr[row][col];
        }
    }
    return result;
}

TEMP_matrix auto operator*(MAT_T lhs, Arithmetic rhs)
{
    MAT_T result;
    for (size_t row = 0; row < rows; row++)
    {
        for (size_t col = 0; col < cols; col++)
        {
            result.arr[row][col] = lhs.arr[row][col] * rhs;
        }
    }
    return result;
}

TEMP_matrix auto operator*(MAT_T lhs, Vector<Arithmetic, cols> &rhs)
{
    MAT_T result;
    for (size_t i = 0; i < rows; i++)
    {
        result.rowVectors[i] = lhs.rowVectors[i] * rhs;
    }
    return result;
}

TEMP_matrix auto
operator/(MAT_T lhs, MAT_T &rhs)
{
    MAT_T result;
    for (size_t row = 0; row < rows; row++)
    {
        for (size_t col = 0; col < cols; col++)
        {
            result.arr[row][col] = lhs.arr[row][col] / rhs.arr[row][col];
        }
    }
    return result;
}

TEMP_matrix auto operator/(MAT_T lhs, Arithmetic rhs)
{
    MAT_T result;
    for (size_t row = 0; row < rows; row++)
    {
        for (size_t col = 0; col < cols; col++)
        {
            result.arr[row][col] = lhs.arr[row][col] / rhs;
        }
    }
    return result;
}

TEMP_matrix auto operator/(MAT_T lhs, Vector<Arithmetic, cols> &rhs)
{
    MAT_T result;
    for (size_t i = 0; i < rows; i++)
    {
        result.rowVectors[i] = lhs.rowVectors[i] / rhs;
    }
    return result;
}

template <typename Arithmetic, size_t dim, typename isArithmetic>
template <size_t dim2>
auto Vector<Arithmetic, dim, isArithmetic>::rowTimesCol(Vector<Arithmetic, dim2> &vec)
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

TEMP_matrix static void getCfactor(Arithmetic M[rows][cols], Arithmetic t[rows - 1][cols - 1], size_t p, size_t q)
{
    static_assert(rows == cols, "Not a square matrix");
    int n = rows;
    int i = 0, j = 0;
    for (int r = 0; r < n; r++)
    {
        for (int c = 0; c < n; c++) //Copy only those elements which are not in given row r and column c: {
            if (r != p && c != q)
            {
                t[i][j++] = M[r][c]; //If row is filled increase r index and reset c index
                if (j == n - 1)
                {
                    j = 0;
                    i++;
                }
            }
    }
}

MAT_FUN(cof, (size_t p, size_t q))
{
    Matrix<Arithmetic, rows - 1, cols - 1> cofactor;
    getCfactor<Arithmetic, rows, cols>(this->arr, cofactor.arr, p, q);
    return cofactor;
}

MAT_FUN(adjoint, ())
{
    Matrix<Arithmetic, rows, cols> adj;
    Arithmetic sign = 1;
    for (size_t row = 0; row < rows; row++)
    {
        for (size_t col = 0; col < cols; col++)
        {
            adj.arr[row][col] = sign * this->cof(row, col).determinant();
            sign = - sign;
        }
    }
    return adj;
}

MAT_FUN(inverse, ())
{
    Arithmetic det = this->determinant();

    assert(det != 0);

    return this->adjoint().transpose() * (1 / det);
}


MAT_FUN(toFloat, ())
{
    Matrix<float, rows, cols> fm;
    for (size_t row = 0; row < rows; row++)
    {
        for (size_t col = 0; col < cols; col++)
        {
            fm.arr[row][col] = this->arr[row][col];
        }
    }
    return fm;
}

} // namespace Math

} // namespace WarpDrive

#undef MAT_FUN
#undef MAT_TMP_FUN
#undef VEC
#undef TEMP_matrix
#undef TEMP_mat_mul
#undef MAT_T
