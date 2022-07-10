#pragma once

#include <cmath>

struct Vector3 {
	float x, y, z;
	inline Vector3()
		: x(0)
		, y(0)
		, z(0) {
	}
	inline Vector3(float x, float y, float z = 0)
		: x(x)
		, y(y)
		, z(z) {
	}
	inline float SquaredLength() const {
		return x * x + y * y + z * z;
	}
	inline float Length() const {
		return std::sqrt(x * x + y * y + z * z);
	}
	inline float Length2D() const {
		return std::sqrt(x * x + y * y);
	}
	inline float Dot(const Vector3& vOther) const {
		return Vector3::DotProduct(*this, vOther);
	}
	inline Vector3 operator*(float fl) const {
		Vector3 res;
		res.x = x * fl;
		res.y = y * fl;
		res.z = z * fl;
		return res;
	}
	inline Vector3& operator*=(float fl) {
		x = x * fl;
		y = y * fl;
		z = z * fl;
		return *this;
	}
	inline Vector3 operator/(float fl) const {
		return *this * (1 / fl);
	}
	inline Vector3& operator+=(const Vector3& vec) {
		x = x + vec.x;
		y = y + vec.y;
		z = z + vec.z;
		return *this;
	}
	inline Vector3 operator+(const Vector3 vec) const {
		Vector3 res;
		res.x = x + vec.x;
		res.y = y + vec.y;
		res.z = z + vec.z;
		return res;
	}
	inline Vector3& operator-=(const Vector3& vec) {
		x -= vec.x;
		y -= vec.y;
		z -= vec.z;
		return *this;
	}
	inline Vector3 operator-(const Vector3 vec) const {
		Vector3 res;
		res.x = x - vec.x;
		res.y = y - vec.y;
		res.z = z - vec.z;
		return res;
	}
	inline Vector3 operator-() const {
		return Vector3{ 0, 0, 0 } - *this;
	}
	inline float& operator[](int i) {
		return ((float*)this)[i];
	}
	inline float operator[](int i) const {
		return ((float*)this)[i];
	}
	inline bool operator==(const Vector3 vec) const {
		return x == vec.x && y == vec.y && z == vec.z;
	}
	inline bool operator!=(const Vector3 vec) const {
		return !(*this == vec);
	}
	static inline float DotProduct(const Vector3& a, const Vector3& b) {
		return a.x * b.x + a.y * b.y + a.z * b.z;
	}
	inline Vector3 Cross(const Vector3& v) {
		Vector3 out;
		out.x = this->y * v.z - this->z * v.y;
		out.y = this->z * v.x - this->x * v.z;
		out.z = this->x * v.y - this->y * v.x;
		return out;
	}
	inline Vector3 Normalize() {
		return *this / this->Length();
	}
};


struct Vector2 {
	float x, y;
	inline Vector2()
		: x(0)
		, y(0)  {
	}
	inline Vector2(float x, float y)
		: x(x)
		, y(y) {
	}
	inline float SquaredLength() const {
		return x * x + y * y;
	}
	inline float Length() const {
		return std::sqrt(x * x + y * y);
	}
	inline float Dot(const Vector2& vOther) const {
		return Vector2::DotProduct(*this, vOther);
	}
	inline Vector2 operator*(float fl) const {
		Vector2 res;
		res.x = x * fl;
		res.y = y * fl;
		return res;
	}
	inline Vector2& operator*=(float fl) {
		x = x * fl;
		y = y * fl;
		return *this;
	}
	inline Vector2 operator/(float fl) const {
		return *this * (1 / fl);
	}
	inline Vector2& operator+=(const Vector2& vec) {
		x = x + vec.x;
		y = y + vec.y;
		return *this;
	}
	inline Vector2 operator+(const Vector2 vec) const {
		Vector2 res;
		res.x = x + vec.x;
		res.y = y + vec.y;
		return res;
	}
	inline Vector2& operator-=(const Vector2& vec) {
		x -= vec.x;
		y -= vec.y;
		return *this;
	}
	inline Vector2 operator-(const Vector2 vec) const {
		Vector2 res;
		res.x = x - vec.x;
		res.y = y - vec.y;
		return res;
	}
	inline Vector2 operator-() const {
		return Vector2{ 0, 0 } - *this;
	}
	inline float& operator[](int i) {
		return ((float*)this)[i];
	}
	inline float operator[](int i) const {
		return ((float*)this)[i];
	}
	inline bool operator==(const Vector2 vec) const {
		return x == vec.x && y == vec.y;
	}
	inline bool operator!=(const Vector2 vec) const {
		return !(*this == vec);
	}
	static inline float DotProduct(const Vector2& a, const Vector2& b) {
		return a.x * b.x + a.y * b.y;
	}
	inline Vector2 Normalize() {
		return *this / this->Length();
	}
	inline operator Vector3() const { return Vector3(x, y); }
};