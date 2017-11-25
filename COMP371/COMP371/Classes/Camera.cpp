#include "Camera.h"

void Camera::setPerspective(float fov, GLfloat aspect, GLfloat near, GLfloat far, GLuint projectionLoc) {
	glm::mat4 projection_matrix;
	projection_matrix = glm::perspective(fov, aspect, near, far);
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection_matrix));
}

void Camera::setLookAt(glm::mat4 &view_matrix, GLuint viewMatrixLoc) {
	view_matrix = glm::lookAt(eye, eye + center, up);
	glUniformMatrix4fv(viewMatrixLoc, 1, GL_FALSE, glm::value_ptr(view_matrix));
}

//Working but still testing
void Camera::checkCollision(glm::vec3 point, float offset, int value) {
	for (int i = 0; i < boxes.size(); i++) {
		if (boxes[i]->intersect(point, offset)) {
			cout << "intersecting working" << endl;
			switch (value) {
			case 1: eye -= mv_distance * center;
				break;
			case 2: eye += mv_distance * center;
				break;
			case 3: eye += glm::normalize(glm::cross(center, up)) * mv_distance;
				break;
			case 4: eye -= glm::normalize(glm::cross(center, up)) * mv_distance;
				break;
			case 5: eye.y -= mv_distance;
				break;
			case 6: eye.y += mv_distance;
				break;
			}
			//to see where it is colliding
			cout << "eye coord" << endl;
			cout << eye.x << "," << eye.y << "," << eye.z << endl;
			cout << "object bounds" << endl;
			cout << boxes[i]->getMinExtent().x << "," << boxes[i]->getMaxExtent().x << endl;
			cout << boxes[i]->getMinExtent().y << "," << boxes[i]->getMaxExtent().y << endl;
			cout << boxes[i]->getMinExtent().z << "," << boxes[i]->getMaxExtent().z << endl;
		}
	}
}

void Camera::checkTerrainCollision() {//establish a boundary that camera can't pass for terrain
	if (eye.y < terrain_bounds) {
		eye.y = terrain_bounds;
	}
}

