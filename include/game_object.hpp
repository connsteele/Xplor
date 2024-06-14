#pragma once

#include "matrix_stack.hpp"
#include "xplor_types.hpp"
#include "shader.hpp"
#include "geometry.hpp"
#include "GLFW/glfw3.h"
#include "shader_manager.hpp"
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
		
		void addTexture(std::string imagePath, ImageFormat format);

		void initTextures();

		// Ideally this should support multiple shaders 
		void addShader(std::shared_ptr<Shader> shader)
		{
			m_shader = shader;
		}

		void addGeometry(float* geometryData, size_t dataSize, unsigned int stepSize, uint32_t indexCount);

		/// <summary>
		/// Add geometry data overloaded for supporting EBOs
		/// </summary>
		/// <param name="geometryData"></param>
		/// <param name="dataSize"></param>
		/// <param name="ebo"></param>
		/// <param name="eboSize"></param>
		/// <param name="stepSize"></param>
		void addGeometry(float* geometryData, size_t dataSize, unsigned int* ebo, size_t eboSize, unsigned int stepSize);

		void initGeometry();

		void initBoundingBoxDraw();

		void update(const float delta_time);

		void updatePosition(const float delta_time);

		void updateBoundingBox();

		void updateModelMatrix();

		void draw(glm::mat4 view_matrix, glm::mat4 projection_matrix, const std::string & name = "");

		void drawBoundingBox(const glm::mat4& view_matrix, const glm::mat4& projection_matrix);

		void Delete();

		void addImpulse(glm::vec3 impulse);

		/// <summary>
		/// Set the position of the object. Also updates the object's bounding box.
		/// </summary>
		/// <param name="pos">Position to place the object in the world.</param>
		void setPosition(const glm::vec3& position);

		void setScale(const glm::vec3& scale);

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

		const glm::vec3 getPosition()
		{
			return m_position;
		}
		
		const glm::vec3 getVelocity()
		{
			return m_velocity;
		}

		const glm::mat4& getModelMatrix()
		{
			return m_model_matrix;
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

	private:
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
		glm::vec3 m_scale{ 1.0f };

		glm::vec3 m_rotation_axis{};
		float m_rotation_amount{};

		Xplor::GameObjectType m_object_type{ Xplor::GameObjectType::GameObject };
		Geometry m_geometry;
		// Axis Alinged Bounding Box for Collisions
		BoundingBox m_bbox;
		// Bounding box VAO and VBO
		GLuint m_bboxVAO = 0, m_bboxVBO = 0, m_bboxEBO = 0;

		// Want a matrix stack instead of all of these
		glm::mat4 m_model_matrix{ 1.0f };

	}; // end class


	class PropObject : public GameObject
	{
		Xplor::GameObjectType object_type{ Xplor::GameObjectType::PropObject };

		Xplor::GameObjectType getObjectType() const override
		{
			return object_type;
		}

	};


	/// <summary>
	/// Transformation Gizmo for selected object manipulation
	/// </summary>
	struct Gizmo
	{
		enum mode { NONE, TRANSLATE, ROTATE, SCALE };
		mode current_mode{ NONE };
		glm::vec3 axis; // axis being manipulated
		GLuint VAO;
		unsigned int index_count;
		std::shared_ptr<Shader> shader;

		bool is_hovered{ false };
		bool is_active{ false };

		void handleInput(GLFWwindow* window, const std::shared_ptr<GameObject> object, glm::mat4 view_matrix, glm::mat4 projection_matrix);

		/// <summary>
		/// Draw the Gizmo ontop of the game object
		/// </summary>
		/// <param name="object">Selected game object to draw the gizmo on</param>
		/// <param name="view_matrix"></param>
		/// <param name="projection_matrix"></param>
		void draw(const std::shared_ptr<GameObject> object, glm::mat4 view_matrix, glm::mat4 projection_matrix)
		{
			glm::mat4 mat_model = object->getModelMatrix();

			drawArrow(glm::vec3(1, 0, 0), mat_model, view_matrix, projection_matrix); // X
			//drawArrow(glm::vec3(0, 1, 0), mat_model, view_matrix, projection_matrix); // Y
			//drawArrow(glm::vec3(0, 0, 1), mat_model, view_matrix, projection_matrix); // Z
		}


		/// <summary>
		/// Draw an arrow for the gizmo in the axis described by the parameters
		/// </summary>
		/// <param name="axis">Axis which to draw the arrow along</param>
		/// <param name="model_matrix"></param>
		/// <param name="view_matrix"></param>
		/// <param name="projection_matrix"></param>
		void drawArrow(glm::vec3 axis, glm::mat4 model_matrix, glm::mat4 view_matrix, glm::mat4 projection_matrix)
		{
			// pick a shader (refactor this to be included with the gizmo so its faster)
			shader->useProgram();
			shader->setUniform("model", model_matrix);
			shader->setUniform("view", view_matrix);
			shader->setUniform("projection", projection_matrix);

			// Draw an simple arrow along an axis
			glBindVertexArray(VAO);
			//glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(index_count), GL_UNSIGNED_INT, 0);
			GLsizei cube_indices = 36;
			glDrawArrays(GL_TRIANGLES, 0, cube_indices);
			glBindVertexArray(0);

			shader->endProgram();
		}

		void initCylinderVAO(const std::vector<float>& verts, const std::vector<unsigned int>& indices)
		{
			GLuint VAO, VBO, EBO;
			glGenVertexArrays(1, &VAO); // bind the VAO
			glGenBuffers(1, &VBO);
			glGenBuffers(1, &EBO);

			glBindVertexArray(VAO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

			glBufferData(VBO, verts.size() * sizeof(float), verts.data(), GL_STATIC_DRAW);
			glBufferData(EBO, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

			// position
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6, (void*)0);
			glEnableVertexAttribArray(0);

			// normals
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6, (void*)(3 * sizeof(float)));
			glEnableVertexAttribArray(1);


			glBindVertexArray(0); // unbind VAO
			this->VAO = VAO;
			index_count = indices.size();
		}

		void initCylinderVAO(std::array<float, 108> cube_data)
		{
			GLuint VBO;

			glGenVertexArrays(1, &VAO);
			glGenBuffers(1, &VBO);

			glBindVertexArray(VAO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(cube_data), &cube_data, GL_STATIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);
			
			glBindVertexArray(0); // unbind VAO
		}

		void initGizmoShaders()
		{
			auto shader_manager = ShaderManager::getInstance();
			std::string shader_name = "gizmo";
			const std::string resources = "..//resources";
			std::string vertex_path = resources + "//shaders//gizmo.vs";
			std::string frag_path = resources + "//shaders//gizmo.fs";
			ShaderInfo shader_info = { shader_name, vertex_path, frag_path };

			try
			{
				this->shader = shader_manager->createShader(shader_info);
			}
			catch (const std::runtime_error& err)
			{
				__debugbreak(); // Windows only
				std::cerr << "Failed to create shader '" + shader_name + "'";
			}
		}
	};

}; // end namespace

