#include <Windows.h>
#include"KamataEngine.h"
#include"GameScene.h"
using namespace KamataEngine;

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {
	

	KamataEngine::Initialize(L"GC2C_03_シュウ_シビ_AL3");

	DirectXCommon* dXCommon = DirectXCommon::GetInstance();
	GameScene* gameScene = new GameScene();
	ImGuiManager* imguiManager = ImGuiManager::GetInstance();
	gameScene->Initialize();

	while (true) {
	
		if (KamataEngine::Update()) {
			break;
		}

		imguiManager->Begin();

		gameScene->Update();

		imguiManager->End();

		dXCommon->PreDraw();

		gameScene->Draw();

		imguiManager->Draw();

		dXCommon->PostDraw();

	}

	delete gameScene;

	gameScene = nullptr;

	KamataEngine::Finalize();

	return 0;
}
