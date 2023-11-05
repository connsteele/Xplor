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
		std::shared_ptr<T> RegisterSystem();

		template<typename T>
		void SetMask(EntityMask mask);

		void EntityDeleted(EntityID entity);

		void EntityMaskChange(EntityID entity, EntityMask mask);


	private:
		std::unordered_map<const char*, EntityMask> m_mapMasks{};

		std::unordered_map<const char*, std::shared_ptr<System>> m_mapSystems{};

	};
}; // end namespace