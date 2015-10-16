#pragma once
#include "Sprite.h"
#include "Input.h"
#include "Statistics.h"

class Entity;
class Tree;
class Fire;
class Boat;
class Mob;
class Rabbit;
class Enemy;
class Wolf;
class Player;

class Entity
{
public:
	Entity(Rect imageRect, Rect collisionRect, Shader* shader, int health, const float drawLine);
	Entity(){}
	~Entity();

	//collision
	Rect getCollisionRect();
	static float maxMapY;

	//health
	virtual void setHealth(int health)
	{
		this->health = health;
		if (this->health < 0)
			this->health = 0;

		if (this->health > maxHealth)
			this->health = maxHealth;
	}
	inline int getHealth() { return health; }
	virtual void die(Player* player){}
	virtual void hurtPlayer(){}

	//core
	virtual void draw(){};
	void drawCollisionRect();
	virtual void move();
	virtual void move(Player* player){};
	bool getOnScreen();
	inline bool getActive(){ return active; }
	Sprite* getImage() { return &image; }
	inline bool getNeedsPlayer() { return needsPlayer; }
	inline bool getUsingAttackRect() { return UsingAttackRect; }
	virtual Rect getAttackRect(){ return Rect(0, 0, 0, 0); }

	inline void setVelocityX(float velocity) { velocityX = velocity; }
	inline bool getVelocitX() { return velocityX; }
	inline void setVelocityY(float velocity) { velocityY = velocity; }
	inline bool getVelocityY() { return velocityY; }


protected:
	Rect collisionRect;
	WorldSprite image;
	WorldSprite imageReflection;
	Shader* shader;

	//movement
	float velocityX = 0;
	float velocityY = 0;

	//health
	int health;
	int maxHealth;

	//flags
	bool lockedInScreen = false;
	bool active = false;
	bool needsPlayer = false;
	bool UsingAttackRect = false;
};

class Tree : public Entity
{
public:
	Tree(Rect imageRect, Rect collisionRect, Shader* shader, int health) : Entity(imageRect, collisionRect, shader, health, ENTITY_LINE)
	{
		animation[ALIVE] = Animation(160, 0, 32, 64, 1, 1000);
		animation[DEAD] = Animation(192, 0, 32, 64, 1, 1000);
	}

	void draw()
	{
		image.draw(&animation[currentAnimation]);
	}
	void die(Player* player);

private:

	//animations
	enum
	{
		ALIVE,
		DEAD,

		NUM_ANIMATIONS
	};
	Animation animation[NUM_ANIMATIONS];
	int currentAnimation = 0;
};

class Fire : public Entity
{
public:
	Fire(Rect imageRect, Rect collisionRect, Shader* shader, int health) : Entity(imageRect, collisionRect, shader, health, ENTITY_LINE)
	{
		animation[ALIVE] = Animation(0, 288, 32, 32, 3, 100);
		animation[DEAD] = Animation(96, 288, 32, 32, 1, 1000);

		lockedInScreen = true;
	}

	void draw()
	{
		image.draw(&animation[currentAnimation]);
	}
	void die(Player* player);

private:
	//animations
	enum
	{
		ALIVE,
		DEAD,

		NUM_ANIMATIONS
	};
	Animation animation[NUM_ANIMATIONS];
	int currentAnimation = 0;
};

class Boat : public Entity
{
public:
	Boat(Rect imageRect, Rect collisionRect, Shader* shader, int health) : Entity(imageRect, collisionRect, shader, health, FLAT_ENTITY_LINE)
	{
		animation[BOAT] = Animation(0, 320, 96, 64, 1, 1000);
		animation[WATER_SPLASH] = Animation(224, 24, 1, 1, 1, 1000);
	}

private:
	void draw()
	{
		image.draw(&animation[BOAT]);
	}

	//animations
	enum
	{
		BOAT,
		WATER_SPLASH,

		NUM_ANIMATIONS
	};
	Animation animation[NUM_ANIMATIONS];
};

class Mob : public Entity
{
public:
	Mob(Rect imageRect, Rect collisionRect, Shader* shader, int health) : Entity(imageRect, collisionRect, shader, health, ENTITY_LINE)
	{
	}
	Mob(){}

