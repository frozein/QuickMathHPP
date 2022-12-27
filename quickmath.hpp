/* ------------------------------------------------------------------------
 *
 * quickmath.hpp
 * author: Daniel Elwell (2022)
 * license: MIT
 * description: a single-header library for common vector, matrix, and quaternion math
 * functions designed for games and graphics programming.
 * 
 * ------------------------------------------------------------------------
 * 
 * to change or disable the function prefix (the default is "QM_"), change the macro on
 * line 113 to contain the desired prefix, or "#define QM_PREFIX" for no prefix
 * 
 * if you wish not to use SSE3 intrinsics (if they are not supported for example),
 * change the macro on line 97 to "#define QM_USE_SSE 0"
 * 
 * to disable the need to link with the C runtime library, change the macros beginning
 * on line 119 and the #include on line 117 to the appropirate functions/files
 * 
 * ------------------------------------------------------------------------
 * 
 * the following functions are defined:
 * (QMvecn means a vector of dimension, 2, 3, or 4, named QMvec2, QMvec3, and QMvec4)
 * (QMmatn means a matrix of dimensions 3x3 or 4x4, named QMmat3 and QMmat4)
 * 
 * QMvecn       QM_dot                        (QMvecn v1, QMvecn v2);
 * QMvec3       QM_cross                      (QMvec3 v1, QMvec3 v2);
 * float        QM_length                     (QMvecn v);
 * QMvecn       QM_normalize                  (QMvecn v);
 * float        QM_distance                   (QMvecn v1, QMvecn v2);
 * QMvecn       QM_min                        (QMvecn v1, QMvecn v2);
 * QMvecn       QM_max                        (QMvecn v1, QMvecn v2);
 * 
 * QMmatn       QM_matn_identity              ();
 * QMmatn       QM_transpose                  (QMmatn m);
 * QMmatn       QM_inverse                    (QMmatn m);
 * 
 * QMmat3       QM_translate                  (QMvec2 t);
 * QMmat4       QM_translate                  (QMvec3 t);
 * QMmat3       QM_scale                      (QMvec2 s);
 * QMmat4       QM_scale                      (QMvec3 s);
 * QMmat3       QM_rotate                     (float angle);
 * QMmat4       QM_rotate                     (QMvec3 axis, float angle);
 * QMmat4       QM_rotate                     (QMvec3 euler);
 * QMmat3       QM_top_left                   (QMmat4 m);
 *
 * QMmat4       QM_perspective                (float fov, float aspect, float near, float far);
 * QMmat4       QM_orthographic               (float left, float right, float bot, float top, float near, float far);
 * QMmat4       QM_look                       (QMvec3 pos, QMvec3 dir   , QMvec3 up);
 * QMmat4       QM_lookat                     (QMvec3 pos, QMvec3 target, QMvec3 up);
 * 
 * QMquaternion QM_quaternion_identity        ();
 * QMquaternion QM_dot                        (QMquaternion q1, QMquaternion q2);
 * float        QM_length                     (QMquaternion q);
 * QMquaternion QM_normalize                  (QMquaternion q);
 * QMquaternion QM_conjugate                  (QMquaternion q);
 * QMquaternion QM_inverse                    (QMquaternion q);
 * QMquaternion QM_slerp                      (QMquaternion q1, QMquaternion q2, float a);
 * QMquaternion QM_quaternion_from_axis_angle (QMvec3 axis, float angle);
 * QMquaternion QM_quaternion_from_euler      (QMvec3 angles);
 * QMmat4       QM_quaternion_to_mat4         (QMquaternion q);
 * 
 * the following operators are defined:
 * (QMvecn means a vector of dimension, 2, 3, or 4, named QMvec2, QMvec3, and QMvec4)
 * (QMmatn means a matrix of dimensions 3x3 or 4x4, named QMmat3 and QMmat4)
 * 
 * QMvecn + QMvecn              -> QMvecn
 * QMvecn - QMvecn              -> QMvecn
 * QMvecn * QMvecn              -> QMvecn
 * QMvecn * float               -> QMvecn
 * float * QMvecn               -> QMvecn
 * QMvecn / float               -> QMvecn
 * float / QMvecn               -> QMvecn
 * QMvecn == QMvecn             -> bool
 * 
 * QMmatn + QMmatn              -> QMmatn
 * QMmatn - QMmatn              -> QMmatn
 * QMmatn * QMmatn              -> QMmatn
 * QMmatn * QMvecn              -> QMvecn
 * 
 * QMquaternion + QMquaternion  -> QMquaternion
 * QMquaternion - QMquaternion  -> QMquaternion
 * QMquaternion * QMquaternion  -> QMquaternion
 * QMquaternion * float         -> QMquaternion
 * float * QMquaternion         -> QMquaternion
 * QMquaternion / float         -> QMquaternion
 * float / QMquaternion         -> QMquaternion
 * QMquaternion == QMquaternion -> bool
 */

#ifndef QM_MATH_H
#define QM_MATH_H

//if you wish NOT to use SSE3 SIMD intrinsics, simply change the
//#define to 0
#define QM_USE_SSE 1
#if QM_USE_SSE
	#include <xmmintrin.h>
	#include <pmmintrin.h>
#endif

//if you wish NOT to include iostream, simply change the
//#define to 0
#define QM_INCLUDE_IOSTREAM 1
#if QM_INCLUDE_IOSTREAM
	#include <iostream>
#endif

#define QM_INLINE static inline

//if you wish to set your own function prefix or remove it entirely,
//simply change this macro:
#define QM_PREFIX(name) QM_##name

//if you wish to not use any of the CRT functions, you must #define your
//own versions of the below functions and #include the appropriate header
#include <math.h>

#define QM_SQRTF sqrtf
#define QM_SINF  sinf
#define QM_COSF  cosf
#define QM_TANF  tanf
#define QM_ACOSF acosf

//----------------------------------------------------------------------//
//STRUCT DEFINITIONS:

typedef int QMbool;

//a 2-dimensional vector of floats
union QMvec2
{
	float v[2] = {};
	struct{ float x, y; };
	struct{ float w, h; };

	QMvec2() = default;
	QMvec2(float _x, float _y) { x = _x, y = _y; };

	float& operator[](size_t i) { return v[i]; };
};

//a 3-dimensional vector of floats
union QMvec3
{
	float v[3] = {};
	struct{ float x, y, z; };
	struct{ float w, h, d; };
	struct{ float r, g, b; };

	QMvec3() = default;
	QMvec3(float _x, float _y, float _z) { x = _x, y = _y, z = _z; };
	QMvec3(QMvec2 _xy, float _z) { x = _xy.x, y = _xy.y, z = _z; };
	QMvec3(float _x, QMvec3 _yz) { x = _x, y = _yz.x, z = _yz.y; };

	float& operator[](size_t i) { return v[i]; };
};

//a 4-dimensional vector of floats
union QMvec4
{
	float v[4] = {};
	struct{ float x, y, z, w; };
	struct{ float r, g, b, a; };

	#if QM_USE_SSE

	__m128 packed;

	#endif

	QMvec4() = default;
	QMvec4(float _x, float _y, float _z, float _w) { x = _x, y = _y, z = _z, w = _w; };
	QMvec4(QMvec3 _xyz, float _w) { x = _xyz.x, y = _xyz.y, z = _xyz.z, w = _w; };
	QMvec4(float _x, QMvec3 _yzw) { x = _x, y = _yzw.x, z = _yzw.y, w = _yzw.z; };
	QMvec4(QMvec2 _xy, QMvec2 _zw) { x = _xy.x, y = _xy.y, z = _zw.x, w = _zw.y; };

	float& operator[](size_t i) { return v[i]; };
};

//-----------------------------//
//matrices are column-major

union QMmat3
{
	float m[3][3] = {};
	QMvec3 v[3];

	QMmat3() = default;

	QMvec3& operator[](size_t i) { return v[i]; };
};

union QMmat4
{
	float m[4][4] = {};
	QMvec4 v[4];

	#if QM_USE_SSE

