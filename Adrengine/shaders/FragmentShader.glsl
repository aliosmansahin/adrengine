#version 330 core

//DEFINITIONS FOR TYPES OF LIGHTS
#define LIGHT_TYPE_DIRECTIONAL 0
#define LIGHT_TYPE_POINT 1
#define LIGHT_TYPE_SPOT 2

//Result color of shader
out vec4 FragColor;

//Light struct and variables
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

//Got from vertex shader
in vec3 Normal;
in vec3 FragPos;
in vec4 WorldPosition;
in vec2 TexCoord;

//Current Position
uniform vec3 viewPos;

//Texture
uniform sampler2D objTexture;
uniform bool hasTexture;
uniform bool useGammaCorrection;

//Material
uniform vec3 materialAmbient;   // Ka
uniform vec3 materialDiffuse;   // Kd
uniform vec3 materialSpecular;  // Ks
uniform float materialShininess; // Ns (Brightness)
uniform float materialOpacity;   // d (Opacity)
uniform vec3 materialEmission;   // Ke (emissive)
uniform float materialRefractiveIndex; // Ni
uniform int materialIllum;       // illum model

//Shadows
uniform sampler2D shadowMaps[MAX_LIGHTS];
uniform mat4 lightSpaceMatrices[MAX_LIGHTS];
uniform samplerCube shadowCubeMaps[MAX_LIGHTS];
uniform float far_planes[MAX_LIGHTS];

//For point lights
const vec2 gridSamplingDisk[20] = vec2[20](
    vec2(1.0, 0.0), vec2(-1.0, 0.0), vec2(0.0, 1.0), vec2(0.0, -1.0),
    vec2(0.707, 0.707), vec2(0.707, -0.707), vec2(-0.707, 0.707), vec2(-0.707, -0.707),
    vec2(0.5, 0.5), vec2(0.5, -0.5), vec2(-0.5, 0.5), vec2(-0.5, -0.5),
    vec2(0.3, 0.3), vec2(0.3, -0.3), vec2(-0.3, 0.3), vec2(-0.3, -0.3),
    vec2(0.0, 0.0), vec2(1.0, 1.0), vec2(-1.0, -1.0), vec2(1.0, -1.0)
);

//Function to calculate shadow of point lights
float PointLightShadow(vec3 fragPos, int lightIndex, vec3 lightPos) {
    vec3 fragToLight = fragPos - lightPos;
    float currentDepth = length(fragToLight);
    float shadow = 0.0;
    int samples = 20;
    float bias = 0.025;
    float offset = 0.05;

    for (int i = 0; i < samples; ++i) {
        float closestDepth = texture(shadowCubeMaps[lightIndex], fragToLight + offset * vec3(gridSamplingDisk[i], 0.0)).r;
        closestDepth *= far_planes[lightIndex];
        if (currentDepth - bias > closestDepth)
            shadow += 1.0;
    }
    shadow /= float(samples);
    return shadow;
}

//Function to calculate shadow of directional lights
float DirectionalLightShadow(vec4 fragPosLightSpace, vec3 normal, vec3 lightDir, sampler2D shadowMap) {
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;
    float closestDepth = texture(shadowMap, projCoords.xy).r;
    float currentDepth = projCoords.z;
    float bias = max(0.0005 * (1.0 - dot(normal, lightDir)), 0.00005);

    float shadow = 0.0;
    if (projCoords.x < 0.0 || projCoords.x > 1.0 ||
        projCoords.y < 0.0 || projCoords.y > 1.0 ||
        projCoords.z > 1.0)
        return 0.0;

    int sampleRadius = 2;
    vec2 pixelSize = 1.0 / textureSize(shadowMap, 0);
    for (int y = -sampleRadius; y <= sampleRadius; y++) {
        for (int x = -sampleRadius; x <= sampleRadius; x++) {
            float sampledDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * pixelSize).r;
            if (currentDepth > sampledDepth + bias)
                shadow += 1.0;
        }
    }
    shadow /= pow((sampleRadius * 2 + 1), 2);
    return shadow;
}

