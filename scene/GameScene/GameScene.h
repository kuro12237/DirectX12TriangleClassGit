#pragma once

class Game;

class GameScene
{
public:
	GameScene();
	~GameScene();

	virtual void Initialize(Game*game)=0;

	virtual void Update(Game* game)=0;

	virtual void Draw(Game* game) = 0;

	virtual void Release(Game* game)=0;


private:

};
