#include "Building.h"

void Building::createBuildings(int num) {
	//set the vertices, normals and Uvs
	CubePrimitive::loadCube(vertices, normals, UVs);

	//create the number of cubes you want
	for (int i = 0; i < num; i++) {
		GLuint VAO;
		CubePrimitive::bindCube(VAO, vertices, normals, UVs);
		buildings.push_back(VAO);
	}

	//set the min/min for vertices, to be used for bounding box
	Utility::setMinMaxVert(min_x, max_x, min_y, max_y, min_z, max_z, vertices);
	min = glm::vec3(min_x, min_y, min_z);
	max = glm::vec3(max_x, max_y, max_z);
}

void Building::render(vector<BoundingBox*> &b, GLuint transformLoc, GLuint texture_option, GLuint texture_matrix, GLuint scale_UV) {
	float scale = 1.0f;//testing
	float translate = -8.0f;//testing
	
	for (int i = 0; i < buildings.size(); i++) {
		glm::vec3 current_min = min;
		glm::vec3 current_max = max;

		glBindVertexArray(buildings[i]);
		//define transformation (in order of scale then translate)
		glm::mat4 cube;
		cube = glm::translate(cube, glm::vec3(translate, scale, 0.0f));
		cube = glm::scale(cube, glm::vec3(1.0f, scale, 1.0f));
	
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(cube));
		//for UV scaling, in order to have repeated texture and not stretch texture
		glUniformMatrix4fv(texture_matrix, 1, GL_FALSE, glm::value_ptr(cube));

		glUniform1i(texture_option, 2);
		glUniform1i(scale_UV, 1);//true
		glDrawArrays(GL_TRIANGLES, 0, vertices.size());
		glUniform1i(scale_UV, 0);//false
		glBindVertexArray(0);
		
		Utility::applyTransformation(current_min, current_max, cube);
		
		BoundingBox* temp = new BoundingBox(current_min, current_max);
		b.push_back(temp);

		scale += 1.0f;
		translate += 4.0f;
	}
		
}
