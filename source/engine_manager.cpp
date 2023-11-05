#include "engine_manager.hpp"

void Xplor::EngineManager::Init()
{
	m_entityManager = std::make_unique<EntityManager>();
	m_componentManager = std::make_unique<ComponentManager>();
	m_systemManager = std::make_unique<SystemManager>();
}

Xplor::EntityID Xplor::EngineManager::CreateEntity()
{
	return m_entityManager->CreateEntity();
}

void Xplor::EngineManager::DeleteEntity(EntityID entity)
{
	m_entityManager->DeleteEntity(entity);
}


template<typename T>
void Xplor::EngineManager::RegisterComponent()
{
	m_componentManager->RegisterComponent<T>();
}

template<typename T>
void Xplor::EngineManager::AddComponent(EntityID entity, T component)
{
	// Update the relevant component array
	m_componentManager->AddComponent<T>(entity, component);

	// Set the bitmask associated with the entity
	auto mask = m_entityManager->GetMask(entity);
	mask.set(m_componentManager->GetComponentType<T>(), true);
	m_entityManager->SetMask(entity, mask);

	// Alert the system manager of a mask change
	m_systemManager->EntityMaskChange(entity, mask);
}

template<typename T>
void Xplor::EngineManager::RemoveComponent(EntityID entity)
{
	m_componentManager->RemoveComponent<T>(entity);

	// Update the bitmask assocaited with the entity
	auto mask = m_entityManager->GetMask(entity);
	mask.set(m_componentManager->GetComponentType<T>(), false);
	m_entityManager->SetMask(entity, mask);

	// Alert the system manager of a mask change
	m_systemManager->EntityMaskChange(entity, mask);
}

template<typename T>
T& Xplor::EngineManager::GetComponent(EntityID entity)
{
	return m_componentManager->GetComponent<T>(entity);
}


template<typename T>
Xplor::ComponentType Xplor::EngineManager::GetComponentType()
{
	return m_componentManager->GetComponentType<T>();
}

template<typename T>
std::shared_ptr<T> Xplor::EngineManager::RegisterSystem()
{
	return m_systemManager->RegisterSystem<T>();
}

template<typename T>
void Xplor::EngineManager::SetSystemMask(EntityMask mask)
{
	m_systemManager->SetMask<T>(mask);
}



