#version 150

varying vec3 rgbColor0;
varying vec2 texCoords0;

uniform int pixilation;

uniform sampler2D diffuse;

void main()
{	
	vec4 s1 = texture(diffuse, texCoords0 - 1.0 / 300.0 - 1.0 / 200.0) * vec4(rgbColor0, 1);
	vec4 s2 = texture(diffuse, texCoords0 + 1.0 / 300.0 - 1.0 / 200.0) * vec4(rgbColor0, 1);
	vec4 s3 = texture(diffuse, texCoords0 - 1.0 / 300.0 + 1.0 / 200.0) * vec4(rgbColor0, 1);
	vec4 s4 = texture(diffuse, texCoords0 + 1.0 / 300.0 + 1.0 / 200.0) * vec4(rgbColor0, 1);
	vec4 sx = 4.0 * ((s4 + s3) - (s2 + s1));
	vec4 sy = 4.0 * ((s2 + s4) - (s1 + s3));
	vec4 sobel = sqrt(sx * sx + sy * sy);
	gl_FragColor = sobel;
}