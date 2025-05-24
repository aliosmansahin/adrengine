#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal;

uniform mat4 uModel;
uniform mat4 uProjection;
uniform mat4 uView;

out vec3 FragPos;
out vec3 Normal;
out vec4 WorldPosition;
out vec2 TexCoord;

void main() {
    WorldPosition = uModel * vec4(aPos, 1.0);
    FragPos = vec3(WorldPosition);
    TexCoord = aTexCoord;
    Normal = mat3(transpose(inverse(uModel))) * aNormal;

    gl_Position = uProjection * uView * WorldPosition;
}