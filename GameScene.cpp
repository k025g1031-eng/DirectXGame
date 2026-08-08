#include "GameScene.h"
#include "Transform.h"

using namespace KamataEngine;

GameScene::GameScene() {}

GameScene::~GameScene() {

	// 3Dモデルデータの解放
	delete blockModel_;
	blockModel_ = nullptr;

	// ブロックのワールド変換を解放
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {

		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {

			delete worldTransformBlock;
		}
	}

	worldTransformBlocks_.clear();

	// カメラの解放
	delete camera_;
	camera_ = nullptr;

	// デバッグカメラの解放
	delete debugCamera_;
	debugCamera_ = nullptr;
}

void GameScene::Initialize() {

	// 3Dモデルデータの生成
	blockModel_ = Model::Create();

	// カメラの生成
	camera_ = new Camera();
	camera_->Initialize();

	// デバッグカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);

	// 要素数
	const uint32_t kNumBlockVertical = 10;
	const uint32_t kNumBlockHorizontal = 20;

	// ブロック1個分のサイズ
	const float kBlockWidth = 2.0f;
	const float kBlockHeight = 2.0f;

	// 縦方向の要素数を設定
	worldTransformBlocks_.resize(kNumBlockVertical);

	for (uint32_t i = 0; i < kNumBlockVertical; ++i) {

		// 横方向の要素数を設定
		worldTransformBlocks_[i].resize(kNumBlockHorizontal);
	}

	// ブロックの生成
	for (uint32_t i = 0; i < kNumBlockVertical; ++i) {

		for (uint32_t j = 0; j < kNumBlockHorizontal; ++j) {

			worldTransformBlocks_[i][j] = new WorldTransform();

			worldTransformBlocks_[i][j]->Initialize();

			// 横方向の座標
			worldTransformBlocks_[i][j]->translation_.x = kBlockWidth * j;

			// 縦方向の座標
			worldTransformBlocks_[i][j]->translation_.y = kBlockHeight * i;
		}
	}
}
void GameScene::Update() {

#ifdef _DEBUG

	if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		isDebugCameraActive_ = !isDebugCameraActive_;
	}

#endif

	// カメラの処理
	if (isDebugCameraActive_) {

		// デバッグカメラの更新
		debugCamera_->Update();

		// デバッグカメラの行列をコピー
		camera_->matView = debugCamera_->GetCamera().matView;
		camera_->matProjection = debugCamera_->GetCamera().matProjection;

		// 転送
		camera_->TransferMatrix();

	} else {

		// 通常カメラの更新
		camera_->UpdateMatrix();
	}

	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {

		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {

			if (!worldTransformBlock) {
				continue;
			}

			worldTransformBlock->matWorld_ = MakeAffineMatrix(
				worldTransformBlock->scale_, 
				worldTransformBlock->rotation_, 
				worldTransformBlock->translation_);

			worldTransformBlock->TransferMatrix();
		}
	}
}

void GameScene::Draw() {

	Model::PreDraw();

	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {

		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {

			// nullptrなら処理しない
			if (!worldTransformBlock) {
				continue;
			}

			blockModel_->Draw(*worldTransformBlock, *camera_);
		}
	}

	Model::PostDraw();
}