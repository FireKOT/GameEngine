#pragma once

#include <array.h>
#include <Geometry.h>
#include <Vector.h>

namespace GameEngine
{
	namespace RenderCore
	{
		namespace DefaultGeometry
		{
			static Geometry::Ptr Cube()
			{
				constexpr Core::array<Geometry::VertexType, 8> vertices =
				{
					Math::Vector3f(-1.0f, -1.0f, -1.0f),
					Math::Vector3f(-1.0f, +1.0f, -1.0f),
					Math::Vector3f(+1.0f, +1.0f, -1.0f),
					Math::Vector3f(+1.0f, -1.0f, -1.0f),
					Math::Vector3f(-1.0f, -1.0f, +1.0f),
					Math::Vector3f(-1.0f, +1.0f, +1.0f),
					Math::Vector3f(+1.0f, +1.0f, +1.0f),
					Math::Vector3f(+1.0f, -1.0f, +1.0f)
				};

				constexpr Core::array<Geometry::IndexType, 36> indices =
				{
					// front face
					0, 1, 2,
					0, 2, 3,

					// back face
					4, 6, 5,
					4, 7, 6,

					// left face
					4, 5, 1,
					4, 1, 0,

					// right face
					3, 2, 6,
					3, 6, 7,

					// top face
					1, 5, 6,
					1, 6, 2,

					// bottom face
					4, 0, 3,
					4, 3, 7
				};

				return std::make_shared<Geometry>((Geometry::VertexType*)vertices.begin(), vertices.size(), (Geometry::IndexType*)indices.begin(), indices.size());
			}

			static Geometry::Ptr d20 () {

				constexpr float t = 1.618; //(1.f + sqrtf(5.f)) / 2.f

				constexpr Core::array<Geometry::VertexType, 12> vertices =
				{
					Math::Vector3f(-1.f,  t, 0.f),
					Math::Vector3f( 1.f,  t, 0.f),
					Math::Vector3f(-1.f, -t, 0.f),
					Math::Vector3f( 1.f, -t, 0.f),

					Math::Vector3f(0.f, -1.f,  t),
					Math::Vector3f(0.f,  1.f,  t),
					Math::Vector3f(0.f, -1.f, -t),
					Math::Vector3f(0.f,  1.f, -t),

					Math::Vector3f( t,   0.f, -1.f),
					Math::Vector3f( t,   0.f,  1.f),
					Math::Vector3f(-t,   0.f, -1.f),
					Math::Vector3f(-t,   0.f,  1.f)
				};

				constexpr Core::array<Geometry::IndexType, 60> indices =
				{
					0,  11, 5,
					0,  5,  1,
					0,  1,  7,
					0,  7,  10,
					0,  10, 11,

					1,  5,  9,
					5,  11, 4,
					11, 10, 2,
					10, 7,  6,
					7,  1,  8,

					3,  9,  4,
					3,  4,  2,
					3,  2,  6,
					3,  6,  8,
					3,  8,  9,

					4,  9,  5,
					2,  4,  11,
					6,  2,  10,
					8,  6,  7,
					9,  8,  1
				};

				return std::make_shared<Geometry>((Geometry::VertexType*)vertices.begin(), vertices.size(), (Geometry::IndexType*)indices.begin(), indices.size());
			}
		}
	}
}