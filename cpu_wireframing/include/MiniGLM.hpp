#pragma once
#include <cmath>
#include <cassert>

namespace MiniGLM {

struct ivec2;
struct vec2;
struct vec3;
struct vec4;
struct mat4;

constexpr float pi = 3.14159265358979323846f;

inline float radians(float deg) {
    return deg * (pi / 180.0f);
}

struct vec2 {
    float x, y;

    vec2() : x(0), y(0) {}
    vec2(float v) : x(v), y(v) {}
    vec2(float x_, float y_) : x(x_), y(y_) {}

    float& operator[](int i) { assert(i>=0 && i<2); return (&x)[i]; }
    const float& operator[](int i) const { assert(i>=0 && i<2); return (&x)[i]; }

    vec2 operator+(const vec2& r) const { return vec2(x+r.x, y+r.y); }
    vec2 operator-(const vec2& r) const { return vec2(x-r.x, y-r.y); }
    vec2 operator*(float s) const { return vec2(x*s, y*s); }
    vec2 operator/(float s) const { return vec2(x/s, y/s); }
    vec2& operator+=(const vec2& r){ x+=r.x; y+=r.y; return *this; }
    vec2& operator-=(const vec2& r){ x-=r.x; y-=r.y; return *this; }
    vec2& operator*=(float s){ x*=s; y*=s; return *this; }
    vec2& operator/=(float s){ x/=s; y/=s; return *this; }
};

inline vec2 operator*(float s, const vec2& v) { return v*s; }

inline float dot(const vec2& a, const vec2& b) { return a.x*b.x + a.y*b.y; }

inline float length(const vec2& v) {
    return std::sqrt(dot(v, v));
}

inline vec2 normalize(const vec2& v) {
    float len = length(v);
    if (len == 0.0f) return vec2(0.0f);
    return v / len;
}

struct ivec2 {
    int x, y;

    ivec2() : x(0), y(0) {}
    ivec2(int v) : x(v), y(v) {}
    ivec2(int x_, int y_) : x(x_), y(y_) {}

    int& operator[](int i) { assert(i>=0 && i<2); return (&x)[i]; }
    const int& operator[](int i) const { assert(i>=0 && i<2); return (&x)[i]; }

    ivec2 operator+(const ivec2& r) const { return ivec2(x + r.x, y + r.y); }
    ivec2 operator-(const ivec2& r) const { return ivec2(x - r.x, y - r.y); }
    ivec2 operator*(int s) const { return ivec2(x * s, y * s); }
    ivec2 operator/(int s) const { return ivec2(x / s, y / s); }
    ivec2& operator+=(const ivec2& r) { x += r.x; y += r.y; return *this; }
    ivec2& operator-=(const ivec2& r) { x -= r.x; y -= r.y; return *this; }
    ivec2& operator*=(int s) { x *= s; y *= s; return *this; }
    ivec2& operator/=(int s) { x /= s; y /= s; return *this; }
};

// Scalar on left
inline ivec2 operator*(int s, const ivec2& v) { return v * s; }

// Utility functions
inline int dot(const ivec2& a, const ivec2& b) { return a.x * b.x + a.y * b.y; }
inline int lengthSq(const ivec2& v) { return dot(v, v); }
inline int manhattan(const ivec2& v) { return std::abs(v.x) + std::abs(v.y); }
inline bool operator==(const ivec2& a, const ivec2& b) { return a.x == b.x && a.y == b.y; }
inline bool operator!=(const ivec2& a, const ivec2& b) { return !(a == b); }



struct vec3 {
    float x, y, z;

    vec3() : x(0), y(0), z(0) {}
    vec3(float v) : x(v), y(v), z(v) {}
    vec3(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}

    float& operator[](int i) { assert(i>=0 && i<3); return (&x)[i]; }
    const float& operator[](int i) const { assert(i>=0 && i<3); return (&x)[i]; }

