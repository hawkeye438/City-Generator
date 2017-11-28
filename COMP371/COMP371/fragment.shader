#version 330 core

in vec3 skyboxCoord;//skybox
in vec2 outUV;//cube uv for texture rendering
in vec3 outNormal; // The vertice's normal vector
in vec3 fragPosition;
in vec3 lightPosition;
in vec4 ShadowCoord;
in vec4 viewSpace;

out vec4 color;

uniform sampler2D textureNumber[6];//multiple texture support, simply change array size to numnber of texture you want
uniform samplerCube skybox; //skybox sampling
uniform sampler2D shadowMap;

uniform bool drawingSkybox;
uniform int textureOption;
uniform mat4 textureMatrix;//for scaling uv and allow texture to scale
uniform bool scaleUV;//only scale the when true

in vec3 viewPosition;

// Fog uniforms
uniform int fogOption;
uniform int fogDebugDepth;
// Linear fog
uniform float fogStart;
uniform float fogEnd;
// Exponential fog
uniform float fogDensity;

// Fog constants
const vec3 fogColour = vec3(0.4f, 0.4f, 0.4f);

// Scattering fog
const vec3 be = vec3(0.042, 0.042, 0.042); // Fog extinction factors
const vec3 bi = vec3(0.1, 0.1, 0.1); // Fog inscattering factors

// Return the depth in grayscale color
vec4 debugDepthFog(float fogFactor)
{
	fogFactor = 1 - fogFactor;
	return vec4(fogFactor, fogFactor, fogFactor, 1.0);
}

vec4 scatteringFog(float dist)
{
	dist = -dist;
	// Scattering algorithm from http://www.iquilezles.org/www/articles/fog/fog.htm
	vec3 extColour = vec3(exp(dist * be.x), exp(dist * be.y), exp(dist * be.z));
	vec3 insColour = vec3(exp(dist * bi.x), exp(dist * bi.y), exp(dist * bi.z));
	return vec4(vec3(color) * extColour + (1 - insColour) * fogColour, color[3]);
}

vec4 applyFog()
{
	float dist = length(viewSpace);
	float fogFactor;
	if (fogOption == 1) // Linear fog
		fogFactor = clamp((fogEnd - dist) / (fogEnd - fogStart), 0.0, 1.0);
	else if (fogOption == 2) // Exponential fog
		fogFactor = clamp(1.0 / exp((dist * fogDensity)), 0.0, 1.0);
	else if (fogOption == 3) // Exponential squared fog
		fogFactor = clamp(1.0 / exp((dist * fogDensity) * (dist * fogDensity)), 0.0, 1.0);
	else if (fogOption == 4) // Exponential fog split (extinction/inscattering)
		return scatteringFog(dist);
	else
		return color;
	if (fogDebugDepth == 1)
		return debugDepthFog(fogFactor);
	return vec4(mix(fogColour, vec3(color), fogFactor), color[3]);
}

//Shadow algorithm referenced from www.learnopengl.com
float ShadowCalculation(vec4 fragPosLightSpace)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow
    float shadow = currentDepth > closestDepth  ? 1.0 : 0.0;
	if(projCoords.z > 1.0)
        shadow = 0.0;

    return shadow;
} 

//Light algorithms referenced from www.learnOpengl.com
struct DirLight {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
	
	float ambient_strength;
};  
uniform DirLight dirLight;

struct PointLight {      

	vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
	
	float ambient_strength;
	float constant;
    float linear;
    float quadratic;
};
#define NR_POINT_LIGHTS 26  
uniform PointLight pointLights[NR_POINT_LIGHTS];

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 fragPos, vec3 viewDir, float shadow)
{
    vec3 lightDir = normalize(lightPosition - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
	float specular_strength = 0.5f;
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    // combine results
    vec3 ambient  = light.ambient;
    vec3 diffuse  = (1.0 - shadow) * light.diffuse  * diff * light.ambient_strength;
    vec3 specular = (1.0 - shadow) * light.specular * spec * specular_strength;
    return (ambient + diffuse + specular);
}  

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, float shadow)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    float specular_strength = 0.1f;
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    // attenuation
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
  			     light.quadratic * (distance * distance));    
    // combine results
    vec3 ambient  = light.ambient;
    vec3 diffuse  = (1.0 - shadow) * light.diffuse  * diff  * light.ambient_strength;
    vec3 specular = (1.0 - shadow) * light.specular * spec * specular_strength;
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
} 

void main()
{	//UV scaling for repeated texture rather than texture stretching
	vec4 UV = vec4(outUV, 0.0f, 0.0f);//convert to vec4
	vec4 newUV = textureMatrix * UV;//Scale the UV by x and y
	vec2 lastUV = vec2(newUV);//convert to vec2
	
	vec3 finalColour;
	vec3 lamp;
	vec3 moon;
	
	vec3 norm = normalize(outNormal);
	vec3 viewDir = normalize(viewPosition - fragPosition);
	
	//must not have 4 arguments
	//vec3 cubeColour = vec3(1.0f, 0.8f, 0.8f);
	vec3 cubeColour = vec3(1.0f, 1.0f, 1.0f);
	
	//Shadows
	//float bias = 0.005;
	//float visibility = 1.0f;
	//if ( texture( shadowMap, ShadowCoord.xy ).z  <  ShadowCoord.z){
	//	visibility = 0.3f;
	//}
	
    vec3 lightColour[26];
	lightColour[0] = vec3(0.13333333f, 0.23921569f, 0.41960784f);
	//lightColour[0] = vec3(0.9f, 0.9f, 0.9f);
	
	for (int i = 1; i < 26; i++) {
		lightColour[i] = vec3(0.81f, 0.64f, 0.07f);
	}
	
	float shadow = 0.0f;
    //float shadow = ShadowCalculation(ShadowCoord);

	moon = CalcDirLight(dirLight, norm, fragPosition, viewDir, shadow);
	
	//lamp = vec3(1.0f, 1.0f, 1.0f);
	for(int i = 0; i < NR_POINT_LIGHTS; i++)
        lamp += CalcPointLight(pointLights[i], norm, fragPosition, viewDir, shadow);
	
	finalColour = lamp * cubeColour * moon;

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
	else if (textureOption == 5) {
		if (scaleUV) {
			color = texture(textureNumber[4], lastUV) * vec4(finalColour, 1.0f);
		}
		else {
			color = texture(textureNumber[4], outUV) * vec4(finalColour, 1.0f);
		}
	}
	else if (textureOption == 6) {
		if (scaleUV) {
			color = texture(textureNumber[5], lastUV) * vec4(finalColour, 1.0f);
		}
		else {
			color = texture(textureNumber[5], outUV) * vec4(finalColour, 1.0f);
		}
	}
	color = applyFog();
}