	void draw(){};
	virtual void move();

	//health
	virtual void setHealth(int health)
	{
		if (health - this->health <= 0)
			activateDamageNumber(image.getX() + image.getW() / 2, image.getY() + image.getH() / 2, this->health - health);

		Entity::setHealth(health);
	}

	//static damage functions
	static void activateDamageNumber(float x, float y, int number)
	{
		damageNumber[nextDamageNumber].setAlpha(1);
		damageNumber[nextDamageNumber].setX(x - 40);
		damageNumber[nextDamageNumber].setY(y);

		currentNumberAnimation[nextDamageNumber] = number;
		damageNumberActive[nextDamageNumber] = true;

		nextDamageNumber++;
		if (nextDamageNumber >= 20)
			nextDamageNumber = 0;
	}
	static void drawDamageNumbers()
	{
		for (int i = 0; i < 20; i++)
		{
			if (damageNumberActive[i])
			{
				numberAnimation[currentNumberAnimation[i]].getAnimation();
				damageNumber[i].draw(&numberAnimation[currentNumberAnimation[i]]);
				damageNumber[i].setAlpha(damageNumber[i].getAlpha() - 0.01f);
				damageNumber[i].setY(damageNumber[i].getY() + 0.1f);

				if (damageNumber[i].getAlpha() <= 0)
					damageNumberActive[i] = false;
			}
		}
	}

	//damageImage
	static WorldSprite damageNumber[20];
	static bool damageNumberActive[20];
	static int currentNumberAnimation[20];
	static int nextDamageNumber;
	static Animation numberAnimation[10];

protected:

	//movement
	float accelerationX;
	float accelerationY;
	float friction = 0.1f;
	float maxSpeed;

	//flags
	bool facing = false; //false: left
};

class Rabbit : public Mob
{
public:
	Rabbit(Rect imageRect, Rect collisionRect, Shader* shader, int health) : Mob(imageRect, collisionRect, shader, health)
	{
		animation[IDLE] = Animation(0, 160, 16, 16, 4, 100);
		animation[HOPPING] = Animation(0, 176, 16, 16, 5, 50);
		animation[DEAD] = Animation(64, 160, 16, 16, 1, 1000);

		spawnY = imageRect.y;

		friction = 0.0f;
		maxSpeed = 0.2f;

		lockedInScreen = true;
	}

	void draw()
	{
		image.draw(&animation[currentAnimation]);
	}
	void move()
	{
		if (hopping)
		{
			if (!animation[currentAnimation].getOneCycle())
			{
				hopping = false;
				currentAnimation = IDLE;

				velocityX = 0;
				velocityY = 0;
			}
		}
		else
		{
			if (rand() % 3000 == 1)
			{

				hopping = true;
				currentAnimation = HOPPING;
				animation[currentAnimation].cycleOnce();

				velocityX = ((rand() % 200 - 100) / 100.0f) * maxSpeed;
				velocityY = sqrt(maxSpeed * maxSpeed - velocityX * velocityX);
				if (rand() % 2 == 0)
					velocityY *= -1;
			}
		}

		Mob::move();
	}
	void die(Player* player);

private:

	float spawnY;

	bool hopping = false;

	//animations
	enum
	{
		IDLE,
		HOPPING,
		DEAD,

		NUM_ANIMATIONS
	};
	Animation animation[NUM_ANIMATIONS];
	int currentAnimation = 0;
};

class Enemy : public Mob
{
public:
	Enemy(Rect imageRect, Rect collisionRect, Shader* shader, int health) : Mob(imageRect, collisionRect, shader, health)
	{
		needsPlayer = true;
	}

	void checkAngered(Player* player);
	Rect getAttackRect()
	{
		if (facing)
			return Rect((image.getX() + image.getW()) - (attackRect.x + attackRect.w), image.getY() + attackRect.y, attackRect.w, attackRect.h);

		return Rect(image.getX() + attackRect.x, image.getY() + attackRect.y, attackRect.w, attackRect.h);
	}
	void hurtPlayer() { UsingAttackRect = false; }

protected:

	Rect attackRect;

