#include "App.hpp"

#include <iostream>

#include "Imgui/imgui.h"
#include "Imgui/imgui_impl_glfw.h"
#include "Imgui/imgui_impl_opengl3.h"

#include "Log.hpp"

#include "Credit.hpp"
#include "Setting.hpp"

namespace Core
{
	Resources::Scene* App::currentScene = nullptr;
	Resources::ResourceManager App::resources;
	GLFWwindow* App::window = nullptr;
	bool App::stopGame = false;

	App::App()
		: width(0)
		, height(0)
		, inputsManager(window)
	{
	}

	App::~App()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();

		// glfw: terminate, clearing all previously allocated GLFW resources.
		glfwTerminate();
	}

	bool App::Init(const AppInit& p_appInit)
	{
		width = p_appInit.width;
		height = p_appInit.height;

		// glfw: initialize and configure
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, p_appInit.major);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, p_appInit.minor);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

		// glfw window creation
		window = glfwCreateWindow(p_appInit.width, p_appInit.height, p_appInit.name, NULL, NULL);
		if (window == NULL)
		{
			DEBUG_LOG("Failed to create GLFW window");
			glfwTerminate();
			return false;
		}
		glfwMakeContextCurrent(window);
		glfwSetFramebufferSizeCallback(window, p_appInit.framebuffer_size_callback);

		// glad: load all OpenGL function pointers
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			DEBUG_LOG("Failed to initialize GLAD");
			return false;
		}

		GLint flags = 0;
		glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
		if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
		{
			glEnable(GL_DEBUG_OUTPUT);
			glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
			glDebugMessageCallback(p_appInit.glDebugOutput, nullptr);
			glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
		}

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);

		// Setup Dear ImGui
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();

		ImGui::StyleColorsDark();
		ImGuiStyle& style = ImGui::GetStyle();

		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 130");

		// == Temp == 

		InitResource();

		currentScene = resources.GetResource<Resources::Scene>("Scene1");
		InitScene1();

		currentScene = resources.GetResource<Resources::Scene>("Menu");

		inputsManager.SetWindow(window);


		return true;
	}

	void App::InitRenderer()
	{
		m_renderer.CreatePrimitifsMeshs();
	}

	void App::Update()
	{
		Core::Inputs inputs;
		// while ((!glfwWindowShouldClose(window) && currentScene->GetName() == "Menu") && !stopGame)
		while (!stopGame)
		{
			// input
			glfwPollEvents();
			inputs = inputsManager.Update();


			// render
			glClearColor(0.06f, 0.01f, 0.34f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);
			glClear(GL_DEPTH_BUFFER_BIT);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

			timer.Update();
			currentScene->Update(inputs,timer.GetDeltaTime());
			currentScene->Draw(inputs);

			// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
			glfwSwapBuffers(window);
		}
	}

	void App::ChangeScene(const std::string& p_name, Resources::SceneType p_type)
	{
		Debug::Log::Print("Change scene " + p_name + "\n", Debug::LogLevel::Notification);
		
		switch (p_type)
		{
		case Resources::SceneType::ST_Game:
			currentScene = resources.GetResource<Resources::Scene>(p_name);
			break;

		case Resources::SceneType::ST_Menu:
			currentScene = resources.GetResource<Resources::Menu>(p_name);
			break;

		case Resources::SceneType::ST_Setting:
			currentScene = resources.GetResource<Resources::Setting>(p_name);
			break;

		case Resources::SceneType::ST_Credit:
			currentScene = resources.GetResource<Resources::Credit>(p_name);
			break;

		default:
			break;
		}
	}

	void App::InitResource()
	{
		Core::Debug::Log::Print("---------\n", Core::Debug::LogLevel::None);
		Core::Debug::Log::Print("Init resource\n", Core::Debug::LogLevel::Notification);

		std::string name = "Cube";
		std::string path = "Resources/Obj/cube.obj";
		resources.Create<Resources::Mesh>(name, path)->Init();

		 name = "Sphere";
		 path = "Resources/Obj/Sphere.obj";
		resources.Create<Resources::Mesh>(name, path)->Init();

		name = "BasicShader";
		std::string pathVert = "Resources/Shaders/VertexShaderSource.vert";
		std::string pathFrag = "Resources/Shaders/FragmentShaderSource.frag";
		resources.Create<Resources::Shader>(name, pathVert, pathFrag)->Init();

		name = "ColliderShader";
		pathVert = "Resources/Shaders/VertexShaderSource.vert";
		pathFrag = "Resources/Shaders/ColliderFrag.frag";
		resources.Create<Resources::Shader>(name, pathVert, pathFrag)->Init();

		name = "Wall";
		path = "Resources/Textures/wall.jpg";
		resources.Create<Resources::Texture>(name, path)->Init();

		name = "Sample";
		path = "Resources/Textures/sample.png";
		resources.Create<Resources::Texture>(name, path)->Init();

		// Slime
		name = "Slime";
		path = "Resources/Obj/Slime.obj";
		resources.Create<Resources::Mesh>(name, path)->Init();

		name = "SlimeText";
		path = "Resources/Textures/Slime.png";
		resources.Create<Resources::Texture>(name, path)->Init();

		// Frying_pan
		name = "FryingPan";
		path = "Resources/Obj/frying_pan.obj";
		resources.Create<Resources::Mesh>(name, path)->Init();

		name = "FryingPanText";
		path = "Resources/Textures/frying_pan.png";
		resources.Create<Resources::Texture>(name, path)->Init();

		// Pistol
		name = "Pistol";
		path = "Resources/Obj/c_pistol.obj";
		resources.Create<Resources::Mesh>(name, path)->Init();

		name = "PistolText";
		path = "Resources/Textures/c_pistol.png";
		resources.Create<Resources::Texture>(name, path)->Init();

		// Patrick
		name = "Patrick";
		path = "Resources/Obj/patrick.obj";
		resources.Create<Resources::Mesh>(name, path)->Init();

		name = "PatrickText";
		path = "Resources/Textures/patrick.png";
		resources.Create<Resources::Texture>(name, path)->Init();

		// Scene1
		name = "Scene1";
		resources.Create<Resources::Scene>(name, "")->Init(width, height);

		InitRenderer();
		name = "BoxCollider";
		Resources::Mesh* cube = resources.Create<Resources::Mesh>(name, "");
		*cube = m_renderer.CreateCubePrimitif();
		cube->LoadMesh();

		name = "SphereCollider";
		Resources::Mesh* sphere = resources.Create<Resources::Mesh>(name, "");
		*sphere = m_renderer.CreateSpherePrimitif();
		sphere->LoadMesh();

		name = "CapsuleCollider";
		Resources::Mesh* capsule = resources.Create<Resources::Mesh>(name, "");
		*capsule = m_renderer.CreateCapsulePrimitif();
		capsule->LoadMesh();

		// Menu
		name = "Menu";
		resources.Create<Resources::Menu>(name, "")->Init(width, height);

		// Credit
		name = "Credit";
		resources.Create<Resources::Credit>(name, "")->Init(width, height);

		// Setting
		name = "Setting";
		resources.Create<Resources::Setting>(name, "")->Init(width, height, &inputsManager);
	}

	void App::InitScene1()
	{
		Core::Debug::Log::Print("---------\n", Core::Debug::LogLevel::None);
		Core::Debug::Log::Print("Init scene 1\n", Core::Debug::LogLevel::Notification);

		LowRenderer::Model basicBox = LowRenderer::Model(
			resources.GetResource<Resources::Mesh>("Cube"),
			resources.GetResource<Resources::Shader>("BasicShader"),
			resources.GetResource<Resources::Texture>("Wall"));

		LowRenderer::Model basicSphere = LowRenderer::Model(
			resources.GetResource<Resources::Mesh>("Sphere"),
			resources.GetResource<Resources::Shader>("BasicShader"),
			resources.GetResource<Resources::Texture>("Wall"));

		LowRenderer::GameObject* box1 = new LowRenderer::GameObject(basicBox, Physics::Transform(
			Core::Maths::Vec3(0.f, -5.f, 0.f),
			Core::Maths::Vec3(10.f, 1.f, 10.f),
			Core::Maths::Vec3(0.f, 0.f, 0.f)), "Box1");

		currentScene->AddGameObject(box1);
		currentScene->CreateCollider(resources.GetResource<Resources::Mesh>("BoxCollider"), resources.GetResource<Resources::Shader>("ColliderShader"),Physics::ColliderTypes::Box, true);
		box1->SetCollider(currentScene->GetLastCollider());
		box1->GetRigidbody().useGravity = false;

		LowRenderer::GameObject* box2 = new LowRenderer::GameObject(basicBox, Physics::Transform(
			Core::Maths::Vec3(0.f, -3.f, 4.f),
			Core::Maths::Vec3(1.f, 1.f, 1.f),
			Core::Maths::Vec3(0.f, 0.f, 0.f)), "Box2");

		currentScene->CreateCollider(resources.GetResource<Resources::Mesh>("BoxCollider"), resources.GetResource<Resources::Shader>("ColliderShader"), Physics::ColliderTypes::Box, true);
		currentScene->AddGameObject(box2);
		box2->SetCollider(currentScene->GetLastCollider());
		box2->GetRigidbody().useGravity = false;

		LowRenderer::GameObject* box3 = new LowRenderer::GameObject(basicBox, Physics::Transform(
			Core::Maths::Vec3(4.f, 5.0f, 0.f),
			Core::Maths::Vec3(1.f, 1.f, 1.f),
			Core::Maths::Vec3(0.f, 0.f, 0.f)), "Box3");

		// ========= Phyics =========
		currentScene->CreateCollider(resources.GetResource<Resources::Mesh>("BoxCollider"), resources.GetResource<Resources::Shader>("ColliderShader"),Physics::ColliderTypes::Box, true);
		currentScene->AddGameObject(box3);
		box3->SetCollider(currentScene->GetLastCollider());
		// ========= ==================== =========


		LowRenderer::GameObject* box4 = new LowRenderer::GameObject(basicBox, Physics::Transform(
			Core::Maths::Vec3(0.f, -2.f, 0.f),
			Core::Maths::Vec3(1.f, 1.f, 1.f),
			Core::Maths::Vec3(0.f, 0.f, 0.f)), "Box4");

		currentScene->CreateCollider(resources.GetResource<Resources::Mesh>("BoxCollider"), resources.GetResource<Resources::Shader>("ColliderShader"), Physics::ColliderTypes::Box, true);
		currentScene->AddGameObject(box4);
		box4->SetCollider(currentScene->GetLastCollider());
		box4->GetRigidbody().useGravity = false;

		LowRenderer::GameObject* box5 = new LowRenderer::GameObject(basicBox, Physics::Transform(
			Core::Maths::Vec3(-4.f, 0.f, 0.f),
			Core::Maths::Vec3(1.f, 1.f, 1.f),
			Core::Maths::Vec3(0.f, 0.f, 0.f)), "Box5");

		currentScene->CreateCollider(resources.GetResource<Resources::Mesh>("BoxCollider"), resources.GetResource<Resources::Shader>("ColliderShader"), Physics::ColliderTypes::Box,true);
		currentScene->AddGameObject(box5);
		box5->SetCollider(currentScene->GetLastCollider());
		box5->GetRigidbody().useGravity = false;

		LowRenderer::GameObject* box6 = new LowRenderer::GameObject(basicBox, Physics::Transform(
			Core::Maths::Vec3(-8.f, 2.f, 0.f),
			Core::Maths::Vec3(2.f, .5f, 3.f),
			Core::Maths::Vec3(0.f, 0.f, 0.f)), "Box6");

		currentScene->CreateCollider(resources.GetResource<Resources::Mesh>("BoxCollider"), resources.GetResource<Resources::Shader>("ColliderShader"), Physics::ColliderTypes::Box, true);
		currentScene->AddGameObject(box6);
		box6->SetCollider(currentScene->GetLastCollider());
		box6->GetRigidbody().useGravity = false;

		LowRenderer::GameObject* box7 = new LowRenderer::GameObject(basicBox, Physics::Transform(
			Core::Maths::Vec3(-20.f, 2.f, 0.f),
			Core::Maths::Vec3(2.f, .5f, 3.f),
			Core::Maths::Vec3(0.f, 0.f, 0.f)), "Box7");

		currentScene->CreateCollider(resources.GetResource<Resources::Mesh>("BoxCollider"), resources.GetResource<Resources::Shader>("ColliderShader"), Physics::ColliderTypes::Box, true);
		currentScene->AddGameObject(box7);
		box7->SetCollider(currentScene->GetLastCollider());
		box7->GetRigidbody().useGravity = false;

		LowRenderer::GameObject* box8 = new LowRenderer::GameObject(basicBox, Physics::Transform(
			Core::Maths::Vec3(-30.f, 2.f, 0.f),
			Core::Maths::Vec3(1.5f, .5f,1.5f),
			Core::Maths::Vec3(0.f, 0.f, 0.f)), "Box8");


		currentScene->CreateCollider(resources.GetResource<Resources::Mesh>("BoxCollider"), resources.GetResource<Resources::Shader>("ColliderShader"), Physics::ColliderTypes::Box, true);
		currentScene->AddGameObject(box8);
		box8->SetCollider(currentScene->GetLastCollider());
		box8->GetRigidbody().useGravity = false;


		LowRenderer::GameObject* box9 = new LowRenderer::GameObject(basicBox, Physics::Transform(
			Core::Maths::Vec3(-35.f, -2.f, 0.f),
			Core::Maths::Vec3(1.5f, .5f, 1.5f),
			Core::Maths::Vec3(0.f, 0.f, 0.f)), "Box9");


		currentScene->CreateCollider(resources.GetResource<Resources::Mesh>("BoxCollider"), resources.GetResource<Resources::Shader>("ColliderShader"), Physics::ColliderTypes::Box, true);
		currentScene->AddGameObject(box9);
		box9->SetCollider(currentScene->GetLastCollider());
		box9->GetRigidbody().useGravity = false;

		LowRenderer::GameObject* box10 = new LowRenderer::GameObject(basicBox, Physics::Transform(
			Core::Maths::Vec3(-42.f, -2.f, 4.f),
			Core::Maths::Vec3(1.5f, .5f, 1.5f),
			Core::Maths::Vec3(0.f, 0.f, 0.f)), "Box10");


		currentScene->CreateCollider(resources.GetResource<Resources::Mesh>("BoxCollider"), resources.GetResource<Resources::Shader>("ColliderShader"), Physics::ColliderTypes::Box, true);
		currentScene->AddGameObject(box10);
		box10->SetCollider(currentScene->GetLastCollider());
		box10->GetRigidbody().useGravity = false;

		LowRenderer::GameObject* box11 = new LowRenderer::GameObject(basicBox, Physics::Transform(
			Core::Maths::Vec3(-49.f, -2.f, -4.f),
			Core::Maths::Vec3(1.5f, .5f, 1.5f),
			Core::Maths::Vec3(0.f, 0.f, 0.f)), "Box11");


		currentScene->CreateCollider(resources.GetResource<Resources::Mesh>("BoxCollider"), resources.GetResource<Resources::Shader>("ColliderShader"), Physics::ColliderTypes::Box, true);
		currentScene->AddGameObject(box11);
		box11->SetCollider(currentScene->GetLastCollider());
		box11->GetRigidbody().useGravity = false;

		LowRenderer::GameObject* box12 = new LowRenderer::GameObject(basicBox, Physics::Transform(
			Core::Maths::Vec3(-56.f, -2.f, 4.f),
			Core::Maths::Vec3(1.5f, .5f, 1.5f),
			Core::Maths::Vec3(0.f, 0.f, 0.f)), "Box12");


		currentScene->CreateCollider(resources.GetResource<Resources::Mesh>("BoxCollider"), resources.GetResource<Resources::Shader>("ColliderShader"), Physics::ColliderTypes::Box, true);
		currentScene->AddGameObject(box12);
		box12->SetCollider(currentScene->GetLastCollider());
		box12->GetRigidbody().useGravity = false;

		LowRenderer::GameObject* box13 = new LowRenderer::GameObject(basicBox, Physics::Transform(
			Core::Maths::Vec3(-63.f, -2.f, -4.f),
			Core::Maths::Vec3(1.5f, .5f, 1.5f),
			Core::Maths::Vec3(0.f, 0.f, 0.f)), "Box13");


		currentScene->CreateCollider(resources.GetResource<Resources::Mesh>("BoxCollider"), resources.GetResource<Resources::Shader>("ColliderShader"), Physics::ColliderTypes::Box, true);
		currentScene->AddGameObject(box13);
		box13->SetCollider(currentScene->GetLastCollider());
		box13->GetRigidbody().useGravity = false;

		LowRenderer::GameObject* sphere1 = new LowRenderer::GameObject(basicSphere, Physics::Transform(
			Core::Maths::Vec3(0.f, 4.0f, -6.f),
			Core::Maths::Vec3(4.0f, 4.0f, 4.0f),
			Core::Maths::Vec3(0.f, 0.f, 0.f)), "Sphere1");


		currentScene->CreateCollider(resources.GetResource<Resources::Mesh>("SphereCollider"), resources.GetResource<Resources::Shader>("ColliderShader"), Physics::ColliderTypes::Sphere, true);
		currentScene->AddGameObject(sphere1);
		sphere1->SetCollider(currentScene->GetLastCollider());
		sphere1->GetRigidbody().useGravity = false;
		Physics::Collider* playerCollider = sphere1->GetCollider();
		Physics::SphereCollider* sphereCollider = dynamic_cast<Physics::SphereCollider*>(playerCollider);
		sphereCollider->radius = 1.46f;


		LowRenderer::GameObject* box14 = new LowRenderer::GameObject(basicBox, Physics::Transform(
			Core::Maths::Vec3(-73.f, -2.f, -4.f),
			Core::Maths::Vec3(2.5f, .5f, 2.5f),
			Core::Maths::Vec3(0.f, 0.f, 0.f)), "Box14");


		currentScene->CreateCollider(resources.GetResource<Resources::Mesh>("BoxCollider"), resources.GetResource<Resources::Shader>("ColliderShader"), Physics::ColliderTypes::Box, true);
		currentScene->AddGameObject(box14);
		box14->SetCollider(currentScene->GetLastCollider());
		box14->GetRigidbody().useGravity = false;

		
		// Pistol	
		LowRenderer::GameObject* pistol = new LowRenderer::GameObject(LowRenderer::Model(
			resources.GetResource<Resources::Mesh>("Pistol"),
			resources.GetResource<Resources::Shader>("BasicShader"),
			resources.GetResource<Resources::Texture>("PistolText")),
			Physics::Transform(
				Core::Maths::Vec3(.25f, .3f, -.1f),
				Core::Maths::Vec3(.1f, .1f, .1f),
				Core::Maths::Vec3(0.f, 0.f, -35.f)), "Pistol");

		currentScene->AddGameObject(pistol);

		LowRenderer::Camera* camera = new LowRenderer::Camera();
		currentScene->AddGameObject(camera, LowRenderer::GOType::Camera);

		// Patrick (PLAYER)
		LowRenderer::GameObject* skipper = new LowRenderer::GameObject(LowRenderer::Model(
			resources.GetResource<Resources::Mesh>("Patrick"),
			resources.GetResource<Resources::Shader>("BasicShader"),
			resources.GetResource<Resources::Texture>("PatrickText")),
			Physics::Transform(
				Core::Maths::Vec3(7.f, 1.f, 0.f),
				Core::Maths::Vec3(2.f, 2.f, 2.f),
				Core::Maths::Vec3(0.f, 0.f, 0.f)), "Player");

		currentScene->CreateCollider(resources.GetResource<Resources::Mesh>("SphereCollider"), resources.GetResource<Resources::Shader>("ColliderShader"), Physics::ColliderTypes::Sphere, false);
		skipper->SetCollider(currentScene->GetLastCollider());	
		skipper->GetRigidbody().useGravity = true;
		skipper->SetEnablePlayerControler(true);
		playerCollider = skipper->GetCollider();
		 sphereCollider = dynamic_cast<Physics::SphereCollider*>(playerCollider);
		sphereCollider->center = { 0,0.5f,0.0f };
		sphereCollider->radius = 0.5f;
		currentScene->AddGameObject(skipper, LowRenderer::GOType::Player);

		Assertion(currentScene->SetParent("Player", "Camera"), "Fail to set parent");
		Assertion(currentScene->SetParent("Player", "Pistol"), "Fail to set parent");

		LowRenderer::InitLight initLight
		{
			Core::Maths::Vec3(2.f, 1.f, 0.f),	   // position
			Core::Maths::Vec4(0.15f, 0.1f, 0.44f, 1.0f), // ambientColor
			Core::Maths::Vec4(0.15f, 0.1f, 0.44f, 1.0f), // diffuseColor
			Core::Maths::Vec4(.9f, .9f, .9f, 1.f)  // specularColor 
		};


		currentScene->AddDirectionLight(LowRenderer::DirectionLight(initLight, Core::Maths::Vec3(0.f, 0.f, 1.f)));

		currentScene->AddPointLight(LowRenderer::PointLight(initLight, 1.f, 0.09f, 0.032f));

		initLight.position = Core::Maths::Vec3(-2.f, 8.f, 0.f);
		currentScene->AddSpotLight(LowRenderer::SpotLight(initLight, Core::Maths::Vec3(0.f, -1.f, 0.f), 12.5f, 0.82f));
	}
}