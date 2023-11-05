#include <component_manager.hpp>
#include <transform_component.hpp>


template<typename T>
void Xplor::ComponentManager::RemoveComponent(EntityID entity)
{
	// Remove a component element from the component array for an entity
	GetComponentArray<T>()->RemoveData(entity);
}

template<typename T>
T& Xplor::ComponentManager::GetComponent(EntityID entity)
{
	// Get component element to the component array for an entity
	return GetComponentArray<T>()->GetData(entity);
}

void Xplor::ComponentManager::EntityDestroyed(EntityID entity)
{
	// Notify each component array that an entity has been destroyed
	// If it has a component for that entity, it will remove it
	for (auto const& pair : m_mapComponentArrays)
	{
		auto const& component = pair.second;

		component->EntityDeleted(entity);
	}
}
