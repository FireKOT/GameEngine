#include <Camera.h>
#include <DefaultGeometry.h>
#include <Game.h>
#include <GameObject.h>
#include <Input/InputHandler.h>
#include <random>

namespace GameEngine
{
	Game::Game(
		std::function<bool()> PlatformLoopFunc
	) :
		PlatformLoop(PlatformLoopFunc)
	{
		std::srand(1317);


		Core::g_MainCamera = new Core::Camera();
		Core::g_MainCamera->SetPosition(Math::Vector3f(22.5f, 40.0f, -30.0f));
		Core::g_MainCamera->SetViewDir(Math::Vector3f(0.0f, -6.0f, 6.0f).Normalized());

		m_renderThread = std::make_unique<Render::RenderThread>();

		for (size_t y = 0; y < 10; ++y) {

			for (size_t x = 0; x < 10; ++x) {

				int random = std::rand() % 3;
				if (random == 0) {

					m_Objects.push_back(new BouncingGameObject());
				}
				else if (random == 1) {

					m_Objects.push_back(new ControlledGameObject());
				}
				else if (random == 2) {

					m_Objects.push_back(new MovingGameObject(Math::Vector3f(x, 0.f, y) * 5));
				}

				Render::RenderObject** renderObject = m_Objects.back()->GetRenderObjectRef();
				m_renderThread->EnqueueCommand(Render::ERC::CreateRenderObject, RenderCore::DefaultGeometry::Cube(), renderObject);

				m_Objects.back()->SetPosition(Math::Vector3f(x, 0.f, y) * 5, m_renderThread->GetMainFrame());
			}
		}


		Core::g_InputHandler->RegisterCallback("GoForward", [&]() { Core::g_MainCamera->Move(Core::g_MainCamera->GetViewDir()); });
		Core::g_InputHandler->RegisterCallback("GoBack", [&]() { Core::g_MainCamera->Move(-Core::g_MainCamera->GetViewDir()); });
		Core::g_InputHandler->RegisterCallback("GoRight", [&]() { Core::g_MainCamera->Move(Core::g_MainCamera->GetRightDir()); });
		Core::g_InputHandler->RegisterCallback("GoLeft", [&]() { Core::g_MainCamera->Move(-Core::g_MainCamera->GetRightDir()); });
	}

	void Game::Run()
	{
		assert(PlatformLoop != nullptr);

		m_GameTimer.Reset();

		bool quit = false;
		while (!quit)
		{
			m_GameTimer.Tick();
			float dt = m_GameTimer.GetDeltaTime();

			Core::g_MainWindowsApplication->Update();
			Core::g_InputHandler->Update();
			Core::g_MainCamera->Update(dt);

			Update(dt);

			m_renderThread->OnEndFrame();

			// The most common idea for such a loop is that it returns false when quit is required, or true otherwise
			quit = !PlatformLoop();
		}
	}

	void Game::Update(float dt)
	{
		for (size_t i = 0; i < m_Objects.size(); ++i) {

			m_Objects[i]->Update(dt, m_renderThread->GetMainFrame());
		}
	}
}