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
const GLuint WIDTH = 1024, HEIGHT = 1024;

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

	//Terrain
	Terrain terrain;
	terrain.loadTerrain(10,10);//width and heeight of terrain

	Terrain road;
	int city_dim = 4;
	float city_scale = 25.0f;
	road.loadTerrain(city_dim, city_dim);//width and heeight of terrain

	 //Building created from cubes
	 //this is base on road terrain MxM and Scale value
	//example 4x4, to center in world, it become -1.5, 0, 1.5
	//scale this by 25, it become 37.5 - offset of 1 = 36.5. Rounded to 36
	//36/2 = 18. 18 - offset of 2 = 16.
	//16 by 16 is 256
	Building buildings;
	int num_of_building = (int) (((0.5 * (city_dim - 1) * city_scale) - 1)/2) - 2 ;
	int total_buildings = num_of_building * num_of_building;
	buildings.createBuildings(total_buildings);

	//Shadows
	//The framebuffer, which regroups 0, 1, or more textures, and 0 or 1 depth buffer.
	unsigned int depthMapFBO;
	glGenFramebuffers(1, &depthMapFBO);

	// Depth texture. Slower than a depth buffer, but you can sample it later in your shader
	GLuint depthMap;
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, 1024, 1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glViewport(0, 0, 1024, 1024);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);

	//texturizing
	Texture::loadTexture(1, "brick.jpg");//texture1
	Texture::loadTexture(2, "building.jpg");//texture2
	Texture::loadTexture(3, "grass.jpg");//texture3
	Texture::loadTexture(4, "road.jpg");//texture3

	glUniform1i(glGetUniformLocation(shader_program.getShaderId(), "textureNumber[0]"), 1); //cubeTexture should read from texture unit 1, skybox is 0
	glUniform1i(glGetUniformLocation(shader_program.getShaderId(), "textureNumber[1]"), 2); //cubeTexture should read from texture unit 2, skybox is 0
	glUniform1i(glGetUniformLocation(shader_program.getShaderId(), "textureNumber[2]"), 3); //cubeTexture should read from texture unit 3, skybox is 0
	glUniform1i(glGetUniformLocation(shader_program.getShaderId(), "textureNumber[3]"), 4); //cubeTexture should read from texture unit 4, skybox is 0
	glUniform1i(glGetUniformLocation(shader_program.getShaderId(), "shadowMap"), 1);

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
	GLuint light_pos = glGetUniformLocation(shader_program.getShaderId(), "light_matrix");
	GLuint view_pos = glGetUniformLocation(shader_program.getShaderId(), "viewPosition");
	GLuint depthBiasMatrixID = glGetUniformLocation(shader_program.getShaderId(), "DepthBiasMVP");
	
	//Camera set up
	glm::vec3 eye(0.0f, 20.0f, 3.0f);
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

		float time1 = glfwGetTime();
		float movementz1 = sin(time1);
		float movementx1 = cos(time1);

		glm::vec3 light_pers(movementx1, 0.0, movementz1);

		// Render scene from light's perspective
		glm::vec3 lightInvDir = glm::vec3(-2.0f, 4.0f, -1.0f);

		float near_plane = 1.0f, far_plane = 10.0f;
		glm::mat4 depthProjectionMatrix = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
		glm::mat4 depthViewMatrix = glm::lookAt(light_pers, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
		glm::mat4 depthModelMatrix = glm::mat4(1.0);
		glm::mat4 depthMVP = depthProjectionMatrix * depthViewMatrix;

		glm::mat4 biasMatrix(
			0.5, 0.0, 0.0, 0.0,
			0.0, 0.5, 0.0, 0.0,
			0.0, 0.0, 0.5, 0.0,
			0.5, 0.5, 0.5, 1.0
		);
		glm::mat4 depthBiasMVP = biasMatrix*depthMVP;
		glUniformMatrix4fv(depthBiasMatrixID, 1, GL_FALSE, glm::value_ptr(depthBiasMVP));

		glViewport(0, 0, 1024, 1024);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);
		glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, woodTexture);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// reset viewport
		glViewport(0, 0, 1024, 1024);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// 2. render scene as normal using the generated depth/shadow map 
		glViewport(0, 0, 1024, 1024);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 view_matrix;
		camera->setLookAt(view_matrix, viewMatrixLoc);

		glm::mat4 model_matrix;
		model_matrix = glm::scale(model_matrix, triangle_scale);

		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(model_matrix));

		//Create light
		glm::mat4 light;
		float time = glfwGetTime();
		float movementz = sin(time);
		float movementx = cos(time);
		light = glm::translate(light, glm::vec3(3 * movementx, 2.0f, 3 * movementz));
		//Pass view position for specular lighting
		glUniform3fv(view_pos, 1, glm::value_ptr(eye));
		glUniformMatrix4fv(light_pos, 1, GL_FALSE, glm::value_ptr(light));

		//Shadows
		//Send our transformation to the currently bound shader,
		//in the "MVP" uniform
		glUniformMatrix4fv(depthBiasMatrixID, 1, GL_FALSE, glm::value_ptr(depthBiasMVP));
		//glUniformMatrix4fv(depthMatrixID, 1, GL_FALSE, &depthMVP[0][0]);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		
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
		road.render(transformLoc, city_scale, 0.01f);

		//Draw the textured cube and instances
		buildings.render(boxes, transformLoc, texture_option, texture_matrix, scale_UV, city_dim, city_scale);

		//set the boxes for camera collision
		camera->setCameraBoxes(boxes);

		// Swap the screen buffers
		glfwSwapBuffers(window);
	}

	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();
	return 0;
}