    vec3 operator+(const vec3& r) const { return vec3(x+r.x, y+r.y, z+r.z); }
    vec3 operator-(const vec3& r) const { return vec3(x-r.x, y-r.y, z-r.z); }
    vec3 operator*(float s) const { return vec3(x*s, y*s, z*s); }
    vec3 operator/(float s) const { return vec3(x/s, y/s, z/s); }
    vec3& operator+=(const vec3& r){ x+=r.x; y+=r.y; z+=r.z; return *this; }
    vec3& operator-=(const vec3& r){ x-=r.x; y-=r.y; z-=r.z; return *this; }
    vec3& operator*=(float s){ x*=s; y*=s; z*=s; return *this; }
    vec3& operator/=(float s){ x/=s; y/=s; z/=s; return *this; }
};

inline vec3 operator*(float s, const vec3& v) { return v*s; }

inline float dot(const vec3& a, const vec3& b) { return a.x*b.x + a.y*b.y + a.z*b.z; }

inline vec3 cross(const vec3& a, const vec3& b) {
    return vec3(
        a.y*b.z - a.z*b.y,
        a.z*b.x - a.x*b.z,
        a.x*b.y - a.y*b.x
    );
}

inline float length(const vec3& v) {
    return std::sqrt(dot(v, v));
}

inline vec3 normalize(const vec3& v) {
    float len = length(v);
    if (len == 0.0f) return vec3(0.0f);
    return v / len;
}

struct vec4 {
    float x, y, z, w;

    vec4() : x(0), y(0), z(0), w(0) {}
    vec4(float v) : x(v), y(v), z(v), w(v) {}
    vec4(float x_, float y_, float z_, float w_) : x(x_), y(y_), z(z_), w(w_) {}
    vec4(const vec3& v, float w_) : x(v.x), y(v.y), z(v.z), w(w_) {}

    float& operator[](int i) { assert(i>=0 && i<4); return (&x)[i]; }
    const float& operator[](int i) const { assert(i>=0 && i<4); return (&x)[i]; }

    vec4 operator+(const vec4& r) const { return vec4(x+r.x, y+r.y, z+r.z, w+r.w); }
    vec4 operator-(const vec4& r) const { return vec4(x-r.x, y-r.y, z-r.z, w-r.w); }
    vec4 operator*(float s) const { return vec4(x*s, y*s, z*s, w*s); }
    vec4 operator/(float s) const { return vec4(x/s, y/s, z/s, w/s); }
    vec4& operator+=(const vec4& r){ x+=r.x; y+=r.y; z+=r.z; w+=r.w; return *this; }
    vec4& operator-=(const vec4& r){ x-=r.x; y-=r.y; z-=r.z; w-=r.w; return *this; }
    vec4& operator*=(float s){ x*=s; y*=s; z*=s; w*=s; return *this; }
    vec4& operator/=(float s){ x/=s; y/=s; z/=s; w/=s; return *this; }
    
    vec3 xyz() const { return vec3(x, y, z); }
};

inline vec4 operator*(float s, const vec4& v) { return v*s; }

inline float dot(const vec4& a, const vec4& b) { return a.x*b.x + a.y*b.y + a.z*b.z + a.w*b.w; }

inline float length(const vec4& v) {
    return std::sqrt(dot(v, v));
}

inline vec4 normalize(const vec4& v) {
    float len = length(v);
    if (len == 0.0f) return vec4(0.0f);
    return v / len;
}


struct mat4 {
    float m[16];

    mat4() {
        for (int i=0;i<16;++i) m[i] = 0.0f;
        m[0]=m[5]=m[10]=m[15]=1.0f;
    }

    explicit mat4(float diag) {
        for (int i=0;i<16;++i) m[i] = 0.0f;
        m[0]=m[5]=m[10]=m[15]=diag;
    }

    static mat4 identity() { return mat4(1.0f); }

