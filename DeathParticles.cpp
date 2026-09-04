#include "DeathParticles.h"

#include "Transform.h"
#include <algorithm>
#include <cassert>
#include <cmath>

using namespace KamataEngine;

namespace {
constexpr float kPi = 3.14159265358979323846f;
constexpr float kGravity = -0.012f;
constexpr float kInitialScale = 0.18f;
} // namespace

void DeathParticles::Initialize(Model* model, uint32_t textureHandle, Camera* camera) {
	assert(model);
	assert(camera);
	model_ = model;
	textureHandle_ = textureHandle;
	camera_ = camera;

	for (Particle& particle : particles_) {
		particle.worldTransform.Initialize();
	}
}

void DeathParticles::Spawn(const Vector3& position) {
	for (std::size_t i = 0; i < particles_.size(); ++i) {
		Particle& particle = particles_[i];
		const float angle =
			2.0f * kPi * static_cast<float>(i) / static_cast<float>(particles_.size()) +
			(NextRandom() - 0.5f) * 0.35f;
		const float speed = 0.10f + NextRandom() * 0.20f;

		particle.worldTransform.translation_ = position;
		particle.worldTransform.translation_.z = -1.2f;
		particle.worldTransform.rotation_ = {0.0f, 0.0f, angle};
		particle.worldTransform.scale_ = {kInitialScale, kInitialScale, kInitialScale};
		particle.velocity = {std::cos(angle) * speed, std::sin(angle) * speed + 0.12f, 0.0f};
		particle.angularVelocity = (NextRandom() - 0.5f) * 0.35f;
		particle.age = 0;
		particle.lifeTime = 45u + static_cast<uint32_t>(NextRandom() * 31.0f);
		particle.active = true;
	}
}

void DeathParticles::Update() {
	for (Particle& particle : particles_) {
		if (!particle.active) {
			continue;
		}

		particle.worldTransform.translation_.x += particle.velocity.x;
		particle.worldTransform.translation_.y += particle.velocity.y;
		particle.velocity.y += kGravity;
		particle.worldTransform.rotation_.z += particle.angularVelocity;
		++particle.age;

		if (particle.age >= particle.lifeTime) {
			particle.active = false;
			continue;
		}

		const float remaining = 1.0f - static_cast<float>(particle.age) / static_cast<float>(particle.lifeTime);
		const float scale = kInitialScale * std::clamp(remaining, 0.0f, 1.0f);
		particle.worldTransform.scale_ = {scale, scale, scale};
		particle.worldTransform.matWorld_ = MakeAffineMatrix(
			particle.worldTransform.scale_, particle.worldTransform.rotation_, particle.worldTransform.translation_);
		particle.worldTransform.TransferMatrix();
	}
}

void DeathParticles::Draw() {
	for (Particle& particle : particles_) {
		if (particle.active) {
			model_->Draw(particle.worldTransform, *camera_, textureHandle_);
		}
	}
}

float DeathParticles::NextRandom() {
	randomState_ = randomState_ * 1664525u + 1013904223u;
	return static_cast<float>(randomState_ & 0x00ffffffu) / static_cast<float>(0x01000000u);
}
