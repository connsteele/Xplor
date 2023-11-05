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
		void RegisterComponent();

		template<typename T>
		Component GetComponentType();

		template<typename T>
		void AddComponent(EntityID entity, T component);

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
		std::shared_ptr<ComponentArray<T>> GetComponentArray();

	}; // end class
}; // end namespace