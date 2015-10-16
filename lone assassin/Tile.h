#pragma once
#include "Sprite.h"

class Tile
{
public:
	Tile(Rect imageRect, Shader* shader, int animation, bool isWater);
	Tile(){};
	~Tile();

	void draw();
	void setAnimation(int newAnimation) { animationNumber = newAnimation; }

	//flags
	inline bool getIsWater() { return isWater; }
	inline void setIsWater(bool isWater) { this->isWater = isWater; }

private:

	WorldSprite image;
	Shader* shader;
	int animationNumber;

	//flags
	bool isWater;
};

enum
{
	HAY_FIELD_FULL,
	HAY_BORDERING_WATER,
	WATER,

	NUM_ANIMATIONS
};
static Animation animation[NUM_ANIMATIONS] = {

	Animation(0, 96, 32, 32, 1, 1000),						//1: HAY_FIELD_FULL
	Animation(32, 96, 32, 32, 1, 1000),						//2: HAY_BORDERING_WATER
	Animation(0, 128, 32, 32, 1, 1000),						//2: WATER
};