//Function to calculate shadow of spot lights
float SpotLightShadow(vec4 fragPosLight, vec3 lightDirection, vec3 normal, sampler2D shadowMap) {
    float shadow = 0.0f;
	// Sets lightCoords to cull space
	vec3 lightCoords = fragPosLight.xyz / fragPosLight.w;
	if(lightCoords.z <= 1.0f)
	{
		// Get from [-1, 1] range to [0, 1] range just like the shadow map
		lightCoords = (lightCoords + 1.0f) / 2.0f;
		float currentDepth = lightCoords.z;
		// Prevents shadow acne
		float bias = max(0.00025f * (1.0f - dot(normal, lightDirection)), 0.000005f);

		// Smoothens out the shadows
		int sampleRadius = 2;
		vec2 pixelSize = 1.0 / textureSize(shadowMap, 0);
		for(int y = -sampleRadius; y <= sampleRadius; y++)
		{
		    for(int x = -sampleRadius; x <= sampleRadius; x++)
		    {
		        float closestDepth = texture(shadowMap, lightCoords.xy + vec2(x, y) * pixelSize).r;
				if (currentDepth > closestDepth + bias)
					shadow += 1.0f;     
		    }    
		}
		// Get average shadow
		shadow /= pow((sampleRadius * 2 + 1), 2);

	}

    return shadow;
}

void main() {
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    // Emissive component (Lights itself)
    vec3 emissive = materialEmission;

    // If the material has a texture
    vec3 diffuseColor = hasTexture ? texture(objTexture, TexCoord).rgb : materialDiffuse;
    vec3 ambient = materialAmbient * diffuseColor;

    vec3 diffuse;
    vec3 specular;

    vec3 result = emissive + ambient;

    for (int i = 0; i < numLights; ++i) {
        vec3 lightDir;
        float attenuation = 1.0;
        float intensity = 1.0;
        float shadow = 0.0;

        // SHADOWS ARE DISABLED EXCEPT FOR DIRECTIONAL
        // Calculate direction of the light and shadow
        if (lights[i].type == LIGHT_TYPE_DIRECTIONAL) {
            lightDir = normalize(-lights[i].direction);
            shadow = DirectionalLightShadow(lightSpaceMatrices[i] * WorldPosition, norm, lightDir, shadowMaps[i]);
        } else {
            lightDir = normalize(lights[i].position - FragPos);
            float dist = length(lights[i].position - FragPos);
            attenuation = 1.0 / (lights[i].constant + lights[i].linear * dist + lights[i].quadratic * dist * dist);
            if (lights[i].type == LIGHT_TYPE_POINT) {
                //shadow = PointLightShadow(FragPos, i, lights[i].position);
                shadow = 0;
            }
        }

        if (lights[i].type == LIGHT_TYPE_SPOT) {
            float theta = dot(lightDir, normalize(-lights[i].direction));
            float epsilon = max(lights[i].cutOff - lights[i].outerCutOff, 0.001); // prevent epsilon = 0
            intensity = clamp((theta - lights[i].outerCutOff) / epsilon, 0.0, 1.0);
            //shadow = SpotLightShadow(lightSpaceMatrices[i] * WorldPosition, lightDir, norm, shadowMaps[i]);
            shadow = 0; // 0 NO SHADOW 1 SHADOW
        }

        // Calculate diffuse and specular (Phong)
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = 0.0;

        if(diff > 0.0)
            spec = pow(max(dot(viewDir, reflectDir), 0.0), materialShininess);

        diffuse = diff * diffuseColor * lights[i].color;
        specular = spec * materialSpecular * lights[i].color;

        // Reduce light effect if there is a shadow
        float shadowFactor = 1.0 - shadow;

        result += intensity * attenuation * shadowFactor * (diffuse + specular);
    }

    // Opacity
    float alpha = materialOpacity;

    // Gamma correction
    if(useGammaCorrection) {
        vec3 gammaCorrected = pow(result, vec3(1.0/2.2));

        FragColor = vec4(gammaCorrected, alpha);
    }
    else {
        FragColor = vec4(result, alpha);
    }

    //----- DEBUG -----
    //Disable gamma correction
    //FragColor = vec4(result, alpha);

    //Only show ambient
    //FragColor = vec4(ambient, 1.0);
    
    //Only show diffuse
    //FragColor = vec4(diffuse, 1.0);
    
    //Only show specular
    //FragColor = vec4(specular, 1.0);
    
    //Only show emissive
    //FragColor = vec4(emissive, 1.0);
}