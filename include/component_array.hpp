#pragma once

#include <xplor_types.hpp>
#include <unordered_map>
#include <array>

// Uses virtual inheritance, this interface is used to the component manager
// can tell generic component arrays an entity has been destroyed and update
// array mappings
class IComponentArray
{
public:
	virtual ~IComponentArray() = default;
	virtual void EntityDeleted(Xplor::EntityID entityID) = 0;
};

namespace Xplor
{
	template <typename T>
	class ComponentArray : public IComponentArray
	{	

	public:

		void AddData(EntityID entityID, T component)
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

		void RemoveData(EntityID entityID)
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

		T& GetData(EntityID entityID)
		{
			// Return the location in the array where the data for the input
			// entity begins
			return m_components[m_mapEntityToIndex[entityID]];
		}

		void EntityDeleted(EntityID entityID) override
		{
			// Notify each component array that an entity has been destroyed
			// If it has a component for that entity, it will remove it
			if (m_mapEntityToIndex.find(entityID) != m_mapEntityToIndex.end())
			{
				// Remove the entity's component if it existed
				RemoveData(entityID);
			}
		}



	private:
		// Packed array of components (of type T)
		std::array<T, MAX_ENTITIES> m_components{};


		// NOTE: unordered maps have a performance penatly since it is
		// not contiguous. Arrays can be used instead, but maps support features
		// like find, insert and delete allowing for easy checking

		// Map from an EntityID to a component array index
		std::unordered_map<EntityID, size_t> m_mapEntityToIndex{};

		// Map from a component array index to an EntityID
		std::unordered_map<size_t, EntityID> m_mapIndexToEntity{};


		// Current active array size
		size_t m_activeSize{};


	}; // end class

}; // end namespace