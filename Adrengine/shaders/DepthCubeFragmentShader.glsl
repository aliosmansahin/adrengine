#version 330 core

in vec3 FragPos;

uniform vec3 lightPos;
uniform float far_plane;

out vec4 FragColor;

void main() {
    float lightDistance = length(FragPos - lightPos);
    // normalize to [0,1] range
    float depth = lightDistance / far_plane;
    
    // Instead of using gl_FragDepth, write the normalized depth to a depth texture.
    // The depth texture will be automatically filled with the fragment's depth value
    FragColor = vec4(vec3(depth), 1.0);
}