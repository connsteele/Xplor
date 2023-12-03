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

		void AddGeometry(float* geometryData, size_t dataSize, unsigned int stepSize)
		{
			glGenVertexArrays(1, &m_VAO); // Generate one VAO
			glGenBuffers(1, &m_VBO); // Generate one buffer object in the OGL Context'
			glGenBuffers(1, &m_EBO); // EBO allows us to use indicies for drawing order


			glBindVertexArray(m_VAO); // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
			glBindBuffer(GL_ARRAY_BUFFER, m_VBO); // Bind the buffer object to a buffer type

			// Copy data into the buffer object bound to target. The target here
			// is  GL_ARRAYBUFFER which is bound to the VBO.
			glBufferData(GL_ARRAY_BUFFER, dataSize * sizeof(float), geometryData, GL_STATIC_DRAW);


			// Tell OpenGL how to interpret the vertex data per attribute
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stepSize * sizeof(float), reinterpret_cast<void*>(0));
			glEnableVertexAttribArray(0);
			// define and enable texture coordinates input
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stepSize * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
			glEnableVertexAttribArray(1);
		}

		void InitGeom()
		{
			// Transformations
			uint32_t liveTransformLoc = glGetUniformLocation(m_shader->getID(), "liveTransform");

			// 3D Coordinates
			modelMatrix = glm::mat4(1.0f);
			modelMatrix = glm::rotate(modelMatrix, glm::radians(-45.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		}

		void Update();

		void Render(glm::mat4 viewMatrix, glm::mat4 projectionMatrix)
		{
			m_shader->useProgram();
			glClear(GL_DEPTH_BUFFER_BIT);
			glBindVertexArray(m_VAO);

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

			// Draw 10 cubes
			//glBindVertexArray(m_VAO);
			//for (unsigned int i = 0; i < 10; i++)
			//{
			//	glm::mat4 model = glm::mat4(1.0f);
			//	model = glm::translate(model, cubePositions[i]);
			//	float angle = 20.0f * i;
			//	if (i % 3 == 0)  // every 3rd iteration (including the first) we set the angle using GLFW's time function.
			//		angle = glfwGetTime() * 45.0f;
			//	model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			//	//m_shader->setMat4("model", model);
			//	//glUniformMatrix4fv(glGetUniformLocation(m_shader->getID(), "model"), 1, GL_FALSE, glm::value_ptr(model));
			//	m_shader->setUniform("model", model);

			//	glDrawArrays(GL_TRIANGLES, 0, 36);
			//}

			// Transform and draw the object
			glBindVertexArray(m_VAO);
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, m_position);
			// rotate
			m_shader->setUniform("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 36);

			


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

		void SetPosition(glm::vec3 pos)
		{
			m_position = pos;
		};

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
		

	protected:
		// Unique identifier for object
		uint32_t m_id;
		const std::string resources = "..//resources//";
		std::vector<uint32_t> m_textures{};
		std::shared_ptr<Shader> m_shader;
		uint32_t m_VBO, m_VAO, m_EBO;
		glm::vec3 m_position;

		// Want a matrix stack instead of all of these
		glm::mat4 modelMatrix{};

	private:

	}; // end class


	class PropObject : public GameObject
	{

	};

}; // end namespace