	__m128 packed[4]; //array of columns

	#endif

	QMmat4() = default;

	QMvec4& operator[](size_t i) { return v[i]; };
};

//-----------------------------//

union QMquaternion
{
	float q[4] = {};
	struct{ float x, y, z, w; };

	#if QM_USE_SSE

	__m128 packed;

	#endif

	QMquaternion() = default;
	QMquaternion(float _x, float _y, float _z, float _w) { x = _x, y = _y, z = _z, w = _w; };
	QMquaternion(QMvec3 _xyz, float _w) { x = _xyz.x, y = _xyz.y, z = _xyz.z, w = _w; };
	QMquaternion(float _x, QMvec3 _yzw) { x = _x, y = _yzw.x, z = _yzw.y, w = _yzw.z; };
	QMquaternion(QMvec2 _xy, QMvec2 _zw) { x = _xy.x, y = _xy.y, z = _zw.x, w = _zw.y; };

	float operator[](size_t i) { return q[i]; };
};

//----------------------------------------------------------------------//
//HELPER FUNCS:

#define QM_MIN(x, y) ((x) < (y) ? (x) : (y))
#define QM_MAX(x, y) ((x) > (y) ? (x) : (y))
#define QM_ABS(x) ((x) > 0 ? (x) : -(x))

QM_INLINE float QM_PREFIX(rad_to_deg)(float rad)
{
	return rad * 57.2957795131f;
}

QM_INLINE float QM_PREFIX(deg_to_rad)(float deg)
{
	return deg * 0.01745329251f;
}

#if QM_USE_SSE

QM_INLINE __m128 QM_PREFIX(mat4_mult_column_sse)(__m128 c1, QMmat4 m2)
{
	__m128 result;

	result =                    _mm_mul_ps(_mm_shuffle_ps(c1, c1, _MM_SHUFFLE(0, 0, 0, 0)), m2.packed[0]);
	result = _mm_add_ps(result, _mm_mul_ps(_mm_shuffle_ps(c1, c1, _MM_SHUFFLE(1, 1, 1, 1)), m2.packed[1]));
	result = _mm_add_ps(result, _mm_mul_ps(_mm_shuffle_ps(c1, c1, _MM_SHUFFLE(2, 2, 2, 2)), m2.packed[2]));
	result = _mm_add_ps(result, _mm_mul_ps(_mm_shuffle_ps(c1, c1, _MM_SHUFFLE(3, 3, 3, 3)), m2.packed[3]));

	return result;
}

#endif

//----------------------------------------------------------------------//
//VECTOR FUNCTIONS:

#if QM_INCLUDE_IOSTREAM

//output:

std::ostream& operator<<(std::ostream& os, const QMvec2& v)
{
	os << v.x << ", " << v.y;
	return os;
}

std::ostream& operator<<(std::ostream& os, const QMvec3& v)
{
	os << v.x << ", " << v.y << ", " << v.z;
	return os;
}

std::ostream& operator<<(std::ostream& os, const QMvec4& v)
{
	os << v.x << ", " << v.y << ", " << v.z << ", " << v.w;
	return os;
}

//input:

std::istream& operator>>(std::istream& is, QMvec2& v)
{
	is >> v.x >> v.y;
	return is;
}

std::istream& operator>>(std::istream& is, QMvec3& v)
{
	is >> v.x >> v.y >> v.z;
	return is;
}

std::istream& operator>>(std::istream& is, QMvec4& v)
{
	is >> v.x >> v.y >> v.z >> v.w;
	return is;
}

#endif

//addition:

QM_INLINE QMvec2 operator+(const QMvec2& v1, const QMvec2& v2)
{
	QMvec2 result;

	result.x = v1.x + v2.x;
	result.y = v1.y + v2.y;

	return result;
}

QM_INLINE QMvec3 operator+(const QMvec3& v1, const QMvec3& v2)
{
	QMvec3 result;

	result.x = v1.x + v2.x;
	result.y = v1.y + v2.y;
	result.z = v1.z + v2.z;

	return result;
}

QM_INLINE QMvec4 operator+(const QMvec4& v1, const QMvec4& v2)
{
	QMvec4 result;

	#if QM_USE_SSE

	result.packed = _mm_add_ps(v1.packed, v2.packed);

	#else

	result.x = v1.x + v2.x;
	result.y = v1.y + v2.y;
	result.z = v1.z + v2.z;
	result.w = v1.w + v2.w;

	#endif

	return result;
}

//subtraction:

QM_INLINE QMvec2 operator-(const QMvec2& v1, const QMvec2& v2)
{
	QMvec2 result;

	result.x = v1.x - v2.x;
	result.y = v1.y - v2.y;

	return result;
}

QM_INLINE QMvec3 operator-(const QMvec3& v1, const QMvec3& v2)
{
	QMvec3 result;

	result.x = v1.x - v2.x;
	result.y = v1.y - v2.y;
	result.z = v1.z - v2.z;

	return result;
}

QM_INLINE QMvec4 operator-(const QMvec4& v1, const QMvec4& v2)
{
	QMvec4 result;

	#if QM_USE_SSE

	result.packed = _mm_sub_ps(v1.packed, v2.packed);

	#else

	result.x = v1.x - v2.x;
	result.y = v1.y - v2.y;
	result.z = v1.z - v2.z;
	result.w = v1.w - v2.w;

	#endif

	return result;
}

//multiplication:

QM_INLINE QMvec2 operator*(const QMvec2& v1, const QMvec2& v2)
{
	QMvec2 result;

	result.x = v1.x * v2.x;
	result.y = v1.y * v2.y;

	return result;
}

QM_INLINE QMvec3 operator*(const QMvec3& v1, const QMvec3& v2)
{
	QMvec3 result;

	result.x = v1.x * v2.x;
	result.y = v1.y * v2.y;
	result.z = v1.z * v2.z;

	return result;
}

QM_INLINE QMvec4 operator*(const QMvec4& v1, const QMvec4& v2)
{
	QMvec4 result;

	#if QM_USE_SSE

	result.packed = _mm_mul_ps(v1.packed, v2.packed);

	#else

	result.x = v1.x * v2.x;
	result.y = v1.y * v2.y;
	result.z = v1.z * v2.z;
	result.w = v1.w * v2.w;

	#endif

	return result;
}

//division:

QM_INLINE QMvec2 operator/(const QMvec2& v1, const QMvec2& v2)
{
	QMvec2 result;

	result.x = v1.x / v2.x;
	result.y = v1.y / v2.y;

	return result;
}

QM_INLINE QMvec3 operator/(const QMvec3& v1, const QMvec3& v2)
{
	QMvec3 result;

	result.x = v1.x / v2.x;
	result.y = v1.y / v2.y;
	result.z = v1.z / v2.z;

	return result;
}

QM_INLINE QMvec4 operator/(const QMvec4& v1, const QMvec4& v2)
{
	QMvec4 result;

	#if QM_USE_SSE

	result.packed = _mm_div_ps(v1.packed, v2.packed);

	#else

	result.x = v1.x / v2.x;
	result.y = v1.y / v2.y;
	result.z = v1.z / v2.z;
	result.w = v1.w / v2.w;

	#endif

	return result;
}

//scalar multiplication:

QM_INLINE QMvec2 operator*(const QMvec2& v, float s)
{
	QMvec2 result;

	result.x = v.x * s;
	result.y = v.y * s;

	return result;
}

QM_INLINE QMvec3 operator*(const QMvec3& v, float s)
{
	QMvec3 result;

	result.x = v.x * s;
	result.y = v.y * s;
	result.z = v.z * s;

	return result;
}

QM_INLINE QMvec4 operator*(const QMvec4& v, float s)
{
	QMvec4 result;

	#if QM_USE_SSE

	__m128 scale = _mm_set1_ps(s);
	result.packed = _mm_mul_ps(v.packed, scale);

	#else

	result.x = v.x * s;
	result.y = v.y * s;
	result.z = v.z * s;
	result.w = v.w * s;

	#endif

	return result;
}

