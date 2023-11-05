#pragma once

#include <queue>
#include <array>
#include <xplor_types.hpp>

namespace Xplor {
	class EntityManager
	{
	public:
		EntityManager();

		~EntityManager();

		EntityID CreateEntity();

		void DeleteEntity(EntityID entityID);

		void SetMask(EntityID entityID, EntityMask mask);

		EntityMask GetMask(EntityID entityID);


	private:
		// Later on I want to write my own memory manager instead of using
		// the built in queue and array classes. I think the array class in particular
		// is going to leave gaps in the data when entities are deleted

		/// <summary>
		/// 
		/// </summary>
		std::queue<EntityID> m_openEntityIDs{};


		/// Array of Masks where the EntityID is the index into it
		std::array<EntityMask, MAX_ENTITIES> m_entityMasks;

		/// <summary>
		/// 
		/// </summary>
		size_t m_activeEntities{};


	};// end class
}; // end namespace