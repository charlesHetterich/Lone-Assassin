#include "Entity.h"

//////////////////////////////////////////////////////////////////////////////////////////
//////ENTITY
//////////////////////////////////////////////////////////////////////////////////////////

Entity::Entity(Rect imageRect, Rect collisionRect, Shader* shader, int health, const float drawLine)
{
	this->shader = shader;
	this->collisionRect = collisionRect;
	image = WorldSprite(imageRect.x, imageRect.y, imageRect.w, imageRect.h, shader, drawLine);
	imageReflection = WorldSprite(imageRect.x, imageRect.y, imageRect.w, -imageRect.h, shader, REFLECTION_LINE);
	imageReflection.setAlpha(0.1f);

	maxHealth = health;
	this->health = maxHealth;
}

Entity::~Entity()
{
}

void Entity::move()
{
	image.setX(image.getX() + velocityX);
	image.setY(image.getY() + velocityY);

	if (lockedInScreen)
	{
		if (image.getX() + collisionRect.x < 0)
		{
			image.setX(-collisionRect.x);
		}
		else if (image.getX() + collisionRect.x + collisionRect.w > SCREEN_WIDTH * 2)
		{
			image.setX(SCREEN_WIDTH * 2 - collisionRect.x - collisionRect.w);
		}

		if (image.getY() < 0)
		{
			image.setY(0);
		}
		else if (image.getY() > maxMapY)
		{
			image.setY(maxMapY);
		}
	}

	imageReflection.setX(image.getX());
	imageReflection.setY(image.getY());
}

bool Entity::getOnScreen()
{
	if (image.getY() > (WorldSprite::cameraFocusY - 5000) && image.getY() < (WorldSprite::cameraFocusY + 5000))
		active = true;
	else
		active = false;

	return active && image.getY() + image.getH() > (WorldSprite::cameraFocusY - SCREEN_HEIGHT) && image.getY() < (SCREEN_HEIGHT + WorldSprite::cameraFocusY);
}

Rect Entity::getCollisionRect()
{
	return Rect(image.getX() + collisionRect.x, image.getY() + collisionRect.y, collisionRect.w, collisionRect.h);
}

//////////////////////////////////////////////////////////////////////////////////////////
//////TREE
//////////////////////////////////////////////////////////////////////////////////////////

void Tree::die(Player* player)
{
	currentAnimation = DEAD;
	player->setWoodCount(player->getWoodCount() + 1);
}


//////////////////////////////////////////////////////////////////////////////////////////
//////FIRE
//////////////////////////////////////////////////////////////////////////////////////////

void Fire::die(Player* player)
{
	if (player->getMeatCount() > 0)
	{
		currentAnimation = DEAD;
		player->setMeatCount(player->getMeatCount() - 1);
		player->setHealth(player->getHealth() + 1);
	}
}



//////////////////////////////////////////////////////////////////////////////////////////
//////MOB
//////////////////////////////////////////////////////////////////////////////////////////

void Mob::move()
{
	//accelerate x
	velocityX += accelerationX;
	if (velocityX > 0)
	{
		velocityX -= friction;
		if (velocityX < 0)
			velocityX = 0;
		else if (velocityX > maxSpeed)
			velocityX = maxSpeed;
	}
	else if (velocityX < 0)
	{
		velocityX += friction;
		if (velocityX > 0)
			velocityX = 0;
		else if (velocityX < -maxSpeed)
			velocityX = -maxSpeed;
	}

	//accelerate y
	velocityY += accelerationY;
	if (velocityY > 0)
	{
		velocityY -= friction;
		if (velocityY < 0)
			velocityY = 0;
		else if (velocityY > maxSpeed)
			velocityY = maxSpeed;
	}
	else if (velocityY < 0)
	{
		velocityY += friction;
		if (velocityY > 0)
			velocityY = 0;
		else if (velocityY < -maxSpeed)
			velocityY = -maxSpeed;
	}

	//facing flag
	if (velocityX != 0)
		facing = velocityX > 0;

	image.setHorizontalFlip(facing);
	imageReflection.setHorizontalFlip(facing);

	Entity::move();
}


//////////////////////////////////////////////////////////////////////////////////////////
//////RABBIT
//////////////////////////////////////////////////////////////////////////////////////////

void Rabbit::die(Player* player)
{
	currentAnimation = DEAD;
	player->setMeatCount(player->getMeatCount() + 1);
}


//////////////////////////////////////////////////////////////////////////////////////////
//////ENEMY
//////////////////////////////////////////////////////////////////////////////////////////

void Enemy::checkAngered(Player* player)
{
	//check if angered
	Rect myRect = getCollisionRect();
	Rect pRect = player->getCollisionRect();

	float distanceX = (pRect.x + pRect.w / 2) - (myRect.x + myRect.w / 2);
	float distanceY = (pRect.y + pRect.h / 2) - (myRect.y + myRect.h / 2);

	distanceFromPlayer = sqrt(distanceX * distanceX + distanceY * distanceY);

	if (distanceFromPlayer < angeredRadius)
	{
		angered = true;
		angleWithPlayer = atan2f(distanceY, distanceX);
	}
	else
		angered = false;
}


