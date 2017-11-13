#include "stdafx.h"

#include "..\glew\glew.h"	// include GL Extension Wrangler
#include "..\glfw\glfw3.h"	// include GLFW helper library
#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include "objloader.hpp"
#include "Classes/Shader.h"
#include "Classes/Skybox.h"
#include "Classes/Texture.h"
#include "Classes/Terrain.h"
#include "Classes/BoundingBox.h"
#include "Classes/Camera.h"
#include "Classes/MyWindow.h"

using namespace std;

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 800;

//scaling
glm::vec3 triangle_scale;

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

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> UVs;
	loadOBJ("cube.obj", vertices, normals, UVs);

	GLuint VAO, vertices_VBO, normals_VBO, UVs_VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &vertices_VBO);

	// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, vertices_VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices.front(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &normals_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, normals_VBO);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals.front(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(1);

	glGenBuffers(1, &UVs_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, UVs_VBO);
	glBufferData(GL_ARRAY_BUFFER, UVs.size() * sizeof(glm::vec2), &UVs.front(), GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs), remember: do NOT unbind the EBO, keep it bound to this VAO

	//Testing bounding box
	//min max for cube
	glm::vec3 min, max;
	vector<BoundingBox*> boxes;

	GLfloat
		min_x, max_x,
		min_y, max_y,
		min_z, max_z;
	min_x = max_x = vertices[0].x;
	min_y = max_y = vertices[0].y;
	min_z = max_z = vertices[0].z;
	for (int i = 0; i < vertices.size(); i++) {
		if (vertices[i].x < min_x) min_x = vertices[i].x;
		if (vertices[i].x > max_x) max_x = vertices[i].x;
		if (vertices[i].y < min_y) min_y = vertices[i].y;
		if (vertices[i].y > max_y) max_y = vertices[i].y;
		if (vertices[i].z < min_z) min_z = vertices[i].z;
		if (vertices[i].z > max_z) max_z = vertices[i].z;
	}
	cout << min_x << "," << max_x << endl;
	cout << min_y*2 << "," << max_y*2 << endl;
	cout << min_z << "," << max_z << endl;
	
	//first cube with no transformation
	min = glm::vec3(min_x, min_y, min_z);
	max = glm::vec3(max_x, max_y, max_z);
	
	BoundingBox* temp = new BoundingBox(min, max);
	boxes.push_back(temp);

	//second cube with transformation to match ones done during render
	//will have to store transformations in bounding box first before passing it to rendering
	glm::mat4 cube;
	cube = glm::scale(cube, glm::vec3(1.0f, 2.0f, 1.0f));
	cube = glm::translate(cube, glm::vec3(4.0f, 0.0f, 0.0f));

	glm::vec4 test(min, 1.0f);
	glm::vec4 test2(max, 1.0f);
	test = cube * test;
	test2 = cube * test2;
	min = glm::vec3(test);
	max = glm::vec3(test2);
	temp = new BoundingBox(min, max);
	boxes.push_back(temp);

	//Terrain
	Terrain terrain;
	terrain.loadTerrain(4,4);//width and heeight of terrain

	//texturizing
	Texture::loadTexture(1, "brick.jpg");//texture1
	Texture::loadTexture(2, "building.jpg");//texture2
	Texture::loadTexture(3, "grass.jpg");//texture3

	glUniform1i(glGetUniformLocation(shader_program.getShaderId(), "textureNumber[0]"), 1); //cubeTexture should read from texture unit 1, skybox is 0
	glUniform1i(glGetUniformLocation(shader_program.getShaderId(), "textureNumber[1]"), 2); //cubeTexture should read from texture unit 2, skybox is 0
	glUniform1i(glGetUniformLocation(shader_program.getShaderId(), "textureNumber[2]"), 3); //cubeTexture should read from texture unit 3, skybox is 0

	//skybox
	//skybox texture from https://93i.de/p/free-skybox-texture-set/
	Skybox skybox;
	skybox.loadSkyBox("Fullmoon/right.png", "Fullmoon/left.png", "Fullmoon/top.png",
		"Fullmoon/bottom.png", "Fullmoon/back.png", "Fullmoon/front.png", shader_program.getShaderId());
	
	triangle_scale = glm::vec3(1.0f);

	GLuint projectionLoc = glGetUniformLocation(shader_program.getShaderId(), "projection_matrix");
	GLuint viewMatrixLoc = glGetUniformLocation(shader_program.getShaderId(), "view_matrix");
	GLuint transformLoc = glGetUniformLocation(shader_program.getShaderId(), "model_matrix");
	GLuint drawing_skybox_id = glGetUniformLocation(shader_program.getShaderId(), "drawingSkybox");
	GLuint texture_option = glGetUniformLocation(shader_program.getShaderId(), "textureOption");
	GLuint texture_Matrix = glGetUniformLocation(shader_program.getShaderId(), "textureMatrix");
	GLuint scale_UV = glGetUniformLocation(shader_program.getShaderId(), "scaleUV");
	
	//Camera set up
	glm::vec3 eye(0.0f, 0.0f, 3.0f);
	glm::vec3 center(0.0f, 0.0f, -1.0f);
	glm::vec3 up(0.0f, 1.0f, 0.0f);
	Camera* camera = new Camera(eye, center, up, boxes);//boxes to test camera collision with world objects
	camera->setPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f, projectionLoc);
	// Set the required callback functions
	MyWindow* myWindow = new MyWindow(camera);
	glfwSetWindowUserPointer(window, myWindow);
	auto func = [](GLFWwindow* window, int key, int scancode, int action, int mode)
	{
		static_cast<MyWindow*>(glfwGetWindowUserPointer(window))->cameraControls(window, key,scancode, action, mode);
	};

	//*NOTE-glfw cannot take class methods as callback unless it is static (due to glfw base on C)
	//*NOTE-from GLFW Faq "use static methods or regular functions as callbacks, store the pointer to the object you wish to call in some location reachable from the callbacks and use it to call methods on your object"
	//set key call back
	glfwSetKeyCallback(window, func);
	
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
		camera->setLookAt(view_matrix, viewMatrixLoc);
		camera->rotateCamera(view_matrix, viewMatrixLoc);

		glm::mat4 model_matrix;
		model_matrix = glm::scale(model_matrix, triangle_scale);

		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(model_matrix));
		
		//skybox
		skybox.render(view_matrix, viewMatrixLoc, drawing_skybox_id);

		//set view back to normal
		glUniformMatrix4fv(viewMatrixLoc, 1, GL_FALSE, glm::value_ptr(view_matrix));

		//Terrain
		glUniform1i(texture_option, 3);
		terrain.render(transformLoc);
		
		//Draw the textured cube and instances
		glBindVertexArray(VAO);
		glm::mat4 cube;
		cube = glm::translate(cube, glm::vec3(0.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(cube));
		glUniform1i(texture_option, 1);
		glDrawArrays(GL_TRIANGLES, 0, vertices.size());

		//draw a second cube with different texture and scale
		cube = glm::scale(cube, glm::vec3(1.0f, 2.0f, 1.0f));
		cube = glm::translate(cube, glm::vec3(4.0f,0.0f,0.0f));
		
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(cube));
		//for UV scaling, in order to have repeated texture and not stretch texture
		glUniformMatrix4fv(texture_Matrix, 1, GL_FALSE, glm::value_ptr(cube));
		
		glUniform1i(texture_option, 2);
		glUniform1i(scale_UV, 1);//true
		glDrawArrays(GL_TRIANGLES, 0, vertices.size());
		glUniform1i(scale_UV, 0);//false
		glBindVertexArray(0);

		// Swap the screen buffers
		glfwSwapBuffers(window);
	}

	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();
	return 0;
}