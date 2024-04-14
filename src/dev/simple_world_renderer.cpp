#include "dev/simple_world_renderer.hpp"

#include <stdlib.h>

#include "math/camera.hpp"
#include "shader_program.hpp"
#include "game/enterable.hpp"
#include "game/world_data.hpp"

#include "list.tcc"

namespace {
ShaderProgram program("shader/game");

float frand() {
	return (float) rand() / (float) RAND_MAX;
}

void getColor(float *dst, TerrainType type)
{
	switch(type) {
	case TERRAIN_TYPE_GROUND:
		dst[0] = 0.1f * frand();
		dst[1] = 0.5f + 0.1f * frand();
		dst[2] = 0.1f * frand();
		break;
	case TERRAIN_TYPE_WATER:
		dst[0] = 0.2f + 0.1f * frand();
		dst[1] = 0.3f + 0.1f * frand();
		dst[2] = 0.8f + 0.2f * frand();
		break;
	default:
		dst[0] = frand();
		dst[1] = frand();
		dst[2] = frand();
		break;
	}
}

} // namespace

class SimpleWorldRenderer::RenderableTownie
{
  public:
	RenderableTownie(Townie *);
	Townie *townie;
};

class SimpleWorldRenderer::RenderableGameObject
{
  public:
	RenderableGameObject(GameObject *object);
	GameObject *object;
};

SimpleWorldRenderer::RenderableTownie::RenderableTownie(Townie *townie)
{
	this->townie = townie;
}

SimpleWorldRenderer::RenderableGameObject::RenderableGameObject(GameObject *object)
{
	this->object = object;
}

SimpleWorldRenderer::SimpleWorldRenderer(WorldData *world, Camera *cam)
	: WorldRenderer(world, cam)
{
	world->addGameObjectListener(this);
	world->addTownieListener(this);
}

