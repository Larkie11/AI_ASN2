#include <iostream>
using namespace std;

#include "MenuState.h"
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
#include "../Lua/LuaInterface.h"


CMenuState::CMenuState()
{
	
}

CMenuState::~CMenuState()
{

}

void CMenuState::Init()
{
	// Create and attach camera to scene
	camera.Init(Vector3(0, 0, 10), Vector3(0, 0, 0), Vector3(0, 1, 0));
	myState = 0;
	currState = static_cast<States>(myState);
	GraphicsManager::GetInstance()->AttachCamera(&camera);
	logoypos = 600;
	arrowypos = 300;
	float halfWidth = Application::GetInstance().GetWindowWidth() / 2.0f;
	float halfHeight = Application::GetInstance().GetWindowHeight() / 2.0f;
	MeshBuilder::GetInstance()->GenerateQuad("MENUSTATE_BG", Color(1, 1, 1), 1.f);
	lua_getglobal(CLuaInterface::GetInstance()->theLuaState, "pictures");

	MeshBuilder::GetInstance()->GetMesh("MENUSTATE_BG")->textureID = LoadTGA(CLuaInterface::GetInstance()->GetStringField("menupic"));
	MenuStateBG = Create::Sprite2DObject("MENUSTATE_BG", Vector3(halfWidth, halfHeight, 0.0f), Vector3(800.f, 600.0f, 0.0f));

	MeshBuilder::GetInstance()->GenerateQuad("LOGO", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("LOGO")->textureID = LoadTGA(CLuaInterface::GetInstance()->GetStringField("menulogo"));
	Logo = Create::Sprite2DObject("LOGO", Vector3(280, logoypos, 0.1f), Vector3(450, 100.0f, 0.0f));

	MeshBuilder::GetInstance()->GenerateQuad("ARROW", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("ARROW")->textureID = LoadTGA(CLuaInterface::GetInstance()->GetStringField("menuarrow"));
	Arrow = Create::Sprite2DObject("ARROW", Vector3(100, arrowypos, 0.1f), Vector3(80, 50.0f, 0.0f));
	
	MeshBuilder::GetInstance()->GetMesh("quad")->textureID = LoadTGA("Image//calibri.tga");
	MeshBuilder::GetInstance()->GenerateText("text", 16, 16);
	MeshBuilder::GetInstance()->GetMesh("text")->textureID = LoadTGA("Image//calibri.tga");
	MeshBuilder::GetInstance()->GetMesh("text")->material.kAmbient.Set(1, 0, 0);
	float windowWidth = Application::GetInstance().GetWindowWidth();
	float windowHeight = Application::GetInstance().GetWindowHeight();
	textObj[0] = Create::Text2DObject("text", Vector3(windowWidth*0.2, windowHeight *0.5, 2.0f), "", Vector3(30, 30, 1), Color(1.0f, 0.0f, 0.0f));
	for (int i = 1; i < 4; i++)
	{
		textObj[i] = Create::Text2DObject("text", Vector3(windowWidth*0.2, textObj[i - 1]->GetPosition().y - 30, 2.0f), "", Vector3(30, 30, 1), Color(1.0f, 0.0f, 0.0f));
	}
	textObj[5] = Create::Text2DObject("text", Vector3(windowWidth*0.25, windowHeight *0.1, 2.0f), "", Vector3(30, 30, 1), Color(1.0f, 0.0f, 0.0f));
	lua_getglobal(CLuaInterface::GetInstance()->theLuaState, "music");

	showLoading = false;
	cout << "Menu loaded\n" << endl;
	if (!Sound::GetInstance()->isPlayingMenu())
	{
		Sound::GetInstance()->setPlayingMenu(true);
		music = CLuaInterface::GetInstance()->GetStringField("Menu");
		Sound::GetInstance()->playMusic(music);
	}
	lua_getglobal(CLuaInterface::GetInstance()->theLuaState, "texts");
}
void CMenuState::Update(double dt)
{
	if (logoypos > 480)
	logoypos -= 100*dt;

	Logo->SetPosition(Vector3(Logo->GetPosition().x, logoypos, 0.1));
	if (KeyboardController::GetInstance()->IsKeyReleased(VK_UP))
	{
		if (myState > 0)
		{
			myState--;
			arrowypos += 30;
		}
		currState = static_cast<States>(myState);
	}
	if (KeyboardController::GetInstance()->IsKeyReleased(VK_DOWN))
	{
		if (myState < MAX_STATES-1)
		{
			myState++;
			arrowypos -= 30;
		}
		currState = static_cast<States>(myState);
	}
	Arrow->SetPosition(Vector3(Arrow->GetPosition().x, arrowypos, 0.1));

	switch (currState)
	{
	case GAMEPLAY:
		if (KeyboardController::GetInstance()->IsKeyReleased(VK_SPACE))
		{
			cout << "Loading CMenuState" << endl;
			showLoading = true;
			Sound::GetInstance()->stopMusic(music);
			Sound::GetInstance()->setPlayingMenu(false);
			SceneManager::GetInstance()->SetActiveScene("GameState1");
		}
		break;
	case INSTRUCTION:
		if (KeyboardController::GetInstance()->IsKeyReleased(VK_SPACE))
		{
			cout << "Loading CMenuState" << endl;
			showLoading = true;
			SceneManager::GetInstance()->SetActiveScene("InstructionsState");
		}
		break;
	case SETTINGS:
		if (KeyboardController::GetInstance()->IsKeyReleased(VK_SPACE))
		{
			cout << "Loading CMenuState" << endl;
			showLoading = true;
			SceneManager::GetInstance()->SetActiveScene("SettingsState");
		}
		break;
	case QUIT:		
		break;
	case MAX_STATES:
		break;
	default:
		break;
	}
	//if (KeyboardController::GetInstance()->IsKeyReleased(VK_SPACE))
	//{
	//	cout << "Loading CMenuState" << endl;
	//	showLoading = true;
	//	SceneManager::GetInstance()->SetActiveScene("GameState1");
	//}
	//if (KeyboardController::GetInstance()->IsKeyReleased('1'))
	//{
	//	SceneManager::GetInstance()->SetActiveScene("SettingsState");
	//}
}
void CMenuState::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//GraphicsManager::GetInstance()->UpdateLightUniforms();

	GraphicsManager::GetInstance()->SetPerspectiveProjection(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f);
	GraphicsManager::GetInstance()->AttachCamera(&camera);

	EntityManager::GetInstance()->Render();
	std::ostringstream ss;
	ss.str("");
	ss << CLuaInterface::GetInstance()->GetStringField("menutext1");
	textObj[0]->SetText(ss.str());
	ss.str("");
	ss << CLuaInterface::GetInstance()->GetStringField("menutext2");
	textObj[1]->SetText(ss.str());
	ss.str("");
	ss << CLuaInterface::GetInstance()->GetStringField("menutext3");
	textObj[2]->SetText(ss.str());
	ss.str("");
	ss << CLuaInterface::GetInstance()->GetStringField("menutext4");
	textObj[3]->SetText(ss.str());

	if (showLoading)
	{
		ss.str("");
		ss << CLuaInterface::GetInstance()->GetStringField("loading");
		textObj[5]->SetText(ss.str());
	}

	GraphicsManager::GetInstance()->SetOrthographicProjection(0, Application::GetInstance().GetWindowWidth(), 0, Application::GetInstance().GetWindowHeight(), -10, 10);
	GraphicsManager::GetInstance()->DetachCamera();

	EntityManager::GetInstance()->RenderUI();
}
void CMenuState::Exit()
{
	EntityManager::GetInstance()->RemoveEntity(MenuStateBG);
	EntityManager::GetInstance()->RemoveEntity(Logo);
	EntityManager::GetInstance()->RemoveEntity(Arrow);

	MeshBuilder::GetInstance()->RemoveMesh("MENUSTATE_BG");
	MeshBuilder::GetInstance()->RemoveMesh("LOGO");
	MeshBuilder::GetInstance()->RemoveMesh("ARROW");

	GraphicsManager::GetInstance()->DetachCamera();
	for (int i = 0; i < sizeof(textObj); i++)
	{
		EntityManager::GetInstance()->RemoveEntity(textObj[i]);
	}
}