QM_INLINE QMvec2 operator*(float s, const QMvec2& v)
{
	return v * s;
}

QM_INLINE QMvec3 operator*(float s, const QMvec3& v)
{
	return v * s;
}

QM_INLINE QMvec4 operator*(float s, const QMvec4& v)
{
	return v * s;
}

//scalar division:

QM_INLINE QMvec2 operator/(const QMvec2& v, float s)
{
	QMvec2 result;

	result.x = v.x / s;
	result.y = v.y / s;

	return result;
}

QM_INLINE QMvec3 operator/(const QMvec3& v, float s)
{
	QMvec3 result;

	result.x = v.x / s;
	result.y = v.y / s;
	result.z = v.z / s;

	return result;
}

QM_INLINE QMvec4 operator/(const QMvec4& v, float s)
{
	QMvec4 result;

	#if QM_USE_SSE

	__m128 scale = _mm_set1_ps(s);
	result.packed = _mm_div_ps(v.packed, scale);

	#else

	result.x = v.x / s;
	result.y = v.y / s;
	result.z = v.z / s;
	result.w = v.w / s;

	#endif

	return result;
}

QM_INLINE QMvec2 operator/(float s, const QMvec2& v)
{
	QMvec2 result;

	result.x = s / v.x;
	result.y = s / v.y;

	return result;
}

QM_INLINE QMvec3 operator/(float s, const QMvec3& v)
{
	QMvec3 result;

	result.x = s / v.x;
	result.y = s / v.y;
	result.z = s / v.z;

	return result;
}

QM_INLINE QMvec4 operator/(float s, const QMvec4& v)
{
	QMvec4 result;

	#if QM_USE_SSE

	__m128 scale = _mm_set1_ps(s);
	result.packed = _mm_div_ps(scale, v.packed);

	#else

	result.x = s / v.x;
	result.y = s / v.y;
	result.z = s / v.z;
	result.w = s / v.w;

	#endif

	return result;
}

//dot product:

QM_INLINE float QM_PREFIX(dot)(const QMvec2& v1, const QMvec2& v2)
{
	float result;

	result = v1.x * v2.x + v1.y * v2.y;

	return result;
}

QM_INLINE float QM_PREFIX(dot)(const QMvec3& v1, const QMvec3& v2)
{
	float result;

	result = v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;

	return result;
}

QM_INLINE float QM_PREFIX(dot)(const QMvec4& v1, const QMvec4& v2)
{
	float result;

	#if QM_USE_SSE

	__m128 r = _mm_mul_ps(v1.packed, v2.packed);
	r = _mm_hadd_ps(r, r);
	r = _mm_hadd_ps(r, r);
	_mm_store_ss(&result, r);

	#else

	result = v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w;

	#endif

	return result;
}

//cross product

QM_INLINE QMvec3 QM_PREFIX(cross)(const QMvec3& v1, const QMvec3& v2)
{
	QMvec3 result;

	result.x = (v1.y * v2.z) - (v1.z * v2.y);
	result.y = (v1.z * v2.x) - (v1.x * v2.z);
	result.z = (v1.x * v2.y) - (v1.y * v2.x);

	return result;
}

//length:

QM_INLINE float QM_PREFIX(length)(const QMvec2& v)
{
	float result;

	result = QM_SQRTF(QM_PREFIX(dot)(v, v));

	return result;
}

QM_INLINE float QM_PREFIX(length)(const QMvec3& v)
{
	float result;

	result = QM_SQRTF(QM_PREFIX(dot)(v, v));

	return result;
}

QM_INLINE float QM_PREFIX(length)(const QMvec4& v)
{
	float result;

	result = QM_SQRTF(QM_PREFIX(dot)(v, v));

	return result;
}

//normalize:

QM_INLINE QMvec2 QM_PREFIX(normalize)(const QMvec2& v)
{
	QMvec2 result;

	float len = QM_PREFIX(length)(v);
	if(len != 0.0f)
		result = v / len;

	return result;
}

QM_INLINE QMvec3 QM_PREFIX(normalize)(const QMvec3& v)
{
	QMvec3 result;

	float len = QM_PREFIX(length)(v);
	if(len != 0.0f)
		result = v / len;

	return result;
}

QM_INLINE QMvec4 QM_PREFIX(normalize)(const QMvec4& v)
{
	QMvec4 result;

	float len = QM_PREFIX(length)(v);
		result = v / len;

	return result;
}

//distance:

QM_INLINE float QM_PREFIX(distance)(const QMvec2& v1, const QMvec2& v2)
{
	float result;

	QMvec2 to = v1 - v2;
	result = QM_PREFIX(length)(to);

	return result;
}

QM_INLINE float QM_PREFIX(distance)(const QMvec3& v1, const QMvec3& v2)
{
	float result;

	QMvec3 to = v1 - v2;
	result = QM_PREFIX(length)(to);

	return result;
}

QM_INLINE float QM_PREFIX(distance)(const QMvec4& v1, const QMvec4& v2)
{
	float result;

	QMvec4 to = v1 - v2;
	result = QM_PREFIX(length)(to);

	return result;
}

//equality:

QM_INLINE QMbool operator==(const QMvec2& v1, const QMvec2& v2)
{
	QMbool result;

	result = (v1.x == v2.x) && (v1.y == v2.y); 

	return result;	
}

QM_INLINE QMbool operator==(const QMvec3& v1, const QMvec3& v2)
{
	QMbool result;

	result = (v1.x == v2.x) && (v1.y == v2.y) && (v1.z == v2.z); 

	return result;	
}

QM_INLINE QMbool operator==(const QMvec4& v1, const QMvec4& v2)
{
	QMbool result;

	//TODO: there are SIMD instructions for floating point equality, find a way to get a single bool from them
	result = (v1.x == v2.x) && (v1.y == v2.y) && (v1.z == v2.z) && (v1.w == v2.w); 

	return result;	
}

//min:

QM_INLINE QMvec2 QM_PREFIX(min)(const QMvec2& v1, const QMvec2& v2)
{
	QMvec2 result;

	result.x = QM_MIN(v1.x, v2.x);
	result.y = QM_MIN(v1.y, v2.y);

	return result;
}

QM_INLINE QMvec3 QM_PREFIX(min)(const QMvec3& v1, const QMvec3& v2)
{
	QMvec3 result;

	result.x = QM_MIN(v1.x, v2.x);
	result.y = QM_MIN(v1.y, v2.y);
	result.z = QM_MIN(v1.z, v2.z);

	return result;
}

QM_INLINE QMvec4 QM_PREFIX(min)(const QMvec4& v1, const QMvec4& v2)
{
	QMvec4 result;

	#if QM_USE_SSE

	result.packed = _mm_min_ps(v1.packed, v2.packed);

	#else

	result.x = QM_MIN(v1.x, v2.x);
	result.y = QM_MIN(v1.y, v2.y);
	result.z = QM_MIN(v1.z, v2.z);
	result.w = QM_MIN(v1.w, v2.w);

	#endif

	return result;
}

//max:

QM_INLINE QMvec2 QM_PREFIX(max)(const QMvec2& v1, const QMvec2& v2)
{
	QMvec2 result;

	result.x = QM_MAX(v1.x, v2.x);
	result.y = QM_MAX(v1.y, v2.y);

	return result;
}

QM_INLINE QMvec3 QM_PREFIX(max)(const QMvec3& v1, const QMvec3& v2)
{
	QMvec3 result;

	result.x = QM_MAX(v1.x, v2.x);
	result.y = QM_MAX(v1.y, v2.y);
	result.z = QM_MAX(v1.z, v2.z);

	return result;
}

QM_INLINE QMvec4 QM_PREFIX(max)(const QMvec4& v1, const QMvec4& v2)
{
	QMvec4 result;

	#if QM_USE_SSE

	result.packed = _mm_max_ps(v1.packed, v2.packed);

	#else

	result.x = QM_MAX(v1.x, v2.x);
	result.y = QM_MAX(v1.y, v2.y);
	result.z = QM_MAX(v1.z, v2.z);
	result.w = QM_MAX(v1.w, v2.w);

	#endif

	return result;
}

