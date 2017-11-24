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

void Building::render(vector<BoundingBox*> &b, GLuint transformLoc, GLuint texture_option, GLuint texture_matrix, GLuint scale_UV, int city_dim, float city_scale) {
	float scale = 1.0f;//testing
	//float x_translate = -36.5f;//testing
	//float z_translate = 36.5f;//testing
	float x_translate = ((-0.5f * (city_dim - 1.0f))*city_scale) + 1.0f;
	float z_translate = ((0.5f * (city_dim - 1.0f))*city_scale) - 1.0f;
	int num_of_building = (int)(((0.5 * (city_dim - 1) * city_scale) - 1) / 2) - 2;
	int x = 0;
	int z = 0;
	for (int i = 0; i < buildings.size(); i++) {
		glm::vec3 current_min = min;
		glm::vec3 current_max = max;

		glBindVertexArray(buildings[i]);
		//define transformation (in order of scale then translate)
		glm::mat4 cube;
		cube = glm::translate(cube, glm::vec3(x_translate, scale, z_translate));
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

		//scale += 1.0f;
		if (x < (city_dim - 2)) {
			x_translate += city_dim;
			x++;
		}
		else if (x == (city_dim - 2)) {
			x_translate += 2*city_dim;
			x++;
		}
		else if (x < (city_dim + 3)) {
			x_translate += city_dim;
			x++;
		}
		else if (x == (city_dim + 3)) {
			x_translate += 2*city_dim;
			x++;
		}
		else if (x < (city_dim + 8)) {
			x_translate += city_dim;
			x++;
		}
		else if (x == (city_dim + 8)) {
			x_translate += 2* city_dim;
			x++;
		}
		else {
			x_translate += city_dim;
			x++;
		}
		if (x % num_of_building == 0) {
			x = 0;
			if (z < (city_dim - 2)) {
				z_translate -= city_dim;
			}
			else if (z == (city_dim - 2)) {
				z_translate -= 2* city_dim;
			}
			else if (z < (city_dim  + 3)) {
				z_translate -= city_dim;
			}
			else if (z == (city_dim + 3)) {
				z_translate -= 2*city_dim;
			}
			else if (z < (city_dim + 8)) {
				z_translate -= city_dim;
			}
			else if (z == (city_dim + 8)) {
				z_translate -= 2*city_dim;
			}
			else {
				z_translate -= city_dim;
			}
			z++;
			x_translate = ((-0.5f * (city_dim - 1.0f))*city_scale) + 1.0f;
		}
	}
		
}
