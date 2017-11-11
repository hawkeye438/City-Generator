#version 330 core

in vec3 skyboxCoord;//skybox
in vec2 outUV;//cube uv for texture rendering

out vec4 color;

uniform sampler2D cubeTexture;//texture sampling
uniform samplerCube skybox; //skybox sampling

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