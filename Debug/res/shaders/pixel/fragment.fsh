#version 150

varying vec3 rgbColor0;
varying vec2 texCoords0;

uniform int pixilation;

uniform sampler2D diffuse;

void main()
{	
//	if ((int)texCoords0.x % pixilation0 == 0 && (int)texCoords0.y % pixilation0 == 0)
//	{
//		for (int x = 0; x < pixilation0; x++)
//		{
//			for (int y = 0; y < pixilation0; y++)
//			{
				//gl_FragColor = (texture2D(diffuse, vec2(texCoords0.x, texCoords0.y)) * vec4(rgbColor0, 1));
//			}
//		}
//	}
	
	
	//vec2 newTexCoords = vec2((texCoords0.x / pixilation0) * pixilation0, (texCoords0.y / pixilation0) * pixilation0);
	
	//gl_FragColor = (texture(diffuse, vec2(float((int(texCoords0.x * 1000) / pixilation) * pixilation) / 1000, float((int(texCoords0.y * 1000) / pixilation) * pixilation) / 1000)) * vec4(rgbColor0, 1));
	
	gl_FragColor = (texture(diffuse, vec2(float(int(texCoords0.x * 1000) - (int(texCoords0.x * 1000) % pixilation)) / 1000, float(int(texCoords0.y * 1000) - (int(texCoords0.y * 1000) % pixilation)) / 1000)) * vec4(rgbColor0, 1));
	
	if ((texCoords0.x < 0.01 || texCoords0.x > 0.99) || (texCoords0.y < 0.01 || texCoords0.y > 0.99))
		gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0) * vec4(rgbColor0, 1);
	
	//gl_FragColor = (texture(diffuse, texCoords0) * vec4(rgbColor0, 1));
}