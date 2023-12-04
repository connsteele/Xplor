#pragma once

#include "matrix_stack.hpp"
#include "xplor_types.hpp"
#include "shader.hpp"
#include <stb_image.h>
#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <memory>

struct ImageData
{
	int width;
	int height;
	int channels;
	unsigned char* data;
};

namespace Xplor {

	// A Game Object should contain also relevant information about where to place an object in the game world
	// and how to render it

	class GameObject
	{
	public:
		void Init();
		
		void AddTexture(std::string imagePath, ImageFormat format)
		{
			ImageData imageBox;
			stbi_set_flip_vertically_on_load(true); // Align the coordinates
			std::string fullPath = resources + imagePath;
			// Fill Variables with image data
			imageBox.data = stbi_load(fullPath.c_str(), &imageBox.width, &imageBox.height, &imageBox.channels, 0);
			if (!imageBox.data)
			{
				std::cout << "Error: Image failed to load" << std::endl;
			}
			//--- Texture generation
			uint32_t texture1;
			// Generate one texture and store it
			glGenTextures(1, &texture1);
			// Bind the texture
			glBindTexture(GL_TEXTURE_2D, texture1);
			// Set the texture filtering and wrapping
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			// Generate the bound texture
			if (format == ImageFormat::jpg)
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageBox.width, imageBox.height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageBox.data);
			else if (format == ImageFormat::png)
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageBox.width, imageBox.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageBox.data);

			
			// Generate mipmaps for the bound texture
			glGenerateMipmap(GL_TEXTURE_2D);
			// Free the image data once the texture has been created
			stbi_image_free(imageBox.data);