//////////////////////////////////////////////////////////////////////////////////////////
//////WOLF
//////////////////////////////////////////////////////////////////////////////////////////

void Wolf::die(Player* player)
{
	currentAnimation = DEAD;
	player->setMeatCount(player->getMeatCount() + 1);
}

void Wolf::move(Player* player)
{
	//move
	if (striking)
	{
		currentAnimation = SURGING;

		velocityX = maxSpeed * cosf(angleWithPlayer);
		velocityY = maxSpeed * sinf(angleWithPlayer);

		if (wolfTimer + strikeTime < SDL_GetTicks())
		{
			striking = false;
			recovering = true;
			UsingAttackRect = false;
			wolfTimer = SDL_GetTicks();
		}
	}
	else if (recovering)
	{
		currentAnimation = IDLE;

		velocityX = 0;
		velocityY = 0;

		if (wolfTimer + recoverTime < SDL_GetTicks())
		{
			recovering = false;
			wolfTimer = SDL_GetTicks();
		}
	}
	else
	{
		checkAngered(player);

		if (angered)
		{
			currentAnimation = RUNNING;

			velocityX = maxSpeed * 0.5f * cosf(angleWithPlayer);
			velocityY = maxSpeed * 0.5f * sinf(angleWithPlayer);
		}
		else
		{
			velocityX = 0;
			velocityY = 0;
			currentAnimation = IDLE;
		}

		if (distanceFromPlayer < strikeRange)
		{
			striking = true;
			UsingAttackRect = true;
			wolfTimer = SDL_GetTicks();
		}
	}

	Mob::move();
}


//////////////////////////////////////////////////////////////////////////////////////////
//////PLAYER
//////////////////////////////////////////////////////////////////////////////////////////

void Player::draw()
{
	stats.displayStats(health, woodCount, meatCount);
	image.draw(&animation[currentAnimation]);
}

void Player::input(Input input)
{
	if (!overRiddingInput)
	{
		if (!attacking)
		{
			if (input.getKey(SDL_SCANCODE_A))
				accelerationX = -0.0005f;
			else if (input.getKey(SDL_SCANCODE_D))
				accelerationX = 0.0005f;
			else
				accelerationX = 0.0f;

			if (input.getKey(SDL_SCANCODE_S))
				accelerationY = -0.0005f;
			else if (input.getKey(SDL_SCANCODE_W))
				accelerationY = 0.0005f;
			else
				accelerationY = 0.0f;

			if (accelerationX != 0 || accelerationY != 0)
			{
				currentAnimation = RUNNING;
				sleeping = false;
			}
			else if (!sleeping)
				currentAnimation = IDLE;

			if (input.getKeyDown(SDL_SCANCODE_RSHIFT))
			{
				sleeping = true;
				currentAnimation = SLEEPING;
			}

			if (input.getKey(SDL_SCANCODE_SPACE))
			{
				attacking = true;
				currentAnimation = MELEE;
				animation[currentAnimation].cycleOnce();
			}
			if (input.getKeyDown(SDL_SCANCODE_RETURN) && woodCount >= 5)
			{
				makingFire = true;
				woodCount -= 5;
			}
		}
		else
		{
			sleeping = false;
			accelerationY = 0.0f;
			accelerationX = 0.0f;

			if (!animation[currentAnimation].getOneCycle())
				attacking = false;

			attackNow = false;

			if (animation[currentAnimation].getCurrentFrame() == 4)
				attackNow = true;
		}
	}
}

void Player::move()
{
	if (overRiddingInput)
	{
		if (image.getX() + image.getW() / 2 < destinationX)
			accelerationX = 0.0005f;
		else if (image.getX() + image.getW() / 2 > destinationX)
			accelerationX = -0.0005f;

		if (abs(image.getX() + image.getW() / 2 - destinationX) < 10)
			accelerationX = 0.0f;

		if (image.getY() + image.getH() / 2 < destinationY)
			accelerationY = 0.0005f;
		else if (image.getY() + image.getH() / 2 > destinationY)
			accelerationY = -0.0005f;

		if (abs(image.getY() + image.getH() / 2 - destinationY) < 10)
			accelerationY = 0.0f;

		if (accelerationX == 0.0f && accelerationY == 0.0f)
			currentAnimation = IDLE;
	}

	Mob::move();
}

Rect Player::getAttackRect()
{
	if (facing)
		return Rect((image.getX() + image.getW()) - (attackRect.x + attackRect.w), image.getY() + attackRect.y, attackRect.w, attackRect.h);

	return Rect(image.getX() + attackRect.x, image.getY() + attackRect.y, attackRect.w, attackRect.h);
}