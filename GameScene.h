#pragma once

#include "KamataEngine.h"
#include "MapChipField.h"
#include "Player.h"
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
	Skydome* skydome_ = nullptr;

	// 天球モデル
	KamataEngine::Model* skydomeModel_ = nullptr;

	// マップチップフィールド
	MapChipField* mapChipField_ = nullptr;

	// 自キャラ
	KamataEngine::Model* playerModel_ = nullptr;
	Player* player_ = nullptr;
	uint32_t playerTextureHandle_ = 0;
};
