#include "Sprite.h"

Sprite::Sprite(float x, float y, float w, float h, Shader* shader, const float zDrawingLine)
{
	rect = { x, y, w, h };
	drawingLine = zDrawingLine;
	Vertex positions[] = {
			{ -0.5f, -0.5f, 0.0f, 1, 1, 1, 0.0f, 1.0f },
			{ 0.5f, -0.5f, 0.0f, 1, 1, 1, 1.0f, 1.0f },
			{ 0.5f, 0.5f, 0.0f, 1, 1, 1, 1.0f, 0.0f },
			{ -0.5f, 0.5f, 0.0f, 1, 1, 1, 0.0f, 0.0f },
	};

	 GLuint indicies[] = { 0, 1, 2, 2, 3, 0 };

	 mesh = new Mesh(positions, sizeof(positions) / sizeof(positions[0]), indicies, sizeof(indicies) / sizeof(indicies[0]));

	 this->shader = shader;
}

Sprite::~Sprite()
{
}

void Sprite::draw(Animation* animation)
{
	shader->update(getTransform(), getColor());
	mesh->setTexCoords(animation->getAnimation());
	mesh->draw();
}

glm::mat4 Sprite::getTransform()
{
	return glm::scale(glm::vec3(1 / SCREEN_WIDTH, 1 / SCREEN_HEIGHT, 1)) * glm::translate(glm::vec3(rect.x + rect.w/2, rect.y + rect.h/2, drawingLine)) * glm::scale(glm::vec3(rect.w, rect.h, 1)) * glm::rotate(180.0f * horizontalFlip, glm::vec3(0, 1, 0));
}