#pragma once
#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <fstream>
#include <streambuf>
#include <glew.h>
#include <iostream>


class Shader
{
public:
	GLuint pID;
	Shader(const GLchar* vertPath, const GLchar* fragPath);
	Shader(const GLchar* vertPath, const GLchar* fragPath, const GLchar* geoPath);
	void Use();
};

#endif