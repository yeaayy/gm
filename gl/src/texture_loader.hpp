#ifndef TEXTURE_LOADER_HPP
#define TEXTURE_LOADER_HPP

#include "gl.h"

GLuint loadPNGTexture(const char* name,
					  int *width = 0, int *height = 0,
					  GLenum wrapS = GL_CLAMP_TO_EDGE,
					  GLenum wrapT = GL_CLAMP_TO_EDGE,
					  GLenum minFilter = GL_NEAREST,
					  GLenum magFilter = GL_NEAREST);
bool loadPNGTextureIfNeeded(GLuint *dst,
							const char *name,
							int *width = 0, int *height = 0,
							GLenum wrapS = GL_CLAMP_TO_EDGE,
							GLenum wrapT = GL_CLAMP_TO_EDGE,
							GLenum minFilter = GL_NEAREST,
							GLenum magFilter = GL_NEAREST);

#endif /* TEXTURE_LOADER_HPP */