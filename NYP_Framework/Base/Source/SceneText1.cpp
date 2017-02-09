#include "SceneText1.h"
#include "GL\glew.h"
#include <iomanip>

#include "shader.hpp"
#include "MeshBuilder.h"
#include "Application.h"
#include "Utility.h"
#include "LoadTGA.h"
#include <sstream>
#include "KeyboardController.h"
#include "MouseController.h"
#include "SceneManager.h"
#include "GraphicsManager.h"
#include "ShaderProgram.h"
#include "EntityManager.h"

#include "GenericEntity.h"
#include "GroundEntity.h"
#include "TextEntity.h"
#include "SpriteEntity.h"
#include "Light.h"
#include "SkyBox/SkyBoxEntity.h"
#include "SceneGraph\SceneGraph.h"
#include "SpatialPartition\SpatialPartition.h"
#include "Lua\LuaInterface.h"
#include "../Sound.h"

#include <iostream>
using namespace std;


//SceneText1* SceneText1::sInstance = new SceneText1(SceneManager::GetInstance());

SceneText1::SceneText1()
{
}

//SceneText1::SceneText1(SceneManager* _sceneMgr)
//{
//	_sceneMgr->AddScene("Start", this);
//}

SceneText1::~SceneText1()
{
	CSpatialPartition::GetInstance()->RemoveCamera();
	CSceneGraph::GetInstance()->Destroy();
}

