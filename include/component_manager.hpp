#pragma once

#include <xplor_types.hpp>
#include <component_array.hpp>
#include <component.hpp>
#include <unordered_map>
#include <memory>

namespace Xplor
{
	class ComponentManager
	{
	public:
		// I want to change this to work with my component class instead
		// probably
		template<typename T>
		void RegisterComponent()
		{
			const char* typeName = typeid(T).name();

			assert((m_mapComponentTypes.find(typeName) == m_mapComponentTypes.end())
				&& "Cannot register a component type more than once");

			// Add the component type to the map
			m_mapComponentTypes.insert({ typeName, m_freeComponentType });

			// Create a component array for the new component type and add it to the map
			m_mapComponentArrays.insert({ typeName, std::make_shared < ComponentArray<T> >() });

			m_freeComponentType++;

		}

		template<typename T>
		ComponentType GetComponentType()
		{
			const char* typeName = typeid(T).name();

			assert((m_mapComponentTypes.find(typeName) != m_mapComponentTypes.end()) &&
				"Component type is not registered");

			// Return the component type to create masks
			return m_mapComponentTypes[typeName];
		}

		template<typename T>
		void AddComponent(EntityID entity, T component)
		{
			// Add a component element to the component array for an entity
			GetComponentArray<T>()->AddData(entity, component);
		}

		template<typename T>
		void RemoveComponent(EntityID entity);

		template <typename T>
		T& GetComponent(EntityID entity);


		void EntityDestroyed(EntityID entity);


	private:
		// Map from type string pointer to a component type
		std::unordered_map<const char*, ComponentType> m_mapComponentTypes{};

		// Map from type string pointer to a component array
		std::unordered_map<const char*, std::shared_ptr<IComponentArray>> m_mapComponentArrays{};

		// Number that is assigned to a component representing its type. Starts at 0.
		ComponentType m_freeComponentType{};

		// Convenience function to get the statically casted pointer to the ComponentArray of type T.
		template<typename T>
		std::shared_ptr<ComponentArray<T>> GetComponentArray()
		{
			const char* typeName = typeid(T).name();

			assert((m_mapComponentTypes.find(typeName) != m_mapComponentTypes.end())
				&& "Component not registered");

			return std::static_pointer_cast<ComponentArray<T>>(m_mapComponentArrays[typeName]);
		}

	}; // end class
}; // end namespace