
#version 330 core
  
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 UV;
layout(location = 3) in vec3 aOffset;//instancing arrays

out vec3 skyboxCoord;
out vec2 outUV;

uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;

void main()
{
    gl_Position = projection_matrix * view_matrix * model_matrix * vec4(position + aOffset, 1.0);
	outUV = UV;
	skyboxCoord = position;
}