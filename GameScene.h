#pragma once
#include "KamataEngine.h"
#include "Skydome.h"
#include <vector>

class GameScene {
public:
	GameScene();
	~GameScene();

	void Initialize();
	void Update();
	void Draw();

private:
	// ブロックモデル
	KamataEngine::Model* blockModel_ = nullptr;

	// ブロックのワールド変換
	std::vector<std::vector<KamataEngine::WorldTransform*>> worldTransformBlocks_;

	// カメラ
	KamataEngine::Camera* camera_ = nullptr;

	// デバッグカメラ
	KamataEngine::DebugCamera* debugCamera_ = nullptr;

	// デバッグカメラ有効
	bool isDebugCameraActive_ = false;

	// 天球
	KamataEngine::Model* skydomeModel_ = nullptr;
	Skydome* skydome_ = nullptr;
};