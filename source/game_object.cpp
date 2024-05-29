#include "game_object.hpp"
#include "generator_geometry.hpp"


void Xplor::GameObject::DrawBoundingBox(const glm::mat4& view_matrix, const glm::mat4& projection_matrix)
{
	// If the VAO has not been created
	if (m_bboxVAO == 0)
	{
		// Generate and bind VAO
		glGenBuffers(1, &m_bboxVAO);
		glBindVertexArray(m_bboxVAO);

		// Generate VBO and EBO
		glGenBuffers(1, &m_bboxVBO);
		glGenBuffers(1, &m_bboxEBO);

		// Get bounding box vertices and indices
		std::vector<float> vertices = GeometryGenerator::GenerateBoundingBoxVertices(m_bbox.min, m_bbox.max);
		std::vector<unsigned int> indices = GeometryGenerator::GenerateBoundingBoxIndices();

		// Bind VBO
		glBindBuffer(GL_ARRAY_BUFFER, m_bboxVBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

		// Bind EBO
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_bboxEBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

		// Vertex Positions
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glBindVertexArray(0); // unbind VAO
	}

	// Set the shader
	std::string vertex_full_path = resources + "shaders//bounding.vs";
	std::string fragment_full_path = resources + "shaders//bounding_color.fs";

	std::unique_ptr<Xplor::Shader> bbox_shader = std::make_unique<Xplor::Shader>(vertex_full_path.c_str(), fragment_full_path.c_str());
	bbox_shader->useProgram();

	// Set the view and projection matrices
	bbox_shader->setUniform("view", view_matrix);
	bbox_shader->setUniform("projection", projection_matrix);


	// No need to transalte as the bounding box already takes the objects position into account
	glm::mat4 model = glm::mat4(1.0f);
	// model = glm::translate(model, m_position);
	// Scale too?
	/*if (m_rotation_amount)
	{
		model = glm::rotate(model, glm::radians(m_rotation_amount), m_rotation_axis);
	}*/
	bbox_shader->setUniform("model", model);


	// Enable line drawing mode
	/*glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glEnable(GL_DEPTH_TEST);*/

	// Draw the bounding box
	glBindVertexArray(m_bboxVAO);
	GLsizei bounding_index_count = 24;
	//glDrawElements(GL_LINES, bounding_index_count, GL_UNSIGNED_INT, 0);
	glDrawElements(GL_TRIANGLES, bounding_index_count, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0); // unbind VAO

	bbox_shader->endProgram();

	// Restore polygon mode to fill (default)
	/*glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);*/
}
