#pragma once
#include "Sprite.h"

class PlayerStatistics
{
public:
	PlayerStatistics(Shader * shader);
	PlayerStatistics(){}
	~PlayerStatistics();

	void displayStats(int health, int woodCount, int meatCount);

private:
	
	//health
	Sprite healthImage;
	Animation healthAnimation[11];

	//wood
	Sprite woodImage[10];
	Animation woodAnimation;

	//meat
	Sprite meatImage[10];
	Animation meatAnimation;

	//Animation(224 + 64 * (10 - i), 0, 64, 8, 1, 1000);
};