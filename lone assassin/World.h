#pragma once
#include "Tile.h"
#include "Entity.h"

class World
{
public:
	World(int Worldwidth, int Worldheight, float tileWidth, float tileHeight, Shader* shader);
	World(){}
	~World();

	void draw();
	void moveEntities(Player* player);
	void playerAttack(Player* player);
	void attackPlayer(Player* player);
	void createFire(float x, float y, Shader* shader);
	void checkPlayerNearBoat(Player* player);
	void reset()
	{
		entities.clear();
		onScreenEntities.clear();
		for (int y = 0; y < worldHeight; y++)
		{
			if (y <= worldHeight - 500)
			{
				for (int x = 0; x < worldWidth; x++)
				{
					if (rand() % 20 < abs(worldWidth / 2 - x))
					{
						entities.push_back(new Tree(Rect(x * TILE_WIDTH + 100, y * TILE_HEIGHT, 320, 640), Rect(140, 70, 60, 50), shader, 1));
					}

					if (rand() % 50 == 1)
						entities.push_back(new Rabbit(Rect(x * TILE_WIDTH + 100, y * TILE_HEIGHT, 160, 160), Rect(10, 0, 140, 90), shader, 1));

					if (rand() % 100 == 1)
						entities.push_back(new Wolf(Rect(x * TILE_WIDTH + 100, y * TILE_HEIGHT, 320, 320), Rect(10, 0, 300, 130), shader, 1));
				}
			}
		}

		entities.push_back(new Boat(Rect(SCREEN_WIDTH - 480, (worldHeight - 499) * TILE_HEIGHT, 960, 640), Rect(0, -100, 960, 740), shader, 9999999999));
		boatIndex = entities.size() - 1;

		std::cout << "NUMBER OF ENTITIES: " << entities.size() << std::endl;
	}

private:

	Sprite sky;
	Animation skyAnimation;
	Sprite moon;
	Animation moonAnimation;

	Tile* tile[12][1000];
	std::vector <Entity*> entities;
	std::vector<int> onScreenEntities;

	//timers
	int boatMovingTime = 3000;
	int boatMovingTimer = -1;
	int boatIndex;

	int worldWidth;
	int worldHeight;

	Shader* shader;
};