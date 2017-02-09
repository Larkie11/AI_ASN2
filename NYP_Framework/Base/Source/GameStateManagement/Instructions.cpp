#include <iostream>
using namespace std;

#include "Instructions.h"
#include <iomanip>

#include "GL\glew.h"
#include "../Application.h"
#include "LoadTGA.h"
#include "GraphicsManager.h"
#include "MeshBuilder.h"
#include "../TextEntity.h"
#include "RenderHelper.h"
#include "../SpriteEntity.h"
#include "../EntityManager.h"

#include "KeyboardController.h"
#include "SceneManager.h"
#include <sstream>
#include "../Sound.h"
#include "MyMath.h"
#include "../Lua/LuaInterface.h"

CInstructions::CInstructions()
{
	sound = 0.f;
}

CInstructions::~CInstructions()
{

}

void CInstructions::Init()
{
	// Create and attach camera to scene
	camera.Init(Vector3(0, 0, 10), Vector3(0, 0, 0), Vector3(0, 1, 0));
	GraphicsManager::GetInstance()->AttachCamera(&camera);
	logoypos = 600;
	left = CLuaInterface::GetInstance()->getCharValue("LEFT");
	right = CLuaInterface::GetInstance()->getCharValue("RIGHT");
	up = CLuaInterface::GetInstance()->getCharValue("DOWN");
	down = CLuaInterface::GetInstance()->getCharValue("UP");
	MeshBuilder::GetInstance()->GenerateQuad("MENUSTATE_BG", Color(1, 1, 1), 1.f);

	lua_getglobal(CLuaInterface::GetInstance()->theLuaState, "pictures");
	MeshBuilder::GetInstance()->GetMesh("MENUSTATE_BG")->textureID = LoadTGA(CLuaInterface::GetInstance()->GetStringField("instructpic"));
	float halfWidth = Application::GetInstance().GetWindowWidth() / 2.0f;
	float halfHeight = Application::GetInstance().GetWindowHeight() / 2.0f;
	MenuStateBG = Create::Sprite2DObject("MENUSTATE_BG", Vector3(halfWidth, halfHeight, 0.0f), Vector3(800.f, 600.0f, 0.0f));
	MeshBuilder::GetInstance()->GetMesh("quad")->textureID = LoadTGA(CLuaInterface::GetInstance()->GetStringField("text"));
	MeshBuilder::GetInstance()->GenerateText("text", 16, 16);
	MeshBuilder::GetInstance()->GetMesh("text")->textureID = LoadTGA(CLuaInterface::GetInstance()->GetStringField("text"));
	MeshBuilder::GetInstance()->GetMesh("text")->material.kAmbient.Set(1, 0, 0);
	MeshBuilder::GetInstance()->GenerateQuad("LOGO", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("LOGO")->textureID = LoadTGA(CLuaInterface::GetInstance()->GetStringField("instructlogo"));
	Logo = Create::Sprite2DObject("LOGO", Vector3(320, logoypos, 0.1f), Vector3(500, 100.0f, 0.0f));
	float windowWidth = Application::GetInstance().GetWindowWidth();
	float windowHeight = Application::GetInstance().GetWindowHeight();
	textObj[0] = Create::Text2DObject("text", Vector3(windowWidth*0.1, windowHeight *0.5, 2.0f), "", Vector3(30, 30, 1), Color(1.0f, 0.0f, 0.0f));

	for (int i = 1; i < 6; i++)
	{
		textObj[i] = Create::Text2DObject("text", Vector3(textObj[i-1]->GetPosition().x, textObj[i-1]->GetPosition().y - 30, 2.0f), "", Vector3(30, 30, 1), Color(1.0f, 0.0f, 0.0f));
	}
	showLoading = false;
	lua_getglobal(CLuaInterface::GetInstance()->theLuaState, "texts");

	cout << "Menu loaded\n" << endl;
}
void CInstructions::Update(double dt)
{
	if (logoypos > 480)
		logoypos -= 100 * dt;
	Logo->SetPosition(Vector3(Logo->GetPosition().x, logoypos, 0.1));

	if (KeyboardController::GetInstance()->IsKeyReleased(VK_SPACE))
	{
		cout << "Loading CMenuState" << endl;
		SceneManager::GetInstance()->SetActiveScene("MenuState");
	}
}
void CInstructions::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//GraphicsManager::GetInstance()->UpdateLightUniforms();

	GraphicsManager::GetInstance()->SetPerspectiveProjection(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f);
	GraphicsManager::GetInstance()->AttachCamera(&camera);

	EntityManager::GetInstance()->Render();
	std::ostringstream ss;
	if (showLoading)
	{
		
	}
	ss.str("");
	ss << CLuaInterface::GetInstance()->GetStringField("instruction1");
	textObj[0]->SetText(ss.str());
	ss.str("");
	ss << CLuaInterface::GetInstance()->GetStringField("instruction2")<< left;
	textObj[1]->SetText(ss.str());
	ss.str("");
	ss << CLuaInterface::GetInstance()->GetStringField("instruction3") << right;
	textObj[2]->SetText(ss.str());
	ss.str("");
	ss << CLuaInterface::GetInstance()->GetStringField("instruction4") << up;
	textObj[3]->SetText(ss.str());
	ss.str("");
	ss << CLuaInterface::GetInstance()->GetStringField("instruction5") << down;
	textObj[4]->SetText(ss.str());
	GraphicsManager::GetInstance()->SetOrthographicProjection(0, Application::GetInstance().GetWindowWidth(), 0, Application::GetInstance().GetWindowHeight(), -10, 10);
	GraphicsManager::GetInstance()->DetachCamera();

	EntityManager::GetInstance()->RenderUI();
}
void CInstructions::Exit()
{
	EntityManager::GetInstance()->RemoveEntity(MenuStateBG);
	MeshBuilder::GetInstance()->RemoveMesh("MENUSTATE_BG");
	EntityManager::GetInstance()->RemoveEntity(Logo);
	MeshBuilder::GetInstance()->RemoveMesh("LOGO");
	GraphicsManager::GetInstance()->DetachCamera();
	for (int i = 0; i < 6; i++)
	{
		EntityManager::GetInstance()->RemoveEntity(textObj[i]);
	}
}