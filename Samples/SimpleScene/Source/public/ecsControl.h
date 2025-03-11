#pragma once

#include <flecs.h>

namespace GameEngine::Core
{
	class Camera;
	class Controller;
}

struct ControllerPtr
{
	GameEngine::Core::Controller* ptr;
};

struct JumpSpeed
{
	float value;
};

struct CameraPtr
{
	GameEngine::Core::Camera* ptr;
};

struct Shooter {

public:

	void update(float dt) {

		elapsed_ += dt;

		if (!isReadyToShoot_) {

			if (curMagSize_ == 0 && elapsed_ > reloadTime) {

				curMagSize_ = maxMagSize;
			}

			if (elapsed_ > shootCooldown && curMagSize_ > 0) {

				isReadyToShoot_ = true;
			}
		}
	}

	bool isReadyToShoot() const {

		return isReadyToShoot_;
	}

	void shoot() {

		if (curMagSize_ > 0) {
			
			--curMagSize_;
			isReadyToShoot_ = false;
			elapsed_ = 0.f;
		}
	}

public:

	float shootCooldown;
	float reloadTime;

	uint16_t maxMagSize;

//private:

	float elapsed_ = 0.f;

	bool isReadyToShoot_ = true;

	uint16_t curMagSize_= 3;
};

struct Bullet {};

struct Obstacle {};

struct DeleteAfterFall {

public:

	void update (float dt) {

		if (isFalled) {

			elapsedAfterFall_ += dt;

			if (elapsedAfterFall_ > deleteAfter) {

				wannaDie = true;
			}
		}
	}

public:

	float deleteAfter;

	bool wannaDie = false;

	bool isFalled = false;

//private:

	float elapsedAfterFall_ = 0.f;
};

void RegisterEcsControlSystems(flecs::world& world);

