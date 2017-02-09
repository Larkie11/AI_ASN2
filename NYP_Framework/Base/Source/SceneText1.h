#ifndef SCENE_TEXT1_H
#define SCENE_TEXT1_H

#include "Scene.h"
#include "Mtx44.h"
#include "PlayerInfo/PlayerInfo.h"
#include "GroundEntity.h"
#include "FPSCamera.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "GenericEntity.h"
#include "SceneGraph/UpdateTransformation.h"
#include "Enemy\Enemy.h"
#include "Sound.h"
#include <ctime>
#include "Moving.h"
#include "../SpriteEntity.h"

class ShaderProgram;
class SceneManager;
class TextEntity;
class Light;

class SceneText1 : public Scene
{	
public:
	SceneText1();
	~SceneText1();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

private:
	//SceneText1(SceneManager* _sceneMgr); // This is used to register to SceneManager
	ShaderProgram* currProg;
	CPlayerInfo* playerInfo;
	GroundEntity* groundEntity;
	FPSCamera camera;
	TextEntity* textObj[13];
	Light* lights[2];
	GenericEntity* theCube[5];
	unsigned t0;
	unsigned timepass;
	string waveNo = "1";
	int currWaveEnemy = 0;
	unsigned  elasped = 0;
	Moving* movingEnemy;
	int wave1;
	int wave2;
	int wave3;
	int wave4;
	int survive;
	CEnemy* theEnemy;
	SpriteEntity* volume;
	bool ShowSettings = false;
	Vector3 settingsScale;
	float sound;
	static SceneText1* sInstance; // The pointer to the object that gets registered
};

#endif