void Camera::cameraKeys(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	float offset = 0.5f;
	int value = 0;
	//Camera Controls
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {//Forward
		eye += mv_distance * center;
		value = 1;
		checkCollision(eye, offset, value);
		checkTerrainCollision();
		checkLoopPos();
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {//Backward
		eye -= mv_distance * center;
		value = 2;
		checkCollision(eye, offset, value);
		checkTerrainCollision();
		checkLoopPos();
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {//left
		eye -= glm::normalize(glm::cross(center, up)) * mv_distance;
		value = 3;
		checkCollision(eye, offset, value);
		checkTerrainCollision();
		checkLoopPos();
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {//right
		eye += glm::normalize(glm::cross(center, up)) * mv_distance;
		value = 4;
		checkCollision(eye, offset, value);
		checkTerrainCollision();
		checkLoopPos();
	}

	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {//Up
		eye.y += 0.1f;
		value = 5;
		checkCollision(eye, offset, value);
		checkTerrainCollision();
		checkLoopPos();
	}
	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {//Down
		eye.y -= 0.05f;
		value = 6;
		checkCollision(eye, offset, value);
		checkTerrainCollision();
		checkLoopPos();
	}

	// Fog manipulation
	if (action == GLFW_PRESS || action == GLFW_REPEAT)
	{
		switch (key)
		{
		case GLFW_KEY_U: // Switch the current fog mode
			if (currentFogMode == NONE)
				if (mode & GLFW_MOD_SHIFT)
					currentFogMode = static_cast<fogMode>(NONE - 1);
				else
					currentFogMode = LINEAR;
			else if (currentFogMode == 0 && mode & GLFW_MOD_SHIFT)
				currentFogMode = static_cast<fogMode>(NONE - 1);
			else
				currentFogMode = static_cast<fogMode>(currentFogMode + ((mode & GLFW_MOD_SHIFT) ? -1 : 1));
			glUniform1i(fog_option, currentFogMode);
			break;
		case GLFW_KEY_J: // Switch debug mode
			fogDebugValue = !fogDebugValue;
			glUniform1i(fog_debug, fogDebugValue);
			break;
		case GLFW_KEY_I: // Scale the fog density
			fogDensity += (mode & GLFW_MOD_SHIFT) ? -0.005f : 0.005f;
			fogDensity = glm::clamp(fogDensity, 0.0f, 1.0f);
			glUniform1f(fog_density, fogDensity);
			break;
		case GLFW_KEY_O: // Scale the fog start
			fogStart += (mode & GLFW_MOD_SHIFT) ? -5.0f : 5.0f;
			fogStart = glm::clamp(fogStart, 0.0f, fogEnd);
			glUniform1f(fog_start, fogStart);
			break;
		case GLFW_KEY_P: // Scale the fog end
			fogEnd += (mode & GLFW_MOD_SHIFT) ? -5.0f : 5.0f;
			fogEnd = glm::max(fogEnd, fogStart);
			glUniform1f(fog_end, fogEnd);
			break;
		case GLFW_KEY_K: // Show debug info
			cout << "Fog debug info:" << endl;
			cout << "\t- option: " << currentFogMode << endl;
			cout << "\t- debug: " << boolalpha << fogDebugValue << noboolalpha << endl;
			cout << "\t- start: " << fogStart << endl;
			cout << "\t- end: " << fogEnd << endl;
			cout << "\t- density: " << fogDensity << endl;
			break;
		default:
			break;
		}

		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
			glfwSetWindowShouldClose(window, 1);
	}
}

void Camera::cameraMouse(GLFWwindow* window, double xpos, double ypos) {
	float xoffset;
	float yoffset;
	float sensitivity = 0.005;
	//if either of the mouse buttons are pressed
	if (mouseButtonRightDown || mouseButtonMiddleDown || mouseButtonLeftDown) {
		xoffset = xpos_click - xpos;
		yoffset = ypos_click - ypos;
		xoffset *= sensitivity;
		yoffset *= sensitivity;

		yaw += xoffset;
		pitch += yoffset;

		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;

		//pitch, yaw and roll for camera rotation around its own axis 
		glm::vec3 front;
		front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		front.y = sin(glm::radians(pitch));
		front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		center = glm::normalize(front);
	}
}

void  Camera::cameraMouseButtons(GLFWwindow* window, int button, int action, int mods) {
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
		glfwGetCursorPos(window, &xpos_click, &ypos_click);
		mouseButtonRightDown = true;
	}
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE) {
		mouseButtonRightDown = false;
	}

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS) {
		glfwGetCursorPos(window, &xpos_click, &ypos_click);
		mouseButtonMiddleDown = true;
	}
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_RELEASE) {
		mouseButtonMiddleDown = false;
	}

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
		glfwGetCursorPos(window, &xpos_click, &ypos_click);
		mouseButtonLeftDown = true;
	}
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) {
		mouseButtonLeftDown = false;
	}
}

//set min/max coordinates for x and z to loop back on itself
void Camera::setLoopCoord(int x, int z) {
	float offset = 1.0;//for testing
	min_xd = -x + offset;
	max_xd = x - offset;
	min_zd = -z + offset;
	max_zd = z - offset;
}

//restrict camera to an area and change position to opposite ends when passign min/max coordinates
void Camera::checkLoopPos() {
	if (eye.x < min_xd) {
		eye.x = max_xd;
		generateNewBuildingSetting();
	}
	if (eye.x > max_xd) {
		eye.x = min_xd;
		generateNewBuildingSetting();
	}
	if (eye.z < min_zd) {
		eye.z = max_zd;
		generateNewBuildingSetting();
	}
	if (eye.z > max_zd) {
		eye.z = min_zd;
		generateNewBuildingSetting();
	}
}

void Camera::setBuildingTextureScale(int tb) {
	total_buildings = tb;
	buildingRandomizer();
}

void Camera::generateNewBuildingSetting() {
	building_scales.clear();
	random_texture.clear();
	buildingRandomizer();
}

void Camera::buildingRandomizer() {
	uniform_int_distribution<> disti(1, 5); // create distribution for integers
	uniform_real_distribution<float> distf(0.0, 1.0); // create distribution for floats
	uniform_int_distribution<> distTexture(1, 4); // create distribution for integers
	int num_of_building = sqrt(total_buildings);

	for (int i = 0; i < num_of_building; i++) {
		for (int j = 0; j < num_of_building; j++) {
			glm::vec3 tmp(distf(engine), (float)disti(engine), distf(engine));
			building_scales.push_back(tmp);
		}
	}

	for (int k = 0; k < total_buildings; k++) {
		int tmp = distTexture(engine);
		random_texture.push_back(tmp);
	}
}