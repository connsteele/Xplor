#include <component_array.hpp>

template<typename T>
void Xplor::ComponentArray<T>::DeleteData(EntityID entityID)
{
	// make sure the component exists before carrying through
	assert((m_mapEntityToIndex.find(entityID) != m_mapEntityToIndex.end())
		&& "Cannot delete, component not found in entity");

	// Move the last element into the deleted elements spot
	size_t indexReplace = m_mapEntityToIndex[entityID];
	size_t indexLast = m_activeSize - 1;
	m_components[indexReplace] = m_components[indexLast];

	// update the maps to account for the position swap
	EntityID lastEntity = m_mapIndexToEntity[indexLast];
	m_mapEntityToIndex[lastEntity] = indexReplace;
	m_mapIndexToEntity[indexReplace] = lastEntity;

	// Erase the input element and erase the last entity once it has been
	// swapped into the new space
	m_mapEntityToIndex.erase(entityID);
	m_mapIndexToEntity.erase(indexLast);


	// Update where the end of the active array is
	m_activeSize--;
}

template<typename T>
void Xplor::ComponentArray<T>::EntityDeleted(EntityID entityID)
{
	// Notify each component array that an entity has been destroyed
	// If it has a component for that entity, it will remove it
	if (m_mapEntityToIndex.find(entityID) != m_mapEntityToIndex.end())
	{
		// Remove the entity's component if it existed
		DeleteData(entityID);
	}
}


template<typename T>
T& Xplor::ComponentArray<T>::GetData(EntityID entityID)
{
	// Return the location in the array where the data for the input
	// entity begins
	return m_components[m_mapEntityToIndex[entityID]];
}


template void Xplor::ComponentArray<Xplor::componentTransform>::EntityDeleted(EntityID entityID);