//----------------------------------------------------------------------//
//MATRIX FUNCTIONS:

#if QM_INCLUDE_IOSTREAM

//output:

std::ostream& operator<<(std::ostream& os, const QMmat3& m)
{
	os << m.v[0] << std::endl << m.v[1] << std::endl << m.v[2];
	return os;
}

std::ostream& operator<<(std::ostream& os, const QMmat4& m)
{
	os << m.v[0] << std::endl << m.v[1] << std::endl << m.v[2] << std::endl << m.v[3];
	return os;
}

//input:

std::istream& operator>>(std::istream& is, QMmat3& m)
{
	is >> m.v[0] >> m.v[1] >> m.v[2];
	return is;
}

std::istream& operator>>(std::istream& is, QMmat4& m)
{
	is >> m.v[0] >> m.v[1] >> m.v[2] >> m.v[3];
	return is;
}

#endif

//initialization:

QM_INLINE QMmat3 QM_PREFIX(mat3_identity)()
{
	QMmat3 result = {
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f
	};

	return result;
}

QM_INLINE QMmat4 QM_PREFIX(mat4_identity)()
{
	QMmat4 result = {
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};

	return result;
}

//addition:

QM_INLINE QMmat3 operator+(const QMmat3& m1, const QMmat3& m2)
{
	QMmat3 result;

	result.m[0][0] = m1.m[0][0] + m2.m[0][0];
	result.m[0][1] = m1.m[0][1] + m2.m[0][1];
	result.m[0][2] = m1.m[0][2] + m2.m[0][2];
	result.m[1][0] = m1.m[1][0] + m2.m[1][0];
	result.m[1][1] = m1.m[1][1] + m2.m[1][1];
	result.m[1][2] = m1.m[1][2] + m2.m[1][2];
	result.m[2][0] = m1.m[2][0] + m2.m[2][0];
	result.m[2][1] = m1.m[2][1] + m2.m[2][1];
	result.m[2][2] = m1.m[2][2] + m2.m[2][2];

	return result;
}

QM_INLINE QMmat4 operator+(const QMmat4& m1, const QMmat4& m2)
{
	QMmat4 result;

	#if QM_USE_SSE

	result.packed[0] = _mm_add_ps(m1.packed[0], m2.packed[0]);
	result.packed[1] = _mm_add_ps(m1.packed[1], m2.packed[1]);
	result.packed[2] = _mm_add_ps(m1.packed[2], m2.packed[2]);
	result.packed[3] = _mm_add_ps(m1.packed[3], m2.packed[3]);

	#else

	result.m[0][0] = m1.m[0][0] + m2.m[0][0];
	result.m[0][1] = m1.m[0][1] + m2.m[0][1];
	result.m[0][2] = m1.m[0][2] + m2.m[0][2];
	result.m[0][3] = m1.m[0][3] + m2.m[0][3];
	result.m[1][0] = m1.m[1][0] + m2.m[1][0];
	result.m[1][1] = m1.m[1][1] + m2.m[1][1];
	result.m[1][2] = m1.m[1][2] + m2.m[1][2];
	result.m[1][3] = m1.m[1][3] + m2.m[1][3];
	result.m[2][0] = m1.m[2][0] + m2.m[2][0];
	result.m[2][1] = m1.m[2][1] + m2.m[2][1];
	result.m[2][2] = m1.m[2][2] + m2.m[2][2];
	result.m[2][3] = m1.m[2][3] + m2.m[2][3];
	result.m[3][0] = m1.m[3][0] + m2.m[3][0];
	result.m[3][1] = m1.m[3][1] + m2.m[3][1];
	result.m[3][2] = m1.m[3][2] + m2.m[3][2];
	result.m[3][3] = m1.m[3][3] + m2.m[3][3];

	#endif

	return result;
}

//subtraction:

QM_INLINE QMmat3 operator-(const QMmat3& m1, const QMmat3& m2)
{
	QMmat3 result;

	result.m[0][0] = m1.m[0][0] - m2.m[0][0];
	result.m[0][1] = m1.m[0][1] - m2.m[0][1];
	result.m[0][2] = m1.m[0][2] - m2.m[0][2];
	result.m[1][0] = m1.m[1][0] - m2.m[1][0];
	result.m[1][1] = m1.m[1][1] - m2.m[1][1];
	result.m[1][2] = m1.m[1][2] - m2.m[1][2];
	result.m[2][0] = m1.m[2][0] - m2.m[2][0];
	result.m[2][1] = m1.m[2][1] - m2.m[2][1];
	result.m[2][2] = m1.m[2][2] - m2.m[2][2];

	return result;
}

QM_INLINE QMmat4 operator-(const QMmat4& m1, const QMmat4& m2)
{
	QMmat4 result;

	#if QM_USE_SSE

	result.packed[0] = _mm_sub_ps(m1.packed[0], m2.packed[0]);
	result.packed[1] = _mm_sub_ps(m1.packed[1], m2.packed[1]);
	result.packed[2] = _mm_sub_ps(m1.packed[2], m2.packed[2]);
	result.packed[3] = _mm_sub_ps(m1.packed[3], m2.packed[3]);

	#else

	result.m[0][0] = m1.m[0][0] - m2.m[0][0];
	result.m[0][1] = m1.m[0][1] - m2.m[0][1];
	result.m[0][2] = m1.m[0][2] - m2.m[0][2];
	result.m[0][3] = m1.m[0][3] - m2.m[0][3];
	result.m[1][0] = m1.m[1][0] - m2.m[1][0];
	result.m[1][1] = m1.m[1][1] - m2.m[1][1];
	result.m[1][2] = m1.m[1][2] - m2.m[1][2];
	result.m[1][3] = m1.m[1][3] - m2.m[1][3];
	result.m[2][0] = m1.m[2][0] - m2.m[2][0];
	result.m[2][1] = m1.m[2][1] - m2.m[2][1];
	result.m[2][2] = m1.m[2][2] - m2.m[2][2];
	result.m[2][3] = m1.m[2][3] - m2.m[2][3];
	result.m[3][0] = m1.m[3][0] - m2.m[3][0];
	result.m[3][1] = m1.m[3][1] - m2.m[3][1];
	result.m[3][2] = m1.m[3][2] - m2.m[3][2];
	result.m[3][3] = m1.m[3][3] - m2.m[3][3];

	#endif

	return result;
}

//multiplication:

QM_INLINE QMmat3 operator*(const QMmat3& m1, const QMmat3& m2)
{
	QMmat3 result;

	result.m[0][0] = m1.m[0][0] * m2.m[0][0] + m1.m[1][0] * m2.m[0][1] + m1.m[2][0] * m2.m[0][2];
	result.m[0][1] = m1.m[0][1] * m2.m[0][0] + m1.m[1][1] * m2.m[0][1] + m1.m[2][1] * m2.m[0][2];
	result.m[0][2] = m1.m[0][2] * m2.m[0][0] + m1.m[1][2] * m2.m[0][1] + m1.m[2][2] * m2.m[0][2];
	result.m[1][0] = m1.m[0][0] * m2.m[1][0] + m1.m[1][0] * m2.m[1][1] + m1.m[2][0] * m2.m[1][2];
	result.m[1][1] = m1.m[0][1] * m2.m[1][0] + m1.m[1][1] * m2.m[1][1] + m1.m[2][1] * m2.m[1][2];
	result.m[1][2] = m1.m[0][2] * m2.m[1][0] + m1.m[1][2] * m2.m[1][1] + m1.m[2][2] * m2.m[1][2];
	result.m[2][0] = m1.m[0][0] * m2.m[2][0] + m1.m[1][0] * m2.m[2][1] + m1.m[2][0] * m2.m[2][2];
	result.m[2][1] = m1.m[0][1] * m2.m[2][0] + m1.m[1][1] * m2.m[2][1] + m1.m[2][1] * m2.m[2][2];
	result.m[2][2] = m1.m[0][2] * m2.m[2][0] + m1.m[1][2] * m2.m[2][1] + m1.m[2][2] * m2.m[2][2];

	return result;
}

