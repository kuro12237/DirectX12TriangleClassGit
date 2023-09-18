#pragma once

class GameManager;
class IScene
{
public:
    IScene() {};
	virtual ~IScene() {};
	virtual void Initialize(GameManager* state) = 0;
	virtual void Update(GameManager* state) = 0;
	virtual void Draw(GameManager* state) = 0;
private:

};

