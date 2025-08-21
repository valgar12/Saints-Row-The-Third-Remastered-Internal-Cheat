#pragma once
#include <cmath>
#include "includes.h"

constexpr float TWO_PI = 6.28318530717958647692f;
constexpr float PI = 3.14159265358979323846f;

struct Vec4
{
	float x{}, y{}, z{}, w{};
};

struct Vec3
{
	float x{ 0.f }; float y{ 0.f }; float z{ 0.f };

	constexpr Vec3 operator+(Vec3 other) noexcept
	{
		return { this->x + other.x, this->y + other.y, this->z + other.z };
	}
	constexpr Vec3 operator-(Vec3 other) noexcept
	{
		return { this->x - other.x, this->y - other.y, this->z - other.z };
	}
	constexpr Vec3 operator-(const Vec3& o) const
	{
		return Vec3(x - o.x, y - o.y, z - o.z);
	}
	constexpr Vec3 operator*(Vec3 other) noexcept
	{
		return { this->x * other.x, this->y * other.y, this->z * other.z };
	}
	constexpr Vec3 operator/(Vec3 other) noexcept
	{
		return { this->x / other.x, this->y / other.y, this->z / other.z };
	}
	constexpr bool operator==(Vec3 other) noexcept
	{
		if (this->x == other.x && this->y == other.y && this->z == other.z) return true;
		return false;
	}
	constexpr bool operator>(Vec3 other) noexcept
	{
		if (this->x > other.x && this->y > other.y && this->z > other.z) return true;
		return false;
	}
	constexpr bool operator<(Vec3 other) noexcept
	{
		if (this->x < other.x && this->y < other.y && this->z < other.z) return true;
		return false;
	}
	constexpr Vec3 operator++() noexcept
	{
		return { this->x++, this->y++, this->z++ };
	}
	constexpr Vec3 operator--() noexcept
	{
		return { this->x--, this->y--, this->z-- };
	}

	float DistanceTo(Vec3 other)
	{
		return std::sqrt(((this->x - other.x) * (this->x - other.x)) + ((this->y - other.y) * (this->y - other.y))
			+ ((this->z - other.z) * (this->z - other.z)));
	}
};

struct Vec2
{
	float x{ 0.f }; float y{ 0.f };

	constexpr Vec2 operator+(Vec2 other) noexcept
	{
		return { this->x + other.x, this->y + other.y };
	}
	constexpr Vec2 operator-(Vec2 other) noexcept
	{
		return { this->x - other.x, this->y - other.y };
	}
	constexpr Vec2 operator*(Vec2 other) noexcept
	{
		return { this->x * other.x, this->y * other.y };
	}
	constexpr Vec2 operator/(Vec2 other) noexcept
	{
		return { this->x / other.x, this->y / other.y };
	}
	constexpr bool operator==(Vec2 other) noexcept
	{
		if (this->x == other.x && this->y == other.y) return true;
		return false;
	}
	constexpr bool operator>(Vec2 other) noexcept
	{
		if (this->x > other.x && this->y > other.y) return true;
		return false;
	}
	constexpr bool operator<(Vec2 other) noexcept
	{
		if (this->x < other.x && this->y < other.y) return true;
		return false;
	}
	constexpr Vec2 operator++() noexcept
	{
		return { this->x++, this->y++ };
	}
	constexpr Vec2 operator--() noexcept
	{
		return { this->x--, this->y-- };
	}

	float DistanceTo(Vec2 other)
	{
		return std::sqrt(((this->x - other.x) * (this->x - other.x)) + ((this->y - other.y) * (this->y - other.y)));
	}

	ImVec2 ToImVec2()
	{
		return ImVec2(this->x, this->y);
	}
};


struct ViewMatrix
{
	ViewMatrix() noexcept : data() {}

	float* operator[](int index) noexcept
	{
		return data[index];
	}

	const float* operator[](int index) const noexcept
	{
		return data[index];
	}

	float data[4][4];
};

static inline float NormalizeYaw0To2Pi(float a) {
	a = fmodf(a, TWO_PI);
	if (a < 0.0f) a += TWO_PI;
	return a;
}

static inline float ClampF(float v, float lo, float hi) {
	return (v < lo) ? lo : (v > hi) ? hi : v;
}

inline bool Aimbot(const Vec3& camPos,
	float& camYaw, float& camPitch,
	const Vec3& targetPos,
	bool invertPitch = true,
	float pitchMin = -1.321f,
	float pitchMax = 0.948f)
{
	float dx = targetPos.x - camPos.x;
	float dy = targetPos.y - camPos.y;
	float dz = targetPos.z - camPos.z;

	float dist2 = dx * dx + dy * dy + dz * dz;
	if (dist2 < 1e-8f) return false;

	float horiz2 = dx * dx + dz * dz;
	float yaw;
	if (horiz2 < 1e-8f) {
		yaw = camYaw;
	}
	else {
		yaw = atan2f(dx, dz); 
	}

	float pitch = atan2f(dy, sqrtf(horiz2)); 

	if (invertPitch) pitch = -pitch;
	pitch = ClampF(pitch, pitchMin, pitchMax);

	yaw = NormalizeYaw0To2Pi(yaw);

	camYaw = yaw;
	camPitch = pitch;
	return true;
}


