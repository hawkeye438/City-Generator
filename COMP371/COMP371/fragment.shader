#version 330 core

in vec3 skyboxCoord;//skybox
in vec2 outUV;//cube uv for texture rendering

out vec4 color;

uniform sampler2D textureNumber[2];//multiple texture support, simply change array size to numnber of texture you want
uniform samplerCube skybox; //skybox sampling

uniform bool drawingSkybox;
uniform int textureOption;
uniform mat4 textureMatrix;//for scaling uv and allow texture to scale
uniform bool scaleUV;//only scale the when true

void main()
{	//UV scaling for repeated texture rather than texture stretching
	vec4 UV = vec4(outUV, 0.0f, 0.0f);//convert to vec4
	vec4 newUV = textureMatrix * UV;//Scale the UV by x and y
	vec2 lastUV = vec2(newUV);//convert to vec2
	
	if (drawingSkybox) {
		color = texture(skybox, skyboxCoord);
	}
	else if (textureOption == 1){
		//color = vec4(1.0f, 0.5f, 0.2f, 1.0f);
		if (scaleUV) {
			color = texture(textureNumber[0], lastUV);
		}
		else {
			color = texture(textureNumber[0], outUV);
		}
	} 
	else if (textureOption == 2) {
		//color = vec4(1.0f, 0.5f, 0.2f, 1.0f);
		if (scaleUV) {
			color = texture(textureNumber[1], lastUV);
		}
		else {
			color = texture(textureNumber[1], outUV);
		}
	}
} 