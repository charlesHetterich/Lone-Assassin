#include "Game.h"

int Animation::CANVAS_WIDTH = 2304;
int Animation::CANVAS_HEIGHT = 2304;

double WorldSprite::cameraFocusX = 0.0f;
double WorldSprite::cameraFocusY = 0.0f;

WorldSprite Mob::damageNumber[20];
bool Mob::damageNumberActive[20];
int Mob::currentNumberAnimation[20];
int Mob::nextDamageNumber;
Animation Mob::numberAnimation[10];

Game::Game()
{
	shader[BASIC_SHADER] = new Shader("basic");
	texture[SPRITE_SHEET] = new CTexture("sprites.png");
	texture[SPRITE_SHEET]->bind(0);

	player = Player(Rect(SCREEN_WIDTH, SCREEN_HEIGHT, 320, 320), Rect(130, 0, 60, 150), shader[BASIC_SHADER], 10);

	world = World(12, 1000, 320, 320, shader[BASIC_SHADER]);

	WorldSprite::cameraFocusX = SCREEN_WIDTH;
	WorldSprite::cameraFocusY = player.getImage()->getY();
	
	//set damage number static
	for (int i = 0; i < 20; i++)
	{
		Mob::damageNumber[i] = WorldSprite(0, 0, 80, 80, shader[BASIC_SHADER], STATS_LINE);
		Mob::damageNumberActive[i] = false;
		Mob::currentNumberAnimation[i] = 0;
	}
	Mob::nextDamageNumber = 0;
	for (int i = 0; i < 10; i++)
		Mob::numberAnimation[i] = Animation(224 + 8 * i, 8, 8, 8, 1, 1000);
}

Game::~Game()
{
	delete [] &shader;
	delete [] &texture;
}

void Game::input(Input input)
{
	if (input.getKeyDown(SDL_SCANCODE_F2))
	{
		//SDL_Surface *sshot = SDL_CreateRGBSurface(0, ACTUAL_SCREEN_WIDTH, ACTUAL_SCREEN_HEIGHT, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
		//SDL_RenderReadPixels(sdl_Setup->GetRenderer(), NULL, SDL_PIXELFORMAT_ARGB8888, sshot->pixels, sshot->pitch);

		//SDL_SaveBMP(sshot, "screenshots/screenshot.bmp");
		//SDL_FreeSurface(sshot);
		//std::cout << "hi";
	}

	player.input(input);
}

void Game::update()
{
	//player died
	if (player.getHealth() <= 0)
	{
		player.reset();
		world = World(12, 1000, 320, 320, shader[BASIC_SHADER]);
	}

	//creating entities
	if (player.makingFire)
	{
		world.createFire(player.getImage()->getX(), player.getImage()->getY(), shader[BASIC_SHADER]);
		player.makingFire = false;
	}

	//movement
	player.move();
	world.moveEntities(&player);

	WorldSprite::cameraFocusY = WorldSprite::cameraFocusY + (player.getImage()->getY() - WorldSprite::cameraFocusY + 500) / 500.0f;
	if (WorldSprite::cameraFocusY < SCREEN_HEIGHT)
		WorldSprite::cameraFocusY = SCREEN_HEIGHT;

	//collisions
	if (player.getAttackNow())
		world.playerAttack(&player);

	world.attackPlayer(&player);

	world.checkPlayerNearBoat(&player);
}

void Game::render()
{
	shader[BASIC_SHADER]->bind();

	world.draw();
	player.draw();

	Mob::drawDamageNumbers();
}