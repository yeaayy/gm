#include "view/text_view.hpp"

#include <string.h>
#include <malloc.h>

#include "shader_program.hpp"
#include "texture_loader.hpp"

#include "gl.h"
#include "log.h"

#define max(x, y) ((x)>(y)?(x):(y))

namespace {

GLuint texture = 0;
GLuint ibo = 0;
int iboCapacity = 0;
ShaderProgram program("shader/text");

const int metric[96] = {
	3, 2, 5, 6, 6, 6, 6, 3, 5, 5, 5, 6, 2, 6, 2, 6,
	6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 2, 2, 5, 6, 5, 6,
	7, 6, 6, 6, 6, 6, 6, 6, 6, 4, 6, 6, 6, 6, 6, 6,
	6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 4, 5, 4, 6, 6,
	3, 6, 6, 6, 6, 6, 5, 6, 6, 2, 6, 5, 3, 6, 6, 6,
	6, 6, 6, 6, 4, 6, 6, 6, 6, 6, 6, 5, 2, 5, 7, 6 
};

} // namespace

TextView::TextView(View *parent,
				   ViewSize sizeX,
				   ViewSize sizeY)
	: View(parent, sizeX, sizeY)
{
	size = 36.0f;
	text = strdup("[default]\n%example/2\nHello World\nSome long text");
	color[0] = 1.0f;
	color[1] = 1.0f;
	color[2] = 1.0f;
	color[3] = 1.0f;
	needUpdateBuffer = true;
	align = TEXT_ALIGN_LEFT_START;
}

void TextView::onInitGL()
{
	View::onInitGL();
	loadPNGTextureIfNeeded(&texture, "../assets/font.png");
	program.loadStringIfNeeded(
		#include "shader/text.vertex.glsl"
		,
		#include "shader/text.fragment.glsl"
	);
	glGenBuffers(1, &vbo);
	if(ibo == 0) {
		glGenBuffers(1, &ibo);
	}
	logp("text vbo: %d, ibo: %d", vbo, ibo);
}

void TextView::onResize(float maxWidth, float maxHeight)
{
	if((align & TEXT_ALIGN_LEFT)) {
		mLeft = 0;
		mRight = 0;
	} else if((align & TEXT_ALIGN_RIGHT)) {
		mLeft = -1;
		mRight = 0;
	} else {
		mLeft = -1;
		mRight = -1;
	}
	if((align & TEXT_ALIGN_START)) {
		mTop = 0;
		mBottom = 0;
	} else if((align & TEXT_ALIGN_END)) {
		mTop = -1;
		mBottom = 0;
	} else {
		mTop = -1;
		mBottom = -1;
	}
	updateBuffer(maxWidth, maxHeight);
	prefWidth = textWidth * size;
	prefHeight = textHeight * size;
	View::onResize(maxWidth, maxHeight);
}

