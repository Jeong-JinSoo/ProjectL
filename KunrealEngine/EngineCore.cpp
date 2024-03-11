#include "EngineCore.h"
#include "SceneManager.h"
#include "Scene.h"
#include "GameObject.h"
#include "TimeManager.h"
#include "GraphicsSystem.h"
#include "PhysicsSystem.h"
#include "InputSystem.h"
#include "SoundSystem.h"

#include "ComponentHeaders.h"

/// �� �������� �� ���ж��̺귯���� ������ ����� �������� üũ��
#include <DirectXMath.h>
/// 

KunrealEngine::SceneManager& sceneInstance = KunrealEngine::SceneManager::GetInstance();
KunrealEngine::TimeManager& timeInstance = KunrealEngine::TimeManager::GetInstance();
KunrealEngine::InputSystem* inputInstance = KunrealEngine::InputSystem::GetInstance();
KunrealEngine::SoundSystem& soundInstance = KunrealEngine::SoundSystem::GetInstance();

KunrealEngine::GameObject* player;
KunrealEngine::GameObject* ninaveh;
KunrealEngine::GameObject* claudia;
KunrealEngine::GameObject* kamen;

KunrealEngine::GameObject* testCamera;

DirectX::XMFLOAT3 targetPos;

KunrealEngine::EngineCore::EngineCore()
	:_gInterface(nullptr)
{

}

KunrealEngine::EngineCore::~EngineCore()
{

}

void KunrealEngine::EngineCore::Initialize(HWND hwnd, HINSTANCE hInstance, int screenWidth, int screenHeight)
{
	// Initailize
	sceneInstance.Initialize();
	timeInstance.Initialize();
	GraphicsSystem::GetInstance().Initialize(hwnd, screenWidth, screenHeight);
	PhysicsSystem::GetInstance().Initialize();
	inputInstance->Initialize(hInstance, hwnd, screenHeight, screenWidth);
	soundInstance.Initialize(hwnd);

	/// �ϵ� ����� ��
	PlayGround();
}

void KunrealEngine::EngineCore::Release()
{
	sceneInstance.Release();
	timeInstance.Release();
	KunrealEngine::GraphicsSystem::GetInstance().Release();
	/// �������� release �� �͵� �߰�
}

void KunrealEngine::EngineCore::FixedUpdate()
{
	PhysicsSystem::GetInstance().FixedUpdate();
	sceneInstance.FixedUpdateScene(sceneInstance.GetCurrentScene());
}

bool moveTo = true;

