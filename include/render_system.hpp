#pragma once

#include "system.hpp"

namespace Xplor {
	class RenderSystem : public System
	{
	// Want to move aspects of rendering from main to this system

	public:
		void Init();

		void Update();

	private:


	}; // end class
}; // end namepsace
