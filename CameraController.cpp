#include "CameraController.h"

#include "player.h"
#include <algorithm>
#include <cassert>

using namespace KamataEngine;

namespace {
constexpr float kCameraZ = -30.0f;
constexpr float kTargetOffsetY = 5.0f;
constexpr float kInterpolationRate = 0.10f;

// カメラがマップ外を映さないための移動範囲
constexpr float kCameraLeft = 7.0f;
constexpr float kCameraRight = 92.0f;
constexpr float kCameraBottom = 8.0f;
constexpr float kCameraTop = 12.0f;
} // namespace

void CameraController::Initialize(Camera* camera, const Player* target) {
	assert(camera);
	assert(target);

	camera_ = camera;
	target_ = target;

	const Vector3& targetPosition = target_->GetWorldPosition();
	camera_->translation_ = {
		std::clamp(targetPosition.x, kCameraLeft, kCameraRight),
		std::clamp(targetPosition.y + kTargetOffsetY, kCameraBottom, kCameraTop),
		kCameraZ};
	camera_->UpdateMatrix();
}

void CameraController::Update() {
	const Vector3& targetPosition = target_->GetWorldPosition();

	const float destinationX = std::clamp(targetPosition.x, kCameraLeft, kCameraRight);
	const float destinationY = std::clamp(targetPosition.y + kTargetOffsetY, kCameraBottom, kCameraTop);

	// 急に画面が動かないよう、目的座標へ滑らかに追従する
	camera_->translation_.x += (destinationX - camera_->translation_.x) * kInterpolationRate;
	camera_->translation_.y += (destinationY - camera_->translation_.y) * kInterpolationRate;
	camera_->translation_.z = kCameraZ;
	camera_->UpdateMatrix();
}