QM_INLINE QMmat4 operator*(const QMmat4& m1, const QMmat4& m2)
{
	QMmat4 result;

	#if QM_USE_SSE

	result.packed[0] = QM_PREFIX(mat4_mult_column_sse)(m2.packed[0], m1);
	result.packed[1] = QM_PREFIX(mat4_mult_column_sse)(m2.packed[1], m1);
	result.packed[2] = QM_PREFIX(mat4_mult_column_sse)(m2.packed[2], m1);
	result.packed[3] = QM_PREFIX(mat4_mult_column_sse)(m2.packed[3], m1);

	#else

	result.m[0][0] = m1.m[0][0] * m2.m[0][0] + m1.m[1][0] * m2.m[0][1] + m1.m[2][0] * m2.m[0][2] + m1.m[3][0] * m2.m[0][3];
	result.m[0][1] = m1.m[0][1] * m2.m[0][0] + m1.m[1][1] * m2.m[0][1] + m1.m[2][1] * m2.m[0][2] + m1.m[3][1] * m2.m[0][3];
	result.m[0][2] = m1.m[0][2] * m2.m[0][0] + m1.m[1][2] * m2.m[0][1] + m1.m[2][2] * m2.m[0][2] + m1.m[3][2] * m2.m[0][3];
	result.m[0][3] = m1.m[0][3] * m2.m[0][0] + m1.m[1][3] * m2.m[0][1] + m1.m[2][3] * m2.m[0][2] + m1.m[3][3] * m2.m[0][3];
	result.m[1][0] = m1.m[0][0] * m2.m[1][0] + m1.m[1][0] * m2.m[1][1] + m1.m[2][0] * m2.m[1][2] + m1.m[3][0] * m2.m[1][3];
	result.m[1][1] = m1.m[0][1] * m2.m[1][0] + m1.m[1][1] * m2.m[1][1] + m1.m[2][1] * m2.m[1][2] + m1.m[3][1] * m2.m[1][3];
	result.m[1][2] = m1.m[0][2] * m2.m[1][0] + m1.m[1][2] * m2.m[1][1] + m1.m[2][2] * m2.m[1][2] + m1.m[3][2] * m2.m[1][3];
	result.m[1][3] = m1.m[0][3] * m2.m[1][0] + m1.m[1][3] * m2.m[1][1] + m1.m[2][3] * m2.m[1][2] + m1.m[3][3] * m2.m[1][3];
	result.m[2][0] = m1.m[0][0] * m2.m[2][0] + m1.m[1][0] * m2.m[2][1] + m1.m[2][0] * m2.m[2][2] + m1.m[3][0] * m2.m[2][3];
	result.m[2][1] = m1.m[0][1] * m2.m[2][0] + m1.m[1][1] * m2.m[2][1] + m1.m[2][1] * m2.m[2][2] + m1.m[3][1] * m2.m[2][3];
	result.m[2][2] = m1.m[0][2] * m2.m[2][0] + m1.m[1][2] * m2.m[2][1] + m1.m[2][2] * m2.m[2][2] + m1.m[3][2] * m2.m[2][3];
	result.m[2][3] = m1.m[0][3] * m2.m[2][0] + m1.m[1][3] * m2.m[2][1] + m1.m[2][3] * m2.m[2][2] + m1.m[3][3] * m2.m[2][3];
	result.m[3][0] = m1.m[0][0] * m2.m[3][0] + m1.m[1][0] * m2.m[3][1] + m1.m[2][0] * m2.m[3][2] + m1.m[3][0] * m2.m[3][3];
	result.m[3][1] = m1.m[0][1] * m2.m[3][0] + m1.m[1][1] * m2.m[3][1] + m1.m[2][1] * m2.m[3][2] + m1.m[3][1] * m2.m[3][3];
	result.m[3][2] = m1.m[0][2] * m2.m[3][0] + m1.m[1][2] * m2.m[3][1] + m1.m[2][2] * m2.m[3][2] + m1.m[3][2] * m2.m[3][3];
	result.m[3][3] = m1.m[0][3] * m2.m[3][0] + m1.m[1][3] * m2.m[3][1] + m1.m[2][3] * m2.m[3][2] + m1.m[3][3] * m2.m[3][3];

	#endif

	return result;
}

QM_INLINE QMvec3 operator*(const QMmat3& m, const QMvec3& v)
{
	QMvec3 result;

	result.x = m.m[0][0] * v.x + m.m[1][0] * v.y + m.m[2][0] * v.z;
	result.y = m.m[0][1] * v.x + m.m[1][1] * v.y + m.m[2][1] * v.z;
	result.z = m.m[0][2] * v.x + m.m[1][2] * v.y + m.m[2][2] * v.z;

	return result;
}

QM_INLINE QMvec4 operator*(const QMmat4& m, const QMvec4& v)
{
	QMvec4 result;

	#if QM_USE_SSE

	result.packed = QM_PREFIX(mat4_mult_column_sse)(v.packed, m);

	#else

	result.x = m.m[0][0] * v.x + m.m[1][0] * v.y + m.m[2][0] * v.z + m.m[3][0] * v.w;
	result.y = m.m[0][1] * v.x + m.m[1][1] * v.y + m.m[2][1] * v.z + m.m[3][1] * v.w;
	result.z = m.m[0][2] * v.x + m.m[1][2] * v.y + m.m[2][2] * v.z + m.m[3][2] * v.w;
	result.w = m.m[0][3] * v.x + m.m[1][3] * v.y + m.m[2][3] * v.z + m.m[3][3] * v.w;

	#endif

	return result;
}

//transpose:

QM_INLINE QMmat3 QM_PREFIX(transpose)(const QMmat3& m)
{
	QMmat3 result;

	result.m[0][0] = m.m[0][0];
	result.m[0][1] = m.m[1][0];
	result.m[0][2] = m.m[2][0];
	result.m[1][0] = m.m[0][1];
	result.m[1][1] = m.m[1][1];
	result.m[1][2] = m.m[2][1];
	result.m[2][0] = m.m[0][2];
	result.m[2][1] = m.m[1][2];
	result.m[2][2] = m.m[2][2];

	return result;
}

QM_INLINE QMmat4 QM_PREFIX(transpose)(const QMmat4& m)
{
	QMmat4 result = m;

	#if QM_USE_SSE

	_MM_TRANSPOSE4_PS(result.packed[0], result.packed[1], result.packed[2], result.packed[3]);

	#else

	result.m[0][0] = m.m[0][0];
	result.m[0][1] = m.m[1][0];
	result.m[0][2] = m.m[2][0];
	result.m[0][3] = m.m[3][0];
	result.m[1][0] = m.m[0][1];
	result.m[1][1] = m.m[1][1];
	result.m[1][2] = m.m[2][1];
	result.m[1][3] = m.m[3][1];
	result.m[2][0] = m.m[0][2];
	result.m[2][1] = m.m[1][2];
	result.m[2][2] = m.m[2][2];
	result.m[2][3] = m.m[3][2];
	result.m[3][0] = m.m[0][3];
	result.m[3][1] = m.m[1][3];
	result.m[3][2] = m.m[2][3];
	result.m[3][3] = m.m[3][3];

	#endif

	return result;
}

//inverse:

QM_INLINE QMmat3 QM_PREFIX(inverse)(const QMmat3& m)
{
	QMmat3 result;

	float det;
  	float a = m.m[0][0], b = m.m[0][1], c = m.m[0][2],
	      d = m.m[1][0], e = m.m[1][1], f = m.m[1][2],
	      g = m.m[2][0], h = m.m[2][1], i = m.m[2][2];

	result.m[0][0] =   e * i - f * h;
	result.m[0][1] = -(b * i - h * c);
	result.m[0][2] =   b * f - e * c;
	result.m[1][0] = -(d * i - g * f);
	result.m[1][1] =   a * i - c * g;
	result.m[1][2] = -(a * f - d * c);
	result.m[2][0] =   d * h - g * e;
	result.m[2][1] = -(a * h - g * b);
	result.m[2][2] =   a * e - b * d;

	det = 1.0f / (a * result.m[0][0] + b * result.m[1][0] + c * result.m[2][0]);

	result.m[0][0] *= det;
	result.m[0][1] *= det;
	result.m[0][2] *= det;
	result.m[1][0] *= det;
	result.m[1][1] *= det;
	result.m[1][2] *= det;
	result.m[2][0] *= det;
	result.m[2][1] *= det;
	result.m[2][2] *= det;

	return result;
}

