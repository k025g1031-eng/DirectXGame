#include "Transform.h"
#include <cmath>

using namespace KamataEngine;

Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotation, const Vector3& translation) {

	Matrix4x4 result{};

	float sinX = std::sin(rotation.x);
	float cosX = std::cos(rotation.x);

	float sinY = std::sin(rotation.y);
	float cosY = std::cos(rotation.y);

	float sinZ = std::sin(rotation.z);
	float cosZ = std::cos(rotation.z);

	result.m[0][0] = scale.x * (cosY * cosZ);
	result.m[0][1] = scale.x * (cosY * sinZ);
	result.m[0][2] = scale.x * (-sinY);

	result.m[1][0] = scale.y * (sinX * sinY * cosZ - cosX * sinZ);
	result.m[1][1] = scale.y * (sinX * sinY * sinZ + cosX * cosZ);
	result.m[1][2] = scale.y * (sinX * cosY);

	result.m[2][0] = scale.z * (cosX * sinY * cosZ + sinX * sinZ);
	result.m[2][1] = scale.z * (cosX * sinY * sinZ - sinX * cosZ);
	result.m[2][2] = scale.z * (cosX * cosY);

	result.m[3][0] = translation.x;
	result.m[3][1] = translation.y;
	result.m[3][2] = translation.z;

	result.m[3][3] = 1.0f;

	return result;
}