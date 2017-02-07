#pragma once

#include "Scene.h"
#include "Mtx44.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "../FPSCamera.h"
#include "../SpriteEntity.h"
#include "../GenericEntity.h"


class SceneManager;
class TextEntity;

class CIntroState : public Scene
{
public:
	CIntroState();
	~CIntroState();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

private:
	FPSCamera camera;
	SpriteEntity* IntroStateBG;
	float toNextScene;
	TextEntity* textObj[13];
	float z = 1.f;
	bool fadedInText = false;

};