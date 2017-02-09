#include <iostream>
using namespace std;

#include "Settings.h"
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

CSettings::CSettings()
{
	sound = Sound::GetInstance()->getOnOff();
}

CSettings::~CSettings()
{

}

void CSettings::Init()
{
	// Create and attach camera to scene
	camera.Init(Vector3(0, 0, 10), Vector3(0, 0, 0), Vector3(0, 1, 0));
	GraphicsManager::GetInstance()->AttachCamera(&camera);
	settingypos = 600;

	MeshBuilder::GetInstance()->GenerateQuad("MENUSTATE_BG", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("MENUSTATE_BG")->textureID = LoadTGA("Image//Settings.tga");
	float halfWidth = Application::GetInstance().GetWindowWidth() / 2.0f;
	float halfHeight = Application::GetInstance().GetWindowHeight() / 2.0f;
	MenuStateBG = Create::Sprite2DObject("MENUSTATE_BG", Vector3(halfWidth, halfHeight, 0.0f), Vector3(800.f, 600.0f, 0.0f));
	
	MeshBuilder::GetInstance()->GenerateQuad("SETTINGS", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("SETTINGS")->textureID = LoadTGA("Image//SettingsLogo.tga");
	Settings = Create::Sprite2DObject("SETTINGS", Vector3(270, settingypos, 0.1f), Vector3(450, 100, 0.0f));

	
	MeshBuilder::GetInstance()->GetMesh("quad")->textureID = LoadTGA("Image//calibri.tga");
	MeshBuilder::GetInstance()->GenerateText("text", 16, 16);
	MeshBuilder::GetInstance()->GetMesh("text")->textureID = LoadTGA("Image//calibri.tga");
	MeshBuilder::GetInstance()->GetMesh("text")->material.kAmbient.Set(1, 0, 0);
	float windowWidth = Application::GetInstance().GetWindowWidth();
	float windowHeight = Application::GetInstance().GetWindowHeight();
	textObj[0] = Create::Text2DObject("text", Vector3(windowWidth*0.3, windowHeight *0.5, 2.0f), "", Vector3(30, 30, 1), Color(1.0f, 0.0f, 0.0f));
	textObj[1] = Create::Text2DObject("text", Vector3(windowWidth*0.13, textObj[0]->GetPosition().y - 30, 2.0f), "", Vector3(30, 30, 1), Color(1.0f, 0.0f, 0.0f));
	showLoading = false;
	cout << "Menu loaded\n" << endl;
}
void CSettings::Update(double dt)
{
	if (settingypos > 480)
		settingypos -= 100 * dt;
	Settings->SetPosition(Vector3(Settings->GetPosition().x, settingypos, 0.1));

	if (KeyboardController::GetInstance()->IsKeyReleased(VK_SPACE))
	{
		cout << "Loading CMenuState" << endl;
		SceneManager::GetInstance()->SetActiveScene("MenuState");
	}
	if (KeyboardController::GetInstance()->IsKeyDown(VK_RIGHT) && sound < 1.f)
	{
		sound += dt;
		Sound::GetInstance()->onOff(sound);
	}
	if (KeyboardController::GetInstance()->IsKeyDown(VK_LEFT) && sound > 0.f)
	{
		sound -= dt;
		Sound::GetInstance()->onOff(sound);
	}
	if (sound >= 1.f)
		sound = 1.f;

	if (sound <= 0.f)
		sound = 0.f;
}
void CSettings::Render()
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
	ss << "VOLUME: " << std::setprecision(2)  << Sound::GetInstance()->getOnOff();
	textObj[0]->SetText(ss.str());
	GraphicsManager::GetInstance()->SetOrthographicProjection(0, Application::GetInstance().GetWindowWidth(), 0, Application::GetInstance().GetWindowHeight(), -10, 10);
	GraphicsManager::GetInstance()->DetachCamera();

	EntityManager::GetInstance()->RenderUI();
}
void CSettings::Exit()
{
	EntityManager::GetInstance()->RemoveEntity(MenuStateBG);
	MeshBuilder::GetInstance()->RemoveMesh("MENUSTATE_BG");
	EntityManager::GetInstance()->RemoveEntity(Settings);
	MeshBuilder::GetInstance()->RemoveMesh("SETTINGS");
	GraphicsManager::GetInstance()->DetachCamera();
	EntityManager::GetInstance()->RemoveEntity(textObj[0]);
	EntityManager::GetInstance()->RemoveEntity(textObj[1]);
}