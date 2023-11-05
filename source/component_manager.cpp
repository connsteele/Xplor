#include <component_manager.hpp>
#include <transform_component.hpp>


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
