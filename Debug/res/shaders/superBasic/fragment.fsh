#version 330

varying vec3 rgbColor0;
varying vec2 texCoords0;

uniform int pixilation;

uniform sampler2D diffuse;

void main()
{
	gl_FragColor = texture(diffuse, vec2(texCoords0.x, texCoords0.y));	
}