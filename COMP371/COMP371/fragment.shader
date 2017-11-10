#version 330 core

in vec3 skyboxCoord;
in vec2 outUV;

out vec4 color;

uniform sampler2D cubeTexture;
uniform samplerCube skybox;

uniform bool drawingSkybox;

void main()
{
	if (drawingSkybox) {
		color = texture(skybox, skyboxCoord);
	}
	else {
		//color = vec4(1.0f, 0.5f, 0.2f, 1.0f);
		color = texture(cubeTexture, outUV);
	} 
} 