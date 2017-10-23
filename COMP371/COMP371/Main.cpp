#include "stdafx.h"

#include "..\glew\glew.h"	// include GL Extension Wrangler
#include "..\glfw\glfw3.h"	// include GLFW helper library
#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include "Classes/Shader.h"

using namespace std;

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 800;

// Camera Orientation
float rotate_x = 0.0f;//for rotating around x axis
float rotate_y = 0.0f;//for rotating around y axis
float rotate_z = 0.0f;//for rotating around z axis (if needed)

//Look at vectors
glm::vec3 eye(0.0f, 0.0f, 3.0f);
glm::vec3 center(0.0f, 0.0f, -1.0f);
glm::vec3 up(0.0f, 1.0f, 0.0f);

//scaling
glm::vec3 triangle_scale;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	//Camera Controls
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {//Forward
		eye += 0.05f * center;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {//Backward
		eye -= 0.05f * center;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {//left
		eye -= glm::normalize(glm::cross(center, up)) * 0.05f;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {//right
		eye += glm::normalize(glm::cross(center, up)) * 0.05f;
	}

	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {//Up
		eye.y += 0.05f;
	}
	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {//Down
		eye.y -= 0.05f;
	}

	//World Orientation
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		rotate_y += 1.0f;
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		rotate_y -= 1.0f;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		rotate_x += 1.0f;
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		rotate_x -= 1.0f;
	}

	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
		rotate_z += 1.0f;
	}
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
		rotate_z -= 1.0f;
	}

}

int main()
{
	std::cout << "Starting GLFW context, OpenGL 3.3" << std::endl;
	// Init GLFW
	glfwInit();
	// Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Load one cube", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	// Set the required callback functions
	glfwSetKeyCallback(window, key_callback);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}

	// Define the viewport dimensions
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

	// Vertex shader
	// Read the Vertex Shader code from the file
	string vertex_shader_path = "vertex.shader";
	
	// Read the Fragment Shader code from the file
	string fragment_shader_path = "fragment.shader";
	// Build and compile our shader program
	Shader shader_program(vertex_shader_path, fragment_shader_path);
	shader_program.use();//use program

	//configure global opengl state
	glEnable(GL_DEPTH_TEST);

	float vertices[] = {//vertices for square
		0.5f,  0.5f, 0.0f,  // top right
		0.5f, -0.5f, 0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f,  // bottom left
		-0.5f,  0.5f, 0.0f   // top left 
	};

	unsigned int indices[] = { 
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	};

	int indices_size = sizeof(indices) / sizeof(indices[0]);

	GLuint VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	
	// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs), remember: do NOT unbind the EBO, keep it bound to this VAO

	triangle_scale = glm::vec3(1.0f);
	glm::mat4 projection_matrix;
	projection_matrix = glm::perspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);

	GLuint projectionLoc = glGetUniformLocation(shader_program.getShaderId(), "projection_matrix");
	GLuint viewMatrixLoc = glGetUniformLocation(shader_program.getShaderId(), "view_matrix");
	GLuint transformLoc = glGetUniformLocation(shader_program.getShaderId(), "model_matrix");

	// Game loop
	while (!glfwWindowShouldClose(window))
	{
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();

		// Render
		// Clear the colorbuffer
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 view_matrix;
		view_matrix = glm::lookAt(eye, eye + center, up);
		view_matrix = glm::rotate(view_matrix, glm::radians(rotate_x), glm::vec3(1.0f, 0.0f, 0.0f));//rotate the camera along x axis
		view_matrix = glm::rotate(view_matrix, glm::radians(rotate_y), glm::vec3(0.0f, 1.0f, 0.0f));//rotate the camera along y axis
		view_matrix = glm::rotate(view_matrix, glm::radians(rotate_z), glm::vec3(0.0f, 0.0f, 1.0f));//rotate the camera along z axis

		glm::mat4 model_matrix;
		model_matrix = glm::scale(model_matrix, triangle_scale);

		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(model_matrix));
		glUniformMatrix4fv(viewMatrixLoc, 1, GL_FALSE, glm::value_ptr(view_matrix));
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection_matrix));

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, indices_size, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		// Swap the screen buffers
		glfwSwapBuffers(window);
	}

	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();
	return 0;
}
