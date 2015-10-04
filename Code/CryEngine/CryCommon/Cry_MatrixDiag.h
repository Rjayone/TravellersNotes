// Copyright 2001-2015 Crytek GmbH. All rights reserved.
# pragma once

#include <limits>


template<typename F>
struct Diag33_tpl
{
	F x,y,z;

	ILINE Diag33_tpl()
		#ifdef _DEBUG
		: x(std::numeric_limits<F>::signaling_NaN())
		, y(std::numeric_limits<F>::signaling_NaN())
		, z(std::numeric_limits<F>::signaling_NaN())
		#else
		// No initialization
		#endif
	{
	}

	explicit Diag33_tpl(type_identity) 
		: x(1)
		, y(1)
		, z(1) 
	{
	}

	explicit Diag33_tpl(F v) 
		: x(v)
		, y(v)
		, z(v) 
	{
	}

	explicit Diag33_tpl(F dx, F dy, F dz)
		: x(dx)
		, y(dy)
		, z(dz) 
	{
	}

	explicit Diag33_tpl(const Vec3_tpl<F> &v) 
		: x(v.x)
		, y(v.y)
		, z(v.z) 
	{
	}

	template<class F1> Diag33_tpl& operator=(const Vec3_tpl<F1>& v)
	{ 
		x = v.x;
		y = v.y;
		z = v.z; 
		return *this; 
	}

	template<class F1> Diag33_tpl& operator=(const Diag33_tpl<F1>& diag)
	{ 
		x = diag.x;
		y = diag.y;
		z = diag.z; 
		return *this; 
	}

	const Diag33_tpl& SetIdentity() { x = y = z = 1; return *this; }

	const Diag33_tpl& zero() { x = y = z = 0; return *this; }

	Diag33_tpl& fabs() { x = fabs_tpl(x); y = fabs_tpl(y); z = fabs_tpl(z); return *this; }

	/**
	 * In-place matrix inversion.
	 */
	Diag33_tpl& invert() 
	{
		F det = determinant();
		if (det == 0) { return *this; }
		det = (F)1.0/det;
		const F oldata[3] = { x, y, z };
		x = oldata[1] * oldata[2] * det;
		y = oldata[0] * oldata[2] * det; 
		z = oldata[0] * oldata[1] * det;
		return *this;
	}

	/**
	 * Linear-Interpolation between Diag33(lerp)
	 */
	ILINE void SetLerp(const Diag33_tpl<F>& p, const Diag33_tpl<F>& q, F t)
	{	
		x = p.x * (1 - t) + q.x * t;
		y = p.y * (1 - t) + q.y * t;
		z = p.z * (1 - t) + q.z * t;
	}
	ILINE static Diag33_tpl<F> CreateLerp(const Diag33_tpl<F> &p, const Diag33_tpl<F>& q, F t)
	{	
		Diag33_tpl<F> d;
		d.SetLerp(p, q, t);
		return d;
	}

	F determinant() const {	return x * y * z; }

	ILINE bool IsValid() const
	{
		if (!NumberValid(x)) return false;
		if (!NumberValid(y)) return false;
		if (!NumberValid(z)) return false;
		return true;
	}

	template<class F1>
	Diag33_tpl& operator*=(const Diag33_tpl<F1>& other)
	{
		x *= other.x;
		y *= other.y;
		z *= other.z;
		return *this;
	}

	template<class F1>
	Diag33_tpl& operator/=(const Diag33_tpl<F1>& other)
	{
		x /= other.x;
		y /= other.y;
		z /= other.z;
		return *this;
	}

	template<class F1>
	Diag33_tpl& operator+=(const Diag33_tpl<F1>& other)
	{
		x += other.x;
		y += other.y;
		z += other.z;
		return *this;
	}

	template<class F1>
	Diag33_tpl& operator-=(const Diag33_tpl<F1>& other)
	{
		x -= other.x;
		y -= other.y;
		z -= other.z;
		return *this;
	}
};


