#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 texCoord;

uniform mat4 uTransform;
uniform mat4 uProjection;

out vec2 TexCoord;

void main() {
	gl_Position = uProjection * uTransform * vec4(aPos.x, aPos.y, aPos.z, 1.0);
	TexCoord = texCoord;
}