#include "Skydome.h"
#include "Transform.h"
#include <cassert>

using namespace KamataEngine;

Skydome::Skydome() {}

Skydome::~Skydome() {}

void Skydome::Initialize(Model* model, Camera* camera) {

	// NULLポインタチェック
	assert(model);

	// 引数をメンバ変数に記録
	model_ = model;
	camera_ = camera;

	// ワールド変換の初期化
	worldTransform_.Initialize();

	// 天球を大きくする
	worldTransform_.scale_ = {100.0f, 100.0f, 100.0f};
}

void Skydome::Update() {
	// カメラと同じ位置へ移動し、追従中も天球の端が見えないようにする
	worldTransform_.translation_ = camera_->translation_;

	// アフィン変換行列を作成
	worldTransform_.matWorld_ = MakeAffineMatrix(
		worldTransform_.scale_, 
		worldTransform_.rotation_, 
		worldTransform_.translation_);

	// 定数バッファに転送
	worldTransform_.TransferMatrix();
}

void Skydome::Draw() { model_->Draw(worldTransform_, *camera_); }