	//things relative to player
	float angeredRadius;
	float angleWithPlayer;
	float distanceFromPlayer;

	//flags
	bool angered = false;
};

class Wolf : public Enemy
{
public:
	Wolf(Rect imageRect, Rect collisionRect, Shader* shader, int health) : Enemy(imageRect, collisionRect, shader, health)
	{
		animation[IDLE] = Animation(0, 192, 32, 32, 4, 150);
		animation[RUNNING] = Animation(0, 224, 32, 32, 3, 100);
		animation[DEAD] = Animation(96, 224, 32, 32, 1, 1000);
		animation[SURGING] = Animation(0, 256, 32, 32, 1, 1000);
		animation[ATTACKING] = Animation(32, 256, 32, 32, 4, 100);

		friction = 0.0004f;
		maxSpeed = 0.36f;
		angeredRadius = 1800;

		lockedInScreen = true;
		attackRect = Rect(40, 10, 220, 120);
	}

	void draw()
	{
		image.draw(&animation[currentAnimation]);
	}
	void move(Player* player);
	void die(Player* player);

private:

	float strikeRange = 500;
	unsigned int strikeTime = 700;
	unsigned int recoverTime = 1000;
	unsigned int wolfTimer;

	//flags
	bool striking = false;
	bool recovering = false;

	//animations
	enum
	{
		IDLE,
		RUNNING,
		SURGING,
		ATTACKING,
		DEAD,

		NUM_ANIMATIONS
	};
	Animation animation[NUM_ANIMATIONS];
	int currentAnimation = 0;

};

class Player : public Mob
{
public:
	Player(Rect imageRect, Rect collisionRect, Shader* shader, int health) : Mob(imageRect, collisionRect, shader, health)
	{
		animation[IDLE] = Animation(0, 0, 32, 32, 4, 150);
		animation[RUNNING] = Animation(0, 32, 32, 32, 5, 75);
		animation[MELEE] = Animation(0, 64, 32, 32, 6, 75);
		animation[SLEEPING] = Animation(128, 0, 32, 32, 1, 1000);

		attackRect = Rect(20, 70, 100, 50);

		friction = 0.0002f;
		maxSpeed = 0.2f;

		lockedInScreen = true;

		stats = PlayerStatistics(shader);
	}
	Player(){}

	//inventory;
	void setMeatCount(int meatCount)
	{
		this->meatCount = meatCount;
		if (this->meatCount > 10)
			this->meatCount = 10;
		else if (this->meatCount < 0)
			this->meatCount = 0;
	}
	inline int getMeatCount() { return meatCount; }
	void setWoodCount(int woodCount)
	{
		this->woodCount = woodCount;
		if (this->woodCount > 10)
			this->woodCount = 10;
		else if (this->woodCount < 0)
			this->woodCount = 0;
	}
	inline int getWoodCount() { return woodCount; }
	bool makingFire = false;

	//health
	void die(){ currentAnimation = SLEEPING; }

	//winning
	void madeItToBoat(float x, float y)
	{
		overRiddingInput = true;
		lockedInScreen = false;

		destinationX = x;
		destinationY = y;

		sleeping = false;
		attacking = false;
	}

	//loosing
	void reset()
	{
		setHealth(10);
		image.setX(SCREEN_WIDTH);
		image.setY(SCREEN_HEIGHT);

		setWoodCount(0);
		setMeatCount(0);
	}

	//core
	void draw();
	void move();
	void input(Input input);
	Rect getAttackRect();
	inline bool getAttackNow() { return attackNow; }

private:

	//inventory
	int woodCount = 0;
	int meatCount = 0;

	//attacking
	Rect attackRect;

	//flags
	bool attacking = false;
	bool attackNow = false;
	bool sleeping = true;
	bool overRiddingInput = false;

	//game controlling player
	float destinationX;
	float destinationY;

	//statistics
	PlayerStatistics stats;

	//animations
	enum
	{
		IDLE,
		RUNNING,
		MELEE,
		SLEEPING,

		NUM_ANIMATIONS
	};
	Animation animation[NUM_ANIMATIONS];
	int currentAnimation = SLEEPING;
};