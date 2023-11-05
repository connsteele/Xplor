#include "engine_manager.hpp"
#include "transform_component.hpp"

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