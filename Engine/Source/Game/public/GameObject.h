#pragma once

#include <RenderObject.h>
#include <RenderThread.h>
#include <DirectXMath.h>
#include <Vector.h>
#include <Input/InputHandler.h>


namespace GameEngine
{
	class GameObject {

	public:

		GameObject() = default;
		virtual ~GameObject() = default;

	public:

		virtual void Update (float dt, size_t frame) {}

		Render::RenderObject** GetRenderObjectRef() { return &m_RenderObject; }

		void SetPosition(Math::Vector3f position, size_t frame)
		{
			m_Position = position;

			if (m_RenderObject) [[likely]]
			{
				m_RenderObject->SetPosition(position, frame);
			}
		}

		Math::Vector3f GetPosition()
		{
			return m_Position;
		}

	protected:

		Render::RenderObject* m_RenderObject = nullptr;

		Math::Vector3f m_Position = Math::Vector3f::Zero();
	};


	class BouncingGameObject: public GameObject {

	public:

		BouncingGameObject() = default;
		virtual ~BouncingGameObject() = default;

		void Update(float dt, size_t frame) override {

			velocity_ -= g_ * dt;

			if (m_Position.y <= groundLevel_) {

				velocity_ *= -1;
			}

			SetPosition(m_Position + Math::Vector3f(0.f, 1.f, 0.f) * velocity_ * dt, frame);
		}

	private:

		float g_ = 9.8f;
		float velocity_ = 0.f;
		float groundLevel_ = -2.f;
	};

	class ControlledGameObject : public GameObject {

	public:

		ControlledGameObject() {

			Core::g_InputHandler->RegisterCallback("MoveQubeForward", [this]() { moveDirection_ = moveDirection_ + Math::Vector3f( 0.f, 0.f,  1.f); });
			Core::g_InputHandler->RegisterCallback("MoveQubeBack",    [this]() { moveDirection_ = moveDirection_ + Math::Vector3f( 0.f, 0.f, -1.f); });
			Core::g_InputHandler->RegisterCallback("MoveQubeRight",   [this]() { moveDirection_ = moveDirection_ + Math::Vector3f( 1.f, 0.f,  0.f); });
			Core::g_InputHandler->RegisterCallback("MoveQubeLeft",    [this]() { moveDirection_ = moveDirection_ + Math::Vector3f(-1.f, 0.f,  0.f); });
		}
		virtual ~ControlledGameObject() = default;

		void Update(float dt, size_t frame) override {

			SetPosition(m_Position + moveDirection_.Normalized() * velocity_ * dt, frame);

			moveDirection_ = Math::Vector3f::Zero();
		}

	private:

		Math::Vector3f moveDirection_ = Math::Vector3f::Zero();
		float velocity_ = 2.f;
	};

	class MovingGameObject : public GameObject {

	public:

		MovingGameObject() = default;
		MovingGameObject(Math::Vector3f center): center_(center) {}
		virtual ~MovingGameObject() = default;

		void Update(float dt, size_t frame) override {

			static float angle = 0.f;
			angle += frequency_ * dt;
			if (angle > DirectX::XM_2PI) {		//shit solution

				angle -= DirectX::XM_2PI;
			}

			float shift = amplitude_ * sinf(angle);
			SetPosition(center_ + moveAxis_ * shift, frame);
		}

		void setCenter(Math::Vector3f center) {

			center_ = center;
		}

	public:

		Math::Vector3f center_ = Math::Vector3f::Zero();
		Math::Vector3f moveAxis_ = Math::Vector3f(1.f, 0.f, 0.f);

		float frequency_ = 0.5f;
		float amplitude_ = 2.f;
	};
}