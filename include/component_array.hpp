#pragma once

#include <xplor_types.hpp>
#include <unordered_map>

// Uses virtual inheritance, this interface is used to the component manager
// can tell generic component arrays an entity has been destroyed and update
// array mappings
class IComponentArray
{
public:
	virtual ~IComponentArray() = default;
	virtual void EntityDestroyed(Xplor::EntityID entityID) = 0;
};

namespace Xplor
{
	template <typename T>
	class ComponentArray : public IComponentArray
	{	

	public:

		void AddData(EntityID entityID, T component);

		void DeleteData(EntityID entityID);

		T& GetData(EntityID entityID);

		void EntityDeleted(EntityID entityID) override;


	private:
		// Packed array of components (of type T)
		std::array<T, MAX_ENTITIES> m_components;


		// NOTE: unordered maps have a performance penatly since it is
		// not contiguous. Arrays can be used instead, but maps support features
		// like find, insert and delete allowing for easy checking

		// Map from an EntityID to a component array index
		std::unordered_map<EntityID, size_t> m_mapEntityToIndex;

		// Map from a component array index to an EntityID
		std::unordered_map<size_t, EntityID> m_mapIndexToEntity;


		// Current active array size
		size_t m_activeSize;


	}; // end class

}; // end namespace