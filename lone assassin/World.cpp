#include "World.h"

float Entity::maxMapY = 0;

World::World(int Worldwidth, int Worldheight, float tileWidth, float tileHeight, Shader* shader)
{
	Entity::maxMapY = ((Worldheight - 499) * tileHeight) - 50;

	worldWidth = Worldwidth;
	worldHeight = Worldheight;

	TILE_WIDTH = tileWidth;
	TILE_HEIGHT = tileHeight;

	for (int y = 0; y < worldHeight; y++)
	{
		for (int x = 0; x < worldWidth; x++)
		{
			if (y <= worldHeight - 500)
			{
				if (y == worldHeight - 500)
					tile[x][y] = new Tile(Rect(x * tileWidth, y * tileHeight, tileWidth, tileHeight), shader, HAY_BORDERING_WATER, false);
				else
					tile[x][y] = new Tile(Rect(x * tileWidth, y * tileHeight, tileWidth, tileHeight), shader, HAY_FIELD_FULL, false);
			
				if (rand() % 20 < abs(Worldwidth / 2 - x))
				{
					entities.push_back(new Tree(Rect(x * tileWidth + 100, y * tileHeight, 320, 640), Rect(140, 70, 60, 50), shader, 1));
				}
	
				if (rand() % 50 == 1)
					entities.push_back(new Rabbit(Rect(x * tileWidth + 100, y * tileHeight, 160, 160), Rect(10, 0, 140, 90), shader, 1));
	
				if (rand() % 100 == 1)
					entities.push_back(new Wolf(Rect(x * tileWidth + 100, y * tileHeight, 320, 320), Rect(10, 0, 300, 130), shader, 1));
			}
			else if (y > worldHeight - 500)
				tile[x][y] = new Tile(Rect(x * tileWidth, y * tileHeight, tileWidth, tileHeight), shader, WATER, false);
		}
	}
	
	sky = Sprite(-SCREEN_WIDTH, -SCREEN_HEIGHT, SCREEN_WIDTH * 2, SCREEN_HEIGHT * 2, shader, SKY_LINE);
	skyAnimation = Animation(192, 0, 198, 108, 1, 1000);
	moon = Sprite(0, 0, 320, 320, shader, SKY_LINE);
	moonAnimation = Animation(416, 0, 256, 246, 1, 1000);

	entities.push_back(new Boat(Rect(SCREEN_WIDTH - 480, (Worldheight - 499) * tileHeight, 960, 640), Rect(0, -100, 960, 740), shader, 9999999999));
	boatIndex = entities.size() - 1;

	std::cout << "NUMBER OF ENTITIES: " << entities.size() << std::endl;
}

World::~World()
{
}

void World::draw()
{
	moon.draw(&moonAnimation);
	sky.draw(&skyAnimation);

	for (int y = (int)((WorldSprite::cameraFocusY - SCREEN_HEIGHT) / TILE_WIDTH); y < ((SCREEN_HEIGHT + WorldSprite::cameraFocusY) / TILE_HEIGHT) && y < worldHeight; y++)
	{
		for (int x = 0; x < worldWidth; x++)
		{
			tile[x][y]->draw();
		}
	}

	for (unsigned int i = 0; i < onScreenEntities.size(); i++)
	{
		std::cout << onScreenEntities.size() << std::endl;
		entities[onScreenEntities[i]]->draw();
	}
}

void World::moveEntities(Player* player)
{
	onScreenEntities.clear();
	for (unsigned int i = 0; i < entities.size(); i++)
	{
		if (entities[i]->getActive() && entities[i]->getHealth() > 0)
			if(entities[i]->getNeedsPlayer())
				entities[i]->move(player);
			else
				entities[i]->move();

		if (entities[i]->getOnScreen())
			onScreenEntities.push_back(i);
	}
}

void World::playerAttack(Player* player)
{
	for (unsigned int i = 0; i < onScreenEntities.size(); i++)
	{
		if (entities[onScreenEntities[i]]->getHealth() > 0 && AABB(player->getAttackRect(), entities[onScreenEntities[i]]->getCollisionRect()))
		{
			entities[onScreenEntities[i]]->setHealth(entities[onScreenEntities[i]]->getHealth() - 1);
			
			if (entities[onScreenEntities[i]]->getHealth() <= 0)
				entities[onScreenEntities[i]]->die(player);
		}
	}
}

void World::attackPlayer(Player* player)
{
	for (unsigned int i = 0; i < onScreenEntities.size(); i++)
	{
		if (entities[onScreenEntities[i]]->getHealth() > 0 && entities[onScreenEntities[i]]->getUsingAttackRect() && AABB(player->getCollisionRect(), entities[onScreenEntities[i]]->getAttackRect()))
		{
			player->setHealth(player->getHealth() - 1);

			entities[onScreenEntities[i]]->hurtPlayer();

			if (player->getHealth() <= 0)
				player->die();
		}
	}
}

void World::createFire(float x, float y, Shader* shader)
{
	entities.push_back(new Fire(Rect(x, y, 320, 320), Rect(0, 0, 320, 320), shader, 1));
}

void World::checkPlayerNearBoat(Player* player)
{
	if (AABB(player->getCollisionRect(), entities[boatIndex]->getCollisionRect()))
	{
		player->madeItToBoat(entities[boatIndex]->getImage()->getX() + entities[boatIndex]->getImage()->getW() / 2, entities[boatIndex]->getImage()->getY() + entities[boatIndex]->getImage()->getH() / 2);
		if (boatMovingTimer == -1)
			boatMovingTimer = SDL_GetTicks();
	}

	if (boatMovingTimer != -1 && boatMovingTimer + boatMovingTime < SDL_GetTicks())
	{
		entities[boatIndex]->setVelocityY(0.02f);
		player->setVelocityY(0.02f);
	}
}