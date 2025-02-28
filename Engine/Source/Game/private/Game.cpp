#include <Camera.h>
#include <iostream>
#include <fstream>
#include <DefaultGeometry.h>
#include <Game.h>
#include <GameObject.h>
#include <INIReader.h>
#include <windows.h>


namespace {

	bool isKeyPressed(int keyCode) {

		return GetAsyncKeyState(keyCode) & (1 << (sizeof(short) * CHAR_BIT - 1));
	}
}


namespace GameEngine
{
	Game::Game(
		std::function<bool()> PlatformLoopFunc
	) :
		PlatformLoop(PlatformLoopFunc)
	{
		Core::g_MainCamera = new Core::Camera();
		Core::g_MainCamera->SetPosition(Math::Vector3f(0.0f, 6.0f, -6.0f));
		Core::g_MainCamera->SetViewDir(Math::Vector3f(0.0f, -6.0f, 6.0f).Normalized());

		m_renderThread = std::make_unique<Render::RenderThread>();

		// How many objects do we want to create
		for (int i = 0; i < 3; ++i)
		{
			m_Objects.push_back(new GameObject());
			Render::RenderObject** renderObject = m_Objects.back()->GetRenderObjectRef();
			m_renderThread->EnqueueCommand(Render::ERC::CreateRenderObject, RenderCore::DefaultGeometry::Cube(), renderObject);
		}

		//key binds
		Core::INIReader reader("../../../../../controls.ini");
		if (reader.ParseError() < 0) {

			std::cerr << "Can't load \"controls.ini\"\n";
		}
		moveForewardKey_ = reader.GetInteger("controls", "MoveForeward", 87);
		moveBackwardKey_ = reader.GetInteger("controls", "MoveBackward", 83);
		moveRighKey_     = reader.GetInteger("controls", "MoveRight",    68);
		moveLeftKey_     = reader.GetInteger("controls", "MoveLeft",     65);
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

			Update(dt);
			
			// The most common idea for such a loop is that it returns false when quit is required, or true otherwise
			quit = !PlatformLoop();

			m_renderThread->OnEndFrame();
		}
	}

	void Game::Update(float dt)
	{
		for (int i = 0; i < m_Objects.size(); ++i)
		{
			Math::Vector3f pos = m_Objects[i]->GetPosition();

			// Showcase
			if (i == 0)
			{
				pos.x += 0.5f * dt;
			}
			else if (i == 1)
			{
				pos.y -= 0.5f * dt;
			}
			else if (i == 2)
			{
				pos.x += 0.5f * dt;
				pos.y -= 0.5f * dt;
			}
			m_Objects[i]->SetPosition(pos, m_renderThread->GetMainFrame());
		}


		float forewardSpeed = 0.f, rightSpeed = 0.f;
		if (isKeyPressed(moveForewardKey_)) {

			forewardSpeed += 1;
		}
		else if (isKeyPressed(moveBackwardKey_)) {

			forewardSpeed -= 1;
		}
		else if (isKeyPressed(moveRighKey_)) {

			rightSpeed += 1;
		}
		else if (isKeyPressed(moveLeftKey_)) {

			rightSpeed -= 1;
		}

		Math::Vector3f moveDirection = Core::g_MainCamera->GetViewDir() * forewardSpeed + Core::g_MainCamera->GetRightDir() * rightSpeed;

		Core::g_MainCamera->SetPosition(Core::g_MainCamera->GetPosition() + moveDirection * dt);
	}
}