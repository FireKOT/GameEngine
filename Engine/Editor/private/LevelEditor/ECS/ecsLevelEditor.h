#pragma once

#include <Level.h>
#include <flecs.h>

#include <EditorECS/ecsEditor.h>

namespace GameEngine::EntitySystem::LevelEditorECS
{
	struct PositionDesc
	{
		const World::Level::LevelObjectList* objects;
		uint64_t id;
	};

	void RegisterLevelEditorEcsSystems(flecs::world& world);
}