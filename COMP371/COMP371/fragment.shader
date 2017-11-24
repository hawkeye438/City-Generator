#version 330 core

in vec3 skyboxCoord;//skybox
in vec2 outUV;//cube uv for texture rendering
in vec3 outNormal; // The vertice's normal vector
in vec3 fragPosition;
in vec3 lightPosition;
in vec4 ShadowCoord;

out vec4 color;

uniform sampler2D textureNumber[4];//multiple texture support, simply change array size to numnber of texture you want
uniform samplerCube skybox; //skybox sampling
uniform sampler2D shadowMap;

uniform bool drawingSkybox;
uniform int textureOption;
uniform mat4 textureMatrix;//for scaling uv and allow texture to scale
uniform bool scaleUV;//only scale the when true

uniform vec3 viewPosition;

void main()
{	//UV scaling for repeated texture rather than texture stretching
	vec4 UV = vec4(outUV, 0.0f, 0.0f);//convert to vec4
	vec4 newUV = textureMatrix * UV;//Scale the UV by x and y
	vec2 lastUV = vec2(newUV);//convert to vec2
	
	//must not have 4 arguments
	//vec3 cubeColour = vec3(1.0f, 0.8f, 0.8f);
	vec3 cubeColour = vec3(1.0f, 1.0f, 1.0f);
	vec3 lightColour = vec3(1.0f, 1.0f, 1.0f);
	
	//Shadows
	float bias = 0.005;
	float visibility = 1.0f;
	if ( texture( shadowMap, ShadowCoord.xy ).z  <  ShadowCoord.z){
		visibility = 0.3f;
	}
	
   
	//ambient lighting
	float ambientStrength = 0.3f;
	vec3 ambient_contribution = ambientStrength * lightColour;
	
	// diffuse lighting
	//vec3 lightPosition = vec3(2.0f,0.5f,-1.0f); // coordinates in the x,y,z axis
	vec3 norm = normalize(outNormal);
	
	vec3 light_direction = normalize(lightPosition - fragPosition);
	float indident_degree = max(dot(norm, light_direction), 0.0f);
	vec3 diffuse_contribution = visibility * indident_degree * lightColour;
	
	// specular lighting
	float specularStrength = 0.8f;
	//vec3(2.0f,0.5f,1.5f)
	vec3 viewDir = normalize(viewPosition - fragPosition);
	vec3 reflect_direction = reflect(-light_direction, norm);
	float spec = pow(max(dot(viewDir, reflect_direction), 0.0), 32);
	
	vec3 specular = visibility * specularStrength * spec * lightColour;
	
	//vec3 finalColour = (ambient_contribution + diffuse_contribution + specular) * cubeColour;
	vec3 finalColour =  cubeColour;//to disable lights
	if (drawingSkybox) {
		color = texture(skybox, skyboxCoord);
	}
	else if (textureOption == 1){
		//color = vec4(1.0f, 0.5f, 0.2f, 1.0f);
		if (scaleUV) {
			color = texture(textureNumber[0], lastUV) * vec4(finalColour, 1.0f);
		}
		else {
			color = texture(textureNumber[0], outUV) * vec4(finalColour, 1.0f);
		}
	} 
	else if (textureOption == 2) {
		//color = vec4(1.0f, 0.5f, 0.2f, 1.0f);
		if (scaleUV) {
			color = texture(textureNumber[1], lastUV) * vec4(finalColour, 1.0f);
		}
		else {
			color = texture(textureNumber[1], outUV) * vec4(finalColour, 1.0f);
		}
	}
	else if (textureOption == 3) {
		if (scaleUV) {
			color = texture(textureNumber[2], lastUV) * vec4(finalColour, 1.0f);
		}
		else {
			color = texture(textureNumber[2], outUV) * vec4(finalColour, 1.0f);
		}
	}
	else if (textureOption == 4) {
		if (scaleUV) {
			color = texture(textureNumber[3], lastUV) * vec4(finalColour, 1.0f);
		}
		else {
			color = texture(textureNumber[3], outUV) * vec4(finalColour, 1.0f);
		}
	}
} 