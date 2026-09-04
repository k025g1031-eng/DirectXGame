#pragma once

#include "KamataEngine.h"
#include <array>
#include <cstddef>

/// <summary>
/// 自キャラ死亡時の破片パーティクル
/// </summary>
class DeathParticles {
public:
	void Initialize(KamataEngine::Model* model, uint32_t textureHandle, KamataEngine::Camera* camera);
	void Spawn(const KamataEngine::Vector3& position);
	void Update();
	void Draw();
	bool IsFinished() const;

private:
	struct Particle {
		KamataEngine::WorldTransform worldTransform;
		KamataEngine::Vector3 velocity = {0.0f, 0.0f, 0.0f};
		float angularVelocity = 0.0f;
		uint32_t age = 0;
		uint32_t lifeTime = 0;
		bool active = false;
	};

	float NextRandom();

	static constexpr std::size_t kParticleCount = 24;
	std::array<Particle, kParticleCount> particles_{};
	KamataEngine::Model* model_ = nullptr;
	KamataEngine::Camera* camera_ = nullptr;
	uint32_t textureHandle_ = 0;
	uint32_t randomState_ = 0x1234abcd;
};
