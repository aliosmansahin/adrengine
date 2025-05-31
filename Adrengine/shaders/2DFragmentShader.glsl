#version 330 core

in vec2 TexCoord;

uniform sampler2D texture1;

//Tile indicator
//uniform vec3 indicatorColor; //Not using right now
uniform int tileWidth;
uniform int tileHeight;
uniform bool drawTileIndicator;

out vec4 FragColor;

void main() {
	if(drawTileIndicator) {
		float lineWidth = 3.0;

		// Location of current pixel
		float x = TexCoord.x * tileWidth;
		float y = TexCoord.y * tileHeight;

		// How much is this close to border
		if (x < lineWidth || y < lineWidth || x > tileWidth - lineWidth || y > tileHeight - lineWidth) { //Draws lines on the all of borders of the tile
			FragColor = vec4(0.2, 0.6, 0.2, 1.0); // white line
		}
		else {
			FragColor = vec4(0.0, 0.0, 0.0, 0.0); // transparent
		}
	}
	else {
		FragColor = texture(texture1, TexCoord);
	}
}