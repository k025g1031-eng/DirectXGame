#include "TitleScene.h"

using namespace KamataEngine;

TitleScene::~TitleScene() {
	delete backgroundSprite_;
	backgroundSprite_ = nullptr;
}

void TitleScene::Initialize() {
	textureHandle_ = TextureManager::Load("title.png");
	backgroundSprite_ = Sprite::Create(textureHandle_, {0.0f, 0.0f});
}

void TitleScene::Update() {
	if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		startRequested_ = true;
	}
}

void TitleScene::Draw() {
	Sprite::PreDraw();
	backgroundSprite_->Draw();
	Sprite::PostDraw();
}
