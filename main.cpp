#include <Windows.h>
#include"KamataEngine.h"
#include"GameScene.h"
#include "TitleScene.h"
using namespace KamataEngine;

namespace {
enum class SceneType {
	kTitle,
	kGame,
};
}

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {
	

	KamataEngine::Initialize(L"GC2C_03_シュウ_シビ_AL3");

	DirectXCommon* dXCommon = DirectXCommon::GetInstance();
	ImGuiManager* imguiManager = ImGuiManager::GetInstance();

	SceneType sceneType = SceneType::kTitle;
	TitleScene* titleScene = new TitleScene();
	titleScene->Initialize();
	GameScene* gameScene = nullptr;

	while (true) {
	
		if (KamataEngine::Update()) {
			break;
		}

		imguiManager->Begin();

		if (sceneType == SceneType::kTitle) {
			titleScene->Update();
		} else {
			gameScene->Update();
		}

		imguiManager->End();

		dXCommon->PreDraw();

		if (sceneType == SceneType::kTitle) {
			titleScene->Draw();
		} else {
			gameScene->Draw();
		}

		imguiManager->Draw();

		dXCommon->PostDraw();

		// シーンの削除と生成は更新・描画が終わったフレーム境界で行う
		if (sceneType == SceneType::kTitle && titleScene->IsStartRequested()) {
			delete titleScene;
			titleScene = nullptr;

			gameScene = new GameScene();
			gameScene->Initialize();
			sceneType = SceneType::kGame;
		} else if (sceneType == SceneType::kGame && gameScene->IsFinished()) {
			delete gameScene;
			gameScene = nullptr;

			titleScene = new TitleScene();
			titleScene->Initialize();
			sceneType = SceneType::kTitle;
		}

	}

	delete gameScene;
	gameScene = nullptr;
	delete titleScene;
	titleScene = nullptr;

	KamataEngine::Finalize();

	return 0;
}