void KunrealEngine::EngineCore::Update()
{
	inputInstance->Update(GetDeltaTime());
	inputInstance->UpdateEditorMousePos(_editorMousepos);
	sceneInstance.UpdateScene(sceneInstance.GetCurrentScene());
	GraphicsSystem::GetInstance().Update(_editorMousepos.x, _editorMousepos.y);

	CheckMousePosition();

	GRAPHICS->DrawDebugText(100, 100, 20, "FPS : %.2f", 1 / TimeManager::GetInstance().GetDeltaTime());
	//GRAPHICS->DrawDebugText(100, 100, 20, "1610�踶owner������ : %f", sceneInstance.GetCurrentScene()->GetGameObject("modelTest")->GetComponent<Transform>()->GetPosition().x);
	//GRAPHICS->DrawDebugText(100, 300, 30, "%s", sceneInstance.GetCurrentScene()->GetSceneName().c_str());

	// ���� ���� ��ȯ �׽�Ʈ
	BoxCollider* colA = sceneInstance.GetCurrentScene()->GetGameObject("Knife")->GetComponent<BoxCollider>();
	BoxCollider* colB = sceneInstance.GetCurrentScene()->GetGameObject("Rock")->GetComponent<BoxCollider>();
	BoxCollider* playerCol = sceneInstance.GetCurrentScene()->GetGameObject("Player")->GetComponent<BoxCollider>();

	Transform* trr = sceneInstance.GetCurrentScene()->GetGameObject("Knife")->GetComponent<Transform>();

	if (trr->GetPosition().x > 20.f)
	{
		moveTo = false;
	}

	if (trr->GetPosition().x < -20.f)
	{
		moveTo = true;
	}

	if (moveTo)
	{
		trr->SetPosition(trr->GetPosition().x + 0.11f, 5.f, 0.f);
	}
	else
	{
		trr->SetPosition(trr->GetPosition().x - 0.11f, 5.f, 0.f);
	}
	//sceneInstance.GetCurrentScene()->GetGameObject("Knife")->GetComponent<Transform>()->SetPosition(sceneInstance.GetCurrentScene()->GetGameObject("Knife")->GetComponent<Transform>()->GetPosition().x + 0.1f, 0.f, 0.f);

	sceneInstance.GetCurrentScene()->GetGameObject("Rock")->GetComponent<Transform>()->SetPosition(sceneInstance.GetCurrentScene()->GetGameObject("Rock")->GetComponent<Transform>()->GetPosition().x + 0.1f, 5.f, 0.f);



	if (playerCol->GetTargetObject() != nullptr)
	{
		GRAPHICS->DrawDebugText(800, 50, 60, "%s%s", "TargetObj : ", playerCol->GetTargetObject()->GetObjectName().c_str());
	}
	else
	{
		GRAPHICS->DrawDebugText(800, 50, 60, "%s", "TargetObj : None");
	}

	auto x = testCamera->GetComponent<Transform>()->GetPosition().x;
	auto y = testCamera->GetComponent<Transform>()->GetPosition().y;
	auto z = testCamera->GetComponent<Transform>()->GetPosition().z;



	if (inputInstance->KeyInput(KEY::B))
	{
	}
	else if (inputInstance->KeyInput(KEY::N))
	{
	}
	else if (inputInstance->KeyInput(KEY::A))
	{
		testCamera->GetComponent<KunrealEngine::Camera>()->SetCameraPosition(-10.0f * TimeManager::GetInstance().GetDeltaTime() + x, y, z);
	}
	else if (inputInstance->KeyInput(KEY::D))
	{
		testCamera->GetComponent<KunrealEngine::Camera>()->SetCameraPosition(10.0f * TimeManager::GetInstance().GetDeltaTime() + x, y, z);
	}
	else if (inputInstance->KeyInput(KEY::W))
	{
		testCamera->GetComponent<KunrealEngine::Camera>()->SetCameraPosition(x, y, z + 10.0f * TimeManager::GetInstance().GetDeltaTime());
	}
	else if (inputInstance->KeyInput(KEY::S))
	{
		testCamera->GetComponent<KunrealEngine::Camera>()->SetCameraPosition(x, y, z - 10.0f * TimeManager::GetInstance().GetDeltaTime());
	}
	else if (inputInstance->KeyInput(KEY::Q))
	{
		testCamera->GetComponent<KunrealEngine::Camera>()->SetCameraPosition(x, y - 10.0f * TimeManager::GetInstance().GetDeltaTime(), z);
	}
	else if (inputInstance->KeyInput(KEY::E))
	{
		testCamera->GetComponent<KunrealEngine::Camera>()->SetCameraPosition(x, y + 10.0f * TimeManager::GetInstance().GetDeltaTime(), z);
	}
	else if (inputInstance->KeyInput(KEY::R))
	{
		GRAPHICS->GetMainCamera()->RotateCamera({ 0,  -20.0f * TimeManager::GetInstance().GetDeltaTime() });
	}
	else if (inputInstance->KeyInput(KEY::T))
	{
		GRAPHICS->GetMainCamera()->RotateCamera({ 0,  20.0f * TimeManager::GetInstance().GetDeltaTime() });
	}

	inputInstance->GetMousePosition(_ingameMouseX, _ingameMouseY);

	cursorimage->SetPosition(_ingameMouseX, _ingameMouseY);
	cursorimage->SetScale(0.9 * 0.1, 1.6 * 0.1);

	// �÷��̾� ����
	if (player->GetComponent<Player>()->GetPlayerStatus() == Player::Status::IDLE)
	{
		GRAPHICS->DrawDebugText(200, 300, 20, "Player Status : IDLE");
	}
	else if (player->GetComponent<Player>()->GetPlayerStatus() == Player::Status::WALK)
	{
		GRAPHICS->DrawDebugText(200, 300, 20, "Player Status : WALK");
	}
	else if (player->GetComponent<Player>()->GetPlayerStatus() == Player::Status::DASH)
	{
		GRAPHICS->DrawDebugText(200, 300, 20, "Player Status : DASH");
	}
	else
	{
		GRAPHICS->DrawDebugText(200, 300, 20, "Player Status : Idontknow");
	}

	//Updatecoroutine();
}

