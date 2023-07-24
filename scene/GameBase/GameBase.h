#pragma once

class GameScene;

class GameBase
{
public:
	GameBase();
	~GameBase();

	virtual void Initialize()=0;

	virtual void Update()=0;

	virtual void Draw() = 0;

	virtual void Release()=0;


private:

};
