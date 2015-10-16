#pragma once
#include "globalVariablesAndIncludes.h"
#include "Mesh.h"
#include "Texture.h"
#include "Shader.h"

class Animation
{
public:
	static int CANVAS_WIDTH;
	static int CANVAS_HEIGHT;

	Animation(int x = 0, int y = 0, int w = 0, int h = 0, int numFrames = 0, int frameTime = 0)
	{
		basePosX = x;
		basePosY = y;
		frameWidth = w;
		frameHeight = h;
		this->numFrames = numFrames;
		this->frameTime = frameTime;
		frameTimer = SDL_GetTicks();
	}

	Rect getAnimation()
	{
		//checks for next frame
		if (SDL_GetTicks() > frameTimer + frameTime)
		{
			currentFrame++;
			if (currentFrame >= numFrames)
			{
				currentFrame = 0;
			}

			frameTimer = SDL_GetTicks();
		}

		return Rect((basePosX + frameWidth*currentFrame) / (float)CANVAS_WIDTH, basePosY / (float)CANVAS_HEIGHT, frameWidth / (float)CANVAS_WIDTH, frameHeight / (float)CANVAS_HEIGHT);
	}
	inline int getCurrentFrame() { return currentFrame; }

	//single cycle
	void cycleOnce()
	{
		oneCycleTime = SDL_GetTicks();
		currentFrame = 0;
		frameTimer = SDL_GetTicks();
	}
	inline bool getOneCycle()
	{
		return oneCycleTime + frameTime * numFrames > SDL_GetTicks();
	}

private:
	//position
	unsigned int basePosX;
	unsigned int basePosY;

	//frames
	unsigned int frameWidth;
	unsigned int frameHeight;
	unsigned int numFrames;

	//Timing
	unsigned int frameTime; //milliseconds
	unsigned int frameTimer;
	unsigned int currentFrame = 0;

	//flags
	int oneCycleTime;
};

//z-axis lines for drawing order
const float STATS_LINE = -0.2f;
const float	ENTITY_LINE = 0.0f;
const float FLAT_ENTITY_LINE = 0.2f;
const float	TILE_LINE = 0.4f;
const float	REFLECTION_LINE = 0.6f;
const float SKY_LINE = 0.8f;

class Sprite
{
public:
	Sprite(float x, float y, float w, float h, Shader* shader, const float zDrawingLine);
	Sprite(){};
	~Sprite();
	
	//core
	void draw(Animation* animation);
	virtual glm::mat4 getTransform();

	//characteristics
	void setColor(float r, float g, float b) { color = { r, g, b, color.a }; };

	//transformations
	glm::vec4& getColor() { return color; }
	void setX(float x) { rect.x = x; }
	float getX() { return rect.x; }
	void setY(float y) { rect.y = y; }
	float getY() { return rect.y; }
	void setW(float w) { rect.w = w; }
	float getW() { return rect.w; }
	void setH(float h) { rect.h = h; }
	float getH() { return rect.h; }
	void setAlpha(float a) { color.a = a; }
	float getAlpha() { return color.a; }

	//flags
	void setHorizontalFlip(bool flip){ horizontalFlip = flip; }

protected:

	//characteristics
	glm::vec4 color = { 1, 1, 1, 1 };
	float drawingLine;

	//flags
	bool horizontalFlip = false;
	bool verticleFlip = false;

	//transformations
	Rect rect;

	Shader* shader;
	Mesh* mesh;
};

class WorldSprite : public Sprite
{
public:
	static double cameraFocusX;
	static double cameraFocusY;

	WorldSprite(float x, float y, float w, float h, Shader* shader, const float zDrawingLine) : Sprite(x, y, w, h, shader, zDrawingLine){}
	WorldSprite(){}

	glm::mat4 getTransform()
	{
		return glm::scale(glm::vec3(1 / SCREEN_WIDTH, 1 / SCREEN_HEIGHT, 1)) * glm::translate(glm::vec3(rect.x + rect.w/2 - (int)cameraFocusX, rect.y + rect.h/2 - (int)cameraFocusY, ((rect.y - cameraFocusY))*0.0001) + drawingLine) * glm::scale(glm::vec3(rect.w, rect.h, 1)) * glm::rotate(180.0f * horizontalFlip, glm::vec3(0, 1, 0));
	}
};