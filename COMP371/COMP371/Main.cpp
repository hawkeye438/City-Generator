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
#include "Classes/Utility.h"
#include "Classes/CubePrimitive.h"
#include "Classes/Building.h"

using namespace std;

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 800;

//scaling
glm::vec3 triangle_scale;

//redirect functions
auto func = [](GLFWwindow* window, int key, int scancode, int action, int mode)
{
	static_cast<MyWindow*>(glfwGetWindowUserPointer(window))->cameraControls(window, key, scancode, action, mode);
};

auto func2 = [](GLFWwindow* window, double xpos, double ypos)
{
	static_cast<MyWindow*>(glfwGetWindowUserPointer(window))->cameraMouseControls(window, xpos, ypos);
};

auto func3 = [](GLFWwindow* window, int button, int action, int mods)
{
	static_cast<MyWindow*>(glfwGetWindowUserPointer(window))->cameraMouseButtonsControls(window, button, action, mods);
};

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

	//Building created from cubes
	Building buildings;
	buildings.createBuildings(4);

	//Terrain
	Terrain terrain;
	terrain.loadTerrain(10,10);//width and heeight of terrain

	Terrain road;
	road.loadTerrain(4, 4);//width and heeight of terrain

	//texturizing
	Texture::loadTexture(1, "brick.jpg");//texture1
	Texture::loadTexture(2, "building.jpg");//texture2
	Texture::loadTexture(3, "grass.jpg");//texture3
	Texture::loadTexture(4, "road.jpg");//texture3

	glUniform1i(glGetUniformLocation(shader_program.getShaderId(), "textureNumber[0]"), 1); //cubeTexture should read from texture unit 1, skybox is 0
	glUniform1i(glGetUniformLocation(shader_program.getShaderId(), "textureNumber[1]"), 2); //cubeTexture should read from texture unit 2, skybox is 0
	glUniform1i(glGetUniformLocation(shader_program.getShaderId(), "textureNumber[2]"), 3); //cubeTexture should read from texture unit 3, skybox is 0
	glUniform1i(glGetUniformLocation(shader_program.getShaderId(), "textureNumber[3]"), 4); //cubeTexture should read from texture unit 4, skybox is 0

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
	GLuint texture_matrix = glGetUniformLocation(shader_program.getShaderId(), "textureMatrix");
	GLuint scale_UV = glGetUniformLocation(shader_program.getShaderId(), "scaleUV");
	
	//Camera set up
	glm::vec3 eye(0.0f, 0.5f, 3.0f);
	glm::vec3 center(0.0f, 0.0f, -1.0f);
	glm::vec3 up(0.0f, 1.0f, 0.0f);
	Camera* camera = new Camera(eye, center, up);//boxes to test camera collision with world objects
	camera->setPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f, projectionLoc);
	//set loop coord
	camera->setLoopCoord(40,40);//half the length and width of the scaled terrain for now
	
	// Set window pointer to the window we want and then set callbacks using our redirection functions
	MyWindow* myWindow = new MyWindow(camera);
	glfwSetWindowUserPointer(window, myWindow);

	//*NOTE-glfw cannot take class methods as callback unless it is static (due to glfw base on C)
	//*NOTE-from GLFW Faq "use static methods or regular functions as callbacks, store the pointer to the object you wish to call in some location reachable from the callbacks and use it to call methods on your object"
	//set key call back
	glfwSetKeyCallback(window, func);
	glfwSetCursorPosCallback(window, func2);
	glfwSetMouseButtonCallback(window, func3);

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

		glm::mat4 model_matrix;
		model_matrix = glm::scale(model_matrix, triangle_scale);

		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(model_matrix));
		
		//skybox
		skybox.render(view_matrix, viewMatrixLoc, drawing_skybox_id);

		//set view back to normal
		glUniformMatrix4fv(viewMatrixLoc, 1, GL_FALSE, glm::value_ptr(view_matrix));

		//set up for bounding box collisions
		vector<BoundingBox*> boxes;

		//Terrain
		glUniform1i(texture_option, 3);
		terrain.render(transformLoc, 10.0f, 0.0f);
		glUniform1i(texture_option, 4);
		road.render(transformLoc, 25, 0.01f);

		//Draw the textured cube and instances
		buildings.render(boxes, transformLoc, texture_option, texture_matrix, scale_UV);
		//set the boxes for camera collision
		camera->setCameraBoxes(boxes);

		// Swap the screen buffers
		glfwSwapBuffers(window);
	}

	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();
	return 0;
}