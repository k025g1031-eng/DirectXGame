#include "GameScene.h"
#include "Transform.h"

using namespace KamataEngine;

GameScene::GameScene() {}

GameScene::~GameScene() {

	// 天球の解放
	delete skydome_;
	skydome_ = nullptr;

	delete skydomeModel_;
	skydomeModel_ = nullptr;

	// ブロックモデルの解放
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

	// ブロックモデルの生成
	blockModel_ = Model::Create();

	// カメラの生成
	camera_ = new Camera();
	camera_->Initialize();

	// デバッグカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);

	// 天球モデルの生成
	skydomeModel_ = Model::CreateFromOBJ("skydome", true);

	// 天球の生成
	skydome_ = new Skydome();

	// 天球の初期化
	skydome_->Initialize(skydomeModel_, camera_);

	// ブロックの数
	const uint32_t kNumBlockVertical = 10;
	const uint32_t kNumBlockHorizontal = 20;

	// ブロック1個分のサイズ
	const float kBlockWidth = 2.0f;
	const float kBlockHeight = 2.0f;

	// 縦方向の要素数
	worldTransformBlocks_.resize(kNumBlockVertical);

	for (uint32_t i = 0; i < kNumBlockVertical; ++i) {

		// 横方向の要素数
		worldTransformBlocks_[i].resize(kNumBlockHorizontal);
	}

	// ブロックの生成
	for (uint32_t i = 0; i < kNumBlockVertical; ++i) {

		for (uint32_t j = 0; j < kNumBlockHorizontal; ++j) {

			worldTransformBlocks_[i][j] = new WorldTransform();

			worldTransformBlocks_[i][j]->Initialize();

			worldTransformBlocks_[i][j]->translation_.x = kBlockWidth * j;

			worldTransformBlocks_[i][j]->translation_.y = kBlockHeight * i;
		}
	}
}

void GameScene::Update() {

#ifdef _DEBUG

	// デバッグカメラの切り替え
	if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		isDebugCameraActive_ = !isDebugCameraActive_;
	}

#endif

	// カメラの処理
	if (isDebugCameraActive_) {

		debugCamera_->Update();

		camera_->matView = debugCamera_->GetCamera().matView;

		camera_->matProjection = debugCamera_->GetCamera().matProjection;

		camera_->TransferMatrix();

	} else {

		camera_->UpdateMatrix();
	}

	// 天球の更新
	if (skydome_) {
		skydome_->Update();
	}

	// ブロックの更新
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

	// 天球の描画
	if (skydome_) {
		skydome_->Draw();
	}

	// ブロックの描画
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {

		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {

			if (!worldTransformBlock) {
				continue;
			}

			blockModel_->Draw(*worldTransformBlock, *camera_);
		}
	}

	Model::PostDraw();
}