#pragma once

#include <GLFW/glfw3.h>
#include <glad/glad.h>

// Resources
#include "ResourcesManager.hpp"
#include "Scene.hpp"
#include "Menu.hpp"
// LowRenderer
#include "Camera.hpp"
#include "GameObject.hpp"
#include "LightManager.hpp"
#include "Renderer.hpp"
// Core
#include "InputsManager.hpp"
#include "Timer.hpp"

namespace Core
{
	struct AppInit
	{
		unsigned int width;
		unsigned int height;
		unsigned int major;
		unsigned int minor;
		const char* name;
		void (*framebuffer_size_callback) (GLFWwindow* window, int width, int height);
		void (*glDebugOutput) (GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei lenght, const GLchar* message, const void* userParam);
	};

	class App
	{
		// Attribute
	private:

		static GLFWwindow* window;
		unsigned int width;
		unsigned int height;

		Time::Timer timer;
		static Resources::ResourceManager resources;
		InputsManager inputsManager;
		static Resources::Scene* currentScene;
		LowRenderer::Renderer m_renderer;

		static bool stopGame;

		// Methode
	public:
		App();
		~App();

		bool Init(const AppInit& p_appInit);
		void Update();

		static void ChangeScene(const std::string& p_name, Resources::SceneType p_type);

		// Get and Set
		GLFWwindow* GetWindow() { return window; };
		static void StopGame() { stopGame = true; };

	private:
		void InitRenderer();
		void InitResource();
		static void InitScene1();
	};
}