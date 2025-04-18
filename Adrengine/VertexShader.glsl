#version 330 core

layout (location = 0) in vec3 aPos;

uniform mat4 uTransform;
uniform mat4 uProjection;

void main() {
	gl_Position = uProjection * uTransform * vec4(aPos.x, aPos.y, aPos.z, 1.0);
}