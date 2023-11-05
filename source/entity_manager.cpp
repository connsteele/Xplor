#include "entity_manager.hpp"
#include <assert.h>
#include <xplor_types.hpp>

Xplor::EntityManager::EntityManager()
{
	// Make all entities available
	for (EntityID id = 0; id < MAX_ENTITIES; id++)
	{
		m_openEntityIDs.push(id);
	}
}

Xplor::EntityManager::~EntityManager()
{
	return;
}

Xplor::EntityID Xplor::EntityManager::CreateEntity()
{
	assert((m_activeEntities < MAX_ENTITIES) 
		&& "Creation of entity would exceed entity max");

	// Get the first available id
	EntityID newID = m_openEntityIDs.front();
	m_openEntityIDs.pop();
	m_activeEntities += 1;

	return newID;
}


void Xplor::EntityManager::DeleteEntity(EntityID entityID)
{
	assert((entityID < MAX_ENTITIES)
		&& "Input entityID is out of range");

	// Clear the information in the array at this location
	m_entityMasks[entityID].reset();

	// Place the open ID at the back of the queue
	m_openEntityIDs.push(entityID);
	m_activeEntities -= 1;
}

void Xplor::EntityManager::SetMask(EntityID entityID, EntityMask mask)
{
	assert((entityID < MAX_ENTITIES)
		&& "Input entityID is out of range");

	m_entityMasks[entityID] = mask;
}

Xplor::EntityMask Xplor::EntityManager::GetMask(EntityID entityID)
{
	return m_entityMasks[entityID];
}