QM_INLINE QMmat4 QM_PREFIX(inverse)(const QMmat4& mat)
{
	//TODO: this function is not SIMD optimized, figure out how to do it

	QMmat4 result;

	float tmp[6];
	float det;
	float a = mat.m[0][0], b = mat.m[0][1], c = mat.m[0][2], d = mat.m[0][3],
	      e = mat.m[1][0], f = mat.m[1][1], g = mat.m[1][2], h = mat.m[1][3],
	      i = mat.m[2][0], j = mat.m[2][1], k = mat.m[2][2], l = mat.m[2][3],
	      m = mat.m[3][0], n = mat.m[3][1], o = mat.m[3][2], p = mat.m[3][3];

	tmp[0] = k * p - o * l; 
	tmp[1] = j * p - n * l; 
	tmp[2] = j * o - n * k;
	tmp[3] = i * p - m * l; 
	tmp[4] = i * o - m * k; 
	tmp[5] = i * n - m * j;

	result.m[0][0] =   f * tmp[0] - g * tmp[1] + h * tmp[2];
	result.m[1][0] = -(e * tmp[0] - g * tmp[3] + h * tmp[4]);
	result.m[2][0] =   e * tmp[1] - f * tmp[3] + h * tmp[5];
	result.m[3][0] = -(e * tmp[2] - f * tmp[4] + g * tmp[5]);

	result.m[0][1] = -(b * tmp[0] - c * tmp[1] + d * tmp[2]);
	result.m[1][1] =   a * tmp[0] - c * tmp[3] + d * tmp[4];
	result.m[2][1] = -(a * tmp[1] - b * tmp[3] + d * tmp[5]);
	result.m[3][1] =   a * tmp[2] - b * tmp[4] + c * tmp[5];

	tmp[0] = g * p - o * h;
	tmp[1] = f * p - n * h;
	tmp[2] = f * o - n * g;
	tmp[3] = e * p - m * h;
	tmp[4] = e * o - m * g;
	tmp[5] = e * n - m * f;

	result.m[0][2] =   b * tmp[0] - c * tmp[1] + d * tmp[2];
	result.m[1][2] = -(a * tmp[0] - c * tmp[3] + d * tmp[4]);
	result.m[2][2] =   a * tmp[1] - b * tmp[3] + d * tmp[5];
	result.m[3][2] = -(a * tmp[2] - b * tmp[4] + c * tmp[5]);

	tmp[0] = g * l - k * h;
	tmp[1] = f * l - j * h;
	tmp[2] = f * k - j * g;
	tmp[3] = e * l - i * h;
	tmp[4] = e * k - i * g;
	tmp[5] = e * j - i * f;

	result.m[0][3] = -(b * tmp[0] - c * tmp[1] + d * tmp[2]);
	result.m[1][3] =   a * tmp[0] - c * tmp[3] + d * tmp[4];
	result.m[2][3] = -(a * tmp[1] - b * tmp[3] + d * tmp[5]);
  	result.m[3][3] =   a * tmp[2] - b * tmp[4] + c * tmp[5];

  	det = 1.0f / (a * result.m[0][0] + b * result.m[1][0]
                + c * result.m[2][0] + d * result.m[3][0]);

	#if QM_USE_SSE

	__m128 scale = _mm_set1_ps(det);
	result.packed[0] = _mm_mul_ps(result.packed[0], scale);
	result.packed[1] = _mm_mul_ps(result.packed[1], scale);
	result.packed[2] = _mm_mul_ps(result.packed[2], scale);
	result.packed[3] = _mm_mul_ps(result.packed[3], scale);

	#else

	result.m[0][0] = result.m[0][0] * det;
	result.m[0][1] = result.m[0][1] * det;
	result.m[0][2] = result.m[0][2] * det;
	result.m[0][3] = result.m[0][3] * det;
	result.m[1][0] = result.m[1][0] * det;
	result.m[1][1] = result.m[1][1] * det;
	result.m[1][2] = result.m[1][2] * det;
	result.m[1][3] = result.m[1][3] * det;
	result.m[2][0] = result.m[2][0] * det;
	result.m[2][1] = result.m[2][1] * det;
	result.m[2][2] = result.m[2][2] * det;
	result.m[2][3] = result.m[2][3] * det;
	result.m[3][0] = result.m[3][0] * det;
	result.m[3][1] = result.m[3][1] * det;
	result.m[3][2] = result.m[3][2] * det;
	result.m[3][3] = result.m[3][3] * det;

	#endif

  	return result;
}

//translation:

QM_INLINE QMmat3 QM_PREFIX(translate)(const QMvec2& t)
{
	QMmat3 result = QM_PREFIX(mat3_identity)();

	result.m[2][0] = t.x;
	result.m[2][1] = t.y;

	return result;
}

QM_INLINE QMmat4 QM_PREFIX(translate)(const QMvec3& t)
{
	QMmat4 result = QM_PREFIX(mat4_identity)();

	result.m[3][0] = t.x;
	result.m[3][1] = t.y;
	result.m[3][2] = t.z;

	return result;
}

//scaling:

QM_INLINE QMmat3 QM_PREFIX(scale)(const QMvec2& s)
{
	QMmat3 result = QM_PREFIX(mat3_identity)();

	result.m[0][0] = s.x;
	result.m[1][1] = s.y;

	return result;
}

QM_INLINE QMmat4 QM_PREFIX(scale)(const QMvec3& s)
{
	QMmat4 result = QM_PREFIX(mat4_identity)();

	result.m[0][0] = s.x;
	result.m[1][1] = s.y;
	result.m[2][2] = s.z;

	return result;
}

//rotation:

QM_INLINE QMmat3 QM_PREFIX(rotate)(float angle)
{
	QMmat3 result = QM_PREFIX(mat3_identity)();

	float radians = QM_PREFIX(deg_to_rad)(angle);
	float sine   = QM_SINF(radians);
	float cosine = QM_COSF(radians);

	result.m[0][0] = cosine;
	result.m[1][0] =   sine;
	result.m[0][1] =  -sine;
	result.m[1][1] = cosine;

	return result;
}

QM_INLINE QMmat4 QM_PREFIX(rotate)(const QMvec3& axis, float angle)
{
	QMmat4 result = QM_PREFIX(mat4_identity)();

	QMvec3 normalized = QM_PREFIX(normalize)(axis);

	float radians = QM_PREFIX(deg_to_rad)(angle);
	float sine    = QM_SINF(radians);
	float cosine  = QM_COSF(radians);
	float cosine2 = 1.0f - cosine;

	result.m[0][0] = normalized.x * normalized.x * cosine2 + cosine;
	result.m[0][1] = normalized.x * normalized.y * cosine2 + normalized.z * sine;
	result.m[0][2] = normalized.x * normalized.z * cosine2 - normalized.y * sine;
	result.m[1][0] = normalized.y * normalized.x * cosine2 - normalized.z * sine;
	result.m[1][1] = normalized.y * normalized.y * cosine2 + cosine;
	result.m[1][2] = normalized.y * normalized.z * cosine2 + normalized.x * sine;
	result.m[2][0] = normalized.z * normalized.x * cosine2 + normalized.y * sine;
	result.m[2][1] = normalized.z * normalized.y * cosine2 - normalized.x * sine;
	result.m[2][2] = normalized.z * normalized.z * cosine2 + cosine;

	return result;
}

