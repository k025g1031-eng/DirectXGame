#pragma once

#include "KamataEngine.h"

class MapChipField;

/// <summary>
/// 地形に沿って巡回する雑魚敵
/// </summary>
class Enemy {
public:
	void Initialize(
		KamataEngine::Model* model, uint32_t textureHandle,
		KamataEngine::Camera* camera, MapChipField* mapChipField,
		const KamataEngine::Vector3& position);
	void Update();
	void Draw();

	const KamataEngine::Vector3& GetWorldPosition() const { return worldTransform_.translation_; }

private:
	bool IsBlockAt(const KamataEngine::Vector3& position) const;

	KamataEngine::WorldTransform worldTransform_;
	KamataEngine::Model* model_ = nullptr;
	KamataEngine::Camera* camera_ = nullptr;
	MapChipField* mapChipField_ = nullptr;
	uint32_t textureHandle_ = 0;
	float direction_ = -1.0f;
	float animationTime_ = 0.0f;
};
