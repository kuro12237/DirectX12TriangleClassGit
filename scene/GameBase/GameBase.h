#pragma once

class GameScene;

class GameBase
{
public:
	GameBase();
	~GameBase();

	virtual void Initialize(GameScene*game)=0;

	virtual void Update(GameScene* game)=0;

	virtual void Draw(GameScene* game) = 0;

	virtual void Release(GameScene* game)=0;


private:

};
