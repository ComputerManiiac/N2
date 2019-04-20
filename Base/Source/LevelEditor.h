#ifndef LEVELEDITOR_H
#define LEVELEDITOR_H

#include "Entity2D.h"

class LevelEditor
{
public:
	LevelEditor();
	~LevelEditor();

	void Initialize();
	void Update(Entity* entities[24][32], unsigned int& entityCount);



private:

	void getTileAtCursor(int& x, int &y, float divisor);

	ShaderProgram* tileShader;
	Entity* tileMap;
	Entity* selection;
	Vector2 textureOffset;
	Vector2 textureSize;

	Vector2 start;
	Vector2 end;
	bool defineTileMode;
};

#endif