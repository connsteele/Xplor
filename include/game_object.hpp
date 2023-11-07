#pragma once

#include "matrix_stack.hpp"
#include "xplor_types.hpp"
#include "shader.hpp"
#include <stb_image.h>
#include <string>
#include <vector>
#include <array>

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

	//template <typename T>
	class GameObject
	{
	public:

		GameObject()
		{

		}

		GameObject(std::string vertexShaderPath, std::string fragmentShaderPath)
		{
			AddShader(vertexShaderPath, fragmentShaderPath);
		}

		void Init();
		
		void AddTexture(std::string imagePath, ImageFormat format)
		{
			ImageData imageBox;
			stbi_set_flip_vertically_on_load(true); // Align the coordinates
			std::string fullPath = RESOURCES + imagePath;
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

		void AddShader(std::string vertexShaderPath, std::string fragmentShaderPath)
		{
			std::string fullVertexPath = RESOURCES + vertexShaderPath;
			std::string fullFragmentPath = RESOURCES + fragmentShaderPath;

			m_shader = Xplor::Shader::Shader(fullVertexPath.c_str(), fullFragmentPath.c_str());
			m_shader.useProgram();
			// Inform the shader where the texture samplers are located
			m_shader.setUniform("customTexture1", 0);
			m_shader.setUniform("customTexture2", 1);
			// Enable depth testing
			glEnable(GL_DEPTH_TEST);
			m_shader.endProgram();
		}

		void AddGeometry(const float * geometryArray, size_t size )
		{
			float verticesCube[] = {
			   -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
				0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
				0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
				0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			   -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
			   -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

			   -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
				0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
				0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
				0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
			   -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
			   -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

			   -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			   -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			   -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			   -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			   -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			   -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

				0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
				0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
				0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
				0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
				0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
				0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

			   -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
				0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
				0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
				0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			   -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			   -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

			   -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
				0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
				0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
				0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			   -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
			   -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
			};

			glGenVertexArrays(1, &VAO); // Generate one VAO
			glGenBuffers(1, &VBO); // Generate one buffer object in the OGL Context'
			glGenBuffers(1, &EBO); // EBO allows us to use indicies for drawing order


			glBindVertexArray(VAO); // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
			glBindBuffer(GL_ARRAY_BUFFER, VBO); // Bind the buffer object to a buffer type

			// Copy data into the buffer object bound to target. The target here
			// is  GL_ARRAYBUFFER which is bound to the VBO.
			glBufferData(GL_ARRAY_BUFFER, sizeof(verticesCube), verticesCube, GL_STATIC_DRAW);


			// Tell OpenGL how to interpret the vertex data per attribute
			// Here we are accessing the first attribute and checking the 3 vertex points
			// which are 4 bytes (32bits) each so our stides need to be in steps of 4. We want
			// to begin at the start of the array
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast<void*>(0));
			glEnableVertexAttribArray(0);
			// define and enable texture coordinates input
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
			glEnableVertexAttribArray(1);
		}

		void AddGeometrySimple()
		{
			float vertices[] = {
				-0.5f, -0.5f, 0.0f,
				0.5f, -0.5f, 0.0f,
				 0.0f,  0.5f, 0.0f
			};
			glGenVertexArrays(1, &VAO); // Generate one VAO
			glGenBuffers(1, &VBO); // Generate one buffer object in the OGL Context'

			glBindVertexArray(VAO);
			// 2. copy our vertices array in a buffer for OpenGL to use
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
			// 3. then set our vertex attributes pointers
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);

		}

		void Update();

		void Render()
		{
			// 3D Coordinates
			glm::mat4 modelMatrix = glm::mat4(1.0f);
			modelMatrix = glm::rotate(modelMatrix, glm::radians(-45.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			glm::mat4 viewMatrix = glm::mat4(1.0f);
			viewMatrix = glm::translate(viewMatrix, glm::vec3(0.0f, 0.0f, -3.0f)); // move the camera away from the scene
			glm::mat4 projectionMatrix;
			projectionMatrix = glm::perspective(glm::radians(70.0f), 1280.f / 720.f, 0.1f, 100.f); // aspect ratio should be recalced on viewport size change


			// Cubes
			glm::vec3 cubePositions[] = {
			glm::vec3(0.0f,  0.0f,  0.0f),
			glm::vec3(2.0f,  5.0f, -15.0f),
			glm::vec3(-1.5f, -2.2f, -2.5f),
			glm::vec3(-3.8f, -2.0f, -12.3f),
			glm::vec3(2.4f, -0.4f, -3.5f),
			glm::vec3(-1.7f,  3.0f, -7.5f),
			glm::vec3(1.3f, -2.0f, -2.5f),
			glm::vec3(1.5f,  2.0f, -2.5f),
			glm::vec3(1.5f,  0.2f, -1.5f),
			glm::vec3(-1.3f,  1.0f, -1.5f)
			};

			// Check to make sure a shader is present before proceeding

			m_shader.useProgram();
			glClear(GL_DEPTH_BUFFER_BIT);
			glBindVertexArray(VAO);


			// Send coordinate matrices to the shader
			int locModel = glGetUniformLocation(m_shader.getID(), "model");
			glUniformMatrix4fv(locModel, 1, GL_FALSE, glm::value_ptr(modelMatrix));
			int locView = glGetUniformLocation(m_shader.getID(), "view");
			glUniformMatrix4fv(locView, 1, GL_FALSE, glm::value_ptr(viewMatrix));
			int locProjection = glGetUniformLocation(m_shader.getID(), "projection");
			glUniformMatrix4fv(locProjection, 1, GL_FALSE, glm::value_ptr(projectionMatrix));


			// Bind Relevant Textures
			for (int i = 0; i < m_textures.size(); i++)
			{
				glActiveTexture(GL_TEXTURE0 + i);
				glBindTexture(GL_TEXTURE_2D, m_textures[i]);
			}

			// Draw x cubes
			glBindVertexArray(VAO);
			for (unsigned int i = 0; i < 1; i++)
			{
				glm::mat4 model = glm::mat4(1.0f);
				model = glm::translate(model, cubePositions[i]);
				float angle = 20.0f * i;
				if (i % 3 == 0)  // every 3rd iteration (including the first) we set the angle using GLFW's time function.
					angle = glfwGetTime() * 45.0f;
				model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
				//m_shader.setMat4("model", model);
				//glUniformMatrix4fv(glGetUniformLocation(m_shader.getID(), "model"), 1, GL_FALSE, glm::value_ptr(model));
				m_shader.setUniform("model", model);

				glDrawArrays(GL_TRIANGLES, 0, 36);
			}

			// Draw one cube
			//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // Draw based on indicies
			//glDrawArrays(GL_TRIANGLES, 0, 36);

			// Unbinds
			glBindVertexArray(0); // Unbind the VAO
			glBindTexture(GL_TEXTURE_2D, 0); // Unbind the texture
			m_shader.endProgram();
		}

		void RenderSimple()
		{
			m_shader.useProgram();
			glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLES, 0, 3);
			m_shader.endProgram();
		}

		void Delete()
		{
			/*glDeleteVertexArrays(1, &VAO);
			glDeleteBuffers(1, &VBO);
			glDeleteBuffers(1, &EBO);*/
		}

		const std::vector<uint32_t> GetTextures()
		{
			return m_textures;
		}

		const Shader GetShader()
		{
			return m_shader;
		}
		

	protected:
		const std::string RESOURCES = "..//resources//";
		std::vector<uint32_t> m_textures{};
		// There may be multiple shaders per game object which we can deal with later
		Shader m_shader;
		uint32_t VBO, VAO, EBO;


	private:

	}; // end class


	class PropObject : public GameObject
	{
	public:

	private:
	};

}; // end namespace

