#include "Scene.hpp"

#include "Imgui/imgui.h"
#include "Imgui/imgui_impl_glfw.h"
#include "Imgui/imgui_impl_opengl3.h"

#include "Log.hpp"
#include "App.hpp"

namespace Resources
{
	Scene::Scene(const std::string& p_name, const std::string& p_path1, const std::string& p_path2, const unsigned int p_id)
		: camera(nullptr)
		, gameObjects()
		, lightManager()
		, graph(gameObjects)
	{
		name = p_name;
		path1 = p_path1;
		path2 = p_path2;
		id = p_id;
	}

	Scene::~Scene()
	{
		for (unsigned int i = 0; i < gameObjects.size(); i++)
			delete gameObjects[i];
	}

	void Scene::Init(const unsigned int p_width, const unsigned int p_height)
	{
		width = p_width;
		height = p_height;
		//	m_editor.InitGUI(p_window)
	}

	void Scene::Update(const Core::Inputs& p_Inputs, const double& p_deltaTime)
	{
		if (p_Inputs.quit)
		{
			ReturnToMenu();
			return;
		}


		m_physicsManager.UpdateCollision(p_deltaTime);
		graph.Update(p_Inputs,p_deltaTime);
	}

	void Scene::Draw(const Core::Inputs& p_Inputs)
	{
		StartImGui();

		graph.Draw(*camera, lightManager);
		if (p_Inputs.editor)
		{
			m_physicsManager.DrawColliders(camera->GetViewProjection());
			m_editor.DrawEditorWindow(graph);
		}

		EndImGui();
	}

	void Scene::AddGameObject(LowRenderer::GameObject* p_gameObject, LowRenderer::GOType p_type)
	{
		switch (p_type)
		{
		case LowRenderer::GOType::Player:
			p_gameObject->SetCameraRotation(camera->GetForward(), camera->GetRight());
			camera->SetPlayer(p_gameObject);
			Core::Debug::Log::Print("Player is set\n", Core::Debug::LogLevel::Notification);
			break;
		case LowRenderer::GOType::Camera:
			camera = (LowRenderer::Camera*)p_gameObject;
			Core::Debug::Log::Print("Camera is set\n", Core::Debug::LogLevel::Notification);
			break;
		case LowRenderer::GOType::None:
			break;
		default:
			break;
		}

		gameObjects.push_back(p_gameObject);
		graph.AddNode(gameObjects.size() - 1);
		Core::Debug::Log::Print("Add gameObject " + p_gameObject->GetName() + " in scene\n", Core::Debug::LogLevel::Notification);
	}

	void Scene::AddDirectionLight(const LowRenderer::DirectionLight& p_light)
	{
		lightManager.AddDirectionLight(p_light);
	}

	void Scene::AddPointLight(const LowRenderer::PointLight& p_light)
	{
		lightManager.AddPointLight(p_light);
	}

	void Scene::AddSpotLight(const LowRenderer::SpotLight& p_light)
	{
		lightManager.AddSpotLight(p_light);
	}

	Physics::Collider* Scene::CreateCollider(Resources::Mesh* p_mesh, Resources::Shader* p_shader, Physics::ColliderTypes types, bool isStatic)
	{
		return	m_physicsManager.CreateCollider(p_mesh, p_shader, types, isStatic);
	}

	void Scene::StartImGui() const
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void Scene::EndImGui() const
	{
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void Scene::ReturnToMenu()
	{
		Core::App::ChangeScene("Menu", Resources::SceneType::ST_Menu);
	}

	void Scene::ShowQuit()
	{
		ImGui::SetCursorPos(ImVec2(width - 150, height - 30));
		ImGui::Text("ESC : return menu");
	}
}