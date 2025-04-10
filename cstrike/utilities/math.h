#pragma once

#include "../common.h"
// used: std::is_integral_v
#include <type_traits>
// used: sin, cos, pow, abs, sqrt
#include <corecrt_math.h>// used: MATH::Sin, cos, MATH::Pow, abs, sqrt
// used: rand, srand
#include <cstdlib>
// used: time
#include <ctime>
#include "../sdk/datatypes/matrix.h"
#include "../sdk/datatypes/vector.h"
// convert angle in degrees to radians
#define M_DEG2RAD(DEGREES) ((DEGREES) * (MATH::_PI / 180.f))
// convert angle in radians to degrees
#define M_RAD2DEG(RADIANS) ((RADIANS) * (180.f / MATH::_PI))
/// linearly interpolate the value between @a'X0' and @a'X1' by @a'FACTOR'
#define M_LERP(X0, X1, FACTOR) ((X0) + ((X1) - (X0)) * (FACTOR))
/// trigonometry
#define M_COS(ANGLE) cos(ANGLE)
#define M_SIN(ANGLE) sin(ANGLE)
#define M_TAN(ANGLE) tan(ANGLE)
/// power
#define M_POW(BASE, EXPONENT) pow(BASE, EXPONENT)
/// absolute value
#define M_ABS(VALUE) abs(VALUE)
/// square root
#define M_SQRT(VALUE) sqrt(VALUE)
/// floor
#define M_FLOOR(VALUE) floor(VALUE)

/*
 * MATHEMATICS
 * - basic trigonometry, algebraic mathematical functions and constants
 */
namespace MATH
{
	/* @section: constants */
	// pi value
	inline constexpr float _PI = 3.141592654f;
	// double of pi
	inline constexpr float _2PI = 6.283185307f;
	// half of pi
	inline constexpr float _HPI = 1.570796327f;
	// quarter of pi
	inline constexpr float _QPI = 0.785398163f;
	// reciprocal of double of pi
	inline constexpr float _1DIV2PI = 0.159154943f;
	// golden ratio
	inline constexpr float _PHI = 1.618033988f;

	// capture game's exports
	bool Setup();

	/* @section: algorithm */
	/// alternative of 'std::min'
	/// @returns : minimal value of the given comparable values
	template <typename T>
	[[nodiscard]] CS_INLINE constexpr const T& Min(const T& left, const T& right) noexcept
	{
		return (right < left) ? right : left;
	}
	[[nodiscard]] CS_INLINE void SinCos(float rad, float* sine, float* cosine) {
		*sine = sinf(rad);
		*cosine = cosf(rad);
	}

	void AngleVectors(const QAngle_t& angles, Vector_t* forward, Vector_t* right, Vector_t* up);

	/// alternative of 'std::max'
	/// @returns : maximal value of the given comparable values
	template <typename T>
	[[nodiscard]] CS_INLINE constexpr const T& Max(const T& left, const T& right) noexcept
	{
		return (right > left) ? right : left;
	}

	/// alternative of 'std::clamp'
	/// @returns : value clamped in range ['minimal' .. 'maximal']
	template <typename T>
	[[nodiscard]] CS_INLINE constexpr const T& Clamp(const T& value, const T& minimal, const T& maximal) noexcept
	{
		return (value < minimal) ? minimal : (value > maximal) ? maximal :
																 value;
	}

	[[nodiscard]] inline void vec_angles(Vector_t forward, Vector_t* angles)
	{
		float tmp, yaw, pitch;

		if (forward.y == 0.f && forward.x == 0.f) {
			yaw = 0;
			if (forward.z > 0) {
				pitch = 270;
			}
			else {
				pitch = 90.f;
			}
		}
		else {
			yaw = (float)(atan2(forward.y, forward.x) * 180.f / 3.14159265358979323846f);
			if (yaw < 0) {
				yaw += 360.f;
			}
			tmp = (float)sqrt(forward.x * forward.x + forward.y * forward.y);
			pitch = (float)(atan2(-forward.z, tmp) * 180.f / 3.14159265358979323846f);
			if (pitch < 0) {
				pitch += 360.f;
			}
		}
		angles->x = pitch;
		angles->y = yaw;
		angles->z = 0.f;
	}
	QAngle_t CalcAngles(Vector_t viewPos, Vector_t aimPos);
	Vector_t CalculateCameraPosition(Vector_t anchorPos, float distance, QAngle_t viewAngles);
	QAngle_t NormalizeAngles(QAngle_t angles);


	/* @section: exponential */
	/// @returns: true if given number is power of two, false otherwise
	template <typename T> requires (std::is_integral_v<T>)
	[[nodiscard]] CS_INLINE constexpr bool IsPowerOfTwo(const T value) noexcept
	{
		return value != 0 && (value & (value - 1)) == 0;
	}

	/* @section: random using game's exports */
	inline int(CS_CDECL* fnRandomSeed)(int iSeed) = nullptr;
	inline float(CS_CDECL* fnRandomFloat)(float flMinValue, float flMaxValue) = nullptr;
	inline float(CS_CDECL* fnRandomFloatExp)(float flMinValue, float flMaxValue, float flExponent) = nullptr;
	inline int(CS_CDECL* fnRandomInt)(int iMinValue, int iMaxValue) = nullptr;
	inline float(CS_CDECL* fnRandomGaussianFloat)(float flMean, float flStdDev) = nullptr;
}
