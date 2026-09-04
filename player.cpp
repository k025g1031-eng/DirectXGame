#include "Player.h"
#include "MapChipField.h"
#include "Transform.h"
#include <algorithm>
#include <cassert>
#include <cmath>

using namespace KamataEngine;

namespace {
constexpr float kAcceleration = 0.02f;
constexpr float kAttenuation = 0.85f;
constexpr float kLimitRunSpeed = 0.20f;
constexpr float kJumpAcceleration = 0.42f;
constexpr float kGravityAcceleration = -0.025f;
constexpr float kLimitFallSpeed = -0.50f;
constexpr float kGroundHeight = 1.0f;
constexpr float kPlayerHalfWidth = 0.5f;
constexpr float kPlayerHalfHeight = 0.5f;
constexpr float kCollisionEpsilon = 0.01f;
} // namespace

Player::Player() {}

Player::~Player() {}

void Player::Initialize(Model* model, uint32_t textureHandle, Camera* camera, MapChipField* mapChipField) {

	assert(model);
	assert(camera);
	assert(mapChipField);

	model_ = model;
	textureHandle_ = textureHandle;
	camera_ = camera;
	mapChipField_ = mapChipField;

	worldTransform_.Initialize();
	worldTransform_.translation_ = {5.0f, kGroundHeight, -1.0f};
}

void Player::Update() {
	Input* input = Input::GetInstance();

	// 左右移動（矢印キーとA/Dの両方に対応）
	float accelerationX = 0.0f;
	if (input->PushKey(DIK_LEFT) || input->PushKey(DIK_A)) {
		accelerationX -= kAcceleration;
	}
	if (input->PushKey(DIK_RIGHT) || input->PushKey(DIK_D)) {
		accelerationX += kAcceleration;
	}

	if (accelerationX != 0.0f) {
		velocity_.x += accelerationX;
		velocity_.x = std::clamp(velocity_.x, -kLimitRunSpeed, kLimitRunSpeed);
	} else {
		velocity_.x *= kAttenuation;
		if (std::fabs(velocity_.x) < 0.001f) {
			velocity_.x = 0.0f;
		}
	}

	// 接地中のみジャンプを開始する
	if (onGround_ && input->TriggerKey(DIK_SPACE)) {
		velocity_.y = kJumpAcceleration;
		onGround_ = false;
	}

	if (!onGround_) {
		velocity_.y += kGravityAcceleration;
		velocity_.y = std::max(velocity_.y, kLimitFallSpeed);
	}

	// 移動後の予定位置でマップチップとの衝突を調べる
	CollisionMapInfo collisionInfo{};
	collisionInfo.movement = velocity_;
	CheckMapCollision(collisionInfo);

	worldTransform_.translation_.x += collisionInfo.movement.x;
	worldTransform_.translation_.y += collisionInfo.movement.y;
	ApplyMapCollisionResult(collisionInfo);

	// 今回はマップチップをすり抜け、固定の高さだけを床として扱う
	if (!onGround_ && worldTransform_.translation_.y <= kGroundHeight) {
		worldTransform_.translation_.y = kGroundHeight;
		velocity_.y = 0.0f;
		onGround_ = true;
	}

	worldTransform_.matWorld_ = MakeAffineMatrix(
		worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	worldTransform_.TransferMatrix();
}

void Player::Draw() { model_->Draw(worldTransform_, *camera_, textureHandle_); }

void Player::CheckMapCollision(CollisionMapInfo& info) {
	// 今回の課題では上方向（天井）のみ判定する
	CheckMapCollisionUp(info);
}

void Player::CheckMapCollisionUp(CollisionMapInfo& info) {
	if (info.movement.y <= 0.0f) {
		return;
	}

	Vector3 destination = worldTransform_.translation_;
	destination.x += info.movement.x;
	destination.y += info.movement.y;

	Vector3 checkPoints[2] = {
		GetCornerPosition(destination, false, true),
		GetCornerPosition(destination, true, true),
	};

	// 隣の列を誤判定しないよう、左右の検査点をわずかに内側へ寄せる
	checkPoints[0].x += kCollisionEpsilon;
	checkPoints[1].x -= kCollisionEpsilon;

	for (const Vector3& checkPoint : checkPoints) {
		const MapChipIndexSet indexSet = mapChipField_->GetMapChipIndexSetByPosition(checkPoint);
		if (mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex) != MapChipType::kBlock) {
			continue;
		}

		const MapChipRect blockRect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
		const float correctedMovementY =
			blockRect.bottom - (worldTransform_.translation_.y + kPlayerHalfHeight) - kCollisionEpsilon;

		info.movement.y = std::min(info.movement.y, correctedMovementY);
		info.ceiling = true;
	}
}

void Player::ApplyMapCollisionResult(const CollisionMapInfo& info) {
	if (info.ceiling) {
		// 天井に当たったら上昇を止め、次のフレームから落下へ移る
		velocity_.y = 0.0f;
	}
}

Vector3 Player::GetCornerPosition(const Vector3& center, bool right, bool top) const {
	Vector3 position = center;
	position.x += right ? kPlayerHalfWidth : -kPlayerHalfWidth;
	position.y += top ? kPlayerHalfHeight : -kPlayerHalfHeight;
	return position;
}
