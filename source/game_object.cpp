#include "game_object.hpp"
#include "generator_geometry.hpp"
#include "shader_manager.hpp"

namespace Xplor 
{
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

	void GameObject::draw(glm::mat4 view_matrix, glm::mat4 projection_matrix)
	{
		m_shader->useProgram();

		// Send coordinate matrices to the shader
		updateModelMatrix();
		m_shader->setUniform("model", m_model_matrix);
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

	void GameObject::drawBoundingBox(const glm::mat4& view_matrix, const glm::mat4& projection_matrix)
	{
		// Set the shader
		std::shared_ptr<Shader> bbox_shader;
		ShaderManager::getInstance()->findShader("bounding", bbox_shader);
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
}

