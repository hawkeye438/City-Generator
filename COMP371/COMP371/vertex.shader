
#version 330 core
  
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 UV;

out vec3 skyboxCoord;
out vec2 outUV;
out vec3 outNormal;
out vec3 fragPosition; // world coordinates
out vec3 lightPosition;
out vec3 viewPosition;

out vec4 viewSpace; // needed for fog calc

uniform mat4 DepthBiasMVP;
out vec4 ShadowCoord;

uniform mat4 light_matrix;
//uniform mat4 depthMVP;


uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;
uniform mat4 view_specular_matrix;

void main()
{
	viewSpace = view_matrix * model_matrix * vec4(position, 1.0);
    gl_Position = projection_matrix * viewSpace;
	outUV = UV;
	skyboxCoord = position;
	outNormal = vec3(model_matrix * vec4(normal,0.0f));
	fragPosition = vec3(model_matrix * vec4(position.x, position.y, position.z, 1.0)); 
	lightPosition = vec3(light_matrix * vec4(fragPosition.x, fragPosition.y, fragPosition.z, 1.0));
	viewPosition = vec3(viewSpace);
	ShadowCoord = DepthBiasMVP * vec4(fragPosition, 1.0);
}