#ifndef APPLICATION_H
#define APPLICATION_H

#include "timer.h"
#include "SceneText.h"
#include "GameStateManagement\IntroState.h"
#include "GameStateManagement\MenuState.h"
#include "GameStateManagement\Settings.h"
#include "GameStateManagement\Instructions.h"
#include "GameStateManagement\Highscore.h"

#include "SceneText1.h"

struct GLFWwindow;

class Application
{
public:
	static Application& GetInstance()
	{
		static Application app;
		return app;
	}
	void Init();
	void Run();
	void Exit();

	void UpdateInput();
	void PostInputUpdate();
	
	static void MouseButtonCallbacks(GLFWwindow* window, int button, int action, int mods);
	static void MouseScrollCallbacks(GLFWwindow* window, double xoffset, double yoffset);

	int GetWindowHeight();
	int GetWindowWidth();
	
private:
	Application();
	~Application();

	static bool IsKeyPressed(unsigned short key);

	// Should make these not hard-coded :P
	int m_window_width;
	int m_window_height;
	void InitDisplay(void);
	//Declare a window object
	StopWatch m_timer;
	ShaderProgram* currProg;
};

#endif