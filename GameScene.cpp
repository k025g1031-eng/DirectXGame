#include "GameScene.h"

#include "Transform.h"

using namespace KamataEngine;

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete cameraController_;
	cameraController_ = nullptr;

	// 自キャラの解放
	delete player_;
	player_ = nullptr;

	delete playerModel_;
	playerModel_ = nullptr;

	// マップチップフィールドの解放
	delete mapChipField_;
	mapChipField_ = nullptr;

	// 天球の解放
	delete skydome_;
	skydome_ = nullptr;

	// 天球モデルの解放
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

	// --------------------------------
	// カメラの生成
	// --------------------------------

	camera_ = new Camera();
	camera_->Initialize();

	// --------------------------------
	// デバッグカメラの生成
	// --------------------------------

	debugCamera_ = new DebugCamera(1280, 720);

	// --------------------------------
	// ブロックモデルの生成
	// --------------------------------

	blockModel_ = Model::Create();

	// --------------------------------
	// 天球モデルの生成
	// --------------------------------

	skydomeModel_ = Model::CreateFromOBJ("skydome", true);

	// --------------------------------
	// 天球の生成
	// --------------------------------

	skydome_ = new Skydome();

	skydome_->Initialize(skydomeModel_, camera_);

	// --------------------------------
	// マップチップフィールドの生成
	// --------------------------------

	mapChipField_ = new MapChipField();

	// マップチップの初期化
	mapChipField_->Initialize();

	// CSVからマップチップデータを読み込む
	mapChipField_->LoadMapChipCsv("Resources/blocks.csv");

	// --------------------------------
	// 自キャラの生成
	// --------------------------------
	playerModel_ = Model::Create();
	playerTextureHandle_ = TextureManager::Load("mario.png");
	player_ = new Player();
	player_->Initialize(playerModel_, playerTextureHandle_, camera_);

	// 自キャラを追従するカメラ制御
	cameraController_ = new CameraController();
	cameraController_->Initialize(camera_, player_);

	// --------------------------------
	// ブロックの生成
	// --------------------------------

	const uint32_t kNumBlockVertical = MapChipField::kNumBlockVirtical;

	const uint32_t kNumBlockHorizontal = MapChipField::kNumBlockHorizontal;

	// 縦方向の要素数を設定
	worldTransformBlocks_.resize(kNumBlockVertical);

	for (uint32_t i = 0; i < kNumBlockVertical; ++i) {

		// 横方向の要素数を設定
		worldTransformBlocks_[i].resize(kNumBlockHorizontal);
	}

	// ブロックの生成
	for (uint32_t i = 0; i < kNumBlockVertical; ++i) {

		for (uint32_t j = 0; j < kNumBlockHorizontal; ++j) {

			// ブロックがない場所は生成しない
			if (mapChipField_->GetMapChipTypeByIndex(j, i) != MapChipType::kBlock) {

				worldTransformBlocks_[i][j] = nullptr;
				continue;
			}

			// ワールド変換を生成
			worldTransformBlocks_[i][j] = new WorldTransform();

			// 初期化
			worldTransformBlocks_[i][j]->Initialize();

			// 座標を設定
			worldTransformBlocks_[i][j]->translation_ = mapChipField_->GetMapChipPositionByIndex(j, i);
		}
	}
}

void GameScene::Update() {
	// 先に自キャラを更新し、その最新座標をカメラが追跡する
	if (player_) {
		player_->Update();
	}

#ifdef _DEBUG

	// デバッグカメラの切り替え
	if (Input::GetInstance()->TriggerKey(DIK_F1)) {

		isDebugCameraActive_ = !isDebugCameraActive_;
	}

#endif

	// --------------------------------
	// カメラの処理
	// --------------------------------

	if (isDebugCameraActive_) {

		// デバッグカメラの更新
		debugCamera_->Update();

		// デバッグカメラの行列をコピー
		camera_->matView = debugCamera_->GetCamera().matView;

		camera_->matProjection = debugCamera_->GetCamera().matProjection;

		// ビュープロジェクション行列を転送
		camera_->TransferMatrix();

	} else {
		// 通常時は自キャラを滑らかに追従する
		cameraController_->Update();
	}

	// --------------------------------
	// 天球の更新
	// --------------------------------

	if (skydome_) {
		skydome_->Update();
	}

	// --------------------------------
	// ブロックの更新
	// --------------------------------

	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {

		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {

			// nullptrなら処理しない
			if (!worldTransformBlock) {
				continue;
			}

			// アフィン変換行列を計算
			worldTransformBlock->matWorld_ = MakeAffineMatrix(worldTransformBlock->scale_, worldTransformBlock->rotation_, worldTransformBlock->translation_);

			// 定数バッファに転送
			worldTransformBlock->TransferMatrix();
		}
	}
}

void GameScene::Draw() {

	// 3Dモデル描画開始
	Model::PreDraw();

	// --------------------------------
	// 天球の描画
	// --------------------------------

	if (skydome_) {
		skydome_->Draw();
	}

	// --------------------------------
	// ブロックの描画
	// --------------------------------

	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {

		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {

			// nullptrなら処理しない
			if (!worldTransformBlock) {
				continue;
			}

			blockModel_->Draw(*worldTransformBlock, *camera_);
		}
	}

	// 自キャラの描画
	if (player_) {
		player_->Draw();
	}

	// 3Dモデル描画終了
	Model::PostDraw();
}
