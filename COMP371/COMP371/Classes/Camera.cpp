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

void Camera::rotateCamera(glm::mat4 &view_matrix, GLuint viewMatrixLoc) {
	view_matrix = glm::rotate(view_matrix, glm::radians(rotate_x), glm::vec3(1.0f, 0.0f, 0.0f));//rotate the camera along x axis
	view_matrix = glm::rotate(view_matrix, glm::radians(rotate_y), glm::vec3(0.0f, 1.0f, 0.0f));//rotate the camera along y axis
	view_matrix = glm::rotate(view_matrix, glm::radians(rotate_z), glm::vec3(0.0f, 0.0f, 1.0f));//rotate the camera along z axis

	glUniformMatrix4fv(viewMatrixLoc, 1, GL_FALSE, glm::value_ptr(view_matrix));
}

//Working but still testing
void Camera::checkCollision(glm::vec3 point, float offset, int value) {
	for (int i = 0; i < boxes.size(); i++) {
		if (boxes[i]->intersect(point, offset)) {
			cout << "intersecting working" << endl;
			switch (value) {
			case 1: eye -= 0.1f * center;
				break;
			case 2: eye += 0.1f * center;
				break;
			case 3: eye += glm::normalize(glm::cross(center, up)) * 0.1f;
				break;
			case 4: eye -= glm::normalize(glm::cross(center, up)) * 0.1f;
				break;
			case 5: eye.y -= 0.1f;
				break;
			case 6: eye.y += 0.1f;
				break;
			}
			cout << "eye coord" << endl;
			cout << eye.x << "," << eye.y << "," << eye.z << endl;
			cout << "object bounds" << endl;
			cout << boxes[i]->getMinExtent().x << "," << boxes[i]->getMaxExtent().x << endl;
			cout << boxes[i]->getMinExtent().y << "," << boxes[i]->getMaxExtent().y << endl;
			cout << boxes[i]->getMinExtent().z << "," << boxes[i]->getMaxExtent().z << endl;
		}
	}
}

void Camera::cameraKeys(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	float offset = 0.5f;
	int value = 0;
	//Camera Controls
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {//Forward
		eye += 0.1f * center;
		value = 1;
		checkCollision(eye, offset, value);

	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {//Backward
		eye -= 0.1f * center;
		value = 2;
		checkCollision(eye, offset, value);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {//left
		eye -= glm::normalize(glm::cross(center, up)) * 0.1f;
		value = 3;
		checkCollision(eye, offset, value);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {//right
		eye += glm::normalize(glm::cross(center, up)) * 0.1f;
		value = 4;
		checkCollision(eye, offset, value);
	}

	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {//Up
		eye.y += 0.1f;
		value = 5;
		checkCollision(eye, offset, value);
	}
	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {//Down
		eye.y -= 0.05f;
		value = 6;
		checkCollision(eye, offset, value);
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