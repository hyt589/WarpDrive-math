#pragma once
#include <iostream>
#include <type_traits>
#include <omp.h>

namespace WarpMath
{
template <typename Arithmetic, size_t dim,
          typename = typename std::enable_if<std::is_arithmetic<Arithmetic>::value, Arithmetic>::type>
struct ProtoVector
{
public:
    Arithmetic data[dim];
    ProtoVector(Arithmetic arr[dim])
    {
        for (size_t i = 0; i < dim; i++)
        {
            this->data[i] = arr[i];
        }
    };
};

template <typename Arithmetic>
struct ProtoVector<Arithmetic, 4>
{
public:
    union {
        Arithmetic data[4];
        struct
        {
            Arithmetic x, y, z, w;
        };
        struct
        {
            Arithmetic r, g, b, a;
        };
        ProtoVector<Arithmetic, 3> xyz;
        ProtoVector<Arithmetic, 3> rgb;
        ProtoVector<Arithmetic, 2> xy;
    };
    ProtoVector(Arithmetic arr[4])
    {
        for (size_t i = 0; i < 4; i++)
        {
            this->data[i] = arr[i];
        }
    };
};

template <typename Arithmetic>
struct ProtoVector<Arithmetic, 3>
{
public:
    union {
        Arithmetic data[3];
        struct
        {
            Arithmetic x, y, z;
        };
        struct
        {
            Arithmetic r, g, b;
        };
        ProtoVector<Arithmetic, 2> xy;
    };
    ProtoVector(Arithmetic arr[3])
    {
        for (size_t i = 0; i < 3; i++)
        {
            this->data[i] = arr[i];
        }
    };
};

template <typename Arithmetic>
struct ProtoVector<Arithmetic, 2>
{
public:
    union {
        Arithmetic data[2];
        struct
        {
            Arithmetic x, y;
        };
    };
    ProtoVector(Arithmetic arr[2])
    {
        for (size_t i = 0; i < 2; i++)
        {
            this->data[i] = arr[i];
        }
    };
};

template <typename Arithmetic, size_t dim, typename isArithmetic = Arithmetic>
struct Vector : public ProtoVector<Arithmetic, dim, isArithmetic>
{
    Arithmetic sum();
    Arithmetic max();
    Arithmetic min();
    Vector<int, dim> toInt();
    Vector<float, dim> toFloat();

    template <typename T>
    typename std::enable_if<std::is_arithmetic<T>::value, T>::type toType();

    Vector<Arithmetic, dim> &operator+=(const Vector<Arithmetic, dim> &vec);
    Vector<Arithmetic, dim> &operator-=(const Vector<Arithmetic, dim> &vec);
    Vector<Arithmetic, dim> &operator*=(const Vector<Arithmetic, dim> &vec);
    Vector<Arithmetic, dim> &operator/=(const Vector<Arithmetic, dim> &vec);

    Arithmetic operator[](const size_t index);

    template <typename T, size_t n>
    friend std::ostream &operator<<(std::ostream &out, Vector<T, n> &vec);

    Vector(Arithmetic arr[dim]) : ProtoVector<Arithmetic, dim, isArithmetic>::ProtoVector(arr){};
};

template <typename Arithmetic, size_t dim>
Vector<Arithmetic, dim> add(Vector<Arithmetic, dim> lhs, Vector<Arithmetic, dim> &rhs);

template <typename Arithmetic, size_t dim>
Vector<Arithmetic, dim> add(Vector<Arithmetic, dim> lhs, Arithmetic rhs);

template <typename Arithmetic, size_t dim>
Vector<Arithmetic, dim> sub(Vector<Arithmetic, dim> lhs, Vector<Arithmetic, dim> &rhs);

template <typename Arithmetic, size_t dim>
Vector<Arithmetic, dim> sub(Vector<Arithmetic, dim> lhs, Arithmetic rhs);

template <typename Arithmetic, size_t dim>
Vector<Arithmetic, dim> mul(Vector<Arithmetic, dim> lhs, Vector<Arithmetic, dim> &rhs);

template <typename Arithmetic, size_t dim>
Vector<Arithmetic, dim> mul(Vector<Arithmetic, dim> lhs, Arithmetic rhs);

template <typename Arithmetic, size_t dim>
Vector<Arithmetic, dim> div(Vector<Arithmetic, dim> lhs, Vector<Arithmetic, dim> &rhs);

template <typename Arithmetic, size_t dim>
Vector<Arithmetic, dim> div(Vector<Arithmetic, dim> lhs, Arithmetic rhs);

template <typename Arithmetic, size_t dim>
Arithmetic dot(Vector<Arithmetic, dim> lhs, Vector<Arithmetic, dim> &rhs);

template <typename Arithmetic, size_t dim>
Vector<Arithmetic, dim> operator+(Vector<Arithmetic, dim> lhs, Vector<Arithmetic, dim> &rhs);

template <typename Arithmetic, size_t dim>
Vector<Arithmetic, dim> operator+(Vector<Arithmetic, dim> lhs, Arithmetic &rhs);

template <typename Arithmetic, size_t dim>
Vector<Arithmetic, dim> operator-(Vector<Arithmetic, dim> lhs, Vector<Arithmetic, dim> &rhs);

template <typename Arithmetic, size_t dim>
Vector<Arithmetic, dim> operator-(Vector<Arithmetic, dim> lhs, Arithmetic &rhs);

template <typename Arithmetic, size_t dim>
Vector<Arithmetic, dim> operator*(Vector<Arithmetic, dim> lhs, Vector<Arithmetic, dim> &rhs);

template <typename Arithmetic, size_t dim>
Vector<Arithmetic, dim> operator*(Vector<Arithmetic, dim> lhs, Arithmetic &rhs);

template <typename Arithmetic, size_t dim>
Vector<Arithmetic, dim> operator/(Vector<Arithmetic, dim> lhs, Vector<Arithmetic, dim> &rhs);

template <typename Arithmetic, size_t dim>
Vector<Arithmetic, dim> operator/(Vector<Arithmetic, dim> lhs, Arithmetic &rhs);


typedef Vector<int, 2> vec2i;
typedef Vector<int, 3> vec3i;
typedef Vector<int, 4> vec4i;

typedef Vector<float, 2> vec2f;
typedef Vector<float, 3> vec3f;
typedef Vector<float, 4> vec4f;

vec2i Vec2i(int x, int y);
vec3i Vec3i(int x, int y, int z);
vec4i Vec3i(int x, int y, int z, int w);

vec2f Vec2f(float x, float y);
vec3f Vec3f(float x, float y, float z);
vec4f Vec3f(float x, float y, float z, float w);

// template <typename Arithmetic, size_t dim>
// Vector<Arithmetic, dim> Vec(Arithmetic arr[dim]);

} // namespace WarpMath

