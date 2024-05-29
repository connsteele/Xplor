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
		void init();
		
		void addTexture(std::string imagePath, ImageFormat format)
		{
			m_texture_paths.push_back({ imagePath, format });
		}

		void initTextures()
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
		void addShader(std::shared_ptr<Shader> shader)
		{
			m_shader = shader;
		}

		void addGeometry(float* geometryData, size_t dataSize, unsigned int stepSize, uint32_t indexCount)
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
		void addGeometry(float* geometryData, size_t dataSize, unsigned int* ebo, size_t eboSize, unsigned int stepSize)
		{
			m_geometry.SetData(geometryData, dataSize);
			m_geometry.SetEBO(ebo, eboSize);
			m_geometry.SetStepSize(stepSize);
		}

		void initGeometry();

		void initBoundingBoxDraw();

		void update(const float delta_time)
		{
			glm::vec3 last_position = m_position;
			updatePosition(delta_time);

			if (last_position != m_position)
			{
				updateBoundingBox();
			}			
		}

		void updatePosition(const float delta_time)
		{
			// Transformations
			glm::vec3 update_velocity = delta_time * m_velocity;
			m_position += update_velocity;
		}

		void updateBoundingBox()
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

		void updateModelMatrix()
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, m_position);
			if (m_rotation_amount)
			{
				model = glm::rotate(model, glm::radians(m_rotation_amount), m_rotation_axis);
			}
			m_model_matrix = model;
		}

		void draw(glm::mat4 view_matrix, glm::mat4 projection_matrix);

		void drawBoundingBox(const glm::mat4& view_matrix, const glm::mat4& projection_matrix);

		void Delete()
		{
			if (m_shader)
				m_shader->Delete();

			// Check if these are populated first
			glDeleteVertexArrays(1, &m_VAO);
			glDeleteBuffers(1, &m_VBO);
			glDeleteBuffers(1, &m_EBO);
		}

		void addImpulse(glm::vec3 impulse)
		{
			m_velocity += impulse;
		}

		/// <summary>
		/// Set the position of the object. Also updates the object's bounding box.
		/// </summary>
		/// <param name="pos">Position to place the object in the world.</param>
		void setPosition(const glm::vec3& position)
		{
			m_position = position;
			updateBoundingBox();
		}

		void setScale(const glm::vec3& scale)
		{
			m_scale = scale;
			updateBoundingBox();
		}

		const BoundingBox& getBoundingBox() const
		{
			return m_bbox;
		}

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rot">Holds Pitch, Yaw and Roll in degrees to rotate</param>
		void setRotation(glm::vec3 rotAxis, float rotAmount)
		{
			m_rotation_axis = rotAxis;
			m_rotation_amount = rotAmount;
		}

		const std::vector<uint32_t> getTextures() const
		{
			return m_textures;
		};

		const std::shared_ptr<Shader> getShader() const
		{
			return m_shader;
		}

		void setID(uint32_t id)
		{
			m_id = id;
		}

		const uint32_t getID()
		{
			return m_id;
		}

		const std::string& getName()
		{
			return m_name;
		}

		void setName(std::string name)
		{
			m_name = name;
		}

		void setVelocity(const glm::vec3& velocity)
		{
			m_velocity = velocity;
		}

		json Serialize() const
		{
			return {
				{ "type", getObjectType()},
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
			initGeometry();

			m_texture_paths = j.at("texture paths");
			initTextures();

			m_shader = std::make_shared<Xplor::Shader>();
			m_shader->Deserialize(j.at("shader"));
			m_shader->init();

			m_VAO = j.at("VAO").get<uint32_t>();
			m_VBO = j.at("VBO").get<uint32_t>();
			m_EBO = j.at("EBO").get<uint32_t>();

		}

		/// <summary>
		/// Get the current game objects enum type. This setup allows for working with 
		/// derived classes
		/// </summary>
		/// <returns>An enum representing the game object type</returns>
		virtual Xplor::GameObjectType getObjectType() const
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
		glm::vec3 m_last_position{}; // Used with bounding box drawing
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

		Xplor::GameObjectType getObjectType() const override
		{
			return object_type;
		}

	};

}; // end namespace

