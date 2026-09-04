#pragma once
#include <KamataEngine.h>

class MapChipField;

struct CollisionMapInfo {
	bool ceiling = false;
	KamataEngine::Vector3 movement = {0.0f, 0.0f, 0.0f};
};

class Player {
public:
	Player();
	~Player();

	/// <param name="model">モデル</param>
	/// <param name="textureHandle">テクスチャハンドル</param>
	void Initialize(KamataEngine::Model* model, uint32_t textureHandle, KamataEngine::Camera* camera, MapChipField* mapChipField);

	void Update();
	void Draw();

	const KamataEngine::Vector3& GetWorldPosition() const { return worldTransform_.translation_; }

private:
	void CheckMapCollision(CollisionMapInfo& info);
	void CheckMapCollisionUp(CollisionMapInfo& info);
	void ApplyMapCollisionResult(const CollisionMapInfo& info);
	KamataEngine::Vector3 GetCornerPosition(const KamataEngine::Vector3& center, bool right, bool top) const;

	KamataEngine::Vector3 velocity_ = {0.0f, 0.0f, 0.0f};
	bool onGround_ = true;

	KamataEngine::WorldTransform worldTransform_;

	KamataEngine::Model* model_ = nullptr;

	KamataEngine::Camera* camera_ = nullptr;
	MapChipField* mapChipField_ = nullptr;

	uint32_t textureHandle_ = 0u;
};