typedef Diag33_tpl<f32>  Diag33; //always 32 bit
typedef Diag33_tpl<f64>  Diag33d;//always 64 bit
typedef Diag33_tpl<real> Diag33r;//variable float precision. depending on the target system it can be between 32, 64 or 80 bit


template<class F1, class F2> 
Diag33_tpl<F1> operator*(const Diag33_tpl<F1>& lhs, const Diag33_tpl<F2>& rhs)
{
	Diag33_tpl<F1> out = lhs;
	out *= rhs;
	return out;
}

template<class F1, class F2> 
Diag33_tpl<F1> operator/(const Diag33_tpl<F1>& lhs, const Diag33_tpl<F2>& rhs)
{
	Diag33_tpl<F1> out = lhs;
	out /= rhs;
	return out;
}

template<class F1, class F2> 
Diag33_tpl<F1> operator+(const Diag33_tpl<F1>& lhs, const Diag33_tpl<F2>& rhs)
{
	Diag33_tpl<F1> out = lhs;
	out += rhs;
	return out;
}

template<class F1, class F2> 
Diag33_tpl<F1> operator-(const Diag33_tpl<F1>& lhs, const Diag33_tpl<F2>& rhs)
{
	Diag33_tpl<F1> out = lhs;
	out -= rhs;
	return out;
}

template<class F1, class F2> 
Matrix33_tpl<F2> operator*(const Diag33_tpl<F1>& lsh, const Matrix33_tpl<F2>& rhs)
{
	Matrix33_tpl<F2> res;
	res.m00 = rhs.m00 * lsh.x; res.m01 = rhs.m01 * lsh.x; res.m02 = rhs.m02 * lsh.x;
	res.m10 = rhs.m10 * lsh.y; res.m11 = rhs.m11 * lsh.y; res.m12 = rhs.m12 * lsh.y;
	res.m20 = rhs.m20 * lsh.z; res.m21 = rhs.m21 * lsh.z; res.m22 = rhs.m22 * lsh.z;
	return res;
}

template<class F1, class F2> 
Matrix34_tpl<F2> operator*(const Diag33_tpl<F1>& lhs, const Matrix34_tpl<F2>& rhs) {
	Matrix34_tpl<F2> m;
	m.m00 = lhs.x * rhs.m00; m.m01 = lhs.x * rhs.m01; m.m02 = lhs.x*rhs.m02; m.m03 = lhs.x * rhs.m03;
	m.m10 = lhs.y * rhs.m10; m.m11 = lhs.y * rhs.m11; m.m12 = lhs.y*rhs.m12; m.m13 = lhs.y * rhs.m13;
	m.m20 = lhs.z * rhs.m20; m.m21 = lhs.z * rhs.m21; m.m22 = lhs.z*rhs.m22; m.m23 = lhs.z * rhs.m23;
	return m;
}

template<class F1,class F2>
Vec3_tpl<F2> operator*(const Diag33_tpl<F1>& mtx, const Vec3_tpl<F2>& vec)
{
	return Vec3_tpl<F2>(mtx.x * vec.x, mtx.y * vec.y, mtx.z * vec.z);
}

template<class F1, class F2>
Vec3_tpl<F1> operator*(const Vec3_tpl<F1>& vec, const Diag33_tpl<F2>& mtx)
{
	return Vec3_tpl<F1>(mtx.x * vec.x, mtx.y * vec.y, mtx.z * vec.z);
}

template<class F1, class F2>
bool operator==(const Diag33_tpl<F1>& lhs, const Diag33_tpl<F2>& rhs)
{
	return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z;
}

template<class F1, class F2>
bool operator!=(const Diag33_tpl<F1>& lhs, const Diag33_tpl<F2>& rhs)
{
	return !(lhs == rhs);
}

template <class F> 
bool IsEquivalent(const Diag33_tpl<F>& lhs, const Diag33_tpl<F>& rhs, f32 epsilon = VEC_EPSILON) 
{
	return ((fabs_tpl(lhs.x - rhs.x) <= epsilon) && (fabs_tpl(lhs.y - rhs.y) <= epsilon) && (fabs_tpl(lhs.z - rhs.z) <= epsilon));
}
