#include <array>
#include "xplor_types.hpp"

namespace Xplor
{
	class Geometry
	{
	public:

		json Serialize() const
		{
			return {
				{"vertices", std::vector<float>(m_data, m_data + m_dataSize)},
				{"step size", m_stepSize},
				{"index count", m_indexCount},
				{"elements", std::vector<float>(m_ebo, m_ebo + m_eboSize)}
			};
		}

		void Deserialize(const json& j)
		{
			auto vertices = j.at("vertices").get<std::vector<float>>();
			m_dataSize = vertices.size();
			m_data = new float[m_dataSize];
			std::copy(vertices.begin(), vertices.end(), m_data);
			m_stepSize = j.at("step size").get<unsigned int>();
			m_indexCount = j.at("index count").get<uint32_t>();

			auto elements = j.at("elements").get<std::vector<unsigned int>>();
			m_eboSize = elements.size();
			m_ebo = new unsigned int[m_eboSize];
			std::copy(elements.begin(), elements.end(), m_ebo);
		}

		const void SetData(float* data, size_t size)
		{
			m_data = new float[size];
			std::copy(data, data + size, m_data);
			m_dataSize = size;
		}

		const void SetEBO(unsigned int* ebo, size_t size)
		{
			m_ebo = new unsigned int[size];
			std::copy(ebo, ebo + size, m_ebo);
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

		~Geometry()
		{
			delete[] m_data;
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