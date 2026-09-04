#include "Player.h"
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
} // namespace

Player::Player() {}

Player::~Player() {}

void Player::Initialize(Model* model, uint32_t textureHandle, Camera* camera) {

	assert(model);
	assert(camera);

	model_ = model;
	textureHandle_ = textureHandle;
	camera_ = camera;

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

	worldTransform_.translation_.x += velocity_.x;
	worldTransform_.translation_.y += velocity_.y;

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
