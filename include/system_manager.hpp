#pragma once

#include <xplor_types.hpp>
#include <system.hpp>
#include <memory>
#include <unordered_map>

namespace Xplor
{
	class SystemManager
	{
	public:
		template<typename T>
		std::shared_ptr<T> RegisterSystem()
		{
			const char* systemTypeName = typeid(T).name();

			assert(m_mapSystems.find(systemTypeName) == m_mapSystems.end()
				&& "Cannot register system: System is already registered");

			// Create a pointer to the system
			auto system = std::make_shared<T>();
			m_mapSystems.insert({ systemTypeName, system });
			return system;
		}

		template<typename T>
		void SetMask(EntityMask mask);

		void EntityDeleted(EntityID entity);

		void EntityMaskChange(EntityID entity, EntityMask mask);


	private:
		std::unordered_map<const char*, EntityMask> m_mapMasks{};

		std::unordered_map<const char*, std::shared_ptr<System>> m_mapSystems{};

	};
}; // end namespace