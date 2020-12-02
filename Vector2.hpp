#pragma once

template <typename T>
class Vector2
{
public:
	Vector2(T x, T y);
	Vector2(T v);
	Vector2();

	T x;
	T y;

	Vector2 operator+(const Vector2& other) const;
	Vector2 operator-(const Vector2& other) const;
	Vector2 operator*(const T scalar) const;
	Vector2 operator/(const Vector2& other) const;
	Vector2& operator+=(const Vector2& other);
	Vector2& operator-=(const Vector2& other);
	Vector2& operator*=(const T scalar);
	Vector2& operator/=(const Vector2& other);

	void Set(T x, T y);

	void Normalize();
	Vector2<T> Normalized() const;

	void RotateInplace(T phi);
	Vector2<T> Rotated(T phi) const;

	T Length2() const;
	T Length() const;
	T Distance2(const Vector2& other) const;

	static Vector2<T> Zero();
	static Vector2<T> One();
	static Vector2<T> Up();
	static Vector2<T> Down();
	static Vector2<T> Left();
	static Vector2<T> Right();
};

