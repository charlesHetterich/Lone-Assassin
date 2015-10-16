#pragma once
#include "globalVariablesAndIncludes.h"
#include "Input.h"
#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"
#include "Sprite.h"
#include "Entity.h"
#include "Tile.h"
#include "World.h"

class Game
{
public:
	Game();
	~Game();

	void input(Input input);
	void update();
	void render();

private:

	Player player;
	World world;

	//---------------------------------------------------------------------------
	//CORE
	//---------------------------------------------------------------------------
	//shaders
	enum
	{
		BASIC_SHADER,
		PIXEL_SHADER,
		WAVEY_SHADER,

		NUM_SHADERS
	};
	Shader* shader[NUM_SHADERS];

	//textures
	enum
	{
		NO_TEXTURE,
		SPRITE_SHEET,

		NUM_TEXTURES
	};
	CTexture* texture[NUM_TEXTURES];
};

