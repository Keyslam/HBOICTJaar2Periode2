#include "Vector2.hpp"
#include <cmath>

template <typename T>
Vector2<T>::Vector2(T x, T y): x(x), y(y) { }

template <typename T>
Vector2<T>::Vector2(T v): x(v), y(v) { }

template <typename T>
Vector2<T>::Vector2(): x(0), y(0) { }

template <typename T>
Vector2<T> Vector2<T>::operator+(const Vector2<T>& other) const
{
	Vector2<T> out(x, y);
	out += other;
	return out;
}

template <typename T>
Vector2<T> Vector2<T>::operator-(const Vector2<T>& other) const
{
	Vector2<T> out(x, y);
	out -= other;
	return out;
}

template <typename T>
Vector2<T> Vector2<T>::operator*(const T scalar) const
{
	Vector2<T> out(x, y);
	out *= scalar;
	return out;
}

template <typename T>
Vector2<T> Vector2<T>::operator/(const Vector2<T>& other) const
{
	Vector2<T> out(x, y);
	out /= other;
	return out;
}

template <typename T>
Vector2<T>& Vector2<T>::operator+=(const Vector2<T>& other)
{
	x += other.x;
	y += other.y;
	return *this;
}

template <typename T>
Vector2<T>& Vector2<T>::operator-=(const Vector2<T>& other)
{
	x -= other.x;
	y -= other.y;
	return *this;
}

template <typename T>
Vector2<T>& Vector2<T>::operator*=(const T scalar)
{
	x *= scalar;
	y *= scalar;
	return *this;
}

template <typename T>
Vector2<T>& Vector2<T>::operator/=(const Vector2<T>& other)
{
	if (other.x != 0 && other.y != 0)
	{
		x /= other.x;
		y /= other.y;
	}
	return *this;
}

template <typename T>
void Vector2<T>::Set(T x, T y)
{
	this->x = x;
	this->y = y;
}

template <typename T>
void Vector2<T>::Normalize()
{
	const T length = Length();

	if (length == 0)
		return;

	x = x / length;
	y = y / length;
}

template <typename T>
Vector2<T> Vector2<T>::Normalized() const
{
	Vector2<T> out(x, y);
	out.Normalize();
	return out;
}

template <typename T>
void Vector2<T>::RotateInplace(T phi)
{
	float c = cos(phi);
	float s = sin(phi);

	const T x = (this->x * c) - (this->y * s);
	const T y = (this->x * s) + (this->y * c);

	this->x = x;
	this->y = y;
}

template <typename T>
Vector2<T> Vector2<T>::Rotated(T phi) const
{
	float c = cos(phi);
	float s = sin(phi);

	const T x = (this->x * c) - (this->y * s);
	const T y = (this->x * s) + (this->y * c);

	return Vector2(x, y);
}

template <typename T>
T Vector2<T>::Length() const
{
	return sqrt(Length2());
}

template <typename T>
T Vector2<T>::Length2() const
{
	return x * x + y * y;
}

template <typename T>
T Vector2<T>::Distance2(const Vector2& other) const
{
	
}

template <typename T>
Vector2<T> Vector2<T>::Zero()
{
	return Vector2(0, 0);
}

template <typename T>
Vector2<T> Vector2<T>::One()
{
	return Vector2(1, 1);
}

template <typename T>
Vector2<T> Vector2<T>::Up()
{
	return Vector2(0, -1);
}

template <typename T>
Vector2<T> Vector2<T>::Down()
{
	return Vector2(0, 1);
}

template <typename T>
Vector2<T> Vector2<T>::Left()
{
	return Vector2(-1, 0);
}

template <typename T>
Vector2<T> Vector2<T>::Right()
{
	return Vector2(1, 0);
}

template class Vector2<double>;
template class Vector2<float>;
template class Vector2<int>;
template class Vector2<short>;
template class Vector2<char>;
template class Vector2<unsigned int>;
template class Vector2<unsigned short>;
template class Vector2<unsigned char>;