#include "Lighting.h"

//Sets the positions of the lights
void Lighting::setLightPositions(int num_of_rows, int num_of_cols) {

	int index = 1;
	this->pointLightPositions[0] = glm::vec3(-30.0f, 50.0f, -10.0f);
	float x = -30.0f;
	for (int i = 1; i < num_of_rows + 1; i++) {
		x = x + 10.0f;
		float z = -30.0f;
		for (int j = 0; j < num_of_cols; j++) {
			z = z + 10.0f;
			this->pointLightPositions[index++] = glm::vec3(x, -1.0f, z);
		}
	}
}

void Lighting::setLightAttributes() {

	this->pointLightAmbient[0] = glm::vec3(60.0f, 60.0f, 60.0f);
	for (int i = 1; i < 26; i++) {
		this->pointLightAmbient[i] = glm::vec3(0.05f, 0.05f, 0.05f);
	}
}

glm::vec3* Lighting::getLightPositions() {
	return this->pointLightPositions;
}

glm::vec3* Lighting::getLightAmbient() {
	return this->pointLightAmbient;
}

void Lighting::render(Shader &shader_program, glm::vec3 positions[], glm::vec3 ambient[]) {

	for (GLuint i = 0; i < 26; i++)
	{
		string number = to_string(i);

		glUniform3fv(glGetUniformLocation(shader_program.getShaderId(), ("pointLights[" + number + "].position").c_str()), 1, glm::value_ptr(positions[i]));
		glUniform3fv(glGetUniformLocation(shader_program.getShaderId(), ("pointLights[" + number + "].ambient").c_str()), 1, glm::value_ptr(ambient[i]));
		glUniform3fv(glGetUniformLocation(shader_program.getShaderId(), ("pointLights[" + number + "].diffuse").c_str()), 1, glm::value_ptr(glm::vec3(0.8f, 0.8f, 0.8f)));
		glUniform3fv(glGetUniformLocation(shader_program.getShaderId(), ("pointLights[" + number + "].specular").c_str()), 1, glm::value_ptr(glm::vec3(0.5f, 0.5f, 0.5f)));
		glUniform1f(glGetUniformLocation(shader_program.getShaderId(), ("pointLights[" + number + "].constant").c_str()), 1.0f);
		glUniform1f(glGetUniformLocation(shader_program.getShaderId(), ("pointLights[" + number + "].linear").c_str()), 0.09f);
		glUniform1f(glGetUniformLocation(shader_program.getShaderId(), ("pointLights[" + number + "].quadratic").c_str()), 0.032f);
	}
}


