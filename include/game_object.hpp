#pragma once

#include "matrix_stack.hpp"
#include "xplor_types.hpp"
#include "shader.hpp"
#include "geometry.hpp"
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
			m_texture_paths.push_back({ imagePath, format });
		}

		void InitTextures()
		{
			for (auto pair : m_texture_paths)
			{
				auto imagePath = std::get<0>(pair);
				auto format = std::get<1>(pair);

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
		}

		// Ideally this should support multiple shaders 
		void AddShader(std::shared_ptr<Shader> shader)
		{
			m_shader = shader;
		}

		void AddGeometry(float* geometryData, size_t dataSize, unsigned int stepSize, uint32_t indexCount)
		{
			m_geometry.SetData(geometryData, dataSize);
			m_geometry.SetStepSize(stepSize);
			m_geometry.SetIndexCount(indexCount);
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
			m_geometry.SetData(geometryData, dataSize);
			m_geometry.SetEBO(ebo, eboSize);
			m_geometry.SetStepSize(stepSize);
		}

		void InitGeometry()
		{
			if (!m_geometry.GetData())
			{
				assert(false && "No geometry data to initialize");
				return;
			}


			glGenVertexArrays(1, &m_VAO); // Generate one VAO
			glGenBuffers(1, &m_VBO); // Generate one buffer object in the OGL Context

			glBindVertexArray(m_VAO); // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
			glBindBuffer(GL_ARRAY_BUFFER, m_VBO); // Bind the buffer object to a buffer type

			// Copy data into the buffer object bound to target. The target here
			// is  GL_ARRAYBUFFER which is bound to the VBO.
			glBufferData(GL_ARRAY_BUFFER, m_geometry.GetSize() * sizeof(float), m_geometry.GetData(), GL_STATIC_DRAW);

			// Check to see if the EBO is populated
			if (m_geometry.GetEBO())
			{
				glGenBuffers(1, &m_EBO); // EBO allows us to use indicies for drawing order

				// Setup EBO
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_geometry.GetEBOSize() * sizeof(unsigned int), m_geometry.GetEBO(), GL_STATIC_DRAW);

			}

			// Tell OpenGL how to interpret the vertex data per attribute
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, m_geometry.GetStep() * sizeof(float), reinterpret_cast<void*>(0));
			glEnableVertexAttribArray(0);
			// define and enable texture coordinates input
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, m_geometry.GetStep() * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
			glEnableVertexAttribArray(1);

			glBindVertexArray(0); // Unbind the VAO
		}


		void Update(const float delta_time)
		{
			glm::vec3 last_position = m_position;
			UpdatePosition(delta_time);

			if (last_position != m_position)
			{
				UpdateBoundingBox();
			}			
		}

		void UpdatePosition(const float delta_time)
		{
			// Transformations
			glm::vec3 update_velocity = delta_time * m_velocity;
			m_position += update_velocity;
		}

		void UpdateBoundingBox()
		{
			glm::vec3 min = m_position - glm::vec3(0.5f) * m_scale;
			glm::vec3 max = m_position + glm::vec3(0.5f) * m_scale;
			m_bbox = { min, max };

			//// old method
			// Currently this is assuming the object is a cube
			/*int size = 1.0f;
			m_bbox.min = m_position - glm::vec3(size / 2.0f);
			m_bbox.max = m_position + glm::vec3(size / 2.0f);*/
		}

		void UpdateModelMatrix()
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, m_position);
			if (m_rotation_amount)
			{
				model = glm::rotate(model, glm::radians(m_rotation_amount), m_rotation_axis);
			}
			m_model_matrix = model;
		}

		void Render(glm::mat4 view_matrix, glm::mat4 projection_matrix)
		{
			m_shader->useProgram();

			// Send coordinate matrices to the shader
			UpdateModelMatrix();
			m_shader->setUniform("model", m_model_matrix); // NEED TO CHANGE THE CODE BELOW 
			m_shader->setUniform("view", view_matrix);
			m_shader->setUniform("projection", projection_matrix);

			// Bind Relevant Textures
			for (int i = 0; i < m_textures.size(); i++)
			{
				glActiveTexture(GL_TEXTURE0 + i);
				glBindTexture(GL_TEXTURE_2D, m_textures[i]);
			}


			glBindVertexArray(m_VAO);
			// Check for an EBO
			if (!m_EBO)
			{
				// change this to draw to a variable size
				glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(m_geometry.GetIndexCount()));
			}
			else
			{
				glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_geometry.GetEBOSize()), GL_UNSIGNED_INT, 0);
			}

			// Unbinds
			glBindVertexArray(0); // Unbind the VAO
			glBindTexture(GL_TEXTURE_2D, 0); // Unbind the texture
			m_shader->endProgram();
		}

		void DrawBoundingBox(const glm::mat4& view_matrix, const glm::mat4& projection_matrix);

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

		/// <summary>
		/// Set the position of the object. Also updates the object's bounding box.
		/// </summary>
		/// <param name="pos">Position to place the object in the world.</param>
		void SetPosition(const glm::vec3& position)
		{
			m_position = position;
			UpdateBoundingBox();
		}

		void SetScale(const glm::vec3& scale)
		{
			m_scale = scale;
			UpdateBoundingBox();
		}

		const BoundingBox& GetBoundingBox() const
		{
			return m_bbox;
		}

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rot">Holds Pitch, Yaw and Roll in degrees to rotate</param>
		void SetRotation(glm::vec3 rotAxis, float rotAmount)
		{
			m_rotation_axis = rotAxis;
			m_rotation_amount = rotAmount;
		}

		const std::vector<uint32_t> GetTextures() const
		{
			return m_textures;
		};

		const std::shared_ptr<Shader> GetShader() const
		{
			return m_shader;
		}

		void SetID(uint32_t id)
		{
			m_id = id;
		}

		const uint32_t GetID()
		{
			return m_id;
		}

		const std::string& GetName()
		{
			return m_name;
		}

		void SetName(std::string name)
		{
			m_name = name;
		}

		void SetVelocity(const glm::vec3& velocity)
		{
			m_velocity = velocity;
		}

		json Serialize() const
		{
			return {
				{ "type", GetObjectType()},
				{ "id", m_id },
				{ "name", m_name },
				{ "position", {m_position.x, m_position.y, m_position.z}},
				{ "geometry", m_geometry.Serialize()},
				{ "shader", m_shader->Serialize()},
				{ "VAO", m_VAO},
				{ "VBO", m_VBO},
				{ "EBO", m_EBO},
				{ "texture paths", m_texture_paths}
			};
		}

		virtual void Deserialze(const json& j)
		{
			m_object_type = j.at("type").get<Xplor::GameObjectType>();
			m_id = j.at("id").get<uint32_t>();
			m_name = j.at("name").get<std::string>();
			auto jPosition = j.at("position").get<std::vector<float>>();
			m_position = glm::vec3(jPosition[0], jPosition[1], jPosition[2]);

			m_geometry.Deserialize(j.at("geometry"));
			InitGeometry();

			m_texture_paths = j.at("texture paths");
			InitTextures();

			m_shader = std::make_shared<Xplor::Shader>();
			m_shader->Deserialize(j.at("shader"));
			m_shader->Init();

			m_VAO = j.at("VAO").get<uint32_t>();
			m_VBO = j.at("VBO").get<uint32_t>();
			m_EBO = j.at("EBO").get<uint32_t>();

		}

		/// <summary>
		/// Get the current game objects enum type. This setup allows for working with 
		/// derived classes
		/// </summary>
		/// <returns>An enum representing the game object type</returns>
		virtual Xplor::GameObjectType GetObjectType() const
		{
			return m_object_type;
		}
		

	protected:
		// Unique identifier for object
		uint32_t m_id{};
		// Optional identifier (makes searching for this object easier)
		std::string m_name{};
		const std::string resources = "..//resources//";
		std::vector<uint32_t> m_textures{};
		std::vector<std::tuple<std::string, ImageFormat>> m_texture_paths;
		std::shared_ptr<Shader> m_shader{};
		uint32_t m_VBO{}, m_VAO{}, m_EBO{};
		
		size_t m_index_count{}; // Number of indices needed to be rendered
		glm::vec3 m_position{};
		glm::vec3 m_velocity{};
		glm::vec3 m_scale{1.0f};

		glm::vec3 m_rotation_axis{};
		float m_rotation_amount{};
		
		Xplor::GameObjectType m_object_type{ Xplor::GameObjectType::GameObject };
		Geometry m_geometry;
		// Axis Alinged Bounding Box for Collisions
		BoundingBox m_bbox;
		// Bounding box VAO and VBO
		GLuint m_bboxVAO = 0, m_bboxVBO = 0, m_bboxEBO = 0;

		// Want a matrix stack instead of all of these
		glm::mat4 m_model_matrix{1.0f};

	private:

	}; // end class


	class PropObject : public GameObject
	{
		Xplor::GameObjectType object_type{ Xplor::GameObjectType::PropObject };

		Xplor::GameObjectType GetObjectType() const override
		{
			return object_type;
		}

	};

}; // end namespace

