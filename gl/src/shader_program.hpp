#ifndef SHADER_PROGRAM_HPP
#define SHADER_PROGRAM_HPP

#include "gl.h"

#define STRINGIFY(x) #x

class ShaderProgram {
public:
	GLint vertexPositionIndex;
	GLint vertexColorIndex;
	GLint vertexUVIndex;

	GLint unifomMatrix1Index;
	GLint unifomMatrix2Index;
	GLint uniformPositionIndex;
	GLint uniformColorIndex;
	GLint uniformTextureIndex;
	GLint uniformScaleIndex;

	ShaderProgram(const char *name);
	ShaderProgram(const char *vertexSource, const char *fragmentSource);
	void loadString(const char *vertexSource, const char *fragmentSource);
	void loadStringIfNeeded(const char *vertexSource, const char *fragmentSource);
	void use();
	void dump();

private:
	void createShader(GLuint program, GLenum type, const char *source);
	const char *name;
	GLuint program;
	int attributes;
};

#endif /* SHADER_PROGRAM_HPP */
