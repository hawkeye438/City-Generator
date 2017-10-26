#version 330 core

in vec3 skyboxCoord;
out vec4 color;

uniform samplerCube skybox;

uniform bool drawingSkybox;

void main()
{
	if (drawingSkybox) {
		color = texture(skybox, skyboxCoord);
	}
	else {
		color = vec4(1.0f, 0.5f, 0.2f, 1.0f);
	} 
} 