//implmentation below
namespace WarpMath
{

template <typename Arithmetic, size_t dim, typename isArithmetic>
Arithmetic Vector<Arithmetic, dim, isArithmetic>::operator[](const size_t index)
{
    if (index >= 0 && index < dim)
    {
        return this->data[index];
    }
    else
    {
        throw "Index out of Bounds";
    }
}

template <typename Arithmetic, size_t dim, typename isArithmetic>
Vector<Arithmetic, dim> &Vector<Arithmetic, dim, isArithmetic>::operator+=(const Vector<Arithmetic, dim> &vec)
{
    for (size_t i = 0; i < dim; i++)
    {
        this->data[i] += vec.data[i];
    }
    return *this;
}

template <typename Arithmetic, size_t dim, typename isArithmetic>
Vector<Arithmetic, dim> &Vector<Arithmetic, dim, isArithmetic>::operator-=(const Vector<Arithmetic, dim> &vec)
{
    for (size_t i = 0; i < dim; i++)
    {
        this->data[i] -= vec.data[i];
    }
    return *this;
}

template <typename Arithmetic, size_t dim, typename isArithmetic>
Vector<Arithmetic, dim> &Vector<Arithmetic, dim, isArithmetic>::operator*=(const Vector<Arithmetic, dim> &vec)
{
    for (size_t i = 0; i < dim; i++)
    {
        this->data[i] *= vec.data[i];
    }
    return *this;
}

template <typename Arithmetic, size_t dim, typename isArithmetic>
Vector<Arithmetic, dim> &Vector<Arithmetic, dim, isArithmetic>::operator/=(const Vector<Arithmetic, dim> &vec)
{
    for (size_t i = 0; i < dim; i++)
    {
        this->data[i] /= vec.data[i];
    }
    return *this;
}
template <typename Arithmetic, size_t dim>
std::ostream &operator<<(std::ostream &out, Vector<Arithmetic, dim> &vec)
{
    out << "[";
    for (size_t i = 0; i < dim; i++)
    {
        out << vec[i];
        if (i < dim - 1)
        {
            out << ", ";
        }
    }
    out << "]\n";
    return out;
}

template <typename Arithmetic, size_t dim, typename isArithmetic>
Arithmetic Vector<Arithmetic, dim, isArithmetic>::min()
{
    size_t idx = 0;
    for (size_t i = 0; i < dim; i++)
    {
        if ((*this)[i] <= (*this)[idx])
        {
            idx = i;
        }
    }
    return (*this)[idx];
}

template <typename Arithmetic, size_t dim, typename isArithmetic>
Arithmetic Vector<Arithmetic, dim, isArithmetic>::max()
{
    size_t idx = 0;
    for (size_t i = 0; i < dim; i++)
    {
        if ((*this)[i] >= (*this)[idx])
        {
            idx = i;
        }
    }
    return (*this)[idx];
}

template <typename Arithmetic, size_t dim, typename isArithmetic>
Arithmetic Vector<Arithmetic, dim, isArithmetic>::sum()
{
    Arithmetic result = 0;
    for (size_t i = 0; i < dim; i++)
    {
        result += (*this)[i];
    }
    return result;
}

template <typename Arithmetic, size_t dim>
Vector<Arithmetic, dim> add(Vector<Arithmetic, dim> lhs, Vector<Arithmetic, dim> &rhs)
{
    return lhs += rhs;
}

template <typename Arithmetic, size_t dim>
Vector<Arithmetic, dim> mul(Vector<Arithmetic, dim> lhs, Arithmetic rhs)
{
    for (size_t i = 0; i < dim; i++)
    {
        lhs.data[i] += rhs;
    }
    return lhs;
}

template <typename Arithmetic, size_t dim>
Vector<Arithmetic, dim> sub(Vector<Arithmetic, dim> lhs, Vector<Arithmetic, dim> &rhs)
{
    return lhs -= rhs;
}

template <typename Arithmetic, size_t dim>
Vector<Arithmetic, dim> mul(Vector<Arithmetic, dim> lhs, Arithmetic rhs)
{
    for (size_t i = 0; i < dim; i++)
    {
        lhs.data[i] -= rhs;
    }
    return lhs;
}

template <typename Arithmetic, size_t dim>
Vector<Arithmetic, dim> mul(Vector<Arithmetic, dim> lhs, Vector<Arithmetic, dim> &rhs)
{
    return lhs *= rhs;
}

template <typename Arithmetic, size_t dim>
Vector<Arithmetic, dim> mul(Vector<Arithmetic, dim> lhs, Arithmetic rhs)
{
    for (size_t i = 0; i < dim; i++)
    {
        lhs.data[i] *= rhs;
    }
    return lhs;
}

template <typename Arithmetic, size_t dim>
Vector<Arithmetic, dim> div(Vector<Arithmetic, dim> lhs, Vector<Arithmetic, dim> &rhs)
{
    return lhs /= rhs;
}

template <typename Arithmetic, size_t dim>
Vector<Arithmetic, dim> mul(Vector<Arithmetic, dim> lhs, Arithmetic rhs)
{
    for (size_t i = 0; i < dim; i++)
    {
        lhs.data[i] /= rhs;
    }
    return lhs;
}

template <typename Arithmetic, size_t dim>
Arithmetic dot(Vector<Arithmetic, dim> lhs, Vector<Arithmetic, dim> &rhs)
{
    Arithmetic result = 0;
    for (size_t i = 0; i < dim; i++)
    {
        result += lhs[i] * rhs[i];
    }
    return result;
}

template <typename Arithmetic, size_t dim>
Vector<Arithmetic, dim> operator+(Vector<Arithmetic, dim> lhs, Vector<Arithmetic, dim> &rhs)
{
    return add(lhs, rhs);
}

template <typename Arithmetic, size_t dim>
Vector<Arithmetic, dim> operator-(Vector<Arithmetic, dim> lhs, Vector<Arithmetic, dim> &rhs)
{
    return sub(lhs, rhs);
}

template <typename Arithmetic, size_t dim>
Vector<Arithmetic, dim> operator*(Vector<Arithmetic, dim> lhs, Vector<Arithmetic, dim> &rhs)
{
    return mul(lhs, rhs);
}

template <typename Arithmetic, size_t dim>
Vector<Arithmetic, dim> operator/(Vector<Arithmetic, dim> lhs, Vector<Arithmetic, dim> &rhs)
{
    return div(lhs, rhs);
}

template <typename Arithmetic, size_t dim>
Vector<Arithmetic, dim> operator+(Vector<Arithmetic, dim> lhs, Arithmetic &rhs)
{
    return add(lhs, rhs);
}

template <typename Arithmetic, size_t dim>
Vector<Arithmetic, dim> operator-(Vector<Arithmetic, dim> lhs, Arithmetic &rhs)
{
    return sub(lhs, rhs);
}

template <typename Arithmetic, size_t dim>
Vector<Arithmetic, dim> operator*(Vector<Arithmetic, dim> lhs, Arithmetic &rhs)
{
    return mul(lhs, rhs);
}

template <typename Arithmetic, size_t dim>
Vector<Arithmetic, dim> operator/(Vector<Arithmetic, dim> lhs, Arithmetic &rhs)
{
    return div(lhs, rhs);
}


vec2i Vec2i(int x, int y)
{
    int data[2] = {x, y};
    return Vector<int, 2>(data);
}

vec3i Vec3i(int x, int y, int z)
{
    int data[3] = {x, y, z};
    return Vector<int, 3>(data);
}

vec4i Vec4i(int x, int y, int z, int w)
{
    int data[4] = {x, y, z, w};
    return Vector<int, 4>(data);
}

vec2f Vec2f(float x, float y)
{
    float data[2] = {x, y};
    return Vector<float, 2>(data);
}

vec3f Vec3f(float x, float y, float z)
{
    float data[3] = {x, y, z};
    return Vector<float, 3>(data);
}

vec4f Vec4f(float x, float y, float z, float w)
{
    float data[4] = {x, y, z, w};
    return Vector<float, 4>(data);
}

} // namespace WarpMath