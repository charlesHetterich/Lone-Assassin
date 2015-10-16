#include "Tile.h"


Tile::Tile(Rect imageRect, Shader* shader, int animation, bool isWater)
{
	image = WorldSprite(imageRect.x, imageRect.y, imageRect.w, imageRect.h, shader, TILE_LINE);
	this->shader = shader;
	animationNumber = animation;

	//flags
	this->isWater = isWater;
}


Tile::~Tile()
{
}

void Tile::draw()
{
	image.draw(&animation[animationNumber]);
}