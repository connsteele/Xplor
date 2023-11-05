#include <system_manager.hpp>

template<typename T>
std::shared_ptr<T> Xplor::SystemManager::RegisterSystem()
{
	const char* systemTypeName = typeid(T).name();

	assert(m_mapSystems.find(typeName) == m_mapSystems.end()
		&& "Cannot register system: System is already registered");

	// Create a pointer to the system
	auto system = std::make_shared<T>();
	m_mapSystems.insert({systemTypeName, system});
	return system
}

template<typename T>
void Xplor::SystemManager::SetMask(EntityMask mask)
{
	const char* systemTypeName = typeid(T).name();

	assert(m_mapSystems.find(typeName) != m_mapSystems.end()
		&& "System cannot be found: Register system before using");

	m_mapMasks.insert({systemTypeName, mask});
}

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