void TextView::updateBuffer(float maxWidth, float maxHeight)
{
	// TODO: respect max width and height, it currently ignored
	if(!needUpdateBuffer) return;
	needUpdateBuffer = false;

	int renderedTextCount = 0;
	int lineCount = 1;
	textWidth = 0;
	float x;
	char currentChar = '\0';
	// count the line
	for(int i=0; ( currentChar = text[i] ); i++) {
		if(currentChar == '\n'){
			lineCount++;
		}else if(currentChar != ' '){
			renderedTextCount++;
		}
	}

	// count width of each line
	float lineWidth[lineCount];
	memset(lineWidth, 0, lineCount * sizeof(float));
	x = 0;
	y = 0;
	int line = 0;
	for(int i=0; ( currentChar = text[i] ); i++) {
		if(currentChar == '\n'){
			lineWidth[line] = x;
			line++;
			textWidth = max(x, textWidth);
			x = 0;
			continue;
		}
		x += (float) metric[(int) currentChar - 32] / 8;
	}
	lineWidth[line] = x;
	textWidth = max(x, textWidth);
	textHeight = lineCount;
	drawCount = renderedTextCount * 6;

	float vertexBuffer[renderedTextCount * 16];
	#define SET_OFFSET_X \
		x = align & TEXT_ALIGN_LEFT  ? -1 : \
		  ( align & TEXT_ALIGN_RIGHT ? 1 - lineWidth[j] * 2 / textWidth \
									 : - lineWidth[j] / textWidth )
	int j = 0;
	SET_OFFSET_X;
	y = 1;
	int X, Y;
	for(int i=0, n=0; ( currentChar = text[i] ); i++) {
		float charWidth = (float) metric[(int) currentChar - 32] / 8;
		if(currentChar == '\n'){
			y -= 2.0f / textHeight;
			j++;
			SET_OFFSET_X;
			continue;
		}else if(currentChar == ' '){
			x += charWidth * 2 / textWidth;
			continue;
		}
		Y = (int) currentChar / 16;
		X = (int) currentChar % 16;
		#define PushVertexBuffer(N, vx, vy, tx, ty) \
			vertexBuffer[n * 16 + N    ] = x + vx * 2 / textWidth;\
			vertexBuffer[n * 16 + N + 1] = y + vy * 2 / textHeight;\
			vertexBuffer[n * 16 + N + 2] = ((float) X + tx) / 16;\
			vertexBuffer[n * 16 + N + 3] = ((float) Y + ty) / 16
		PushVertexBuffer( 0,         0,  0,          0, 0);
		PushVertexBuffer( 4, charWidth,  0,  charWidth, 0);
		PushVertexBuffer( 8, charWidth, -1,  charWidth, 1);
		PushVertexBuffer(12,         0, -1,          0, 1);
		#undef PushVertexBuffer
		n++;
		x += charWidth * 2 / textWidth;
	}
	#undef SET_OFFSET_X

	updateIBO(renderedTextCount);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, renderedTextCount * 16 * sizeof(float), (GLvoid*) vertexBuffer, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void TextView::updateIBO(int charCount)
{
	if(iboCapacity >= charCount) return;
	GLushort indexBuffer[charCount * 6];
	for(int i=0; i<charCount; i++) {
		indexBuffer[i * 6    ] = i * 4;
		indexBuffer[i * 6 + 1] = i * 4 + 1;
		indexBuffer[i * 6 + 2] = i * 4 + 2;
		indexBuffer[i * 6 + 3] = i * 4 + 2;
		indexBuffer[i * 6 + 4] = i * 4 + 3;
		indexBuffer[i * 6 + 5] = i * 4;
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, charCount * 6 * sizeof(*indexBuffer), (GLvoid*) indexBuffer, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	iboCapacity = charCount;
}

void TextView::onRender()
{
	View::onRender();

	program.use();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

	glUniform1i(program.uniformTextureIndex, 0);

	glUniform4fv(program.uniformColorIndex, 1, color);

	glVertexAttribPointer(program.vertexPositionIndex, 2, GL_FLOAT, false, 16, (GLvoid*) 0);
	glVertexAttribPointer(program.vertexUVIndex, 2, GL_FLOAT, false, 16, (GLvoid*) 8);

	glDrawElements(GL_TRIANGLES, drawCount, GL_UNSIGNED_SHORT, 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void TextView::setText(const char *txt)
{
	free(text);
	needUpdateBuffer = true;
	text = strdup(txt);
	requestResize();
}

void TextView::setTextSize(float size)
{
	this->size = size;
	requestResize();
}

void TextView::setTextAlign(TextAlign align)
{
	this->align = align;
	requestResize();
}

void TextView::setColor(float r, float g, float b, float a)
{
	color[0] = r;
	color[1] = g;
	color[2] = b;
	color[3] = a;
}

void TextView::setColor(const float *rgba)
{
	memcpy(color, rgba, 4 * sizeof(float));
}
