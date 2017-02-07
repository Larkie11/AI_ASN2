#pragma once

#include "Scene.h"
#include "Mtx44.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "../FPSCamera.h"
#include "../SpriteEntity.h"

class SceneManager;

class TextEntity;

enum States
{
	GAMEPLAY,
	INSTRUCTION,
	SETTINGS,
	QUIT,
	MAX_STATES,
};

class CMenuState : public Scene
{
public:
	CMenuState();
	~CMenuState();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

private:
	FPSCamera camera;
	SpriteEntity* MenuStateBG;
	SpriteEntity* Logo;
	SpriteEntity* Arrow;

	TextEntity* textObj[6];
	bool showLoading;
	float logoypos;
	float arrowypos;
	States currState;
	int myState;
};