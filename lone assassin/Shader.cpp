#include "Shader.h"

static std::string loadShader(const std::string& fileName);
static GLuint createShader(const std::string& text, GLenum shaderType);
static void CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage);

Shader::Shader(std::string filePath)
{
	//---------------------------------------------------------------------------
	//CREATE PROGRAM AND ADD SHADERS
	//---------------------------------------------------------------------------
	shaderProgram = glCreateProgram();
	vertexShader = createShader(loadShader("./res/shaders/" + filePath + "/vertex.vsh"), GL_VERTEX_SHADER);
	fragmentShader = createShader(loadShader("./res/shaders/" + filePath + "/fragment.fsh"), GL_FRAGMENT_SHADER);
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);

	//---------------------------------------------------------------------------
	//ATTRIBUTES
	//---------------------------------------------------------------------------
	glBindAttribLocation(shaderProgram, 0, "position");
	glBindAttribLocation(shaderProgram, 1, "rgbColor");
	glBindAttribLocation(shaderProgram, 2, "texCoords");
	
	//---------------------------------------------------------------------------
	//LINK PROGRAM
	//---------------------------------------------------------------------------
	glLinkProgram(shaderProgram);
	CheckShaderError(shaderProgram, GL_LINK_STATUS, true, "Error: Program linking failed: ");
	glValidateProgram(shaderProgram);
	CheckShaderError(shaderProgram, GL_VALIDATE_STATUS, true, "Error: Program is invalid: ");

	//---------------------------------------------------------------------------
	//UNIFORMS
	//---------------------------------------------------------------------------
	//transform
	uniforms[TRANSFORM] = glGetUniformLocation(shaderProgram, "transform");

	//color
	uniforms[OVERRIDE_COLOR] = glGetUniformLocation(shaderProgram, "overrideColor");
	glUniform3f(uniforms[OVERRIDE_COLOR], 1.0f, 1.0f, 1.0f);
}

Shader::~Shader()
{
	glDetachShader(shaderProgram, vertexShader);
	glDeleteShader(vertexShader);
	glDetachShader(shaderProgram, fragmentShader);
	glDeleteShader(fragmentShader);
}

void Shader::update(glm::mat4 transform, glm::vec4 color)
{
	glUniform4f(uniforms[OVERRIDE_COLOR], color.r, color.g, color.b, color.a);
	glUniformMatrix4fv(uniforms[TRANSFORM], 1, GL_FALSE, glm::value_ptr(transform));
}

void Shader::bind()
{
	glUseProgram(shaderProgram);
}

GLuint Shader::getProgram()
{
	return shaderProgram;
}

static std::string loadShader(const std::string& fileName)
{
	std::ifstream file;
	file.open(fileName.c_str());

	std::string output;
	std::string line;

	if (file.is_open())
	{
		while (file.good())
		{
			getline(file, line);
			output.append(line + "\n");
		}
	}
	else
	{
		std::cerr << "Unable to load shader: " << fileName << std::endl;
	}

	return output;
}

static GLuint createShader(const std::string& text, GLenum shaderType)
{
	GLuint shader = glCreateShader(shaderType);

	if (shader == 0)
		std::cerr << "Error: Shader Creation failed!" << std::endl;

	const GLchar* shaderSourceStrings[1];
	GLint shaderSourceStringLengths[1];

	shaderSourceStrings[0] = text.c_str();
	shaderSourceStringLengths[0] = text.length();

	glShaderSource(shader, 1, shaderSourceStrings, shaderSourceStringLengths);
	glCompileShader(shader);

	CheckShaderError(shader, GL_COMPILE_STATUS, false, "Error: Program compilation failed: ");

	return shader;
}

static void CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage)
{
	GLint success = 0;
	GLchar error[1024] = { 0 };

	if (isProgram)
		glGetProgramiv(shader, flag, &success);
	else
		glGetShaderiv(shader, flag, &success);

	if (success == GL_FALSE)
	{
		if (isProgram)
			glGetProgramInfoLog(shader, sizeof(error), NULL, error);
		else
			glGetShaderInfoLog(shader, sizeof(error), NULL, error);

		std::cerr << errorMessage << ": '" << error << "'" << std::endl;
	}
}