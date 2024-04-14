#include "texture/atlas.hpp"

#include <string.h>
#include <math.h>
#include <tinyxml2.h>

#include "gl.h"
#include "log.h"
#include "math/mat4.h"

#include "math/camera.hpp"
#include "shader_program.hpp"
#include "texture_loader.hpp"
#include "texture/renderable_sprite.hpp"
#include "texture/simple_sprite.hpp"

#include "list.tcc"
#include "map.tcc"

template<>
int Comparator<const char*>::operator() (const char *a, const char *b)
{
	return strcmp(a, b);
}

template class Map<const char*, const TextureSprite*, Comparator<const char*>>;

namespace {
ShaderProgram program("shader/texture_atlas");
const TextureAtlas *lastTextureAtlas = nullptr;

int parseSpaceSparatedList(int **dst, const char *str)
{
	// count the element
	int count = 1;
	int i = 0;
	while(str[i] != '\0') {
		if(str[i] == ' ') {
			count++;
		}
		i++;
	}

	int *result = new int[count];
	char ch, *tmp;
	i = 0;
	int last = 0;
	int n = 0;
	do {
		ch = str[i];
		if(ch == ' ' || ch == '\0') {
			tmp = strndup(str + last, i - last);
			result[n++] = atoi(tmp);
			free(tmp);
			last = i + 1;
		}
		i++;
	} while(ch != '\0');
	*dst = result;
	return count;
}

TextureSprite *parseSpriteElement(
	TextureAtlas *textureAtlas,
	List<float> *vertexBuffer,
	List<unsigned short> *indexBuffer,
	tinyxml2::XMLElement *el,
	float *transformation
) {
	assert_streq(el->Name(), "sprite");
	const char *name = NULL;
	int x, y, width, height;
	assert_ieq(tinyxml2::XML_SUCCESS, el->QueryStringAttribute("n", &name));
	assert_ieq(tinyxml2::XML_SUCCESS, el->QueryIntAttribute("x", &x));
	assert_ieq(tinyxml2::XML_SUCCESS, el->QueryIntAttribute("y", &y));
	assert_ieq(tinyxml2::XML_SUCCESS, el->QueryIntAttribute("w", &width));
	assert_ieq(tinyxml2::XML_SUCCESS, el->QueryIntAttribute("h", &height));
	int offsetX = el->IntAttribute("oX");
	int offsetY = el->IntAttribute("oY");
	int originWidth = el->IntAttribute("oW");
	int originHeight = el->IntAttribute("oH");
	bool rotate = el->FindAttribute("r") != NULL;
	int vertexCount;
	int *vertices;
	int *verticesUV;
	int indicesCount;
	int *indices;
	if(el->NoChildren()) {
		vertexCount = 4;
		indicesCount = 6;
		verticesUV = new int[8] {
			x        , y,
			x + width, y,
			x + width, y+ height,
			x        , y+ height
		};
		indices = new int[6]{0, 1, 2, 0, 2, 3};
		if(rotate) {
			vertices = new int[8] {
				offsetX         , offsetY + width,
				offsetX         , offsetY,
				offsetX + height, offsetY,
				offsetX + height, offsetY + width
			};
		} else {
			vertices = new int[8] {
				offsetX        , offsetY,
				offsetX + width, offsetY,
				offsetX + width, offsetY + height,
				offsetX        , offsetY + height
			};
		}
	} else {
		vertexCount = parseSpaceSparatedList(&vertices, el->FirstChildElement("vertices")->GetText()) / 2;
		parseSpaceSparatedList(&verticesUV, el->FirstChildElement("verticesUV")->GetText());
		indicesCount = parseSpaceSparatedList(&indices, el->FirstChildElement("triangles")->GetText());
	}

	const float imgWidth = textureAtlas->getImageWidth();
	const float imgHeight = textureAtlas->getImageHeight(); 
	int vertexIndex = vertexBuffer->size() / 5;
	int indexStart = indexBuffer->size();
	assert_ile(vertexIndex, 0xffff);
	for(int i=0; i<indicesCount; i++){
		indexBuffer->push(vertexIndex + indices[i]);
	}
	float scale = 64 * M_SQRT1_2;
	for(int i=0; i<vertexCount; i++){
		float v[3] = {
			(vertices[2 * i ] - originWidth / 2) / scale,
			(originHeight - vertices[2 * i + 1]) / scale,
			0
		};
		if(transformation) mat4_mulmv3_normalize(v, transformation, v);
		vertexBuffer->push(v[0]);
		vertexBuffer->push(v[1]);
		vertexBuffer->push(v[2]);
		vertexBuffer->push((float) verticesUV[2 * i    ] / imgWidth);
		vertexBuffer->push((float) verticesUV[2 * i + 1] / imgHeight);
	}
	delete[] indices;
	delete[] vertices;
	delete[] verticesUV;
	return new SimpleTextureSprite(name, textureAtlas, indexStart, indicesCount);
}

} // namespace

