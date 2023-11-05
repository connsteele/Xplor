#pragma once

#include <vector>
#include <entity_manager.hpp>
#include <component_manager.hpp>
#include <system_manager.hpp>
#include <xplor_types.hpp>
#include <memory>

namespace Xplor
{
    class EngineManager
    {
        // The engine manager should manage all other managers and fit all
        // the piece together. In the editor the engine manager is what will
        // be instaniated to use the xplor engine
   
        

    public:
        //EngineManager();

        //~EngineManager();

        /// <summary>
        /// Creates pointers for each manager it manages
        /// </summary>
        void Init();

        EntityID CreateEntity();

        void DeleteEntity(EntityID entity);

        //----- Component Methods
        template<typename T>
        void RegisterComponent()
        {
            m_componentManager->RegisterComponent<T>();
        }

        template<typename T>
        void AddComponent(EntityID entity, T component)
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
        void RemoveComponent(EntityID entity)
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
        T& GetComponent(EntityID entity)
        {
            return m_componentManager->GetComponent<T>(entity);
        }

        template<typename T>
        ComponentType GetComponentType()
        {
            return m_componentManager->GetComponentType<T>();
        }

        //----- System Methods
        template<typename T>
        std::shared_ptr<T> RegisterSystem()
        {
            return m_systemManager->RegisterSystem<T>();
        }

        template<typename T>
        void SetSystemMask(EntityMask mask)
        {
            m_systemManager->SetMask<T>(mask);
        }


    private:
        std::unique_ptr<EntityManager> m_entityManager;
        std::unique_ptr<ComponentManager> m_componentManager;
        std::unique_ptr<SystemManager> m_systemManager;

    }; // end class
}; // end namespace