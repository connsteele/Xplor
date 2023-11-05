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
        void AddComponent(EntityID entity, T component);

        template<typename T>
        void RemoveComponent(EntityID entity);

        template<typename T>
        T& GetComponent(EntityID entity);

        template<typename T>
        ComponentType GetComponentType();

        //----- System Methods
        template<typename T>
        std::shared_ptr<T> RegisterSystem();

        template<typename T>
        void SetSystemMask(EntityMask mask);


    private:
        std::unique_ptr<EntityManager> m_entityManager;
        std::unique_ptr<ComponentManager> m_componentManager;
        std::unique_ptr<SystemManager> m_systemManager;

    }; // end class
}; // end namespace