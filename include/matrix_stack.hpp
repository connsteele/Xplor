#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stack>

namespace Xplor
{
	class MatrixStack
	{
	public:
		

	private:
		std::stack<glm::mat4> m_stack;

	};
};