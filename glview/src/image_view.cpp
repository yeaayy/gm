#include "view/image_view.hpp"

#include <string.h>

#include "texture_loader.hpp"
#include "shader_program.hpp"

#include "log.h"

namespace {

ShaderProgram program("shader/text");
GLuint vbo = 0;

} // namespace

ImageView::ImageView(View *parent,
					 ViewSize sizeX,
					 ViewSize sizeY)
	: View(parent, sizeX, sizeY)
{
	imagesName = new char*[1];
	imagesId = nullptr;
	count = 1;
}

void ImageView::setImage(const char *name)
{
	setImage(0, name);
}

void ImageView::setImage(int idx, const char *name)
{
	if(imagesId != nullptr) {
		loge("error: ImageView: cannot set image after initialized");
	}
	if(toggleCycle != count) {
		delete[] imagesName;
		imagesName = new char*[toggleCycle];
		count = toggleCycle;
	}
	imagesName[idx] = strdup(name);
}

void ImageView::onInitGL()
{
	imagesId = new unsigned int[count];
	program.loadStringIfNeeded(
		#include "shader/text.vertex.glsl"
		,
		#include "shader/text.fragment.glsl"
	);
	for(int i=0; i<count; i++) {
		imagesId[i] = loadPNGTexture(imagesName[i]);
	}
	if(vbo == 0) {
		float vertex[16] = {
			-1, -1, 0, 1,
			 1, -1, 1, 1,
			 1,  1, 1, 0,
			-1,  1, 0, 0
		};
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, 16 * sizeof(float), (GLvoid*) vertex, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}

void ImageView::onRender()
{
	float color[4] = {1.0f, 1.0f, 1.0f, 1.0f};
	program.use();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, imagesId[toggleState]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glUniform1i(program.uniformTextureIndex, 0);

	glUniform4fv(program.uniformColorIndex, 1, color);

	glVertexAttribPointer(program.vertexPositionIndex, 2, GL_FLOAT, false, 16, (GLvoid*) 0);
	glVertexAttribPointer(program.vertexUVIndex, 2, GL_FLOAT, false, 16, (GLvoid*) 8);

	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	View::onRender();
}
