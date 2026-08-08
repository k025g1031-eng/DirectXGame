#pragma once

#include "KamataEngine.h"

/// <summary>
/// 天球
/// </summary>
class Skydome {
public:
	Skydome();
	~Skydome();

	void Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera);

	void Update();
	void Draw();

private:
	// 天球のワールド変換
	KamataEngine::WorldTransform worldTransform_;

	// 天球のモデル
	KamataEngine::Model* model_ = nullptr;

	// カメラ
	KamataEngine::Camera* camera_ = nullptr;
};