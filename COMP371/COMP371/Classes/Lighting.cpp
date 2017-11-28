#include "Lighting.h"

//Sets the positions of the lights
void Lighting::setCityLightPositions(int num_of_rows, int num_of_cols) {

	int index = 1;
	this->pointLightPositions[0] = glm::vec3(-30.0f, 60.0f, -10.0f);
	float x = -30.0f;
	for (int i = 1; i < num_of_rows + 1; i++) {
		x = x + 10.0f;
		float z = -30.0f;
		for (int j = 0; j < num_of_cols; j++) {
			z = z + 10.0f;
			this->pointLightPositions[index++] = glm::vec3(x, 0.1f, z);
		}
	}
}

void Lighting::setCityLightAttributes(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular) {

	for (int i = 1; i < 26; i++) {
		this->pointLightAmbient[i] = glm::vec3(0.1f, 0.1f, 0.1f);
		this->pointLightDiffuse[i] = glm::vec3(0.81f, 0.64f, 0.07f);
		this->pointLightSpecular[i] = glm::vec3(0.81f, 0.64f, 0.07f);
	}
}

void Lighting::setCityAmbientStrength(float strength) {
	for (int i = 1; i < 26; i++) {
		this->pointAmbStrength[i] = strength;
	}
}

void Lighting::setSunLightPosition(glm::vec3 position) {
	this->pointLightPositions[0] = position;
}
void Lighting::setSunLightAttributes(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular) {
	this->pointLightAmbient[0] = ambient;
	this->pointLightDiffuse[0] = diffuse;
	this->pointLightSpecular[0] = specular;
}

glm::vec3 Lighting::getSunLightPosition() {
	return this->pointLightPositions[0];
}

void Lighting::setSunAmbientStrength(float strength) {
	this->pointAmbStrength[0] = strength;
}

void Lighting::setMoonLightDirection(glm::vec3 direction) {
	this->moonLightDirection = direction;
}

void Lighting::setMoonLightAttributes(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular) {
	this->moonLightAmbient = ambient;
	this->moonLightDiffuse = diffuse;
	this->moonLightSpecular = specular;
}

void Lighting::setMoonAmbientStrength(float strength) {
	this->moonAmbStrength = strength;
}

void Lighting::render(Shader &shader_program) {

	glm::vec3* pos = this->pointLightPositions;
	glm::vec3* amb = this->pointLightAmbient;
	glm::vec3* spec = this->pointLightSpecular;
	float* amb_str = this->pointAmbStrength;

	for (GLuint i = 0; i < 26; i++)
	{
		string number = to_string(i);

		glUniform3fv(glGetUniformLocation(shader_program.getShaderId(), ("pointLights[" + number + "].position").c_str()), 1, glm::value_ptr(pos[i]));
		glUniform3fv(glGetUniformLocation(shader_program.getShaderId(), ("pointLights[" + number + "].ambient").c_str()), 1, glm::value_ptr(amb[i]));
		glUniform3fv(glGetUniformLocation(shader_program.getShaderId(), ("pointLights[" + number + "].diffuse").c_str()), 1, glm::value_ptr(spec[i]));
		glUniform3fv(glGetUniformLocation(shader_program.getShaderId(), ("pointLights[" + number + "].specular").c_str()), 1, glm::value_ptr(glm::vec3(0.81f, 0.64f, 0.07f)));
		glUniform1f(glGetUniformLocation(shader_program.getShaderId(), ("pointLights[" + number + "].ambient_strength").c_str()), amb_str[i]);
		glUniform1f(glGetUniformLocation(shader_program.getShaderId(), ("pointLights[" + number + "].constant").c_str()), 1.0f);
		glUniform1f(glGetUniformLocation(shader_program.getShaderId(), ("pointLights[" + number + "].linear").c_str()), 0.09f);
		glUniform1f(glGetUniformLocation(shader_program.getShaderId(), ("pointLights[" + number + "].quadratic").c_str()), 0.032f);
	}

	//Moon
	glm::mat4 moon;
	moon = glm::translate(moon, this->moonLightDirection);

	glUniformMatrix4fv(glGetUniformLocation(shader_program.getShaderId(), "light_matrix"), 1, GL_FALSE, glm::value_ptr(moon));
	glUniform3fv(glGetUniformLocation(shader_program.getShaderId(), "dirLight.ambient"), 1, glm::value_ptr(this->moonLightAmbient));
	glUniform3fv(glGetUniformLocation(shader_program.getShaderId(), "dirLight.diffuse"), 1, glm::value_ptr(this->moonLightDiffuse));
	glUniform3fv(glGetUniformLocation(shader_program.getShaderId(), "dirLight.specular"), 1, glm::value_ptr(this->moonLightSpecular));
	glUniform1f(glGetUniformLocation(shader_program.getShaderId(), "dirLight.ambient_strength"), this->moonAmbStrength);
}


