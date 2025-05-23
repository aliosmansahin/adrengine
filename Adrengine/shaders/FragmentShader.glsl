#version 330 core

#define LIGHT_TYPE_DIRECTIONAL 0
#define LIGHT_TYPE_POINT 1
#define LIGHT_TYPE_SPOT 2

out vec4 FragColor;

struct Light {
    int type;
    vec3 position;
    vec3 direction;
    vec3 color;

    float cutOff;
    float outerCutOff;
    float constant;
    float linear;
    float quadratic;
};

#define MAX_LIGHTS 10
uniform int numLights;
uniform Light lights[MAX_LIGHTS];

in vec3 Normal;  
in vec3 FragPos;
in vec4 WorldPosition;

uniform vec3 viewPos;
uniform vec3 objectColor;

uniform sampler2D texture1;  // Normal texture

uniform sampler2D shadowMaps[MAX_LIGHTS];
uniform mat4 lightSpaceMatrices[MAX_LIGHTS];

uniform samplerCube shadowCubeMaps[MAX_LIGHTS];
uniform float far_planes[MAX_LIGHTS];

const vec2 gridSamplingDisk[20] = vec2[20](
    vec2(1.0, 0.0), vec2(-1.0, 0.0), vec2(0.0, 1.0), vec2(0.0, -1.0),
    vec2(0.707, 0.707), vec2(0.707, -0.707), vec2(-0.707, 0.707), vec2(-0.707, -0.707),
    vec2(0.5, 0.5), vec2(0.5, -0.5), vec2(-0.5, 0.5), vec2(-0.5, -0.5),
    vec2(0.3, 0.3), vec2(0.3, -0.3), vec2(-0.3, 0.3), vec2(-0.3, -0.3),
    vec2(0.0, 0.0), vec2(1.0, 1.0), vec2(-1.0, -1.0), vec2(1.0, -1.0)
);

float PointLightShadow(vec3 fragPos, int lightIndex, vec3 lightPos)
{
    vec3 fragToLight = fragPos - lightPos;
    float currentDepth = length(fragToLight);

    float shadow = 0.0;
    int samples = 20;
    float bias = 0.025;
    float offset = 0.05;

    for(int i = 0; i < samples; ++i)
    {
        float closestDepth = texture(shadowCubeMaps[lightIndex], fragToLight + offset * vec3(gridSamplingDisk[i], 0.0)).r;
        closestDepth *= far_planes[lightIndex];

        if(currentDepth - bias > closestDepth)
            shadow += 1.0;
    }

    shadow /= float(samples);
    return shadow;
}


float ShadowCalculation(vec4 fragPosLightSpace, vec3 normal, vec3 lightDir, sampler2D shadowMap)
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
    float bias = max(0.0005 * (1.0 - dot(normal, lightDir)), 0.00005);

    float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;

    if (projCoords.x < 0.0 || projCoords.x > 1.0 ||
        projCoords.y < 0.0 || projCoords.y > 1.0 ||
        projCoords.z > 1.0)
        return 0.0;

    int sampleRadius = 2;
		vec2 pixelSize = 1.0 / textureSize(shadowMap, 0);
		for(int y = -sampleRadius; y <= sampleRadius; y++)
		{
		    for(int x = -sampleRadius; x <= sampleRadius; x++)
		    {
		        float closestDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * pixelSize).r;
				if (currentDepth > closestDepth + bias)
					shadow += 1.0f;     
		    }    
		}
		// Get average shadow
		shadow /= pow((sampleRadius * 2 + 1), 2);

    return shadow;
}

void main() {
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 result = vec3(0.1);  // Ambiyans ýþýðý

    for (int i = 0; i < numLights; ++i) {
        vec4 FragPosLightSpace = lightSpaceMatrices[i] * WorldPosition;
        vec3 lightDir;
        float attenuation = 1.0;
        float intensity = 1.0;
        float shadow = 0.0f;

        if(lights[i].type == LIGHT_TYPE_DIRECTIONAL) {
            lightDir = normalize(-lights[i].direction);
            
            shadow = ShadowCalculation(FragPosLightSpace, norm, lightDir, shadowMaps[i]);  
        }
        else {
            lightDir = normalize(lights[i].position - FragPos);
            float dist = length(lights[i].position - FragPos);
            attenuation = 1.0f / (lights[i].constant + lights[i].linear * dist + lights[i].quadratic * dist * dist);


            if (lights[i].type == LIGHT_TYPE_POINT) {
                shadow = PointLightShadow(FragPos, i, lights[i].position);
            }
        }

        if(lights[i].type == LIGHT_TYPE_SPOT) {
            float theta = dot(lightDir, normalize(-lights[i].direction));
            float epsilon = lights[i].cutOff - lights[i].outerCutOff;
            intensity = clamp((theta - lights[i].outerCutOff) / epsilon, 0.0, 1.0);
            shadow = ShadowCalculation(FragPosLightSpace, norm, lightDir, shadowMaps[i]);  
        }

        // Diffuse ve Specular hesaplamalarý
        vec3 halfwayDir = normalize(lightDir + viewDir);
        float diff = max(dot(norm, lightDir), 0.0);
        float spec = pow(max(dot(viewDir, halfwayDir), 0.0), 32);

        vec3 diffuse = diff * lights[i].color;
        vec3 specular = spec * lights[i].color;
        
        // Gölge varsa, aydýnlatma etkisini azaltýyoruz
        result += intensity * attenuation * (diffuse + specular) * (1.0f - shadow);
        result = clamp(result, 0.0, 1.0); // ya da tonemapping ile sýnýrlama
    }
    vec3 gammaCorrected = pow(result * objectColor, vec3(1.0/2.2));
    FragColor = vec4(gammaCorrected, 1.0);
}