#include "Statistics.h"


PlayerStatistics::PlayerStatistics(Shader * shader)
{
	healthImage = Sprite(-SCREEN_WIDTH, SCREEN_HEIGHT - 160 - 40, 1280, 160, shader, STATS_LINE);

	for (int i = 0; i < 10; i++)
		meatImage[i] = Sprite(-SCREEN_WIDTH + 80 + i * 80, SCREEN_HEIGHT - 160 - 40 - 180, 160, 160, shader, STATS_LINE);

	for (int i = 0; i < 10; i++)
		woodImage[i] = Sprite(-SCREEN_WIDTH + 80 + i * 80, SCREEN_HEIGHT - 160 - 40 - 360, 160, 160, shader, STATS_LINE);

	for (int i = 0; i < 11; i++)
		healthAnimation[i] = Animation(224 + 64 * (10 - i), 0, 64, 8, 1, 1000);

	woodAnimation = Animation(224, 16, 8, 8, 1, 1000);
	meatAnimation = Animation(232, 16, 8, 8, 1, 1000);
}

PlayerStatistics::~PlayerStatistics()
{
}

void PlayerStatistics::displayStats(int health, int woodCount, int meatCount)
{
	healthImage.draw(&healthAnimation[health]);

	for (int i = 0; i < woodCount; i++)
		woodImage[i].draw(&woodAnimation);

	for (int i = 0; i < meatCount; i++)
		meatImage[i].draw(&meatAnimation);
}