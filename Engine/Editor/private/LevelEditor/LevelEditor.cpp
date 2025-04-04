#include <LevelEditor/LevelEditor.h>
#include <LevelEditor/LevelSerializer.h>
#include <LevelEditor/ECS/ecsLevelEditor.h>

#include <Filesystem.h>
#include <ECS/ecsSystems.h>
#include <Parser/WorldParser.h>

#include <imgui.h>
#include <imgui_stdlib.h>

#include <algorithm>


namespace GameEngine
{
	namespace Editor
	{
		void LevelEditor::spawnECSEntity () {

			World::LevelObject& levelObject = m_Level->GetLevelObjects().back();

			flecs::entity entity = world_.entity(levelObject.GetName().c_str());

			World::LevelObject::ComponentList& componentList = levelObject.GetComponents();

			World::LevelObject::ComponentList::iterator positionAttribute = std::ranges::find_if(componentList,
				[](World::LevelObject::Component& component)
				{
					return !std::strcmp(component.first.c_str(), "Position");
				}
			);

			World::LevelObject::ComponentList::iterator geometryAttribute = std::ranges::find_if(componentList,
				[](World::LevelObject::Component& component)
				{
					return !std::strcmp(component.first.c_str(), "GeometryPtr");
				}
			);

			if (positionAttribute != componentList.end() &&
				geometryAttribute != componentList.end())
			{
				assert(World::WorldParser::GetCustomComponents().contains(geometryAttribute->second));

				entity.set(EntitySystem::LevelEditorECS::PositionDesc{ &positionAttribute->second });

				// Can be set to 0 since it doesn't matter now, will be updated by the system
				entity.set(EntitySystem::EditorECS::Position{ 0.0f, 0.0f, 0.0f });
				entity.set(GeometryPtr{
					reinterpret_cast<RenderCore::Geometry*>(
						World::WorldParser::GetCustomComponents()[geometryAttribute->second]
						)
					});
			}
		}

		LevelEditor::LevelEditor(flecs::world& world): world_(world)
		{
			m_Level = LevelSerializer::Deserialize(Core::g_FileSystem->GetFilePath("Levels/Main.xml").generic_string());

			for (World::LevelObject& levelObject : m_Level->GetLevelObjects())
			{
				flecs::entity entity = world_.entity(levelObject.GetName().c_str());

				World::LevelObject::ComponentList& componentList = levelObject.GetComponents();

				World::LevelObject::ComponentList::iterator positionAttribute = std::ranges::find_if(componentList,
					[](World::LevelObject::Component& component)
					{
						return !std::strcmp(component.first.c_str(), "Position");
					}
				);

				World::LevelObject::ComponentList::iterator geometryAttribute = std::ranges::find_if(componentList,
					[](World::LevelObject::Component& component)
					{
						return !std::strcmp(component.first.c_str(), "GeometryPtr");
					}
				);

				if (positionAttribute != componentList.end() &&
					geometryAttribute != componentList.end())
				{
					assert(World::WorldParser::GetCustomComponents().contains(geometryAttribute->second));

					entity.set(EntitySystem::LevelEditorECS::PositionDesc{ &positionAttribute->second });

					// Can be set to 0 since it doesn't matter now, will be updated by the system
					entity.set(EntitySystem::EditorECS::Position{ 0.0f, 0.0f, 0.0f });
					entity.set(GeometryPtr{
						reinterpret_cast<RenderCore::Geometry*>(
							World::WorldParser::GetCustomComponents()[geometryAttribute->second]
							)
						});
				}
			}

			EntitySystem::LevelEditorECS::RegisterLevelEditorEcsSystems(world);
		}

		void LevelEditor::Draw()
		{
			ImGui::Begin(GetName());

			if (m_Level.has_value()) [[likely]]
			{
				for (World::LevelObject& levelObject : m_Level->GetLevelObjects())
				{
					if (ImGui::TreeNode(levelObject.GetName().c_str()))
					{
						for (World::LevelObject::Component& component : levelObject.GetComponents())
						{
							if (component.first == "Position") {

								float pos[3] = {};

								std::string oldVal = component.second;
								std::replace(oldVal.begin(), oldVal.end(), ',', ' ');

								std::stringstream ss;
								ss << oldVal;
								ss >> pos[0] >> pos[1] >> pos[2];

								ImGui::InputFloat3(component.first.c_str(), pos);

								std::string posStr = std::to_string(pos[0]) + ", " + std::to_string(pos[1]) + ", " + std::to_string(pos[2]);
								component.second = posStr;
							}
							else {

								ImGui::InputText(component.first.c_str(), &component.second);
							}
						}

						ImGui::TreePop();
					}
				}
			}

			if (ImGui::Button("Spawn Cube")) {

				static uint64_t cubeNumber = 0;

				std::string cubeName = "Cube#" + std::to_string(cubeNumber);
				
				World::LevelObject cube;
				cube.SetName(cubeName.c_str());
				cube.AddComponent("Position", "0.0, 0.0, 0.0");
				cube.AddComponent("GeometryPtr", "Cube");

				m_Level->AddLevelObject(cube);

				spawnECSEntity();

				++cubeNumber;
			}

			if (ImGui::Button("Save"))
			{
				m_SaveButtonMessageTimer.Reset();
				m_SaveButtonPressed = true;

				Save();
			}

			if (m_SaveButtonPressed)
			{
				ImGui::SameLine();
				ImGui::Text("Saved!");
			}

			ImGui::End();
		}

		void LevelEditor::Update(float dt)
		{
			m_SaveButtonMessageTimer.Tick();

			if (m_SaveButtonMessageTimer.GetTotalTime() > m_TimeToShowSaveButtonMessage)
			{
				m_SaveButtonPressed = false;
			}
		}

		void LevelEditor::Save()
		{
			assert(m_Level.has_value());
			LevelSerializer::Serialize(Core::g_FileSystem->GetFilePath("Levels/Main.xml").generic_string(), m_Level.value());
		}
	}
}