TextureAtlas::~TextureAtlas()
{
}

TextureAtlas::TextureAtlas(SpriteMap *dst, const char *xmlPath, float *transformation)
{
	tinyxml2::XMLDocument doc;
	doc.LoadFile(xmlPath);
	tinyxml2::XMLElement *root = doc.RootElement();
	const char *imageName = NULL;
	assert_ieq(tinyxml2::XML_SUCCESS, root->QueryIntAttribute("width", &imageWidth));
	assert_ieq(tinyxml2::XML_SUCCESS, root->QueryIntAttribute("height", &imageHeight));
	assert_ieq(tinyxml2::XML_SUCCESS, root->QueryStringAttribute("imagePath", &imageName));

	int lastSeparator = strrchr(xmlPath, '/') - xmlPath + 1;
	char *imagePath = (char*) malloc(lastSeparator + strlen(imageName) + 1);
	strncpy(imagePath, xmlPath, lastSeparator);
	strcpy(imagePath + lastSeparator, imageName);

	this->dst = dst;
	this->vertexBuffer = new List<float>();
	this->indexBuffer = new List<unsigned short>();
	tinyxml2::XMLElement *el = root->FirstChildElement();
	TextureSprite *sprite;
	do {
		sprite = parseSpriteElement(this, vertexBuffer, indexBuffer, el, transformation);
		dst->add(sprite->getName(), sprite);
	} while((el = el->NextSiblingElement()) != NULL);

	texture = loadPNGTexture(imagePath);
	free(imagePath);
}

TextureAtlas *TextureAtlas::copySprite(const char *srcName, const char *dstName, float *transformation)
{
	auto x = dst->operator[](srcName);
	int indexStart = x->getIndexStart();
	int indexCount = x->getIndexCount();
	int minVertexIndex = INT32_MAX;
	int maxVertexIndex = INT32_MIN;
	for(int i=0; i<indexCount; i++) {
		int index = (*indexBuffer)[indexStart + i];
		if(index < minVertexIndex) minVertexIndex = index;
		if(index > maxVertexIndex) maxVertexIndex = index;
	}

	int newVertexIndexStart = vertexBuffer->size() / 5;
	int newIndexStart = indexBuffer->size();
	for(int i=0; i<indexCount; i++){
		int index = newVertexIndexStart + (*indexBuffer)[indexStart + i] - minVertexIndex;
		indexBuffer->push(index);
	}
	for(int i=minVertexIndex; i<=maxVertexIndex; i++){
		int index = i * 5;
		float v[3] = {
			(*vertexBuffer)[index],
			(*vertexBuffer)[index + 1],
			(*vertexBuffer)[index + 2]
		};
		if(transformation) mat4_mulmv3_normalize(v, transformation, v);
		vertexBuffer->push(v[0]);
		vertexBuffer->push(v[1]);
		vertexBuffer->push(v[2]);
		vertexBuffer->push((*vertexBuffer)[index + 3]);
		vertexBuffer->push((*vertexBuffer)[index + 4]);
	}
	TextureSprite *result = new SimpleTextureSprite(
		strdup(dstName),
		this,
		newIndexStart,
		indexCount
	);
	dst->add(result->getName(), result);
	return this;
}

