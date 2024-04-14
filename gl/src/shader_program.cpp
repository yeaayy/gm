#include "shader_program.hpp"

#include <string.h>

#include "log.h"

ShaderProgram::ShaderProgram(const char *name) {
	this->name = name;
	program = 0;
}

ShaderProgram::ShaderProgram(const char *vertexSource, const char *fragmentSource) {
	loadString(vertexSource, fragmentSource);
}

void ShaderProgram::loadStringIfNeeded(const char *vertexSource, const char *fragmentSource) {
	if(program == 0) {
		loadString(vertexSource, fragmentSource);
	}
}

void ShaderProgram::loadString(const char *vertexSource, const char *fragmentSource) {
	const GLint program = this->program = glCreateProgram();
	createShader(program, GL_VERTEX_SHADER, vertexSource);
	createShader(program, GL_FRAGMENT_SHADER, fragmentSource);
	glLinkProgram(program);
	glValidateProgram(program);

	int length;
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
	if(length != 0) {
		char log[length];
		glGetProgramInfoLog(program, length, &length, log);
		logp("program %d: %s", program, log);
	}

	vertexPositionIndex = glGetAttribLocation(program, "aPosition");
	vertexColorIndex = glGetAttribLocation(program, "aColor");
	vertexUVIndex = glGetAttribLocation(program, "aUV");
	unifomMatrix1Index = glGetUniformLocation(program, "uMatrix");
	unifomMatrix2Index = glGetUniformLocation(program, "uMatrix2");
	uniformPositionIndex = glGetUniformLocation(program, "uPosition");
	uniformTextureIndex = glGetUniformLocation(program, "uTexture");
	uniformColorIndex = glGetUniformLocation(program, "uColor");
	uniformScaleIndex = glGetUniformLocation(program, "uScale");

	glGetProgramiv(program, GL_ACTIVE_ATTRIBUTES, &attributes);
}

void ShaderProgram::createShader(GLuint program, GLenum type, const char *source) {
	GLuint shader = glCreateShader(type);
	const char *header = "#version 100\n";
	const char *sources[] = {
		header,
		source,
	};
	GLint lengths[] = {
		(GLint) strlen(header),
		(GLint) strlen(source),
	};
	glShaderSource(shader, 2, sources, lengths);
	glCompileShader(shader);

	int length;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
	if(length != 0) {
		char log[length];
		glGetShaderInfoLog(shader, length, nullptr, log);
		logp("%s", source);
		logp("%s %s shader: %s", name, type == GL_VERTEX_SHADER ? "vertex" : type == GL_FRAGMENT_SHADER ? "fragment" : "compute", log);
	}
	glAttachShader(program, shader);
}

void ShaderProgram::use() {
	assert_ine(program, 0);

	static ShaderProgram *last = nullptr;
	if(last == this) return;
	glUseProgram(program);
	last = this;

	static int activated = 0;
	int activating = attributes;
	if(activated == activating) return;
	if(activated > activating) {
		for(int i=activating; i<activated; i++)
			glDisableVertexAttribArray(i);
	} else {
		for(int i=activated; i<activating; i++)
			glEnableVertexAttribArray(i);
	}
	activated = activating;
}

void ShaderProgram::dump()
{
	logp("attributes = %d", attributes);
	logp("aPosition = %d", vertexPositionIndex);
	logp("aColor = %d", vertexColorIndex);
	logp("aUV = %d", vertexUVIndex);
	logp("uMatrix = %d", unifomMatrix1Index);
	logp("uMatrix2 = %d", unifomMatrix2Index);
	logp("uPosition = %d", uniformPositionIndex);
	logp("uTexture = %d", uniformTextureIndex);
	logp("uColor = %d", uniformColorIndex);
	logp("uScale = %d\n", uniformScaleIndex);
}
