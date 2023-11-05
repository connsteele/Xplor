#pragma once

#include <bitset>
#include <glm/glm.hpp>

namespace Xplor {
	/// <summary>/// Type Alias for Entities /// </summary>
	using EntityID = std::uint32_t;
	const EntityID MAX_ENTITIES = 4096;

	/// <summary>/// Type Alias for Components /// </summary>
	using ComponentType = std::uint8_t;
	const ComponentType MAX_COMPONENTS = 32;

	/// <summary>
	/// Bitfield which tracks which components an entity contains
	/// </summary>
	using EntityMask = std::bitset<MAX_COMPONENTS>;


	// This should be moved to its own file later
	struct Transform {
		glm::mat4 model;
	};


}; // end namespace