TextureAtlas *TextureAtlas::transformSprite(const char *name, float *transformation)
{
	auto x = dst->operator[](name);
	int indexStart = x->getIndexStart();
	int indexCount = x->getIndexCount();
	int minVertexIndex = INT32_MAX;
	int maxVertexIndex = INT32_MIN;
	for(int i=0; i<indexCount; i++) {
		int index = (*indexBuffer)[indexStart + i];
		if(index < minVertexIndex) minVertexIndex = index;
		if(index > maxVertexIndex) maxVertexIndex = index;
	}

	for(int i=minVertexIndex; i<=maxVertexIndex; i++){
		int index = i * 5;
		float v[3] = {
			(*vertexBuffer)[index],
			(*vertexBuffer)[index + 1],
			(*vertexBuffer)[index + 2]
		};
		mat4_mulmv3_normalize(v, transformation, v);
		(*vertexBuffer)[index    ] = v[0];
		(*vertexBuffer)[index + 1] = v[1];
		(*vertexBuffer)[index + 2] = v[2];
	}
	return this;
}

TextureAtlas *TextureAtlas::finish() {
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ibo);
	logp("atlas vbo: %d, ibo: %d", vbo, ibo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertexBuffer->size() * sizeof(float), (GLvoid*) vertexBuffer->values(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexBuffer->size() * sizeof(unsigned short), (GLvoid*) indexBuffer->values(), GL_STATIC_DRAW);
	delete vertexBuffer;
	delete indexBuffer;
	this->vertexBuffer = nullptr;
	this->indexBuffer = nullptr;
	this->dst = nullptr;
	return this;
}

TextureAtlas *TextureAtlas::loadXML(SpriteMap *dst, const char *xmlPath, float *transformation) {
	return new TextureAtlas(dst, xmlPath, transformation);
}

int TextureAtlas::getImageWidth()
const {
	return imageWidth;
}

int TextureAtlas::getImageHeight()
const {
	return imageHeight;
}

void TextureAtlas::initRender()
{
	program.loadStringIfNeeded(
		#include "shader/texture_atlas.vertex.glsl"
		,
		#include "shader/texture_atlas.fragment.glsl"
	);
}

void TextureAtlas::beginRender(Camera *cam)
{
	program.use();
    glUniformMatrix4fv(program.unifomMatrix1Index, 1, GL_FALSE, cam->projectionMatrix);
	glUniformMatrix4fv(program.unifomMatrix2Index, 1, GL_FALSE, cam->viewProjectionMatrix);
	lastTextureAtlas = nullptr;
}

void TextureSprite::renderAt(float x, float y, float z)
const {
	const TextureAtlas *textureAtlas = getTexture();
	if(lastTextureAtlas != textureAtlas){
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureAtlas->texture);
		glUniform1i(program.uniformTextureIndex, 0);

		glBindBuffer(GL_ARRAY_BUFFER, textureAtlas->vbo);
		glVertexAttribPointer(program.vertexPositionIndex, 3, GL_FLOAT, GL_FALSE, 20, (GLvoid*) 0);
		glVertexAttribPointer(program.vertexUVIndex      , 2, GL_FLOAT, GL_FALSE, 20, (GLvoid*) 12);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, textureAtlas->ibo);
		lastTextureAtlas = textureAtlas;
	}
	glUniform4f(program.uniformPositionIndex, x, y, z, 0);
	glDrawElements(GL_TRIANGLES, getIndexCount(), GL_UNSIGNED_SHORT, (GLvoid*) (getIndexStart() * sizeof(GLushort)));
}
