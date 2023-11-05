#include <component_manager.hpp>
#include <transform_component.hpp>


template<typename T>
Xplor::ComponentType Xplor::ComponentManager::GetComponentType()
{
	const char* typeName = typeid(T).name();

	assert((m_mapComponentTypes.find(typeName) != m_mapComponentTypes.end()) &&
		"Component type is not registered");

	// Return the component type to create masks
	return m_mapComponentTypes[typeName];
}

template<typename T>
void Xplor::ComponentManager::AddComponent(EntityID entity, T component)
{
	// Add a component element to the component array for an entity
	GetComponentArray<T>()->AddData(entity, component);
}

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



template<typename T>
std::shared_ptr<Xplor::ComponentArray<T>> Xplor::ComponentManager::GetComponentArray()
{
	const char* typeName = typeid(T).name();

	assert((m_mapComponentTypes.find(typename) != m_mapComponentTypes.end())
		&& "Component not registered");

	return std::static_pointer_cast<ComponentArray<T>>(m_mapComponentArrays[typeName]);
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

