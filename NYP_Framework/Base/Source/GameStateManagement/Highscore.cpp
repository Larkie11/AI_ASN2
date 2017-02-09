#include <iostream>
using namespace std;
#include <iomanip>
#include "HighScore.h"
#include "GL\glew.h"
#include "../Application.h"
#include "LoadTGA.h"
#include "GraphicsManager.h"
#include "MeshBuilder.h"
#include "../TextEntity.h"
#include "RenderHelper.h"
#include "../SpriteEntity.h"
#include "../EntityManager.h"
#include "../Lua/LuaInterface.h"
#include "KeyboardController.h"
#include "SceneManager.h"
#include <sstream>

CHighScore::CHighScore()
{

}

CHighScore::~CHighScore()
{

}

void CHighScore::Init()
{
	//Create and attach camera to scene
	camera.Init(Vector3(0, 0, 10), Vector3(0, 0, 0), Vector3(0, 1, 0));
	GraphicsManager::GetInstance()->AttachCamera(&camera);

	MeshBuilder::GetInstance()->GenerateQuad("INTROSTATE_BG", Color(1, 1, 1), 1.f);
	score = CLuaInterface::GetInstance()->GetScore("Player1");
	lua_getglobal(CLuaInterface::GetInstance()->theLuaState, "pictures");
	MeshBuilder::GetInstance()->GetMesh("INTROSTATE_BG")->textureID = LoadTGA(CLuaInterface::GetInstance()->GetStringField("instructpic"));
	MeshBuilder::GetInstance()->GenerateQuad("quad", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("quad")->textureID = LoadTGA(CLuaInterface::GetInstance()->GetStringField("text"));
	MeshBuilder::GetInstance()->GenerateText("text", 16, 16);
	MeshBuilder::GetInstance()->GetMesh("text")->textureID = LoadTGA(CLuaInterface::GetInstance()->GetStringField("text"));
	MeshBuilder::GetInstance()->GetMesh("text")->material.kAmbient.Set(1, 0, 0);
	float windowWidth = Application::GetInstance().GetWindowWidth();
	float windowHeight = Application::GetInstance().GetWindowHeight();
	IntroStateBG = Create::Sprite2DObject("INTROSTATE_BG", Vector3(windowWidth * 0.5, windowHeight*0.5, 1.f), Vector3(800.f, 600.0f, 1.0f));
	cout << "Intro loaded\n" << endl;
	textObj[0] = Create::Text2DObject("text", Vector3(windowWidth*0.13, windowHeight *0.8, 2.0f), "", Vector3(30, 30, 1), Color(1.0f, 0.0f, 0.0f));
	textObj[1] = Create::Text2DObject("text", Vector3(windowWidth*0.25, textObj[0]->GetPosition().y - 30, 2.0f), "", Vector3(30, 30, 1), Color(1.0f, 0.0f, 0.0f));

	toNextScene = 5.f;
}
void CHighScore::Update(double dt)
{
	toNextScene -= dt;
	if (toNextScene <= 0.f)
		toNextScene = 0;

	if (KeyboardController::GetInstance()->IsKeyReleased(VK_SPACE))
	{
		cout << "Loading menu" << endl;
		SceneManager::GetInstance()->SetActiveScene("MenuState");
	}
}
void CHighScore::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//GraphicsManager::GetInstance()->UpdateLightUniforms();

	GraphicsManager::GetInstance()->SetPerspectiveProjection(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f);
	GraphicsManager::GetInstance()->AttachCamera(&camera);
	EntityManager::GetInstance()->Render();

	std::ostringstream ss;
	ss.str("");
	ss << "Highscore Now: " << score;
	textObj[0]->SetText(ss.str());


	GraphicsManager::GetInstance()->SetOrthographicProjection(0, Application::GetInstance().GetWindowWidth(), 0, Application::GetInstance().GetWindowHeight(), -10, 10);
	
	
	GraphicsManager::GetInstance()->DetachCamera();
	EntityManager::GetInstance()->RenderUI();
	
}
void CHighScore::Exit()
{
	EntityManager::GetInstance()->RemoveEntity(IntroStateBG);
	EntityManager::GetInstance()->RemoveEntity(textObj[0]);
	EntityManager::GetInstance()->RemoveEntity(textObj[1]);

	MeshBuilder::GetInstance()->RemoveMesh("INTROSTATE_BG");
	GraphicsManager::GetInstance()->DetachCamera();
	
	/*for (int i = 0; i < 1; i++)
	{
		if (textObj[i] != NULL)
		{
			
		}
	}*/
}