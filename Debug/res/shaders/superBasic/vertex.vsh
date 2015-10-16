#version 330

attribute vec3 position;
attribute vec3 rgbColor;
attribute vec2 texCoords;

varying vec3 rgbColor0;
varying vec2 texCoords0;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;
uniform vec3 overrideColor;

void main()
{
    gl_Position = vec4(position, 1.0);
	
	rgbColor0 = overrideColor * rgbColor;
	texCoords0 = texCoords;
}