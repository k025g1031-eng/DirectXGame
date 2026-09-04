#pragma once

#include "KamataEngine.h"

/// <summary>
/// タイトルシーン
/// </summary>
class TitleScene {
public:
	~TitleScene();
	void Initialize();
	void Update();
	void Draw();
	bool IsStartRequested() const { return startRequested_; }

private:
	KamataEngine::Sprite* backgroundSprite_ = nullptr;
	uint32_t textureHandle_ = 0;
	bool startRequested_ = false;
};
