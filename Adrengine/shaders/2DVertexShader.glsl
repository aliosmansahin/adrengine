#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 texCoord;

uniform mat4 uModel;
uniform mat4 uProjection;
uniform mat4 uView;

out vec2 TexCoord;

void main() {
	gl_Position = uProjection * uView * uModel * vec4(aPos, 1.0);
	TexCoord = texCoord;
}