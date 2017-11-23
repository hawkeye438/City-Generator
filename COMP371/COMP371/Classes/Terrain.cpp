#include "Terrain.h"

Terrain::Terrain() {}
Terrain::~Terrain() {}

void Terrain::loadTerrain(int width, int height) {
	mesh_width = width;
	mesh_height = height;
	vector<glm::vec2> UVs;
	std::vector<glm::vec3> meshPositions;
	//UV is based on number of vertices
	//example 4 vertices means 4 UVs
	//base on UV mapping, it bottom left, bottom right, top left, top right
	//example 2x2 grid
	//vertex (0,0,0) UV (0.0f, 0.0f);
	//vertex (1,0,0) UV (1.0f, 0.0f);
	//vertex (0,0,1) UV (0.0f, 1.0f);
	//vertex (1,0,1) UV (1.0f, 1.0f);

	for (int i = 0; i < height; i++) {//i is our z value
		for (int j = 0; j < width; j++) {//j is our x value from image
			meshPositions.push_back(glm::vec3((GLfloat)j, 0.0f, (GLfloat)i));
			UVs.push_back(glm::vec2(j % 2, i % 2));
		}
	}

	//Algorithm to calculate elements indices based on MxM (working well for triangles)
	std::vector<unsigned int> indices_space;

	for (int i = 0; i < height - 1; i++) {//i is our z value
		//draw triangle from top-left, bottom-left, bottom-right
		for (int j = 0; j < width - 1; j++) {//j is our x value
			indices_space.push_back(j + (i*width));//top-left
			indices_space.push_back(j + (i*width) + width);//bottom-left
			indices_space.push_back(j + (i*width) + width + 1);//bottom-right
		}
		//draw triangle from top-left, bottom-right, top-right
		for (int j = 0; j < width - 1; j++) {
			indices_space.push_back(j + (i*width));//top-left
			indices_space.push_back(j + (i*width) + width + 1);//bottom-right
			indices_space.push_back(j + (i*width) + 1);//top-right
		}
	}
	
	glGenVertexArrays(1, &mesh_VAO);
	glGenBuffers(1, &mesh_VBO);
	glGenBuffers(1, &mesh_EBO);
	glGenBuffers(1, &mesh_UV);

	glBindVertexArray(mesh_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, mesh_VBO);
	glBufferData(GL_ARRAY_BUFFER, meshPositions.size() * sizeof(glm::vec3), meshPositions.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_space.size() * sizeof(unsigned int), indices_space.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, mesh_UV);
	glBufferData(GL_ARRAY_BUFFER, UVs.size() * sizeof(glm::vec2), &UVs.front(), GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
	
	number_of_indices = indices_space.size();

}

void Terrain::render(GLuint transform_loc, float scale, float y_offset) {
	glm::mat4 tran;
	tran = glm::scale(tran, glm::vec3(scale, 1.0f, scale));
	tran = glm::translate(tran, glm::vec3(-0.5 * (mesh_width -1), 0.0f+ y_offset, -0.5 * (mesh_height - 1)));//center the grid by origin for any 
	glUniformMatrix4fv(transform_loc, 1, GL_FALSE, glm::value_ptr(tran));
	
	glBindVertexArray(mesh_VAO);
	glDrawElements(GL_TRIANGLES, number_of_indices, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

}