void KunrealEngine::EngineCore::LateUpdate()
{
	sceneInstance.LateUpdateScene(sceneInstance.GetCurrentScene());
	GRAPHICS->Update();	// �׷��Ƚ��� ī�޶� ������Ʈ ����
}

void KunrealEngine::EngineCore::UpdateAll()
{
	timeInstance.Update();
	this->FixedUpdate();
	this->Update();
	this->LateUpdate();
	this->Render();
}

void KunrealEngine::EngineCore::Render()
{
	GRAPHICS->Render();
}

void KunrealEngine::EngineCore::SetEditorMousePos(POINT position)
{
	_editorMousepos = position;
	return;
}

void KunrealEngine::EngineCore::PlayGround()
{
	int soundindex = soundInstance.AddSound("Resources/Sound/soundch2.wav", 70);
	//soundInstance.Loop(soundindex);

	// Camera
	KunrealEngine::KunrealMath::Float3 cameraPos = { 0.0f, 30.0f, -55.0f };
	//KunrealEngine::KunrealMath::Float3 cameraPos = { 40.0f, 2.0f, -30.0f };
	KunrealEngine::KunrealMath::Float3 targetPos = { 0.0f, -15.0f, 0.0f };
	testCamera = sceneInstance.GetCurrentScene()->CreateObject("testCamera");
	testCamera->AddComponent<Camera>();
	testCamera->GetComponent<Camera>()->SetCameraPosition(cameraPos.x, cameraPos.y, cameraPos.z);
	testCamera->GetComponent<Camera>()->SetTargetPosition(targetPos.x, targetPos.y, targetPos.z);
	//testCamera->GetComponent<Camera>()->RotateCamera(5.f, 0.f);
	testCamera->GetComponent<Camera>()->SetMainCamera();
	
	//testCamera->GetComponent<Transform>()->SetPosition(-32.f, 45.f, -32.f);
	//testCamera->GetComponent<Transform>()->SetRotation(0.f, 45.f, 0.f);

	// Player
	player = sceneInstance.GetCurrentScene()->CreateObject("Player");
	player->AddComponent<MeshRenderer>();
	player->GetComponent<MeshRenderer>()->SetMeshObject("Kachujin/Kachujin");
	player->GetComponent<Transform>()->SetScale(0.05f, 0.05f, 0.05f);
	player->GetComponent<Transform>()->SetRotation(0.f, 45.f, 0.f);
	player->GetComponent<MeshRenderer>()->SetActive(true);
	player->GetComponent<MeshRenderer>()->SetPickableState(true);
	KunrealEngine::KunrealMath::Float4 reflect = { 0.9f, 0.9f, 0.9f, 0.9f };
	//sceneInstance.GetCurrentScene()->GetGameObject("modelTest")->GetComponent<MeshRenderer>()->SetDiffuseTexture("Resources/Textures/Kachujin/Kachujin_diffuse.png");
	sceneInstance.GetCurrentScene()->GetGameObject("Player")->GetComponent<MeshRenderer>()->SetReflect(reflect.x, reflect.y, reflect.z, reflect.w);
	player->AddComponent<BoxCollider>();
	player->GetComponent<BoxCollider>()->SetBoxSize(3.0f, 8.0f, 5.0f);
	player->AddComponent<Player>();

	ninaveh = sceneInstance.GetCurrentScene()->CreateObject("Nineveh");
	ninaveh->AddComponent<MeshRenderer>();
	ninaveh->GetComponent<MeshRenderer>()->SetMeshObject("Nineveh/Nineveh");
	ninaveh->AddComponent<BoxCollider>();
	ninaveh->GetComponent<BoxCollider>()->SetBoxSize(10.f, 10.f, 10.f);
	ninaveh->GetComponent<BoxCollider>()->SetOffset(0, 10.f, 0);
	ninaveh->GetComponent<Transform>()->SetPosition(10.0f, 0.0f, -20.0f);
	ninaveh->GetComponent<Transform>()->SetScale(20.0f, 20.0f, 20.0f);
	ninaveh->GetComponent<Transform>()->SetRotation(-270.f, -45.f, -180.f);

	claudia = sceneInstance.GetCurrentScene()->CreateObject("Claudia");
	claudia->AddComponent<MeshRenderer>();
	claudia->GetComponent<MeshRenderer>()->SetMeshObject("Claudia/Claudia");
	claudia->GetComponent<Transform>()->SetPosition(-23.0f, 0.0f, 6.0f);
	claudia->GetComponent<Transform>()->SetScale(0.2f, 0.2f, 0.2f);
	claudia->GetComponent<Transform>()->SetRotation(-270.f, -45.f, -180.f);

	kamen = sceneInstance.GetCurrentScene()->CreateObject("Kamen");
	kamen->AddComponent<MeshRenderer>();
	kamen->GetComponent<MeshRenderer>()->SetMeshObject("Kachujin/Kachujin");
	kamen->GetComponent<Transform>()->SetPosition(-50.0f, 0.0f, 10.0f);
	kamen->GetComponent<Transform>()->SetScale(0.05f, 0.05f, 0.05f);
	kamen->GetComponent<Transform>()->SetRotation(0.f, 45.f, 0.f);
	kamen->AddComponent<Kamen>();
	//kamen->GetComponent<Kamen>()->SetPlayer(modelTest);

	// Plane 
	auto plane = sceneInstance.GetCurrentScene()->CreateObject("cube");
	plane->AddComponent<MeshRenderer>();
	plane->GetComponent<MeshRenderer>()->SetMeshObject("cube/cube", true);
	plane->GetComponent<MeshRenderer>()->SetDiffuseTexture(0, "floor.dds");
	plane->GetComponent<MeshRenderer>()->SetNormalTexture(0, "floor_nmap.dds");
	plane->GetComponent<Transform>()->SetScale(100.0f, 1.0f, 100.0f);
	plane->GetComponent<Transform>()->SetPosition(0, -1.0f, 0);

	// cube map test
	GRAPHICS->CreateCubeMap("test", "sunsetcube1024.dds", true);
	auto list = GRAPHICS->GetCubeMapList();
	GRAPHICS->SetMainCubeMap(list.back());

	// light test
	DirectX::XMFLOAT4 diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
	DirectX::XMFLOAT4 ambient = { 0.3f, 0.3f, 0.3f, 1.0f };
	DirectX::XMFLOAT4 specular = { 1.0f, 1.0f, 1.0f, 1.0f };
	DirectX::XMFLOAT3 direction = { 0.5f, -0.5f, 0.5f };

	auto lightTest = sceneInstance.GetCurrentScene()->CreateObject("DirectionalLight");
	lightTest->AddComponent<Light>();
	lightTest->GetComponent<Light>()->CreateDirectionalLight(ambient, diffuse, specular, direction);
	lightTest->GetComponent<Light>()->SetActive(true);
	lightTest->SetActive(true);

	// PointLight ����
	auto pointLight = sceneInstance.GetCurrentScene()->CreateObject("pointLightTest");
	pointLight->AddComponent<Light>();
	pointLight->GetComponent<Transform>()->SetPosition(0.f, 0.f, 0.f);
	pointLight->GetComponent<Light>()->CreatePointLight(ambient, diffuse, specular, 1.0f);
	pointLight->GetComponent<Light>()->SetAmbient(1.0f, 1.0f, 1.0f, 1.0f);
	pointLight->GetComponent<Light>()->SetPointRange(10.f);
	pointLight->GetComponent<Light>()->SetPointRange(100.0f);
	pointLight->SetActive(false);

	auto pointLight2 = sceneInstance.GetCurrentScene()->CreateObject("pointLightTest2");
	pointLight2->AddComponent<Light>();
	pointLight2->GetComponent<Transform>()->SetPosition(-70.f, 0.f, 50.f);
	pointLight2->GetComponent<Light>()->CreatePointLight(ambient, diffuse, specular, 0.0f);
	pointLight2->GetComponent<Light>()->SetAmbient(1.0f, 1.0f, 1.0f, 1.0f);
	pointLight2->GetComponent<Light>()->SetPointRange(50.f);
	pointLight2->SetActive(false);

	// �浹üũ
	auto c1 = sceneInstance.GetCurrentScene()->CreateObject("Knife");
	auto c2 = sceneInstance.GetCurrentScene()->CreateObject("Rock");


	c1->AddComponent<BoxCollider>();
	c1->GetComponent<BoxCollider>()->SetBoxSize(5.f, 10.f, 10.f);
	c1->AddComponent<MeshRenderer>();
	c1->GetComponent<Transform>()->SetPosition(-20.f, 9.f, 0.f);
	c1->GetComponent<Transform>()->SetScale(0.3f, 0.3f, 0.3f);
	c1->GetComponent<MeshRenderer>()->SetMeshObject("Sword/Sword", "bricks.dds");
	c1->GetComponent<MeshRenderer>()->SetPickableState(true);

	c2->AddComponent<BoxCollider>();
	c2->GetComponent<BoxCollider>()->SetBoxSize(11.f, 11.f, 11.f);
	c2->AddComponent<MeshRenderer>();
	c2->GetComponent<Transform>()->SetPosition(-40.f, 15.f, 0.f);
	c2->GetComponent<Transform>()->SetScale(5.f, 5.f, 5.f);
	//c2->GetComponent<MeshRenderer>()->SetMeshObject("Kachujin/Kachujin", "Kachujin/Kachujin_diffuse.png");
	c2->GetComponent<MeshRenderer>()->SetMeshObject("cube/cube", "bricks.dds");
	c2->GetComponent<MeshRenderer>()->SetPickableState(true);

	targetPos = { 0.0f ,0.0f ,0.0f };

	auto imageTest = sceneInstance.GetCurrentScene()->CreateObject("ImageTest1");
	imageTest->AddComponent<ImageRenderer>();
	imageTest->GetComponent<ImageRenderer>()->SetImage("MenuQuitButton.dds");
	imageTest->GetComponent<ImageRenderer>()->SetImageStatus(true);
	imageTest->GetComponent<ImageRenderer>()->SetPosition(500.f, 400.f);
	imageTest->GetComponent<ImageRenderer>()->SetImageStatus(false);

	cursorimage = GRAPHICS->CreateImage("Resources/Textures/floor.dds");
	cursorimage->SetPosition(500.0f);
	cursorimage->SetRenderingState(false);


	// Test
	for (int i = 0; i < 10; i++)
	{
		std::string name = "rightCube" + std::to_string(i);
		auto cube1 = sceneInstance.GetCurrentScene()->CreateObject(name);
		cube1->AddComponent<MeshRenderer>();
		cube1->GetComponent<MeshRenderer>()->SetMeshObject("cube/cube", true);
		cube1->GetComponent<MeshRenderer>()->SetDiffuseTexture(0, "bricks.dds");
		cube1->GetComponent<MeshRenderer>()->SetNormalTexture(0, "bricks_nmap.dds");
		cube1->GetComponent<Transform>()->SetScale(2.0f, 2.0f, 2.0f);
		cube1->GetComponent<Transform>()->SetPosition(40.0f, 2.0f, 50.0f - 10.0f * i);

		if (i % 2 == 0)
		{
			cube1->GetComponent<MeshRenderer>()->SetPickableState(true);
		}
	}

	for (int i = 0; i < 10; i++)
	{
		std::string name = "leftCube" + std::to_string(i);
		auto cube1 = sceneInstance.GetCurrentScene()->CreateObject(name);
		cube1->AddComponent<MeshRenderer>();
		cube1->GetComponent<MeshRenderer>()->SetMeshObject("cube/cube", true);
		cube1->GetComponent<MeshRenderer>()->SetDiffuseTexture(0, "bricks.dds");
		//cube1->GetComponent<MeshRenderer>()->SetNormalTexture("bricks_nmap.dds");
		cube1->GetComponent<Transform>()->SetScale(2.0f, 2.0f, 2.0f);
		cube1->GetComponent<Transform>()->SetPosition(-40.0f, 2.0f, 50.0f - 10.0f * i);

		if (i % 2 == 1)
		{
			cube1->GetComponent<MeshRenderer>()->SetPickableState(true);
		}
	}
}

void KunrealEngine::EngineCore::CheckMousePosition()
{
	// �����ͷ� �������� �ʾ��� ���
	if (_editorMousepos.x == -1 && _editorMousepos.y == -1)
	{
		{
			_finalMousePosition.x = _ingameMouseX;
			_finalMousePosition.y = _ingameMouseY;
		}
	}

	else
	{
		_finalMousePosition = _editorMousepos;
	}
}

float KunrealEngine::EngineCore::GetDeltaTime()
{
	return timeInstance.GetDeltaTime();
}
