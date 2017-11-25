#include "stdafx.h"

#include "..\glew\glew.h"	// include GL Extension Wrangler
#include "..\glfw\glfw3.h"	// include GLFW helper library
#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <random>
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
#include "Classes/Lighting.h"
#include "Classes/SpherePrimitive.h"
#include "Classes/Shadow.h"

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
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Procedural World", nullptr, nullptr);
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

	//Create moon (sphere)
	std::vector<glm::vec3> sphere_vertices;
	std::vector<glm::vec3> sphere_normals;
	std::vector<glm::vec2> sphere_UVs;
	loadOBJ("sphere.obj", sphere_vertices, sphere_normals, sphere_UVs); //read the vertices from the sphere.obj file

	GLuint sphere_vao, sphere_vbo, sphere_ebo; //You can add more VAOs/VBO for different objects
	glGenVertexArrays(1, &sphere_vao);
	glGenBuffers(1, &sphere_vbo);
	glGenBuffers(1, &sphere_ebo);

	SpherePrimitive moon = SpherePrimitive();
	moon.bindSphere(sphere_vao, sphere_vertices, sphere_normals, sphere_UVs);

	//Building created from cubes
	//4.5 ratio to city dimension to populate the entire city
	Building buildings;
	int num_of_building = 12;
	int total_buildings = num_of_building * num_of_building;
	buildings.createBuildings(total_buildings);

	//Terrain
	Terrain terrain;
	terrain.loadTerrain(250,250);//width and height of terrain

	Terrain road;
	int city_dim = (int) ceil(4.5 * num_of_building);
	road.loadTerrain(city_dim, city_dim);//width and height of terrain

	//Initialize the lighting
	Lighting lights = Lighting();
	lights.setLightPositions(5, 5);
	lights.setLightAttributes();

	glm::vec3 *light_positions;
	glm::vec3 *ambient_values;

	light_positions = lights.getLightPositions();
	ambient_values = lights.getLightAmbient();

	//Shadows
	//The framebuffer, which regroups 0, 1, or more textures, and 0 or 1 depth buffer.
	unsigned int depthMapFBO;
	glGenFramebuffers(1, &depthMapFBO);

	// Depth texture for shadowing
	GLuint depthMap;
	glGenTextures(1, &depthMap);

	Shadow shadow = Shadow();
	shadow.bindDepthMap(depthMapFBO, depthMap);

	//texturizing
	Texture::loadTexture(1, "building1.jpg", shader_program.getShaderId(), "textureNumber[0]");//texture1
	Texture::loadTexture(2, "building2.jpg", shader_program.getShaderId(), "textureNumber[1]");//texture2
	Texture::loadTexture(3, "building3.jpg", shader_program.getShaderId(), "textureNumber[2]");//texture3
	Texture::loadTexture(4, "building4.jpg", shader_program.getShaderId(), "textureNumber[3]");//texture3
	Texture::loadTexture(5, "grass.jpg", shader_program.getShaderId(), "textureNumber[4]");//texture3
	Texture::loadTexture(6, "road.jpg", shader_program.getShaderId(), "textureNumber[5]");//texture3

	glUniform1i(glGetUniformLocation(shader_program.getShaderId(), "shadowMap"), 7);

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
	GLuint fog_option = glGetUniformLocation(shader_program.getShaderId(), "fogOption");
	GLuint fog_debug_depth = glGetUniformLocation(shader_program.getShaderId(), "fogDebugDepth");
	GLuint fog_start = glGetUniformLocation(shader_program.getShaderId(), "fogStart");
	GLuint fog_end = glGetUniformLocation(shader_program.getShaderId(), "fogEnd");
	GLuint fog_density = glGetUniformLocation(shader_program.getShaderId(), "fogDensity");

	//Camera set up
	glm::vec3 eye(0.0f, 10.0f, 50.0f);
	glm::vec3 center(0.0f, 0.0f, -1.0f);
	glm::vec3 up(0.0f, 1.0f, 0.0f);
	Camera* camera = new Camera(eye, center, up, fog_option, fog_debug_depth, fog_start, fog_end, fog_density);//boxes to test camera collision with world objects
	camera->setPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 50.0f, projectionLoc);
	//set loop coord
	camera->setLoopCoord(72,72);//value that has the smoothest effect against 250 x 250 terrain
	//set build scales and texture
	camera->setBuildingTextureScale(total_buildings);

	// Set window pointer to the window we want and then set callbacks using our redirection functions
	MyWindow* myWindow = new MyWindow(camera);
	glfwSetWindowUserPointer(window, myWindow);

	//*NOTE-glfw cannot take class methods as callback unless it is static (due to glfw base on C)
	//*NOTE-from GLFW Faq "use static methods or regular functions as callbacks, store the pointer to the object you wish to call in some location reachable from the callbacks and use it to call methods on your object"
	//set key call back
	glfwSetKeyCallback(window, func);
	glfwSetCursorPosCallback(window, func2);
	glfwSetMouseButtonCallback(window, func3);

	//set up for bounding box collisions
	vector<BoundingBox*> boxes;

	// Game loop
	while (!glfwWindowShouldClose(window))
	{
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();

		// Render
		// Clear the colorbuffer
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// 1. Render scene from light's perspective for shadows
		shadow.renderDepthMap(depthMapFBO, depthBiasMatrixID);

		// 2. render scene as normal using the generated depth/shadow map 
		glViewport(0, 0, 1024, 1024);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 view_matrix;
		camera->setLookAt(view_matrix, viewMatrixLoc);

		glm::mat4 model_matrix;
		model_matrix = glm::scale(model_matrix, triangle_scale);

		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(model_matrix));

		//Draw the lights
		lights.render(shader_program, light_positions, ambient_values);
		
		//skybox
		skybox.render(view_matrix, viewMatrixLoc, drawing_skybox_id);

		//set view back to normal
		glUniformMatrix4fv(viewMatrixLoc, 1, GL_FALSE, glm::value_ptr(view_matrix));

		//Terrain
		glUniform1i(texture_option, 5);
		terrain.render(transformLoc, 0.0f);
		glUniform1i(texture_option, 6);
		road.render(transformLoc, 0.01f);

		// Free bounding boxes memory
		while (boxes.size() > 0)
		{
			delete boxes[boxes.size() - 1];
			boxes.pop_back();
		}

		//Draw the textured cube and instances
		buildings.render(boxes, transformLoc, texture_option, texture_matrix, scale_UV, city_dim, camera->getCameraBuildingScales(), camera->getCameraBuildingTexture());

		//Render moon
		glBindVertexArray(sphere_vao);

		glm::mat4 sphere;
		sphere = glm::translate(sphere, light_positions[0]);
		sphere = glm::scale(sphere, glm::vec3(2.0f)); // Make it a smaller cube
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(sphere));
		glDrawArrays(GL_TRIANGLES, 0, sphere_vertices.size());

		glBindVertexArray(0);

		//set the boxes for camera collision
		camera->setCameraBoxes(boxes);

		// Swap the screen buffers
		glfwSwapBuffers(window);
	}

	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();
	return 0;
}