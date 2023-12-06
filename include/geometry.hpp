#include "xplor_types.hpp"

namespace Xplor
{
	class Geometry
	{
	public:

		json Serialize() const
		{

		}

		void Deserialize()
		{

		}

		const void SetData(float* data, size_t size)
		{
			m_data = data;
			m_dataSize = size;
		}

		const void SetEBO(unsigned int* ebo, size_t size)
		{
			m_ebo = ebo;
			m_eboSize = size;
		}

		const void SetIndexCount(uint32_t count)
		{
			m_indexCount = count;
		}

		const void SetStepSize(unsigned int step)
		{
			m_stepSize = step;
		}

		const float* GetData() const { return m_data; }
		const size_t GetSize() const { return m_dataSize; }
		const uint32_t GetIndexCount() const { return m_indexCount; }
		const unsigned int* GetEBO() const { return m_ebo; }
		const size_t GetEBOSize() const { return m_eboSize; }
		const size_t GetStep() const { return m_stepSize; }



	private:
		float* m_data{};
		size_t m_dataSize{};
		uint32_t m_indexCount{};

		unsigned int* m_ebo{};
		size_t m_eboSize{};
		unsigned int m_stepSize{};


	}; // end class
}; // end namespace