void SimpleWorldRenderer::onInitGL()
{
	program.loadStringIfNeeded(
#include "shader/game.vertex.glsl"
	,
#include "shader/game.fragment.glsl"
	);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ibo);

	float r=1, g=1, b=1;
	float vertex1[28] {
		0.25f, 0.25f, 0.02f, r,g,b,1,
		0.75f, 0.25f, 0.02f, r,g,b,1,
		0.75f, 0.75f, 0.02f, r,g,b,1,
		0.25f, 0.75f, 0.02f, r,g,b,1,
	};
	glGenBuffers(1, &rectVbo);
	glBindBuffer(GL_ARRAY_BUFFER, rectVbo);
	glBufferData(GL_ARRAY_BUFFER, 7 * 4 * sizeof(float), (GLvoid*) vertex1, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	float vertex2[28] {
		0.0f, 0.0f, 0.01f, r,g,b,1,
		0.99f, 0.0f, 0.01f, r,g,b,1,
		0.99f, 0.99f, 0.01f, r,g,b,1,
		0.0f, 0.99f, 0.01f, r,g,b,1,
	};
	glGenBuffers(1, &rect2Vbo);
	glBindBuffer(GL_ARRAY_BUFFER, rect2Vbo);
	glBufferData(GL_ARRAY_BUFFER, 7 * 4 * sizeof(float), (GLvoid*) vertex2, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void SimpleWorldRenderer::onWorldChanged()
{
	int width = world->getTerrainWidth();
	int height = world->getTerrainHeight();
	count = width * height;
	float vertex[7 * 4 * count];
	GLushort index[6 * count];
	int i=0;
	int j=0;
	#define put(vx, vy) \
		vertex[i++] = vx - 0.5f; \
		vertex[i++] = vy - 0.5f; \
		vertex[i++] = z * 0.25f; \
		vertex[i++] = color[0]; \
		vertex[i++] = color[1]; \
		vertex[i++] = color[2]; \
		vertex[i++] = 1
	float color[3];
	for(int y=0; y<height; y++) {
		for(int x=0; x<width; x++) {
			int n = i / 7;
			index[j++] = n;
			index[j++] = n + 1;
			index[j++] = n + 2;
			index[j++] = n;
			index[j++] = n + 2;
			index[j++] = n + 3;
			srand(y * width + x);
			getColor(color, world->getTerrain(x, y));
			float z = world->getElevation(x, y);
			put(x, y);
			put(x + 1, y);
			put(x + 1, y + 1);
			put(x, y + 1);
		}
	}
	#undef put

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * 6 * sizeof(*index), (GLvoid*) index, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, count * 7 * 4 * sizeof(float), (GLvoid*) vertex, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void SimpleWorldRenderer::render()
{
	program.use();
	glUniformMatrix4fv(program.unifomMatrix1Index, 1, false, cam->viewProjectionMatrix);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glUniform4f(program.uniformPositionIndex, 0, 0, 0, 0);
	glVertexAttribPointer(program.vertexPositionIndex, 3, GL_FLOAT, GL_FALSE, 28, (GLvoid*) 0);
	glVertexAttribPointer(program.vertexColorIndex, 4, GL_FLOAT, GL_FALSE, 28, (GLvoid*) 12);
	glUniform4f(program.uniformColorIndex, 1, 1, 1, 1);
	glDrawElements(GL_TRIANGLES, 6 * count, GL_UNSIGNED_SHORT, 0);

	// render bulding
	glBindBuffer(GL_ARRAY_BUFFER, rect2Vbo);
	glVertexAttribPointer(program.vertexPositionIndex, 3, GL_FLOAT, GL_FALSE, 28, (GLvoid*) 0);
	glVertexAttribPointer(program.vertexColorIndex, 4, GL_FLOAT, GL_FALSE, 28, (GLvoid*) 12);
	for(int k=0; k<renderableObjects.size(); k++) {
		glUniform4f(program.uniformColorIndex, 0.2f, 0.2f, 0.1f, 1);
		RenderableGameObject *b = renderableObjects[k];
		for(int i=0; i<b->object->getWidth(); i++) {
			for(int j=0; j<b->object->getHeight(); j++) {
				glUniform4f(program.uniformPositionIndex, b->object->getX()+i, b->object->getY()+j, 0.0f, 0);
				glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
			}
		}
		glUniform4f(program.uniformColorIndex, 0.2f, 0.2f, 0.4f, 1);
		Enterable *enterable = dynamic_cast<Enterable*>(b->object);
		if(enterable) {
			const List<const Entrance*> &entrances = enterable->getEntrance();
			for(int i = 0; i < entrances.size(); i++) {
				const Entrance *entrance = entrances[i];
				glUniform4f(program.uniformPositionIndex, entrance->getX(), entrance->getY(), 0.0f, 0);
			}
		}
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	}

	// render townie
	glBindBuffer(GL_ARRAY_BUFFER, rectVbo);
	glUniform4f(program.uniformColorIndex, 0.9f, 0.2f, 0.1f, 1);
	glVertexAttribPointer(program.vertexPositionIndex, 3, GL_FLOAT, GL_FALSE, 28, (GLvoid*) 0);
	glVertexAttribPointer(program.vertexColorIndex, 4, GL_FLOAT, GL_FALSE, 28, (GLvoid*) 12);
	for(int i=0; i<renderableTownies.size(); i++) {
		RenderableTownie *rt = renderableTownies[i];
		if(!rt->townie->isVisible()) continue;
		float tx = rt->townie->getX();
		float ty = rt->townie->getY();
		glUniform4f(program.uniformPositionIndex, tx, ty, world->getElevation((int) tx, (int) ty) * 0.25f, 0);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	}
}

void SimpleWorldRenderer::onTownieAdded(Townie *townie)
{
	renderableTownies.push(new RenderableTownie(townie));
}

void SimpleWorldRenderer::onTownieRemoved(Townie *townie)
{

}

void SimpleWorldRenderer::onGameObjectAdded(GameObject *object)
{
	renderableObjects.push(new RenderableGameObject(object));
}

void SimpleWorldRenderer::onGameObjectRemoved(GameObject *object)
{

}
