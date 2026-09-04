#pragma once

#include "KamataEngine.h"

class Player;

/// <summary>
/// 2Dアクションゲーム用の追従カメラ
/// </summary>
class CameraController {
public:
	void Initialize(KamataEngine::Camera* camera, const Player* target);
	void Update();

private:
	KamataEngine::Camera* camera_ = nullptr;
	const Player* target_ = nullptr;
};