			m_textures.push_back(texture1);
		}

		// Ideally this should support multiple shaders 
		void AddShader(std::shared_ptr<Shader> shader)
		{
			m_shader = shader;
		}

		void AddGeometry(float* geometryData, size_t dataSize, unsigned int stepSize, uint32_t indexCount)
		{
			glGenVertexArrays(1, &m_VAO); // Generate one VAO
			glGenBuffers(1, &m_VBO); // Generate one buffer object in the OGL Context

			glBindVertexArray(m_VAO); // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
			glBindBuffer(GL_ARRAY_BUFFER, m_VBO); // Bind the buffer object to a buffer type

			// Copy data into the buffer object bound to target. The target here
			// is  GL_ARRAYBUFFER which is bound to the VBO.
			m_indexCount = indexCount;
			glBufferData(GL_ARRAY_BUFFER, dataSize * sizeof(float), geometryData, GL_STATIC_DRAW);


			// Tell OpenGL how to interpret the vertex data per attribute
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stepSize * sizeof(float), reinterpret_cast<void*>(0));
			glEnableVertexAttribArray(0);
			// define and enable texture coordinates input
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stepSize * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
			glEnableVertexAttribArray(1);
		}

		/// <summary>
		/// Add geometry data overloaded for supporting EBOs
		/// </summary>
		/// <param name="geometryData"></param>
		/// <param name="dataSize"></param>
		/// <param name="ebo"></param>
		/// <param name="eboSize"></param>
		/// <param name="stepSize"></param>
		void AddGeometry(float* geometryData, size_t dataSize, unsigned int* ebo, size_t eboSize, unsigned int stepSize)
		{
			glGenVertexArrays(1, &m_VAO); // Generate one VAO
			glGenBuffers(1, &m_VBO); // Generate one buffer object in the OGL Context'
			glGenBuffers(1, &m_EBO); // EBO allows us to use indicies for drawing order

			glBindVertexArray(m_VAO); // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
			glBindBuffer(GL_ARRAY_BUFFER, m_VBO); // Bind the buffer object to a buffer type

			// Copy data into the buffer object bound to target. The target here
			// is  GL_ARRAYBUFFER which is bound to the VBO.
			glBufferData(GL_ARRAY_BUFFER, dataSize * sizeof(float), geometryData, GL_STATIC_DRAW);

			// Setup EBO
			m_elementCount = eboSize;
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_elementCount * sizeof(unsigned int), ebo, GL_STATIC_DRAW);


			// Tell OpenGL how to interpret the vertex data per attribute
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stepSize * sizeof(float), reinterpret_cast<void*>(0));
			glEnableVertexAttribArray(0);
			// define and enable texture coordinates input
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stepSize * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
			glEnableVertexAttribArray(1);
		}


		void Update(float deltaTime)
		{
			// Transformations
			glm::vec3 updateVelocity = deltaTime * m_velocity;
			m_position += updateVelocity;
		}

		void Render(glm::mat4 viewMatrix, glm::mat4 projectionMatrix)
		{
			m_shader->useProgram();

			// Send coordinate matrices to the shader
			int locModel = glGetUniformLocation(m_shader->getID(), "model");
			glUniformMatrix4fv(locModel, 1, GL_FALSE, glm::value_ptr(modelMatrix));
			int locView = glGetUniformLocation(m_shader->getID(), "view");
			glUniformMatrix4fv(locView, 1, GL_FALSE, glm::value_ptr(viewMatrix));
			int locProjection = glGetUniformLocation(m_shader->getID(), "projection");
			glUniformMatrix4fv(locProjection, 1, GL_FALSE, glm::value_ptr(projectionMatrix));


			// Bind Relevant Textures
			for (int i = 0; i < m_textures.size(); i++)
			{
				glActiveTexture(GL_TEXTURE0 + i);
				glBindTexture(GL_TEXTURE_2D, m_textures[i]);
			}

			// Transform and draw the object
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, m_position);
			if (m_rotationAmount != 0)
			{
				model = glm::rotate(model, glm::radians(m_rotationAmount), m_rotationAxis);

			}
			m_shader->setUniform("model", model);

			glBindVertexArray(m_VAO);
			// Check for an EBO
			if (m_EBO != 0)
			{
				glDrawElements(GL_TRIANGLES, m_elementCount, GL_UNSIGNED_INT, 0);
			}
			else
			{
				// change this to draw to a variable size
				glDrawArrays(GL_TRIANGLES, 0, m_indexCount);

			}

			// Unbinds
			glBindVertexArray(0); // Unbind the VAO
			glBindTexture(GL_TEXTURE_2D, 0); // Unbind the texture
			m_shader->endProgram();
		}

		void Delete()
		{
			if (m_shader)
				m_shader->Delete();

			// Check if these are populated first
			glDeleteVertexArrays(1, &m_VAO);
			glDeleteBuffers(1, &m_VBO);
			glDeleteBuffers(1, &m_EBO);
		}

		void AddImpulse(glm::vec3 impulse)
		{
			m_velocity += impulse;
		}

		void SetPosition(glm::vec3 pos)
		{
			m_position = pos;
		}

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rot">Holds Pitch, Yaw and Roll in degrees to rotate</param>
		void SetRotation(glm::vec3 rotAxis, float rotAmount)
		{
			m_rotationAxis = rotAxis;
			m_rotationAmount = rotAmount;
		}

		std::vector<uint32_t> GetTextures()
		{
			return m_textures;
		};

		std::shared_ptr<Shader> GetShader()
		{
			return m_shader;
		}

		void SetID(uint32_t id)
		{
			m_id = id;
		}

		uint32_t GetID()
		{
			return m_id;
		}

		virtual json Serialize() const
		{
			return {
				{ "type", Xplor::GameObjectType::GameObject},
				{ "id", m_id },
				{ "name", m_name },
				{ "position", {m_position.x, m_position.y, m_position.z}}
			};
		}

		virtual void Deserialze(const json& j)
		{
			m_id = j.at("id").get<uint32_t>();
			m_name = j.at("name").get<std::string>();
			auto jPosition = j.at("position").get<std::vector<float>>();
			m_position = glm::vec3(jPosition[0], jPosition[1], jPosition[2]);
		}
		

	protected:
		// Unique identifier for object
		uint32_t m_id;
		// Optional identifier (makes searching for this object easier)
		std::string m_name;
		const std::string resources = "..//resources//";
		std::vector<uint32_t> m_textures{};
		std::shared_ptr<Shader> m_shader;
		uint32_t m_VBO, m_VAO, m_EBO{};
		// Number of indices needed to be rendered
		size_t m_indexCount{};
		size_t m_elementCount{};
		glm::vec3 m_position;
		glm::vec3 m_rotationAxis;
		float m_rotationAmount{};
		glm::vec3 m_velocity;

		// Want a matrix stack instead of all of these
		glm::mat4 modelMatrix{};

	private:

	}; // end class


	class PropObject : public GameObject
	{
		json Serialize() const override
		{
			return {
				{ "type", Xplor::GameObjectType::PropObject},
				{ "id", m_id },
				{ "name", m_name },
				{ "position", {m_position.x, m_position.y, m_position.z}}
			};
		}
	};

}; // end namespace

