#pragma once

#include <RenderEngine.h>
#include <RenderThread.h>
#include <Timer.h>
#include <Window/IWindow.h>

namespace GameEngine
{
	class GameObject;

	class Game final
	{
	public:
		Game() = delete;
		Game(
			std::function<bool()> PlatformLoopFunc
		);

	public:
		void Run();
		void Update(float dt);

	private:
		// The main idea behind having this functor is to abstract the common code from the platfrom-specific code
		std::function<bool()> PlatformLoop = nullptr;

	private:
		Core::Timer m_GameTimer;
		std::unique_ptr<Render::RenderThread> m_renderThread;
		std::vector<GameObject*> m_Objects;

		int moveForewardKey_ = 0, moveBackwardKey_ = 0, moveLeftKey_ = 0, moveRighKey_ = 0;

		float velocity_ = 1.f;
	};
}