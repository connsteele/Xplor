#include "game_object.hpp"
#include "generator_geometry.hpp"
#include "shader_manager.hpp"

namespace Xplor 
{
	//----------------------------------------------- Gizmo -----------------------------------------------


	//----------------------------------------------- Game Object -----------------------------------------------

	void GameObject::addTexture(std::string imagePath, ImageFormat format)
	{
		m_texture_paths.push_back({ imagePath, format });
	}

	void GameObject::initTextures()
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

	void GameObject::addGeometry(float* geometryData, size_t dataSize, unsigned int stepSize, uint32_t indexCount)
	{
		m_geometry.SetData(geometryData, dataSize);
		m_geometry.SetStepSize(stepSize);
		m_geometry.SetIndexCount(indexCount);
	}

	void GameObject::addGeometry(float* geometryData, size_t dataSize, unsigned int* ebo, size_t eboSize, unsigned int stepSize)
	{
		m_geometry.SetData(geometryData, dataSize);
		m_geometry.SetEBO(ebo, eboSize);
		m_geometry.SetStepSize(stepSize);
	}

	void GameObject::initGeometry()
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

		initBoundingBoxDraw();
	}

	void GameObject::initBoundingBoxDraw()
	{
		if (m_bboxVAO == 0)
		{
			// Get bounding box vertices and indices
			updateBoundingBox();
			auto vertices = GeometryGenerator::GenerateBoundingBoxVertices(m_bbox.min, m_bbox.max);

			// Generate relevant buffers
			glGenVertexArrays(1, &m_bboxVAO);
			glGenBuffers(1, &m_bboxVBO);

			glBindVertexArray(m_bboxVAO); // Bind the VAO

			// Bind VBO and fill the buffer
			glBindBuffer(GL_ARRAY_BUFFER, m_bboxVBO);
			glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

			// Vertex Positions
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), reinterpret_cast<GLvoid*>(0));
			glEnableVertexAttribArray(0); // enable layout 0

			glBindVertexArray(0); // unbind VAO
		}
	}

	void GameObject::update(const float delta_time)
	{
		glm::vec3 last_position = m_position;

		// Should stack all transformations then just do one application of the model matrix update at the end
		updatePosition(delta_time);

		if (last_position != m_position) // If object moved at all
		{
			updateModelMatrix();
			updateBoundingBox();
		}
	}

	void GameObject::updatePosition(const float delta_time)
	{
		// Transformations
		glm::vec3 update_velocity = delta_time * m_velocity;
		m_position += update_velocity;
	}

	void GameObject::updateBoundingBox()
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

	void GameObject::updateModelMatrix()
	{
		// NEED TO UPDATE TO TAKE SCALE AND ROT INTO ACCOUNT

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, m_position);
		if (m_rotation_amount)
		{
			model = glm::rotate(model, glm::radians(m_rotation_amount), m_rotation_axis);
		}
		m_model_matrix = model;
	}

	void GameObject::draw(glm::mat4 view_matrix, glm::mat4 projection_matrix, const std::string & shader_name)
	{
		std::shared_ptr<Shader> shader;

		if (shader_name.empty())
		{
			shader = m_shader;
		}
		else
		{
			if (!ShaderManager::getInstance()->findShader(shader_name, shader))
			{
				std::cerr << "Error shader with name: '" << shader_name << "' cannot be found." << std::endl;
				return;
			}
		}

		shader->useProgram();

		// Send coordinate matrices to the shader
		shader->setUniform("model", m_model_matrix); // model is updated in the update call prior to draw
		shader->setUniform("view", view_matrix);
		shader->setUniform("projection", projection_matrix);

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
		shader->endProgram();
	}

	void GameObject::drawBoundingBox(const glm::mat4& view_matrix, const glm::mat4& projection_matrix)
	{
		// Set the shader
		std::string shader_name = "bounding";
		std::shared_ptr<Shader> bbox_shader;
		if (!ShaderManager::getInstance()->findShader(shader_name, bbox_shader))
		{
			std::cerr << "Error shader with name: '" << shader_name << "' cannot be found." << std::endl;
			return;
		}
		
		bbox_shader->useProgram();

		// Set the view and projection matrices
		bbox_shader->setUniform("view", view_matrix);
		bbox_shader->setUniform("projection", projection_matrix);


		// No need to translate as the bounding box already takes the objects position into account
		glm::mat4 model = glm::mat4(1.0f);
		//model = glm::translate(model, m_position);
		// Scale too?
		/*if (m_rotation_amount)
		{
			model = glm::rotate(model, glm::radians(m_rotation_amount), m_rotation_axis);
		}*/

		// If the model moves update the geometry of the box to follow precisely
		if (m_last_position != m_position)
		{
			m_last_position = m_position;
			updateBoundingBox();
			auto vertices = GeometryGenerator::GenerateBoundingBoxVertices(m_bbox.min, m_bbox.max);
			glBindBuffer(GL_ARRAY_BUFFER, m_bboxVBO);
			glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
		}

		
		bbox_shader->setUniform("model", model);



		// Draw the bounding box
		glBindVertexArray(m_bboxVAO);
		GLsizei bounding_vertex_count = 36;
		//glDrawElements(GL_LINES, bounding_index_count, GL_UNSIGNED_INT, 0);
		glLineWidth(10.0f);
		glDrawArrays(GL_LINE_STRIP, 0, static_cast<GLsizei>(bounding_vertex_count));
		

		glBindVertexArray(0); // unbind VAO
		bbox_shader->endProgram();
		glLineWidth(1.0f); // restore line width
	}

	void GameObject::Delete()
	{
		if (m_shader)
			m_shader->Delete();

		// Check if these are populated first
		glDeleteVertexArrays(1, &m_VAO);
		glDeleteBuffers(1, &m_VBO);
		glDeleteBuffers(1, &m_EBO);
	}

	void GameObject::addImpulse(glm::vec3 impulse)
	{
		m_velocity += impulse;
	}

	void GameObject::setPosition(const glm::vec3& position)
	{
		m_position = position;
		updateModelMatrix();
		updateBoundingBox();
	}

	void GameObject::setScale(const glm::vec3& scale)
	{
		m_scale = scale;
		updateModelMatrix(); // this function doesn't take scale or rot into account currently
		updateBoundingBox();
	}
}

