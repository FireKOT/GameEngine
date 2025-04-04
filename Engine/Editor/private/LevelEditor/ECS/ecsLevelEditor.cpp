#include <LevelEditor/ECS/ecsLevelEditor.h>
#include <LevelEditor/LevelEditor.h>

namespace
{
	using namespace GameEngine;

	void ParsePosition(
		const EntitySystem::LevelEditorECS::PositionDesc& positionDesc,
		EntitySystem::EditorECS::Position& position
	)
	{
		const World::LevelObject& levelObject = positionDesc.objects->at(positionDesc.id);

		const World::LevelObject::ComponentList& componentList = levelObject.GetComponents();

		World::LevelObject::ComponentList::const_iterator positionAttribute = std::ranges::find_if(componentList,
			[](const World::LevelObject::Component& component)
			{
				return !std::strcmp(component.first.c_str(), "Position");
			}
		);

		if (positionAttribute != componentList.end())
		{
			const char* compValue = positionAttribute->second.c_str();
			char* end;

			float f = std::strtof(compValue, &end);
			position.x = f;
			compValue = end + 1;

			f = std::strtof(compValue, &end);
			position.y = f;
			compValue = end + 1;

			f = std::strtof(compValue, &end);
			position.z = f;
		}
	}
}

namespace GameEngine::EntitySystem::LevelEditorECS
{
	void RegisterLevelEditorEcsSystems(flecs::world& world)
	{
		world.system<PositionDesc, EntitySystem::EditorECS::Position>()
			.each([&](PositionDesc& positionDesc, EntitySystem::EditorECS::Position& position)
				{
					ParsePosition(positionDesc, position);
				});
	}
}