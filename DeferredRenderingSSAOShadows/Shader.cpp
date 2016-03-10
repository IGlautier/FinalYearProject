#include "Shader.h"



Shader::Shader(const GLchar* vertPath, const GLchar* fragPath)
{
	std::string vertSrc, fragSrc;
	GLint success;
	GLchar errorLog[512];

	std::ifstream vin(vertPath, std::ios::in);
	std::ifstream fin(fragPath, std::ios::in);
	if (vin) vertSrc = std::string((std::istreambuf_iterator<char>(vin)), std::istreambuf_iterator<char>());
	else std::cout << "ERROR: Could not load vertex shader" << std::endl;
	if (fin) fragSrc = std::string((std::istreambuf_iterator<char>(fin)), std::istreambuf_iterator<char>());
	else std::cout << "ERROR: Could not load fragmentation shader" << std::endl;

	const GLchar* vertCode = vertSrc.c_str();
	const GLchar* fragCode = fragSrc.c_str();

	GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertShader, 1, &vertCode, NULL);
	glCompileShader(vertShader);
	glGetShaderiv(vertShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertShader, 512, NULL, errorLog);
		std::cout << "ERROR: Failed to compile vertex shader\n" << errorLog << std::endl;
	}

	GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragShader, 1, &fragCode, NULL);
	glCompileShader(fragShader);
	glGetShaderiv(fragShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragShader, 512, NULL, errorLog);
		std::cout << "ERROR: Failed to compile fragment shader\n" << errorLog << std::endl;
	}

	this->pID = glCreateProgram();
	glAttachShader(this->pID, vertShader);
	glAttachShader(this->pID, fragShader);
	glLinkProgram(this->pID);

	glGetProgramiv(this->pID, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(this->pID, 512, NULL, errorLog);
		std::cout << "ERROR: Failed to link or attach shaders\n" << errorLog << std::endl;
	}

	glDeleteShader(vertShader);
	glDeleteShader(fragShader);

}

Shader::Shader(const GLchar* vertPath, const GLchar* fragPath, const GLchar* geoPath)
{
	std::string vertSrc, fragSrc, geoSrc;
	GLint success;
	GLchar errorLog[512];

	std::ifstream vin(vertPath, std::ios::in);
	std::ifstream fin(fragPath, std::ios::in);
	std::ifstream gin(geoPath, std::ios::in);
	if (vin) vertSrc = std::string((std::istreambuf_iterator<char>(vin)), std::istreambuf_iterator<char>());
	else std::cout << "ERROR: Could not load vertex shader" << std::endl;
	if (fin) fragSrc = std::string((std::istreambuf_iterator<char>(fin)), std::istreambuf_iterator<char>());
	else std::cout << "ERROR: Could not load fragmentation shader" << std::endl;
	if (gin) geoSrc = std::string((std::istreambuf_iterator<char>(gin)), std::istreambuf_iterator<char>());
	else std::cout << "ERROR: Could not load geometry shader" << std::endl;

	const GLchar* vertCode = vertSrc.c_str();
	const GLchar* fragCode = fragSrc.c_str();
	const GLchar* geoCode = geoSrc.c_str();

	GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertShader, 1, &vertCode, NULL);
	glCompileShader(vertShader);
	glGetShaderiv(vertShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertShader, 512, NULL, errorLog);
		std::cout << "ERROR: Failed to compile vertex shader\n" << errorLog << std::endl;
	}

	GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragShader, 1, &fragCode, NULL);
	glCompileShader(fragShader);
	glGetShaderiv(fragShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragShader, 512, NULL, errorLog);
		std::cout << "ERROR: Failed to compile fragment shader\n" << errorLog << std::endl;
	}

	GLuint geoShader = glCreateShader(GL_GEOMETRY_SHADER);
	glShaderSource(geoShader, 1, &geoCode, NULL);
	glCompileShader(geoShader);
	glGetShaderiv(geoShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(geoShader, 512, NULL, errorLog);
		std::cout << "ERROR: Failed to compile geometry shader\n" << errorLog << std::endl;
	}

	this->pID = glCreateProgram();
	glAttachShader(this->pID, vertShader);
	glAttachShader(this->pID, fragShader);
	glAttachShader(this->pID, geoShader);
	glLinkProgram(this->pID);

	glGetProgramiv(this->pID, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(this->pID, 512, NULL, errorLog);
		std::cout << "ERROR: Failed to link or attach shaders\n" << errorLog << std::endl;
	}

	glDeleteShader(vertShader);
	glDeleteShader(fragShader);
	glDeleteShader(geoShader);

}

void Shader::Use() {
	glUseProgram(this->pID);
}

