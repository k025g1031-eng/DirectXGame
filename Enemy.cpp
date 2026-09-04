#include "Enemy.h"

#include "MapChipField.h"
#include "Transform.h"
#include <cassert>
#include <cmath>

using namespace KamataEngine;

namespace {
constexpr float kMoveSpeed = 0.06f;
constexpr float kHalfWidth = 0.5f;
constexpr float kHalfHeight = 0.5f;
constexpr float kProbeDistance = 0.08f;
} // namespace

void Enemy::Initialize(
	Model* model, uint32_t textureHandle, Camera* camera,
	MapChipField* mapChipField, const Vector3& position) {
	assert(model);
	assert(camera);
	assert(mapChipField);

	model_ = model;
	textureHandle_ = textureHandle;
	camera_ = camera;
	mapChipField_ = mapChipField;
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
}

void Enemy::Update() {
	animationTime_ += 0.045f;

	// 少し速度を揺らし、単純な等速移動に見えないようにする
	const float speedVariation = 0.88f + std::sin(animationTime_ * 1.7f) * 0.12f;
	const float movementX = direction_ * kMoveSpeed * speedVariation;
	const float nextX = worldTransform_.translation_.x + movementX;
	const float frontX = nextX + direction_ * (kHalfWidth + kProbeDistance);

	const Vector3 wallProbe = {frontX, worldTransform_.translation_.y, worldTransform_.translation_.z};
	const Vector3 floorProbe = {
		frontX,
		worldTransform_.translation_.y - kHalfHeight - kProbeDistance,
		worldTransform_.translation_.z};

	// 壁または足場の端を見つけたら反転する
	if (IsBlockAt(wallProbe) || !IsBlockAt(floorProbe)) {
		direction_ *= -1.0f;
	} else {
		worldTransform_.translation_.x = nextX;
	}

	// 小さな傾きと伸縮で歩いているような印象を加える
	worldTransform_.rotation_.z = direction_ * std::sin(animationTime_ * 2.0f) * 0.06f;
	worldTransform_.scale_.x = 1.0f + std::sin(animationTime_ * 2.0f) * 0.04f;
	worldTransform_.scale_.y = 1.0f - std::sin(animationTime_ * 2.0f) * 0.04f;

	worldTransform_.matWorld_ = MakeAffineMatrix(
		worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	worldTransform_.TransferMatrix();
}

void Enemy::Draw() {
	model_->Draw(worldTransform_, *camera_, textureHandle_);
}

bool Enemy::IsBlockAt(const Vector3& position) const {
	const MapChipIndexSet indexSet = mapChipField_->GetMapChipIndexSetByPosition(position);
	return mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex) == MapChipType::kBlock;
}
