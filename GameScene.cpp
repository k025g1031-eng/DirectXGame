#include"GameScene.h"

using namespace KamataEngine;


GameScene::GameScene() {}

GameScene::~GameScene() {

	
	delete model_;
	model_ = nullptr;

	//delete player_;
	//player_ = nullptr;

	delete camera_;
	camera_ = nullptr;

	delete debugCamera_;
	
}


void GameScene::Initialize() { 
	
	textureHandle_ = TextureManager::Load("mario.jpg");
	soundDataHandle_ = Audio::GetInstance()->LoadWave(" se_sad03.wav");
	sprite_ = Sprite::Create(textureHandle_, {100, 50});
	model_ = Model::Create();
	debugCamera_ = new DebugCamera(1280, 720);
	Audio::GetInstance()->PlayWave(soundDataHandle_);
	voiceHandle_ = Audio::GetInstance()->PlayWave(soundDataHandle_, true);
	//player_ = new Player();
	
	camera_ = new Camera();
	camera_->Initialize();
	PrimitiveDrawer::GetInstance()->SetCamera(camera_);
	PrimitiveDrawer::GetInstance()->DrawLine3d({0, 0, 0}, {0, 10, 0}, {1.0f, 0.0f, 0.0f, 10.f});

	
	/*player_->Initialize(model_, textureHandle_, camera_);*/
	


}
void GameScene::Update() { 
	
	Vector2 position = sprite_->GetPosition();

	position.x += 2.0f;
	position.y += 1.0f;

	sprite_->SetPosition(position);
	//player_->Update();

	if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		Audio::GetInstance()->StopWave(voiceHandle_);
	}

	ImGui::Begin("Debug1");
#ifdef _DEBUG
	ImGui::Text("Kamata Tarouj %d.%d.%d", 2050, 12, 31);
#endif
	ImGui::InputFloat3("InputFloat3", inputFloat3);
	ImGui::SliderFloat3("SliderFloat3", inputFloat3, 0.0f, 1.0f);
	ImGui::ShowDemoWindow();
	ImGui::End();

	debugCamera_->Update();

}
void GameScene::Draw() {
	Sprite::PreDraw();
	sprite_->Draw();
	Sprite::PostDraw();
	Model::PreDraw();
	model_->Draw(worldTransform_, *camera_, textureHandle_);
	Model::PostDraw();
	/*player_->Draw();*/

}

