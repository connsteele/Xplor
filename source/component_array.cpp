#include <component_array.hpp>

template<typename T>
void Xplor::ComponentArray<T>::AddData(EntityID entityID, T component)
{
	// Get the entry at the end of the array
	size_t index = m_activeSize;
	// Update mappings
	m_mapEntityToIndex[entityID] = index;
	m_mapIndexToEntity[index] = entityID;
	// Actually put the component into the array
	m_components[index] = component;

	// Update where the end of the active array is
	m_activeSize++;

	return;
}

template<typename T>
void Xplor::ComponentArray<T>::DeleteData(EntityID entityID)
{
	// make sure the component exists before carrying through
	assert((m_mapEntityToIndex.find(entityID) != m_mapEntityToIndex.end())
	&& "Cannot delete, component not found in entity")

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
	for (auto const& pair : m_mapComponentArrays)
	{
		auto const& component = pair.second;

		component->EntityDeleted(entity);
	}
}


template<typename T>
T& Xplor::ComponentArray<T>::GetData(EntityID entityID)
{
	// Return the location in the array where the data for the input
	// entity begins
	return m_components[m_mapEntityToIndex[entityID]];
}


