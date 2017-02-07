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
	GraphicsManager::GetInstance()->AttachCamera(&camera);

	MeshBuilder::GetInstance()->GenerateQuad("MENUSTATE_BG", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("MENUSTATE_BG")->textureID = LoadTGA("Image//Menu.tga");
	float halfWidth = Application::GetInstance().GetWindowWidth() / 2.0f;
	float halfHeight = Application::GetInstance().GetWindowHeight() / 2.0f;
	MenuStateBG = Create::Sprite2DObject("MENUSTATE_BG", Vector3(halfWidth, halfHeight, 0.0f), Vector3(800.f, 600.0f, 0.0f));

	cout << "Menu loaded\n" << endl;
}
void CMenuState::Update(double dt)
{
	if (KeyboardController::GetInstance()->IsKeyReleased(VK_SPACE))
	{
		cout << "Loading CMenuState" << endl;
		SceneManager::GetInstance()->SetActiveScene("GameState1");
	}
}
void CMenuState::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//GraphicsManager::GetInstance()->UpdateLightUniforms();

	GraphicsManager::GetInstance()->SetPerspectiveProjection(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f);
	GraphicsManager::GetInstance()->AttachCamera(&camera);

	EntityManager::GetInstance()->Render();

	GraphicsManager::GetInstance()->SetOrthographicProjection(0, Application::GetInstance().GetWindowWidth(), 0, Application::GetInstance().GetWindowHeight(), -10, 10);
	GraphicsManager::GetInstance()->DetachCamera();

	EntityManager::GetInstance()->RenderUI();
}
void CMenuState::Exit()
{
	EntityManager::GetInstance()->RemoveEntity(MenuStateBG);
	MeshBuilder::GetInstance()->RemoveMesh("MENUSTATE_BG");
	GraphicsManager::GetInstance()->DetachCamera();
}