void SceneText1::Init()
{
	t0 = clock();
	lua_getglobal(CLuaInterface::GetInstance()->theLuaState, "waves");
	wave1 = CLuaInterface::GetInstance()->GetField("wave1");
	wave2 = CLuaInterface::GetInstance()->GetField("wave2");
	wave3 = CLuaInterface::GetInstance()->GetField("wave3");
	wave4 = CLuaInterface::GetInstance()->GetField("wave4");
	survive = CLuaInterface::GetInstance()->GetField("survive");

	//currProg = GraphicsManager::GetInstance()->LoadShader("default", "Shader//Texture.vertexshader", "Shader//Texture.fragmentshader");
	//// Tell the shader program to store these uniform locations
	//
	//currProg->AddUniform("MVP");
	//currProg->AddUniform("MV");
	//currProg->AddUniform("MV_inverse_transpose");
	//currProg->AddUniform("material.kAmbient");
	//currProg->AddUniform("material.kDiffuse");
	//currProg->AddUniform("material.kSpecular");
	//currProg->AddUniform("material.kShininess");
	//currProg->AddUniform("lightEnabled");
	//currProg->AddUniform("numLights");
	//currProg->AddUniform("lights[0].type");
	//currProg->AddUniform("lights[0].position_cameraspace");
	//currProg->AddUniform("lights[0].color");
	//currProg->AddUniform("lights[0].power");
	//currProg->AddUniform("lights[0].kC");
	//currProg->AddUniform("lights[0].kL");
	//currProg->AddUniform("lights[0].kQ");
	//currProg->AddUniform("lights[0].spotDirection");
	//currProg->AddUniform("lights[0].cosCutoff");
	//currProg->AddUniform("lights[0].cosInner");
	//currProg->AddUniform("lights[0].exponent");
	//currProg->AddUniform("lights[1].type");
	//currProg->AddUniform("lights[1].position_cameraspace");
	//currProg->AddUniform("lights[1].color");
	//currProg->AddUniform("lights[1].power");
	//currProg->AddUniform("lights[1].kC");
	//currProg->AddUniform("lights[1].kL");
	//currProg->AddUniform("lights[1].kQ");
	//currProg->AddUniform("lights[1].spotDirection");
	//currProg->AddUniform("lights[1].cosCutoff");
	//currProg->AddUniform("lights[1].cosInner");
	//currProg->AddUniform("lights[1].exponent");
	//currProg->AddUniform("colorTextureEnabled");
	//currProg->AddUniform("colorTexture");
	//currProg->AddUniform("textEnabled");
	//currProg->AddUniform("textColor");
	//
	//// Tell the graphics manager to use the shader we just loaded
	//GraphicsManager::GetInstance()->SetActiveShader("default");

	//lights[0] = new Light();
	//GraphicsManager::GetInstance()->AddLight("lights[0]", lights[0]);
	//lights[0]->type = Light::LIGHT_DIRECTIONAL;
	//lights[0]->position.Set(0, 20, 0);
	//lights[0]->color.Set(1, 1, 1);
	//lights[0]->power = 1;
	//lights[0]->kC = 1.f;
	//lights[0]->kL = 0.01f;
	//lights[0]->kQ = 0.001f;
	//lights[0]->cosCutoff = cos(Math::DegreeToRadian(45));
	//lights[0]->cosInner = cos(Math::DegreeToRadian(30));
	//lights[0]->exponent = 3.f;
	//lights[0]->spotDirection.Set(0.f, 1.f, 0.f);
	//lights[0]->name = "lights[0]";

	//lights[1] = new Light();
	//GraphicsManager::GetInstance()->AddLight("lights[1]", lights[1]);
	//lights[1]->type = Light::LIGHT_DIRECTIONAL;
	//lights[1]->position.Set(1, 1, 0);
	//lights[1]->color.Set(1, 1, 0.5f);
	//lights[1]->power = 0.4f;
	//lights[1]->name = "lights[1]";

	//currProg->UpdateInt("numLights", 1);
	//currProg->UpdateInt("textEnabled", 0);
	
	// Create the playerinfo instance, which manages all information about the player
	playerInfo = CPlayerInfo::GetInstance();
	playerInfo->Init();

	// Create and attach the camera to the scene
	//camera.Init(Vector3(0, 0, 10), Vector3(0, 0, 0), Vector3(0, 1, 0));
	camera.Init(playerInfo->GetPos(), playerInfo->GetTarget(), playerInfo->GetUp());
	playerInfo->AttachCamera(&camera);
	GraphicsManager::GetInstance()->AttachCamera(&camera);

	// Load all the meshes
	MeshBuilder::GetInstance()->GenerateAxes("reference");
	MeshBuilder::GetInstance()->GenerateCrossHair("crosshair");
	MeshBuilder::GetInstance()->GenerateQuad("quad", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("quad")->textureID = LoadTGA("Image//calibri.tga");
	MeshBuilder::GetInstance()->GenerateText("text", 16, 16);
	MeshBuilder::GetInstance()->GetMesh("text")->textureID = LoadTGA("Image//calibri.tga");
	MeshBuilder::GetInstance()->GetMesh("text")->material.kAmbient.Set(1, 0, 0);
	MeshBuilder::GetInstance()->GenerateRing("ring", Color(1, 0, 1), 36, 1, 0.5f);
	MeshBuilder::GetInstance()->GenerateSphere("lightball", Color(1, 1, 1), 18, 36, 1.f);
	MeshBuilder::GetInstance()->GenerateSphere("sgbullet", Color(1, 0, 0), 18, 36, 0.2f);
	MeshBuilder::GetInstance()->GenerateSphere("sphere", Color(1, 0, 0), 18, 36, 0.5f);
	MeshBuilder::GetInstance()->GenerateCone("cone", Color(0.5f, 1, 0.3f), 36, 10.f, 10.f);
	MeshBuilder::GetInstance()->GenerateCube("cube", Color(1.0f, 1.0f, 0.0f), 1.0f);
	MeshBuilder::GetInstance()->GetMesh("cone")->material.kDiffuse.Set(0.99f, 0.99f, 0.99f);
	MeshBuilder::GetInstance()->GetMesh("cone")->material.kSpecular.Set(0.f, 0.f, 0.f);
	MeshBuilder::GetInstance()->GenerateQuad("GROUND", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("GROUND")->textureID = LoadTGA("Image//ground.tga");
	MeshBuilder::GetInstance()->GenerateCube("cubeSG", Color(1.0f, 0.64f, 0.0f), 1.0f);
	MeshBuilder::GetInstance()->GenerateOBJ("gun", "OBJ//gun.obj");
	MeshBuilder::GetInstance()->GetMesh("gun")->textureID = LoadTGA("Image//gun.tga");
	MeshBuilder::GetInstance()->GenerateQuad("GRIDMESH_GROUND", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("GRIDMESH_GROUND")->textureID = LoadTGA("Image//ground2.tga");

	MeshBuilder::GetInstance()->GenerateOBJ("windmillstick", "OBJ//windmillstick.obj");
	MeshBuilder::GetInstance()->GetMesh("windmillstick")->textureID = LoadTGA("Image//windmillstick.tga");
	//MeshBuilder::GetInstance()->GenerateOBJ("windmillblade", "OBJ//windmillblade.obj");

	//MeshBuilder::GetInstance()->GenerateRectangular("hand", Color(1.0f, 0.64f, 0.0f), 1.0f,5.0f,1.0f);
	////LOD objs for assignment 1
	MeshBuilder::GetInstance()->GenerateOBJ("m24r", "Obj//M24R.obj");
	MeshBuilder::GetInstance()->GetMesh("m24r")->textureID = LoadTGA("Image//M24R.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("mp5k", "Obj//MP5K.obj");
	MeshBuilder::GetInstance()->GetMesh("mp5k")->textureID = LoadTGA("Image//MP5K.tga");

	MeshBuilder::GetInstance()->GenerateOBJ("Camp_high", "Obj//Camp_high.obj"); //LOD MODEL 1
	MeshBuilder::GetInstance()->GetMesh("Camp_high")->textureID = LoadTGA("Image//wood_high.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("Camp_mid", "Obj//Camp_mid.obj");
	MeshBuilder::GetInstance()->GetMesh("Camp_mid")->textureID = LoadTGA("Image//wood_high.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("Camp_low", "Obj//Camp_low.obj");
	MeshBuilder::GetInstance()->GetMesh("Camp_low")->textureID = LoadTGA("Image//wood_high.tga");
	
	MeshBuilder::GetInstance()->GenerateOBJ("Head_high", "Obj//Head_high.obj"); //LOD MODEL 2 (Seperated into head and body for scene node)
	MeshBuilder::GetInstance()->GetMesh("Head_high")->textureID = LoadTGA("Image//head_high.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("Head_mid", "Obj//Head_mid.obj");
	MeshBuilder::GetInstance()->GetMesh("Head_mid")->textureID = LoadTGA("Image//head_high.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("Head_low", "Obj//Head_low.obj");
	MeshBuilder::GetInstance()->GetMesh("Head_low")->textureID = LoadTGA("Image//head_high.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("Body_high", "Obj//body_high.obj");
	MeshBuilder::GetInstance()->GetMesh("Body_high")->textureID = LoadTGA("Image//body_high.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("Body_mid", "Obj//body_mid.obj");
	MeshBuilder::GetInstance()->GetMesh("Body_mid")->textureID = LoadTGA("Image//body_high.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("Body_low", "Obj//body_low.obj");
	MeshBuilder::GetInstance()->GetMesh("Body_low")->textureID = LoadTGA("Image//body_high.tga");

	//MeshBuilder::GetInstance()->GenerateOBJ("Head", "Obj//Head.obj");
	//MeshBuilder::GetInstance()->GenerateOBJ("Body", "Obj//Body.obj");

	//MeshBuilder::GetInstance()->GenerateOBJ("Hand", "Obj//Hand.obj");
	
	MeshBuilder::GetInstance()->GenerateOBJ("Robot_camp1_high", "Obj//robotspawncamp1_high.obj"); //LOD MODEL 3
	MeshBuilder::GetInstance()->GetMesh("Robot_camp1_high")->textureID = LoadTGA("Image//robotspawncamp1_high.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("Robot_camp1_mid", "Obj//robotspawncamp1_mid.obj");
	MeshBuilder::GetInstance()->GetMesh("Robot_camp1_mid")->textureID = LoadTGA("Image//robotspawncamp1_high.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("Robot_camp1_low", "Obj//robotspawncamp1_low.obj");
	MeshBuilder::GetInstance()->GetMesh("Robot_camp1_low")->textureID = LoadTGA("Image//robotspawncamp1_high.tga");

	MeshBuilder::GetInstance()->GenerateOBJ("Robot_camp2_high", "Obj//robotspawncamp2_high.obj"); //LOD MODEL 4
	MeshBuilder::GetInstance()->GetMesh("Robot_camp2_high")->textureID = LoadTGA("Image//robotspawncamp2_high.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("Robot_camp2_mid", "Obj//robotspawncamp2_mid.obj");
	MeshBuilder::GetInstance()->GetMesh("Robot_camp2_mid")->textureID = LoadTGA("Image//robotspawncamp2_high.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("Robot_camp2_low", "Obj//robotspawncamp2_low.obj");
	MeshBuilder::GetInstance()->GetMesh("Robot_camp2_low")->textureID = LoadTGA("Image//robotspawncamp2_high.tga");

	MeshBuilder::GetInstance()->GenerateOBJ("Robot_camp3_high", "Obj//robotspawncamp3_high.obj"); //LOD MODEL 5
	MeshBuilder::GetInstance()->GetMesh("Robot_camp3_high")->textureID = LoadTGA("Image//robotspawncamp3_high.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("Robot_camp3_mid", "Obj//robotspawncamp3_mid.obj");
	MeshBuilder::GetInstance()->GetMesh("Robot_camp3_mid")->textureID = LoadTGA("Image//robotspawncamp3_high.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("Robot_camp3_low", "Obj//robotspawncamp3_low.obj");
	MeshBuilder::GetInstance()->GetMesh("Robot_camp3_low")->textureID = LoadTGA("Image//robotspawncamp3_high.tga");

	MeshBuilder::GetInstance()->GenerateOBJ("robot1_high", "Obj//robot1_high.obj"); //LOD MODEL 6
	MeshBuilder::GetInstance()->GetMesh("robot1_high")->textureID = LoadTGA("Image//ROBOT1_high.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("robot1_mid", "Obj//robot1_mid.obj");
	MeshBuilder::GetInstance()->GetMesh("robot1_mid")->textureID = LoadTGA("Image//ROBOT1_high.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("robot1_low", "Obj//robot1_low.obj");
	MeshBuilder::GetInstance()->GetMesh("robot1_low")->textureID = LoadTGA("Image//ROBOT1_high.tga");

	MeshBuilder::GetInstance()->GenerateOBJ("Android_high", "Obj//android_high.obj"); //LOD MODEL 7
	MeshBuilder::GetInstance()->GetMesh("Android_high")->textureID = LoadTGA("Image//android_high.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("Android_mid", "Obj//android_mid.obj");
	MeshBuilder::GetInstance()->GetMesh("Android_mid")->textureID = LoadTGA("Image//android_high.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("Android_low", "Obj//android_low.obj");
	MeshBuilder::GetInstance()->GetMesh("Android_low")->textureID = LoadTGA("Image//android_high.tga");

	MeshBuilder::GetInstance()->GenerateOBJ("pistolB", "Obj//pistolbullet.obj");
	MeshBuilder::GetInstance()->GetMesh("pistolB")->textureID = LoadTGA("Image//pistolbullet.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("shotgunB", "Obj//shotgunbullet.obj");
	MeshBuilder::GetInstance()->GetMesh("shotgunB")->textureID = LoadTGA("Image//shotgunbullet.tga");

	MeshBuilder::GetInstance()->GenerateOBJ("tree", "Obj//tree_mid.obj");
	MeshBuilder::GetInstance()->GetMesh("tree")->textureID = LoadTGA("Image//tree_mid.tga");
	
	MeshBuilder::GetInstance()->GenerateQuad("SKYBOX_FRONT", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GenerateQuad("SKYBOX_BACK", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GenerateQuad("SKYBOX_LEFT", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GenerateQuad("SKYBOX_RIGHT", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GenerateQuad("SKYBOX_TOP", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GenerateQuad("SKYBOX_BOTTOM", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("SKYBOX_FRONT")->textureID = LoadTGA("Image//SkyBox//skybox_front.tga");
	MeshBuilder::GetInstance()->GetMesh("SKYBOX_BACK")->textureID = LoadTGA("Image//SkyBox//skybox_back.tga");
	MeshBuilder::GetInstance()->GetMesh("SKYBOX_LEFT")->textureID = LoadTGA("Image//SkyBox//skybox_right.tga");
	MeshBuilder::GetInstance()->GetMesh("SKYBOX_RIGHT")->textureID = LoadTGA("Image//SkyBox//skybox_left.tga");
	MeshBuilder::GetInstance()->GetMesh("SKYBOX_TOP")->textureID = LoadTGA("Image//SkyBox//skybox_top.tga");
	MeshBuilder::GetInstance()->GetMesh("SKYBOX_BOTTOM")->textureID = LoadTGA("Image//SkyBox//skybox_bottom.tga");
	MeshBuilder::GetInstance()->GenerateRay("laser", 10.0f);
	MeshBuilder::GetInstance()->GenerateQuad("GRIDMESH", Color(1, 1, 1), 0.f);
	

	// Set up the Spatial Partition and pass it to the EntityManager to manage
	CSpatialPartition::GetInstance()->Init(100, 100, 10, 10);
	CSpatialPartition::GetInstance()->SetMesh("GRIDMESH");
	CSpatialPartition::GetInstance()->SetCamera(&camera);
	CSpatialPartition::GetInstance()->SetLevelOfDistance(40000, 160000.f);
	EntityManager::GetInstance()->SetSpatialPartition(CSpatialPartition::GetInstance());

	// Create entities into the scene
	//Create::Entity("reference", Vector3(0.0f, 0.0f, 0.0f)); // Reference
	//Create::Entity("lightball", Vector3(lights[0]->position.x, lights[0]->position.y, lights[0]->position.z)); // Lightball

	GenericEntity* stick = Create::Asset("windmillstick", Vector3(0.0f, -5.0f, 0.0f));
	CSceneNode* stickNode = CSceneGraph::GetInstance()->AddNode(stick);

	GenericEntity* robot = Create::Entity("robot1_high", Vector3(100.0f, 0.0f, -100.0f));
	robot->SetCollider(true);
	robot->SetAABB(Vector3(1.f, 1.f, 1.f), Vector3(-1.f, -1.f, -1.f));
	robot->InitLOD("robot1_high", "robot1_mid", "robot_low");

	//robot camps
	GenericEntity* RobotCamp1 = Create::Entity("Robot_camp1_high", Vector3(-300.f, -10.f, -330.f), Vector3(5.f, 5.f, 5.f));
	RobotCamp1->InitLOD("Robot_camp1_high", "Robot_camp1_mid", "Robot_camp1_low");
	GenericEntity* RobotCamp2 = Create::Entity("Robot_camp2_high", Vector3(225.f, -5.f, -200.f), Vector3(5.f, 5.f, 5.f));
	RobotCamp2->InitLOD("Robot_camp2_high", "Robot_camp2_mid", "Robot_camp2_low");
	GenericEntity* RobotCamp3 = Create::Entity("Robot_camp3_high", Vector3(-33.5f, -8.f, -400.5f), Vector3(5.f, 5.f, 5.f));
	RobotCamp3->InitLOD("Robot_camp3_high", "Robot_camp3_mid", "Robot_camp3_low");

	GenericEntity* RobotCampC = Create::Entity("Robot_camp1_high", Vector3(-150.f, -5.f, 300.f), Vector3(5.4f, 5.4f, 5.4f));
	RobotCampC->InitLOD("Robot_camp1_high", "Robot_camp1_mid", "Robot_camp1_low");
	GenericEntity* RobotCampD = Create::Entity("Robot_camp1_high", Vector3(-33.5f, -9.f, 300.f), Vector3(7.f, 7.f, 7.f));
	RobotCampD->InitLOD("Robot_camp1_high", "Robot_camp1_high", "Robot_camp1_high");
	GenericEntity* RobotCampE = Create::Entity("Robot_camp1_high", Vector3(70.f, -5.f, 300.f), Vector3(5.4f, 5.4f, 5.4f));
	RobotCampE->InitLOD("Robot_camp1_high", "Robot_camp1_mid", "Robot_camp1_low");
	GenericEntity* RobotCampF = Create::Entity("Robot_camp1_high", Vector3(-80.f, -5.f, 300.f), Vector3(5.4f, 5.4f, 5.4f));
	RobotCampF->InitLOD("Robot_camp1_high", "Robot_camp1_mid", "Robot_camp1_low");
	GenericEntity* RobotCampG = Create::Entity("Robot_camp3_high", Vector3(200.f, -15.f, 300.f), Vector3(7.4f, 7.4f, 7.4f));
	RobotCampG->InitLOD("Robot_camp3_high", "Robot_camp3_mid", "Robot_camp3_low");
	GenericEntity* RobotCampH = Create::Entity("Robot_camp3_high", Vector3(200.f, -15.f, -300.f), Vector3(7.4f, 7.4f, 7.4f));
	RobotCampH->InitLOD("Robot_camp3_high", "Robot_camp3_mid", "Robot_camp3_low");
	GenericEntity* RobotCampI = Create::Entity("Robot_camp3_high", Vector3(-200.f, -8.f, -10.f), Vector3(3.f, 3.f, 3.f));
	RobotCampI->InitLOD("Robot_camp3_high", "Robot_camp3_mid", "Robot_camp3_low");
	GenericEntity* RobotCampJ = Create::Entity("Robot_camp2_high", Vector3(-200.f, -5.f, -50.f), Vector3(3.f, 3.f, 3.f));
	RobotCampJ->InitLOD("Robot_camp2_high", "Robot_camp2_mid", "Robot_camp2_low");
	GenericEntity* RobotCampK = Create::Entity("Robot_camp2_high", Vector3(-200.f, -5.f, 100.f), Vector3(3.f, 3.f, 3.f));
	RobotCampK->InitLOD("Robot_camp2_high", "Robot_camp2_mid", "Robot_camp2_low");
	GenericEntity* RobotCampL = Create::Entity("Robot_camp1_high", Vector3(200.f, -5.f, 28.f), Vector3(5.f, 5.f, 5.f));
	RobotCampL->InitLOD("Robot_camp1_high", "Robot_camp1_mid", "Robot_camp1_low");
	GenericEntity* RobotCampM = Create::Entity("Robot_camp1_high", Vector3(200.f, -5.f, -350.f), Vector3(3.f, 3.f, 3.f));
	RobotCampM->InitLOD("Robot_camp1_high", "Robot_camp1_mid", "Robot_camp1_low");
	GenericEntity* RobotCampN = Create::Entity("Robot_camp1_high", Vector3(-159.f, -5.f, -200.f), Vector3(3.f, 3.f, 3.f));
	RobotCampN->InitLOD("Robot_camp1_high", "Robot_camp1_mid", "Robot_camp1_low");


	//tree1->InitLOD("tree_mid", "tree_mid", "tree_low");

	for (int i = 0; i < 30; i++)
	{
		GenericEntity* tree = Create::Entity("tree", Vector3(Math::RandIntMinMax(-300, 300), 0.0f, Math::RandIntMinMax(-300, 350)), Vector3(3.f, 3.f, 3.f));
	}
	for (int i = 0; i < 30; i++)
	{
		GenericEntity* tree2 = Create::Entity("tree", Vector3(Math::RandIntMinMax(-300, 300), 0.0f, Math::RandIntMinMax(-300, -350)), Vector3(3.f, 3.f, 3.f));
	}
	GenericEntity* anotherCube = Create::Entity("Camp_high", Vector3(0.0f, -5.f, 0.0f));
	anotherCube->SetCollider(true);
	anotherCube->SetAABB(Vector3(23.f, 10.f, 20.f), Vector3(-23.f, -10.f, -20.f));
	anotherCube->InitLOD("Camp_high", "Camp_mid", "Camp_low");
	anotherCube->IsEnemy(false);
	anotherCube->IsCamp("Camp");

	GenericEntity* windmill = Create::Asset("windmillstick", Vector3(0.0f, 5.0f, 0.0f), Vector3(1.0, 0.5f, 1.0f));
	CSceneNode* windmillNode = CSceneGraph::GetInstance()->AddNode(windmill);

	CUpdateTransformation* baseMtx = new CUpdateTransformation();
	baseMtx->ApplyUpdate(100.0f, 0.0f, 0.0f, 1.0f);
	baseMtx->SetSteps(-60, 60);
	windmillNode->SetUpdateTransformation(baseMtx);

	GenericEntity* childwindmill = Create::Asset("windmillstick", Vector3(0.0f, 0.0f, 0.0f));
	CSceneNode* childNode = windmillNode->AddChild(childwindmill);
	childNode->ApplyTranslate(0.0f, 1.0f, 0.0f);

	groundEntity = Create::Ground("GROUND", "GROUND");
	Create::Sprite2DObject("crosshair", Vector3(0.0f, 0.0f, 0.0f), Vector3(10.0f, 10.0f, 10.0f));

	SkyBoxEntity* theSkyBox = Create::SkyBox("SKYBOX_FRONT", "SKYBOX_BACK",
											 "SKYBOX_LEFT", "SKYBOX_RIGHT",
											 "SKYBOX_TOP", "SKYBOX_BOTTOM");

	// Customise the ground entity
	groundEntity->SetPosition(Vector3(0, -10, 0));
	groundEntity->SetScale(Vector3(100.0f, 100.0f, 100.0f));
	groundEntity->SetGrids(Vector3(10.0f, 1.0f, 10.0f));
	playerInfo->SetTerrain(groundEntity);
	
	movingEnemy = new Moving();
	movingEnemy->Init();
	lua_getglobal(CLuaInterface::GetInstance()->theLuaState, "WP1");
	movingEnemy->WayPoints.push_back(Vector3(CLuaInterface::GetInstance()->GetField("x"), CLuaInterface::GetInstance()->GetField("y"), CLuaInterface::GetInstance()->GetField("z")));
	lua_getglobal(CLuaInterface::GetInstance()->theLuaState, "WP2");
	movingEnemy->WayPoints.push_back(Vector3(CLuaInterface::GetInstance()->GetField("x"), CLuaInterface::GetInstance()->GetField("y"), CLuaInterface::GetInstance()->GetField("z")));
	lua_getglobal(CLuaInterface::GetInstance()->theLuaState, "WP3");
	movingEnemy->WayPoints.push_back(Vector3(CLuaInterface::GetInstance()->GetField("x"), CLuaInterface::GetInstance()->GetField("y"), CLuaInterface::GetInstance()->GetField("z")));
	lua_getglobal(CLuaInterface::GetInstance()->theLuaState, "WP4");
	movingEnemy->WayPoints.push_back(Vector3(CLuaInterface::GetInstance()->GetField("x"), CLuaInterface::GetInstance()->GetField("y"), CLuaInterface::GetInstance()->GetField("z")));
	MeshBuilder::GetInstance()->GenerateQuad("VOLUME", Color(1, 1, 1), 1.f);
	settingsScale.SetZero();
	lua_getglobal(CLuaInterface::GetInstance()->theLuaState, "pictures");
	MeshBuilder::GetInstance()->GetMesh("VOLUME")->textureID = LoadTGA(CLuaInterface::GetInstance()->GetStringField("game"));
	volume = Create::Sprite2DObject("VOLUME", Vector3(0, 700, 0.0f), Vector3(500, 250, 0.0f));


	EntityManager::GetInstance()->AddEntity(movingEnemy, true);
	lua_getglobal(CLuaInterface::GetInstance()->theLuaState, "enemywave");
	nowave = CLuaInterface::GetInstance()->GetField("no");
	waveno1 = CLuaInterface::GetInstance()->GetField("a");
	waveno2 = CLuaInterface::GetInstance()->GetField("b");
	waveno3 = CLuaInterface::GetInstance()->GetField("c");
	waveno4 = CLuaInterface::GetInstance()->GetField("d");

	for (int i = 0; i < nowave; i++)
	{
		theEnemy = Create::Enemy(Vector3(Math::RandIntMinMax(-250, 250), 0.0f, Math::RandIntMinMax(-250, 250)), Vector3(Math::RandIntMinMax(-10, 10), 0.0f, Math::RandIntMinMax(-10, 10)), 0, groundEntity);
		theEnemy->SetNumOfFollowers(0);
	}
	for (int i = 0; i < nowave; i++)
	{
		theEnemy = Create::Enemy(Vector3(Math::RandIntMinMax(-250, 250), 0.0f, Math::RandIntMinMax(-250, 240)), Vector3(Math::RandIntMinMax(-10, 10), 0.0f, Math::RandIntMinMax(-10, 10)), 0, groundEntity);
		theEnemy->SetNumOfFollowers(1);
	}
	theEnemy = Create::Enemy(Vector3(Math::RandIntMinMax(-250, 250), 0.0f, Math::RandIntMinMax(-250, 240)), Vector3(Math::RandIntMinMax(-10, 10), 0.0f, Math::RandIntMinMax(-10, 10)), 0, groundEntity);
	theEnemy->SetNumOfFollowers(2);
/*for (int i = 0; i < 10;)
	{
		theEnemy = new CEnemy();
		theEnemy->Init();
		theEnemy->SetTerrain(groundEntity);
		theEnemy->SetPosition(Vector3(Math::RandIntMinMax(-100, 100), 0.0f, Math::RandIntMinMax(0, 100)));
		theEnemy->SetTarget(Vector3(Math::RandIntMinMax(-10, 10), 0.0f, Math::RandIntMinMax(-10, 10)));
		i++;
	}*/
	// Setup the 2D entities
	float halfWindowWidth = Application::GetInstance().GetWindowWidth() / 2.0f;
	float halfWindowHeight = Application::GetInstance().GetWindowHeight() / 2.0f;
	float fontSize = 25.0f;
	float halfFontSize = fontSize / 2.0f;

	for (int i = 2; i < 5; ++i)
	{
		textObj[i] = Create::Text2DObject("text", Vector3(-halfWindowWidth, -halfWindowHeight + fontSize*i + halfFontSize, 0.0f), "", Vector3(fontSize, fontSize, fontSize), Color(0.0f,1.0f,0.0f));
	}
	for (int j = 5; j < 9; ++j)
	{
		textObj[j] = Create::Text2DObject("text", Vector3(-halfWindowWidth+600, -halfWindowHeight + fontSize*(j-5) + halfFontSize, 0.0f), "", Vector3(fontSize, fontSize, fontSize), Color(0.0f, 1.0f, 0.0f));
	}
	textObj[0] = Create::Text2DObject("text", Vector3(-halfWindowWidth + 240, Application::GetInstance().GetWindowHeight() / 2 -200, 2.0f), "", Vector3(fontSize, fontSize, fontSize), Color(1.0f, 0.0f, 0.0f));
	textObj[1] = Create::Text2DObject("text", Vector3(-halfWindowWidth + 300, Application::GetInstance().GetWindowHeight() / 2 - 250, 2.0f), "", Vector3(fontSize, fontSize, fontSize), Color(1.0f, 0.0f, 0.0f));

	textObj[8] = Create::Text2DObject("text", Vector3(-halfWindowWidth + 80, Application::GetInstance().GetWindowHeight() / 2.5, 0.0f), "", Vector3(fontSize, fontSize, fontSize), Color(1.0f, 0.0f, 0.0f));
	textObj[9] = Create::Text2DObject("text", Vector3(-halfWindowWidth + 140, Application::GetInstance().GetWindowHeight() / 3.5, 0.0f), "", Vector3(fontSize, fontSize, fontSize), Color(1.0f, 0.0f, 0.0f));
	textObj[10] = Create::Text2DObject("text", Vector3(-halfWindowWidth + 140, Application::GetInstance().GetWindowHeight() / 3, 0.0f), "", Vector3(fontSize, fontSize, fontSize), Color(1.0f, 0.0f, 0.0f));
	textObj[11] = Create::Text2DObject("text", Vector3(-halfWindowWidth + 140, 400, 0.1f), "", Vector3(fontSize, fontSize, fontSize), Color(1.0f, 0.0f, 0.0f));

	lua_getglobal(CLuaInterface::GetInstance()->theLuaState, "music");
	Sound::GetInstance()->playMusic(CLuaInterface::GetInstance()->GetStringField("BGM"));
	sound = Sound::GetInstance()->getOnOff();

	lua_getglobal(CLuaInterface::GetInstance()->theLuaState, "pictures");
	MeshBuilder::GetInstance()->GenerateQuad("WIN", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("WIN")->textureID = LoadTGA(CLuaInterface::GetInstance()->GetStringField("win"));
	MeshBuilder::GetInstance()->GenerateQuad("LOSE", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("LOSE")->textureID = LoadTGA(CLuaInterface::GetInstance()->GetStringField("lose"));
	win = Create::Sprite2DObject("WIN", Vector3(0, 700, 0.f), Vector3(800.f, 600.0f, 0.0f));
	lose = Create::Sprite2DObject("LOSE", Vector3(0, 700, 0.f), Vector3(800.f, 600.0f, 0.0f));
}

void SceneText1::Update(double dt)
{
	// Update our entities
	if (KeyboardController::GetInstance()->IsKeyReleased('I'))
	{
		if (!ShowSettings)
		{
			ShowSettings = true;
		}
		else if (ShowSettings)
			ShowSettings = false;
	}
	std::ostringstream ss;
	std::ostringstream ss2;

	if (ShowSettings)
	{
		ss.precision(5);
		ss << "Game paused";
		ss2 << "Vol: " << std::setprecision(2) << Sound::GetInstance()->getOnOff();
		volume->SetPosition(Vector3(0, 50, 0));
		if (KeyboardController::GetInstance()->IsKeyDown(VK_LEFT))
		{
			sound -= dt;
			Sound::GetInstance()->onOff(sound);
		}
		if (KeyboardController::GetInstance()->IsKeyDown(VK_RIGHT))
		{
			sound += dt;
			Sound::GetInstance()->onOff(sound);
		}
		if (sound > 1)
			sound = 1;
		if (sound < 0)
			sound = 0;
	}
	else if (!ShowSettings)
	{
		ss << " ";		ss2 << " ";

		volume->SetPosition(Vector3(0, 700, 0));
	}
	textObj[0]->SetText(ss.str());
	textObj[1]->SetText(ss2.str());

	if (!ShowSettings && waveNo != "WIN" && waveNo != "LOSE")
	{
		EntityManager::GetInstance()->Update(dt);
		movingEnemy->Update(dt, playerInfo->GetPos());
		elasped = (clock() - t0) / CLOCKS_PER_SEC;
		// THIS WHOLE CHUNK TILL <THERE> CAN REMOVE INTO ENTITIES LOGIC! Or maybe into a scene function to keep the update clean
		/*if (KeyboardController::GetInstance()->IsKeyDown('1'))
			glEnable(GL_CULL_FACE);
			if (KeyboardController::GetInstance()->IsKeyDown('2'))
			glDisable(GL_CULL_FACE);
			if (KeyboardController::GetInstance()->IsKeyDown('3'))
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			if (KeyboardController::GetInstance()->IsKeyDown('4'))
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);*/

		/*if (KeyboardController::GetInstance()->IsKeyDown('5'))
		{
		lights[0]->type = Light::LIGHT_POINT;
		}
		else if (KeyboardController::GetInstance()->IsKeyDown('6'))
		{
		lights[0]->type = Light::LIGHT_DIRECTIONAL;
		}
		else if (KeyboardController::GetInstance()->IsKeyDown('7'))
		{
		lights[0]->type = Light::LIGHT_SPOT;
		}*/
		if (elasped == wave1 && waveNo == "1")
		{
			for (int i = 0; i <= waveno1; i++)
			{
				theEnemy = Create::Enemy(Vector3(Math::RandIntMinMax(-250, -240), 0.0f, Math::RandIntMinMax(-250, -240)), Vector3(Math::RandIntMinMax(-10, 10), 0.0f, Math::RandIntMinMax(-10, 10)), Math::RandFloatMinMax(5.f, 7.f), groundEntity);
				theEnemy->SetNumOfFollowers(0);
			}
			waveNo = "1 ";
		}

		if (elasped == wave2 && waveNo == "1 ")
		{
			waveNo = "2";
		}
		if (elasped == wave3 && waveNo == "2 ")
		{
			waveNo = "3";
			currWaveEnemy = 0;
		}
		if (elasped == wave4 && waveNo == "3 ")
		{
			waveNo = "4";
			currWaveEnemy = 0;
		}

		if (waveNo == "2")
		{
			for (currWaveEnemy; currWaveEnemy <= waveno2;)
			{
				theEnemy = Create::Enemy(Vector3(Math::RandIntMinMax(250, 200), 0.0f, Math::RandIntMinMax(-200, -200)), Vector3(Math::RandIntMinMax(-25, 2), 0.0f, Math::RandIntMinMax(10, 0)), Math::RandFloatMinMax(4.f, 10.f), groundEntity);
				theEnemy->SetNumOfFollowers(1);
				currWaveEnemy++;
			}
			waveNo = "2 ";
		}
		if (waveNo == "3")
		{
			for (currWaveEnemy; currWaveEnemy <= waveno3;)
			{
				theEnemy = Create::Enemy(Vector3(Math::RandIntMinMax(-32, -35), 0.0f, Math::RandIntMinMax(-455, -400)), Vector3(Math::RandIntMinMax(-25, 2), 0.0f, Math::RandIntMinMax(10, 0)), Math::RandFloatMinMax(5.f, 15.f), groundEntity);
				currWaveEnemy++;
			}
			waveNo = "3 ";
		}
		if (waveNo == "4")
		{
			for (currWaveEnemy; currWaveEnemy <= waveno4;)
			{
				theEnemy = Create::Enemy(Vector3(Math::RandIntMinMax(-32, -35), 0.0f, Math::RandIntMinMax(-455, -400)), Vector3(Math::RandIntMinMax(-25, 2), 0.0f, Math::RandIntMinMax(10, 0)), Math::RandFloatMinMax(2.f, 8.f), groundEntity);
				theEnemy = Create::Enemy(Vector3(Math::RandIntMinMax(-250, -240), 0.0f, Math::RandIntMinMax(-250, -240)), Vector3(Math::RandIntMinMax(-10, 0), 0.0f, Math::RandIntMinMax(10, 0)), Math::RandFloatMinMax(5.f, 7.f), groundEntity);
				theEnemy->SetNumOfFollowers(1);
				theEnemy = Create::Enemy(Vector3(Math::RandIntMinMax(250, 200), 0.0f, Math::RandIntMinMax(-200, -200)), Vector3(Math::RandIntMinMax(-20, 2), 0.0f, Math::RandIntMinMax(10, 0)), Math::RandFloatMinMax(4.f, 10.f), groundEntity);
				theEnemy->SetNumOfFollowers(2);
				currWaveEnemy++;
			}
			waveNo = "4 ";
		}

		if (elasped >= survive)
		{
			waveNo = "WIN";
			win->SetPosition(Vector3(0, 0, 1));
		}
		if (playerInfo->playerHealth <= 0)
		{
			lose->SetPosition(Vector3(0, 0, 1));
			waveNo = "LOSE";
		}

		/*if (KeyboardController::GetInstance()->IsKeyDown('I'))
			lights[0]->position.z -= (float)(10.f * dt);
			if (KeyboardController::GetInstance()->IsKeyDown('K'))
			lights[0]->position.z += (float)(10.f * dt);
			if (KeyboardController::GetInstance()->IsKeyDown('J'))
			lights[0]->position.x -= (float)(10.f * dt);
			if (KeyboardController::GetInstance()->IsKeyDown('L'))
			lights[0]->position.x += (float)(10.f * dt);
			if (KeyboardController::GetInstance()->IsKeyDown('O'))
			lights[0]->position.y -= (float)(10.f * dt);
			if (KeyboardController::GetInstance()->IsKeyDown('P'))
			lights[0]->position.y += (float)(10.f * dt);*/

		if (KeyboardController::GetInstance()->IsKeyReleased('M'))
		{
			CSceneNode* theNode = CSceneGraph::GetInstance()->GetNode(1);
			if (theNode != NULL)
			{
				Vector3 pos = theNode->GetEntity()->GetPosition();
				theNode->GetEntity()->SetPosition(Vector3(pos.x + 40.0f, pos.y, pos.z + 40.0f));
			}
		}
		if (KeyboardController::GetInstance()->IsKeyReleased('N'))
		{
			CSpatialPartition::GetInstance()->PrintSelf();
		}

		// if the left mouse button was released
		if (MouseController::GetInstance()->IsButtonReleased(MouseController::LMB))
		{
			cout << "Left Mouse Button was released!" << endl;
		}
		if (MouseController::GetInstance()->IsButtonReleased(MouseController::RMB))
		{
			cout << "Right Mouse Button was released!" << endl;
		}
		if (MouseController::GetInstance()->IsButtonReleased(MouseController::MMB))
		{
			cout << "Middle Mouse Button was released!" << endl;
		}
		if (MouseController::GetInstance()->GetMouseScrollStatus(MouseController::SCROLL_TYPE_XOFFSET) != 0.0)
		{
			cout << "Mouse Wheel has offset in X-axis of " << MouseController::GetInstance()->GetMouseScrollStatus(MouseController::SCROLL_TYPE_XOFFSET) << endl;
		}
		if (MouseController::GetInstance()->GetMouseScrollStatus(MouseController::SCROLL_TYPE_YOFFSET) != 0.0)
		{
			cout << "Mouse Wheel has offset in Y-axis of " << MouseController::GetInstance()->GetMouseScrollStatus(MouseController::SCROLL_TYPE_YOFFSET) << endl;
		}
		// <THERE>

		// Update the player position and other details based on keyboard and mouse inputs
		playerInfo->Update(dt);
	}
	//camera.Update(dt); // Can put the camera into an entity rather than here (Then we don't have to write this)

	GraphicsManager::GetInstance()->UpdateLights(dt);
	// Update the 2 text object values. NOTE: Can do this in their own class but i'm lazy to do it now :P
	// Eg. FPSRenderEntity or inside RenderUI for LightEntity
	
	/*ss1.str("");
	ss1 << "Enemies: " << theEnemy->GetEnemyCount();
	textObj[5]->SetText(ss1.str());*/

}

void SceneText1::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	GraphicsManager::GetInstance()->UpdateLightUniforms();
	//theEnemy->Render();
	// Setup 3D pipeline then render 3D
	GraphicsManager::GetInstance()->SetPerspectiveProjection(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f);
	GraphicsManager::GetInstance()->AttachCamera(&camera);
	EntityManager::GetInstance()->Render();

	if (playerInfo->GetWeaponType() == CPlayerInfo::WT_PISTOL)
	{
		playerInfo->Render("m24r");
	}
	else if (playerInfo->GetWeaponType() == CPlayerInfo::WT_GUN)
	{
		playerInfo->Render("mp5k");
	}
	
	std::ostringstream ss;
	ss.str("");
	ss << "Health: " << playerInfo->playerHealth;
	textObj[2]->SetText(ss.str());

	ss.str("");
	ss << "Score: " << playerInfo->playerScore;
	textObj[3]->SetText(ss.str());

	std::ostringstream ss1;
	ss1.precision(10);

	ss1.str("");
	ss1 << playerInfo->GetWeaponName();
	textObj[5]->SetText(ss1.str());

	ss1.str("");
	ss1 << "Ammo: " << playerInfo->GetMagRound();
	textObj[6]->SetText(ss1.str());

	ss1.str("");
	ss1 << "TIME: " << elasped << "s";
	textObj[8]->SetText(ss1.str());

	ss1.str("");
	ss1 << "Wave: " << waveNo;
	textObj[9]->SetText(ss1.str());

	if (waveNo == "1")
	{
		ss1.str("");
		ss1 << "Wave 1 in: " << wave1 - elasped << "s";
		textObj[10]->SetText(ss1.str());
	}
	else if (waveNo == "1 ")
	{
		ss1.str("");
		ss1 << "Wave 2 in: " << wave2 - elasped << "s";
		textObj[10]->SetText(ss1.str());
	}
	else if (waveNo == "2" || waveNo == "2 ")
	{
		ss1.str("");
		ss1 << "Wave 3 in: " << wave3 - elasped << "s";
		textObj[10]->SetText(ss1.str());
	}
	else if (waveNo == "3" || waveNo == "3 ")
	{
		ss1.str("");
		ss1 << "Wave 4 in: " << wave4 - elasped << "s";
		textObj[10]->SetText(ss1.str());
	}
	else
	{
		ss1.str("");
		ss1 << "Survive for " << survive - elasped << "s";
		textObj[10]->SetText(ss1.str());
	}
	if (waveNo == "WIN")
	{
		Sound::GetInstance()->stopMusic("Music//Background.mp3");
		float playerScore = playerInfo->playerScore;
		if (!added)
		{
			float currentScore = CLuaInterface::GetInstance()->GetScore("Player1");
			if (currentScore < playerScore)
				CLuaInterface::GetInstance()->saveFloatValue("Player1", playerScore, true);

			added = true;
		}
	}

	if (waveNo == "LOSE")
	{
	}
	// Setup 2D pipeline then render 2D
	int halfWindowWidth = Application::GetInstance().GetWindowWidth() / 2;
	int halfWindowHeight = Application::GetInstance().GetWindowHeight() / 2;
	GraphicsManager::GetInstance()->SetOrthographicProjection(-halfWindowWidth, halfWindowWidth, -halfWindowHeight, halfWindowHeight, -10, 10);
	GraphicsManager::GetInstance()->DetachCamera();
	EntityManager::GetInstance()->RenderUI();	

}

void SceneText1::Exit()
{
	// Detach camera from other entities
	GraphicsManager::GetInstance()->DetachCamera();
	playerInfo->DetachCamera();
	EntityManager::GetInstance()->RemoveEntity(volume);
	EntityManager::GetInstance()->RemoveEntity(win);
	EntityManager::GetInstance()->RemoveEntity(lose);
	MeshBuilder::GetInstance()->RemoveMesh("VOLUME");
	MeshBuilder::GetInstance()->RemoveMesh("WIN");
	MeshBuilder::GetInstance()->RemoveMesh("LOSE");

	if (playerInfo->DropInstance() == false)
	{
#if _DEBUGMODE==1
		cout << "Unable to drop PlayerInfo class" << endl;
#endif
	}

	// Delete the lights
}
