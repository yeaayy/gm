#include "texture_loader.hpp"

#include <malloc.h>
#include <png.h>

#include "list.tcc"
#include "log.h"

typedef struct ImageData {
	GLuint id;
	int width, height;
} ImageData;

namespace {

char* png_version(char *v){
	int i=png_access_version_number();
	sprintf(v, "%d.%d.%d", i/10000, (i/100)%100, i%100);
	return v;
}

void errorHandler(png_structp png, png_const_charp msg) {
	loge("%s", msg);
}

} // namespace

bool loadPNGTextureIfNeeded(GLuint *dst, const char* name, int *width, int *height, GLenum wrapS, GLenum wrapT, GLenum minFilter, GLenum magFilter){
	if(*dst == 0) {
		*dst = loadPNGTexture(name, width, height, wrapS, wrapT, minFilter, magFilter);
		return true;
	}
	return false;
}

GLuint loadPNGTexture(const char* name, int *outWidth, int *outHeight, GLenum wrapS, GLenum wrapT, GLenum minFilter, GLenum magFilter){
	FILE *file = fopen(name, "rb");
	if(file == nullptr) return 0;
	char v[9];
	png_structp png = png_create_read_struct(png_version(v), NULL, errorHandler, errorHandler);
	png_infop info = png_create_info_struct(png);
	png_init_io(png, file);

	png_read_info(png, info);
	png_uint_32 width = png_get_image_width(png, info);
	png_uint_32 height = png_get_image_height(png, info);
	png_byte depth = png_get_bit_depth(png, info);
	png_byte color = png_get_color_type(png, info);

	GLenum glMode = GL_RGBA;
	int component = 4;
	if(color == PNG_COLOR_TYPE_PALETTE){
		png_set_palette_to_rgb(png);
		png_read_update_info(png, info);
		color = png_get_color_type(png, info);
	} else if(color == PNG_COLOR_TYPE_RGB) {
		png_set_add_alpha(png, 255, PNG_FILLER_AFTER);
	}

	png_bytep image = (png_bytep) malloc(component * width * height);
	for(unsigned int y=0; y<height; y++){
		png_read_row(png, &image[component * y * width], NULL);
	}

	png_read_end(png, info);
	png_destroy_read_struct(&png, &info, NULL);

	if(outWidth != nullptr) *outWidth = width;
	if(outHeight != nullptr) *outHeight = height;

	GLuint texture;
	glGenTextures(1, &texture);
	logp("%s res %dx%d depth:%d color:%d glTexture:%d", name, width, height, depth, color, texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, glMode, GL_UNSIGNED_BYTE, image);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
	return texture;
}