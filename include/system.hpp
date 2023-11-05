#pragma once

#include <xplor_types.hpp>
#include <set>

namespace Xplor
{
	class System
	{
	public:
		// In the current implementation every system needs a list or set of entites with certain
		// masks
		std::set<EntityID> m_entities;


	}; // end class
}; // end namespace