QM_INLINE QMmat4 QM_PREFIX(rotate)(const QMvec3& euler)
{
	QMmat4 result = QM_PREFIX(mat4_identity)();

	QMvec3 radians;
	radians.x = QM_PREFIX(deg_to_rad)(euler.x);
	radians.y = QM_PREFIX(deg_to_rad)(euler.y);
	radians.z = QM_PREFIX(deg_to_rad)(euler.z);

	float sinX = QM_SINF(radians.x);
	float cosX = QM_COSF(radians.x);
	float sinY = QM_SINF(radians.y);
	float cosY = QM_COSF(radians.y);
	float sinZ = QM_SINF(radians.z);
	float cosZ = QM_COSF(radians.z);

	result.m[0][0] = cosY * cosZ;
	result.m[0][1] = cosY * sinZ;
	result.m[0][2] = -sinY;
	result.m[1][0] = sinX * sinY * cosZ - cosX * sinZ;
	result.m[1][1] = sinX * sinY * sinZ + cosX * cosZ;
	result.m[1][2] = sinX * cosY;
	result.m[2][0] = cosX * sinY * cosZ + sinX * sinZ;
	result.m[2][1] = cosX * sinY * sinZ - sinX * cosZ;
	result.m[2][2] = cosX * cosY;

	return result;
}

//to mat3:

QM_INLINE QMmat3 QM_PREFIX(top_left)(const QMmat4& m)
{
	QMmat3 result;

	result.m[0][0] = m.m[0][0];
	result.m[0][1] = m.m[0][1];
	result.m[0][2] = m.m[0][2];
	result.m[1][0] = m.m[1][0];
	result.m[1][1] = m.m[1][1];
	result.m[1][2] = m.m[1][2];
	result.m[2][0] = m.m[2][0];
	result.m[2][1] = m.m[2][1];
	result.m[2][2] = m.m[2][2];

	return result;
}

//projection:

QM_INLINE QMmat4 QM_PREFIX(perspective)(float fov, float aspect, float near, float far)
{
	QMmat4 result = {0};

	float scale = QM_TANF(QM_PREFIX(deg_to_rad)(fov * 0.5f)) * near;

	float right = aspect * scale;
	float left  = -right;
	float top   = scale;
	float bot   = -top;

	result.m[0][0] = near / right;
	result.m[1][1] = near / top;
	result.m[2][2] = -(far + near) / (far - near);
	result.m[3][2] = -2.0f * far * near / (far - near);
	result.m[2][3] = -1.0f;

	return result;
}

QM_INLINE QMmat4 QM_PREFIX(orthographic)(float left, float right, float bot, float top, float near, float far)
{
	QMmat4 result = QM_PREFIX(mat4_identity)();

	result.m[0][0] = 2.0f / (right - left);
	result.m[1][1] = 2.0f / (top - bot);
	result.m[2][2] = 2.0f / (near - far);

	result.m[3][0] = (left + right) / (left - right);
	result.m[3][1] = (bot  + top  ) / (bot  - top  );
	result.m[3][2] = (near + far  ) / (near - far  );

	return result;
}

//view matrix:

QM_INLINE QMmat4 QM_PREFIX(look)(const QMvec3& pos, const QMvec3& dir, const QMvec3& up)
{
	QMmat4 result;

	QMvec3 r = QM_PREFIX(normalize)(QM_PREFIX(cross)(up, dir));
	QMvec3 u = QM_PREFIX(cross)(dir, r);

	QMmat4 RUD = QM_PREFIX(mat4_identity)();
	RUD.m[0][0] = r.x;
	RUD.m[1][0] = r.y;
	RUD.m[2][0] = r.z;
	RUD.m[0][1] = u.x;
	RUD.m[1][1] = u.y;
	RUD.m[2][1] = u.z;
	RUD.m[0][2] = dir.x;
	RUD.m[1][2] = dir.y;
	RUD.m[2][2] = dir.z;

	QMvec3 oppPos = {-pos.x, -pos.y, -pos.z};	
	result = RUD * QM_PREFIX(translate)(oppPos);

	return result;
}

QM_INLINE QMmat4 QM_PREFIX(lookat)(const QMvec3& pos, const QMvec3& target, const QMvec3& up)
{
	QMmat4 result;

	QMvec3 dir = QM_PREFIX(normalize)(pos - target);
	result = QM_PREFIX(look)(pos, dir, up);

	return result;
}

//----------------------------------------------------------------------//
//QUATERNION FUNCTIONS:

#if QM_INCLUDE_IOSTREAM

std::ostream& operator<<(std::ostream& os, const QMquaternion& q)
{
	os << q.x << ", " << q.y << ", " << q.z << ", " << q.w;
	return os;
}

std::istream& operator>>(std::istream& is, QMquaternion& q)
{
	is >> q.x >> q.y >> q.z >> q.w;
	return is;
}

#endif

QM_INLINE QMquaternion QM_PREFIX(quaternion_identity)()
{
	QMquaternion result;

	result.x = 0.0f;
	result.y = 0.0f;
	result.z = 0.0f;
	result.w = 1.0f;

	return result;
}

QM_INLINE QMquaternion operator+(const QMquaternion& q1, const QMquaternion& q2)
{
	QMquaternion result;

	#if QM_USE_SSE

	result.packed = _mm_add_ps(q1.packed, q2.packed);

	#else

	result.x = q1.x + q2.x;
	result.y = q1.y + q2.y;
	result.z = q1.z + q2.z;
	result.w = q1.w + q2.w;

	#endif

	return result;
}

QM_INLINE QMquaternion operator-(const QMquaternion& q1, const QMquaternion& q2)
{
	QMquaternion result;

	#if QM_USE_SSE

	result.packed = _mm_sub_ps(q1.packed, q2.packed);

	#else

	result.x = q1.x - q2.x;
	result.y = q1.y - q2.y;
	result.z = q1.z - q2.z;
	result.w = q1.w - q2.w;

	#endif

	return result;
}

QM_INLINE QMquaternion operator*(const QMquaternion& q1, const QMquaternion& q2)
{
	QMquaternion result;

	#if QM_USE_SSE

	__m128 temp1;
	__m128 temp2;

	temp1 = _mm_shuffle_ps(q1.packed, q1.packed, _MM_SHUFFLE(3, 3, 3, 3));
	temp2 = q2.packed;
	result.packed = _mm_mul_ps(temp1, temp2);

	temp1 = _mm_xor_ps(_mm_shuffle_ps(q1.packed, q1.packed, _MM_SHUFFLE(0, 0, 0, 0)), _mm_setr_ps(0.0f, -0.0f, 0.0f, -0.0f));
	temp2 = _mm_shuffle_ps(q2.packed, q2.packed, _MM_SHUFFLE(0, 1, 2, 3));
	result.packed = _mm_add_ps(result.packed, _mm_mul_ps(temp1, temp2));

	temp1 = _mm_xor_ps(_mm_shuffle_ps(q1.packed, q1.packed, _MM_SHUFFLE(1, 1, 1, 1)), _mm_setr_ps(0.0f, 0.0f, -0.0f, -0.0f));
	temp2 = _mm_shuffle_ps(q2.packed, q2.packed, _MM_SHUFFLE(1, 0, 3, 2));
	result.packed = _mm_add_ps(result.packed, _mm_mul_ps(temp1, temp2));

	temp1 = _mm_xor_ps(_mm_shuffle_ps(q1.packed, q1.packed, _MM_SHUFFLE(2, 2, 2, 2)), _mm_setr_ps(-0.0f, 0.0f, 0.0f, -0.0f));
	temp2 = _mm_shuffle_ps(q2.packed, q2.packed, _MM_SHUFFLE(2, 3, 0, 1));
	result.packed = _mm_add_ps(result.packed, _mm_mul_ps(temp1, temp2));

	#else

	result.x = q1.w * q2.x + q1.x * q2.w + q1.y * q2.z - q1.z * q2.y;
    result.y = q1.w * q2.y - q1.x * q2.z + q1.y * q2.w + q1.z * q2.x;
    result.z = q1.w * q2.z + q1.x * q2.y - q1.y * q2.x + q1.z * q2.w;
    result.w = q1.w * q2.w - q1.x * q2.x - q1.y * q2.y - q1.z * q2.z;

	#endif

	return result;
}

