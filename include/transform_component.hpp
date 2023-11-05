
#include "component.hpp"
#include "glm/glm.hpp"

namespace Xplor
{
	class TransformComponent : public Xplor::Component
	{
	private:
		

	public:
		glm::mat4 m_Transform;
	};
};