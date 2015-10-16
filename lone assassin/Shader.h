#pragma once
#include "globalVariablesAndIncludes.h"
class Shader
{
public:
	Shader(std::string filePath);
	~Shader();
	void bind();
	void update(glm::mat4 transform, glm::vec4 color = glm::vec4(0, 0, 0, 0));
	GLuint getProgram();

private:

	enum
	{
		TRANSFORM,
		OVERRIDE_COLOR,

		NUM_UNIFORMS
	};
	GLuint uniforms[NUM_UNIFORMS];

	GLuint vertexShader;
	GLuint fragmentShader;

	GLuint shaderProgram;
};

