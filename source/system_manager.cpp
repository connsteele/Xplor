#include <system_manager.hpp>


void Xplor::SystemManager::EntityDeleted(EntityID entity)
{
	// Erase a destroyed entity from all system lists
	// mEntities is a set so no check needed
	for (auto const& pair : m_mapSystems)
	{
		auto const& system = pair.second;

		system->m_entities.erase(entity);
	}
}

void Xplor::SystemManager::EntityMaskChange(EntityID entity, EntityMask entityMask)
{
	// Notify each system of an entity's mask change
	for (auto const& pair : m_mapSystems)
	{
		auto const& type = pair.first;
		auto const& system = pair.second;
		auto const& systemMask = m_mapMasks[type];

		// bitwise operation to check if the mask contains the system
		if ((entityMask & systemMask) == systemMask)
		{
			system->m_entities.insert(entity);
		}
		// mask doesn't match erase the entity from the set
		else
		{
			system->m_entities.erase(entity);
		}

	}
}
