#include <Camera.h>
#include <ecsControl.h>
#include <ECS/ecsSystems.h>
#include <ecsPhys.h>
#include <ecsMesh.h>
#include <flecs.h>
#include <Input/Controller.h>
#include <Input/InputHandler.h>
#include <Vector.h>
#include <DefaultGeometry.h>
#include <RenderObject.h>

using namespace GameEngine;

void RegisterEcsControlSystems(flecs::world& world)
{
	world.system<Position, CameraPtr, const Speed, const ControllerPtr>()
		.each([&](flecs::entity e, Position& position, CameraPtr& camera, const Speed& speed, const ControllerPtr& controller)
	{
		Math::Vector3f currentMoveDir = Math::Vector3f::Zero();
		if (controller.ptr->IsPressed("GoLeft"))
		{
			currentMoveDir = currentMoveDir - camera.ptr->GetRightDir();
		}
		if (controller.ptr->IsPressed("GoRight"))
		{
			currentMoveDir = currentMoveDir + camera.ptr->GetRightDir();
		}
		if (controller.ptr->IsPressed("GoBack"))
		{
			currentMoveDir = currentMoveDir - camera.ptr->GetViewDir();
		}
		if (controller.ptr->IsPressed("GoForward"))
		{
			currentMoveDir = currentMoveDir + camera.ptr->GetViewDir();
		}
		position.value = position.value + currentMoveDir.Normalized() * speed * world.delta_time();
		camera.ptr->SetPosition(position.value);
	});

	world.system<const Position, Velocity, const ControllerPtr, const BouncePlane, const JumpSpeed>()
		.each([&](const Position& pos, Velocity& vel, const ControllerPtr& controller, const BouncePlane& plane, const JumpSpeed& jump)
	{
		constexpr float planeEpsilon = 0.1f;
		if (plane.value.x * pos.value.x + plane.value.y * pos.value.y + plane.value.z * pos.value.z < plane.value.w + planeEpsilon)
		{
			if (controller.ptr->IsPressed("Jump"))
			{
				vel.value.y = jump.value;
			}
		}
	});


	world.system<CameraPtr, const ControllerPtr, Shooter>()
		.each([&](CameraPtr &camPtr, const ControllerPtr &controlPtr, Shooter &shooter) {

			if (controlPtr.ptr->IsPressed("Shoot")) {

				if (shooter.isReadyToShoot()) {

					shooter.shoot();

					world.entity()
						.set(Position{ camPtr.ptr->GetPosition() + camPtr.ptr->GetViewDir() * 2.f })
						.set(Velocity{ camPtr.ptr->GetViewDir() * 70.f })
						.set(Gravity{ Math::Vector3f(0.f, -9.8065f, 0.f) })
						.set(BouncePlane{ Math::Vector4f(0.f, 1.f, 0.f, 5.f) })
						.set(Bounciness{ 0.5f })
						.set(DeleteAfterFall{ 5.f })
						.set(GeometryPtr{ RenderCore::DefaultGeometry::d20() })
						.set(RenderObjectPtr{ new Render::RenderObject() })
						.set(RigidSphereBody{ 1.f })
						.add<Bullet>();
				}
			}
		});


	world.system<DeleteAfterFall>()
		.each([&](flecs::entity e, DeleteAfterFall &deleter) {

			if (deleter.wannaDie) {

				ecs_delete(world, e);
			}
		});
}

