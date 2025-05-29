#version 330 core

in vec2 TexCoord;

uniform sampler2D texture1;
uniform int tileW;
uniform int tileH;
uniform int textureWidth;
uniform int textureHeight;

out vec4 FragColor;

void main() {
    vec4 texColor = texture(texture1, TexCoord);

    float lineWidth = 1.0;

	// Location of current pixel
	float x = TexCoord.x * textureWidth;
    float y = TexCoord.y * textureHeight;

	// Location in tile
    float fx = mod(x, tileW);
    float fy = mod(y, tileH);

    // How much is this close to border
    //if (fx < lineWidth || fy < lineWidth || fx > tileW - lineWidth || fy > tileH - lineWidth) { //Draws lines on the all of borders of the tile
    if ((fx > tileW - lineWidth || fy > tileH - lineWidth)) { //Draws lines on the end of the borders of the tile
        FragColor = vec4(1.0, 1.0, 1.0, 1.0); // white line
    } else {
        FragColor = texColor; // normal texture
    }
}