#pragma once

#include "Scene.h"
#include "Mtx44.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "../FPSCamera.h"
#include "../SpriteEntity.h"

class SceneManager;

class TextEntity;
class CSettings : public Scene
{
public:
	CSettings();
	~CSettings();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

private:
	FPSCamera camera;
	SpriteEntity* MenuStateBG;
	SpriteEntity* Settings;
	float settingypos;
	TextEntity* textObj[1];
	bool showLoading;
	float sound;
};