    float* data() { return m; }
    const float* data() const { return m; }

    float& at(int col, int row) {
        assert(col>=0 && col<4 && row>=0 && row<4);
        return m[col*4 + row];
    }
    const float& at(int col, int row) const {
        assert(col>=0 && col<4 && row>=0 && row<4);
        return m[col*4 + row];
    }
    struct col_proxy {
        float* base; // points to m[col*4]
        float& operator[](int row) {
            assert(row>=0 && row<4);
            return base[row];
        }
        const float& operator[](int row) const {
            assert(row>=0 && row<4);
            return base[row];
        }
    };

    struct const_col_proxy {
        const float* base;
        const float& operator[](int row) const {
            assert(row>=0 && row<4);
            return base[row];
        }
    };

    col_proxy operator[](int col) {
        assert(col>=0 && col<4);
        return col_proxy{ m + col*4 };
    }
    const const_col_proxy operator[](int col) const {
        assert(col>=0 && col<4);
        return const_col_proxy{ m + col*4 };
    }

    mat4 operator*(const mat4& r) const {
        mat4 out;
        for (int col=0; col<4; ++col) {
            for (int row=0; row<4; ++row) {
                double sum = 0.0;
                for (int k=0; k<4; ++k) {
                    sum += double(at(k, row)) * double(r.at(col, k));
                }
                out.at(col, row) = static_cast<float>(sum);
            }
        }
        return out;
    }

    vec3 multiply_point(const vec3& v) const {
        double x = double(at(0,0))*v.x + double(at(1,0))*v.y + double(at(2,0))*v.z + double(at(3,0))*1.0;
        double y = double(at(0,1))*v.x + double(at(1,1))*v.y + double(at(2,1))*v.z + double(at(3,1))*1.0;
        double z = double(at(0,2))*v.x + double(at(1,2))*v.y + double(at(2,2))*v.z + double(at(3,2))*1.0;
        double w = double(at(0,3))*v.x + double(at(1,3))*v.y + double(at(2,3))*v.z + double(at(3,3))*1.0;
        if (w != 0.0) { x/=w; y/=w; z/=w; }
        return vec3(static_cast<float>(x), static_cast<float>(y), static_cast<float>(z));
    }

    vec3 multiply_direction(const vec3& v) const {
        double x = double(at(0,0))*v.x + double(at(1,0))*v.y + double(at(2,0))*v.z;
        double y = double(at(0,1))*v.x + double(at(1,1))*v.y + double(at(2,1))*v.z;
        double z = double(at(0,2))*v.x + double(at(1,2))*v.y + double(at(2,2))*v.z;
        return vec3(static_cast<float>(x), static_cast<float>(y), static_cast<float>(z));
    }

