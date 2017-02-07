#include <iostream>
using namespace std;

#include "IntroState.h"
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

CIntroState::CIntroState()
{

}

CIntroState::~CIntroState()
{

}

void CIntroState::Init()
{
	//Create and attach camera to scene
	camera.Init(Vector3(0, 0, 10), Vector3(0, 0, 0), Vector3(0, 1, 0));
	GraphicsManager::GetInstance()->AttachCamera(&camera);

	MeshBuilder::GetInstance()->GenerateQuad("INTROSTATE_BG", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("INTROSTATE_BG")->textureID = LoadTGA("Image//Splash.tga");
	MeshBuilder::GetInstance()->GenerateQuad("quad", Color(1, 1, 1), 1.f);

	MeshBuilder::GetInstance()->GetMesh("quad")->textureID = LoadTGA("Image//calibri.tga");
	MeshBuilder::GetInstance()->GenerateText("text", 16, 16);
	MeshBuilder::GetInstance()->GetMesh("text")->textureID = LoadTGA("Image//calibri.tga");
	MeshBuilder::GetInstance()->GetMesh("text")->material.kAmbient.Set(1, 0, 0);
	float halfWidth = Application::GetInstance().GetWindowWidth() / 2.0f;
	float halfHeight = Application::GetInstance().GetWindowHeight() / 2.0f;
	IntroStateBG = Create::Sprite2DObject("INTROSTATE_BG", Vector3(halfWidth, halfHeight, 1.f), Vector3(800.f, 600.0f, 1.0f));
	float halfWindowWidth = Application::GetInstance().GetWindowWidth() / 2.0f;
	float halfWindowHeight = Application::GetInstance().GetWindowHeight() / 2.0f;
	cout << "Intro loaded\n" << endl;
	textObj[0] = Create::Text2DObject("text", Vector3(500, 500, 2.0f), "", Vector3(50, 50, 50), Color(1.0f, 0.0f, 0.0f));


}
void CIntroState::Update(double dt)
{
	if (KeyboardController::GetInstance()->IsKeyReleased(VK_SPACE))
	{
		cout << "Loading menu" << endl;
		SceneManager::GetInstance()->SetActiveScene("MenuState");
	}
}
void CIntroState::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//GraphicsManager::GetInstance()->UpdateLightUniforms();

	GraphicsManager::GetInstance()->SetPerspectiveProjection(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f);
	GraphicsManager::GetInstance()->AttachCamera(&camera);
	EntityManager::GetInstance()->Render();

	std::ostringstream ss;
	ss.str("");
	ss << "Health: ";
	textObj[0]->SetText(ss.str());


	GraphicsManager::GetInstance()->SetOrthographicProjection(0, Application::GetInstance().GetWindowWidth(), 0, Application::GetInstance().GetWindowHeight(), -10, 10);
	
	
	GraphicsManager::GetInstance()->DetachCamera();
	EntityManager::GetInstance()->RenderUI();
	
}
void CIntroState::Exit()
{
	EntityManager::GetInstance()->RemoveEntity(IntroStateBG);
	MeshBuilder::GetInstance()->RemoveMesh("INTROSTATE_BG");
	GraphicsManager::GetInstance()->DetachCamera();
}