QM_INLINE QMquaternion operator*(const QMquaternion& q, float s)
{
	QMquaternion result;

	#if QM_USE_SSE

	__m128 scale = _mm_set1_ps(s);
	result.packed = _mm_mul_ps(q.packed, scale);

	#else

	result.x = q.x * s;
	result.y = q.y * s;
	result.z = q.z * s;
	result.w = q.w * s;

	#endif

	return result;
}

QM_INLINE QMquaternion operator*(float s, const QMquaternion& q)
{
	return q * s;
}

QM_INLINE QMquaternion operator/(const QMquaternion& q, float s)
{
	QMquaternion result;

	#if QM_USE_SSE

	__m128 scale = _mm_set1_ps(s);
	result.packed = _mm_div_ps(q.packed, scale);

	#else

	result.x = q.x / s;
	result.y = q.y / s;
	result.z = q.z / s;
	result.w = q.w / s;

	#endif

	return result;
}

QM_INLINE QMquaternion operator/(float s, const QMquaternion& q)
{
	QMquaternion result;

	#if QM_USE_SSE

	__m128 scale = _mm_set1_ps(s);
	result.packed = _mm_div_ps(scale, q.packed);

	#else

	result.x = s / q.x;
	result.y = s / q.y;
	result.z = s / q.z;
	result.w = s / q.w;

	#endif

	return result;
}

QM_INLINE float QM_PREFIX(dot)(const QMquaternion& q1, const QMquaternion& q2)
{
	float result;

	#if QM_USE_SSE

	__m128 r = _mm_mul_ps(q1.packed, q2.packed);
	r = _mm_hadd_ps(r, r);
	r = _mm_hadd_ps(r, r);
	_mm_store_ss(&result, r);

	#else

	result = q1.x * q2.x + q1.y * q2.y + q1.z * q2.z + q1.w * q2.w;

	#endif

	return result;
}

QM_INLINE float QM_PREFIX(length)(const QMquaternion& q)
{
	float result;

	result = QM_SQRTF(QM_PREFIX(dot)(q, q));

	return result;
}

QM_INLINE QMquaternion QM_PREFIX(normalize)(const QMquaternion& q)
{
	QMquaternion result;

	float len = QM_PREFIX(length)(q);
	if(len != 0.0f)
		result = q / len;

	return result;
}

QM_INLINE QMquaternion QM_PREFIX(conjugate)(const QMquaternion& q)
{
	QMquaternion result;

	result.x = -q.x;
	result.y = -q.y;
	result.z = -q.z;
	result.w = q.w;

	return result;
}

QM_INLINE QMquaternion QM_PREFIX(inverse)(const QMquaternion& q)
{
	QMquaternion result;

	result.x = -q.x;
	result.y = -q.y;
	result.z = -q.z;
	result.w = q.w;

	#if QM_USE_SSE

	__m128 scale = _mm_set1_ps(QM_PREFIX(dot)(q, q));
	_mm_div_ps(result.packed, scale);

	#else

	float invLen2 = 1.0f / QM_PREFIX(quaternion_dot)(q, q);

	result.x *= invLen2;
	result.y *= invLen2;
	result.z *= invLen2;
	result.w *= invLen2;

	#endif

	return result;
}

QM_INLINE QMquaternion QM_PREFIX(slerp)(const QMquaternion& q1, const QMquaternion& q2, float a)
{
	QMquaternion result;

	float cosine = QM_PREFIX(dot)(q1, q2);
	float angle = QM_ACOSF(cosine);

	float sine1 = QM_SINF((1.0f - a) * angle);
	float sine2 = QM_SINF(a * angle);
	float invSine = 1.0f / QM_SINF(angle);

	QMquaternion q1scaled = q1 * sine1;
	QMquaternion q2scaled = q2 * sine2;

	result = q1scaled + q2scaled;
	result = result * invSine;

	return result;
}

QM_INLINE QMbool operator==(const QMquaternion& q1, const QMquaternion& q2)
{
	QMbool result;

	//TODO: there are SIMD instructions for floating point equality, find a way to get a single bool from them
	result = (q1.x == q2.x) && (q1.y == q2.y) && (q1.z == q2.z) && (q1.w == q2.w); 

	return result;	
}

QM_INLINE QMquaternion QM_PREFIX(quaternion_from_axis_angle)(const QMvec3& axis, float angle)
{
	QMquaternion result;

	float radians = QM_PREFIX(deg_to_rad)(angle * 0.5f);
	QMvec3 normalized = QM_PREFIX(normalize)(axis);
	float sine = QM_SINF(radians);

	result.x = normalized.x * sine;
	result.y = normalized.y * sine;
	result.z = normalized.z * sine;
	result.w = QM_COSF(radians);

	return result;
}

QM_INLINE QMquaternion QM_PREFIX(quaternion_from_euler)(const QMvec3& angles)
{
	QMquaternion result;

	QMvec3 radians;
	radians.x = QM_PREFIX(deg_to_rad)(angles.x * 0.5f);
	radians.y = QM_PREFIX(deg_to_rad)(angles.y * 0.5f);
	radians.z = QM_PREFIX(deg_to_rad)(angles.z * 0.5f);

	float sinx = QM_SINF(radians.x);
	float cosx = QM_COSF(radians.x);
	float siny = QM_SINF(radians.y);
	float cosy = QM_COSF(radians.y);
	float sinz = QM_SINF(radians.z);
	float cosz = QM_COSF(radians.z);

	#if QM_USE_SSE

	__m128 packedx = _mm_setr_ps(sinx, cosx, cosx, cosx);
	__m128 packedy = _mm_setr_ps(cosy, siny, cosy, cosy);
	__m128 packedz = _mm_setr_ps(cosz, cosz, sinz, cosz);

	result.packed = _mm_mul_ps(_mm_mul_ps(packedx, packedy), packedz);

	packedx = _mm_shuffle_ps(packedx, packedx, _MM_SHUFFLE(0, 0, 0, 1));
	packedy = _mm_shuffle_ps(packedy, packedy, _MM_SHUFFLE(1, 1, 0, 1));
	packedz = _mm_shuffle_ps(packedz, packedz, _MM_SHUFFLE(2, 0, 2, 2));

	result.packed = _mm_addsub_ps(result.packed, _mm_mul_ps(_mm_mul_ps(packedx, packedy), packedz));

	#else

	result.x = sinx * cosy * cosz - cosx * siny * sinz;
	result.y = cosx * siny * cosz + sinx * cosy * sinz;
	result.z = cosx * cosy * sinz - sinx * siny * cosz;
	result.w = cosx * cosy * cosz + sinx * siny * sinz;

	#endif

	return result;
}

QM_INLINE QMmat4 QM_PREFIX(quaternion_to_mat4)(const QMquaternion& q)
{
	QMmat4 result = QM_PREFIX(mat4_identity)();

	float x2  = q.x + q.x;
    float y2  = q.y + q.y;
    float z2  = q.z + q.z;
    float xx2 = q.x * x2;
    float xy2 = q.x * y2;
    float xz2 = q.x * z2;
    float yy2 = q.y * y2;
    float yz2 = q.y * z2;
    float zz2 = q.z * z2;
    float sx2 = q.w * x2;
    float sy2 = q.w * y2;
    float sz2 = q.w * z2;

	result.m[0][0] = 1.0f - (yy2 + zz2);
	result.m[0][1] = xy2 - sz2;
	result.m[0][2] = xz2 + sy2;
	result.m[1][0] = xy2 + sz2;
	result.m[1][1] = 1.0f - (xx2 + zz2);
	result.m[1][2] = yz2 - sx2;
	result.m[2][0] = xz2 - sy2;
	result.m[2][1] = yz2 + sx2;
	result.m[2][2] = 1.0f - (xx2 + yy2);

	return result;
}

#endif //QM_MATH_H