    vec4 operator*(const vec4& v) const {
        double x = double(at(0,0))*v.x + double(at(1,0))*v.y + double(at(2,0))*v.z + double(at(3,0))*v.w;
        double y = double(at(0,1))*v.x + double(at(1,1))*v.y + double(at(2,1))*v.z + double(at(3,1))*v.w;
        double z = double(at(0,2))*v.x + double(at(1,2))*v.y + double(at(2,2))*v.z + double(at(3,2))*v.w;
        double w = double(at(0,3))*v.x + double(at(1,3))*v.y + double(at(2,3))*v.z + double(at(3,3))*v.w;
        return vec4(static_cast<float>(x), static_cast<float>(y), static_cast<float>(z), static_cast<float>(w));
    }

};

inline mat4 translate(const mat4& m_in, const vec3& t) {
    mat4 T = mat4::identity();
    T.at(3,0) = t.x;
    T.at(3,1) = t.y;
    T.at(3,2) = t.z;
    return T * m_in;
}

inline mat4 translate(const vec3& t) {
    mat4 T = mat4::identity();
    T.at(3,0) = t.x;
    T.at(3,1) = t.y;
    T.at(3,2) = t.z;
    return T;
}

inline mat4 scale(const mat4& m_in, const vec3& s) {
    mat4 S = mat4::identity();
    S.at(0,0)=s.x; S.at(1,1)=s.y; S.at(2,2)=s.z;
    return S * m_in;
}

inline mat4 scale(const vec3& s) {
    mat4 S = mat4::identity();
    S.at(0,0)=s.x; S.at(1,1)=s.y; S.at(2,2)=s.z;
    return S;
}

inline mat4 rotate(const mat4& m_in, float angle, const vec3& axis_raw) {
    vec3 axis = normalize(axis_raw);
    double c = std::cos(double(angle));
    double s = std::sin(double(angle));
    double t = 1.0 - c;

    double x = axis.x, y = axis.y, z = axis.z;

    mat4 R;
    R.at(0,0) = float(t*x*x + c);
    R.at(0,1) = float(t*x*y + s*z);
    R.at(0,2) = float(t*x*z - s*y);
    R.at(0,3) = 0.0f;

    R.at(1,0) = float(t*x*y - s*z);
    R.at(1,1) = float(t*y*y + c);
    R.at(1,2) = float(t*y*z + s*x);
    R.at(1,3) = 0.0f;

    R.at(2,0) = float(t*x*z + s*y);
    R.at(2,1) = float(t*y*z - s*x);
    R.at(2,2) = float(t*z*z + c);
    R.at(2,3) = 0.0f;

    R.at(3,0) = 0.0f; R.at(3,1) = 0.0f; R.at(3,2) = 0.0f; R.at(3,3) = 1.0f;

    return R * m_in;
}

inline mat4 rotate(float angle, const vec3& axis) {
    return rotate(mat4::identity(), angle, axis);
}

inline mat4 perspective(float fovy_radians, float aspect, float zNear, float zFar) {
    assert(aspect != 0.0f);
    assert(zFar != zNear);

    double f = 1.0 / std::tan(double(fovy_radians) / 2.0);
    mat4 P;
    for (int i=0;i<16;++i) P.m[i]=0.0f;

    P.at(0,0) = float(f / aspect);
    P.at(1,1) = float(f);
    P.at(2,2) = float((zFar + zNear) / (zNear - zFar));
    P.at(2,3) = -1.0f;
    P.at(3,2) = float((2.0 * zFar * zNear) / (zNear - zFar));
    return P;
}

inline mat4 ortho(float left, float right, float bottom, float top, float zNear, float zFar) {
    assert(right != left && top != bottom && zFar != zNear);
    mat4 M;
    for (int i=0;i<16;++i) M.m[i]=0.0f;
    M.at(0,0) = 2.0f / (right - left);
    M.at(1,1) = 2.0f / (top - bottom);
    M.at(2,2) = -2.0f / (zFar - zNear);
    M.at(3,3) = 1.0f;

    M.at(3,0) = -(right + left) / (right - left);
    M.at(3,1) = -(top + bottom) / (top - bottom);
    M.at(3,2) = -(zFar + zNear) / (zFar - zNear);
    return M;
}

inline mat4 lookAt(const vec3& eye, const vec3& center, const vec3& up_raw) {
    vec3 f = normalize(center - eye);
    vec3 s = normalize(cross(f, up_raw));
    vec3 u = cross(s, f);

    mat4 M = mat4::identity();

    M.at(0,0) = s.x;
    M.at(1,0) = s.y;
    M.at(2,0) = s.z;

    M.at(0,1) = u.x;
    M.at(1,1) = u.y;
    M.at(2,1) = u.z;

    M.at(0,2) = -f.x;
    M.at(1,2) = -f.y;
    M.at(2,2) = -f.z;

    M.at(3,0) = -dot(s, eye);
    M.at(3,1) = -dot(u, eye);
    M.at(3,2) = dot(f, eye);

    return M;
}

}
