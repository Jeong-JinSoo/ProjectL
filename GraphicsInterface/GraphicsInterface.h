/// <summary>
/// ���� ������ �׷��Ƚ� ������ �������ִ� �������̽�
/// �����Ͽ� ����ؼ� �߰��ϰ� ������ ���� ����
/// 
/// �̰�ȣ ����ȭ
/// </summary>

/// <summary>
/// IRenderable = 3D ������Ʈ
/// </summary>

#pragma once
#include <string>
#include <vector>

namespace KunrealEngine
{
	namespace KunrealMath
	{
		struct Float3;
	}
}

namespace DirectX
{
	struct XMFLOAT3;
	struct XMFLOAT4;
	struct XMMATRIX;
}

namespace ArkEngine
{
	class IRenderable;
}

namespace GInterface
{
	class GraphicsRenderable;
	class GraphicsCamera;
	class GraphicsDirLight;
	class GraphicsPointLight;

	class GraphicsInterface
	{
	public:
		virtual ~GraphicsInterface() {}

	public:
		// �׷��Ƚ� ���� ���� �� �ʱ�ȭ �� �� ���
		virtual void Initialize(long long hwnd, int clientWidth, int clientHeight) abstract;
		// �׷��Ƚ� ���� ���� �� ���ϴ� ����� �ʱ�ȭ �� �� ���
		virtual void Initialize(long long hwnd, int clientWidth, int clientHeight, float backGroundColor[4]) abstract;
		// �׷��Ƚ� ���� ���� ��� ��ü ������Ʈ
		virtual void Update() abstract;
		// �׷��Ƚ� ���� ���� ��� ��ü ������
		virtual void Render() abstract;
		// �׷��Ƚ� ������ ����� �� ���
		virtual void Finalize() abstract;

	public:
		// �������� ������Ʈ�� ���� �� �̸� ��ȯ
		virtual GInterface::GraphicsRenderable* CreateRenderable(const char* fileName, const char* textureName = nullptr, bool isSold = true) abstract;
		// �������� ������Ʈ ����
		virtual void DeleteRenderable(GInterface::GraphicsRenderable* renderable) abstract;

	public:
		// ť��� ����
		virtual void CreateCubeMap(const char* cubeMapName, const char* textureName="", bool isCube = false) abstract;
		// ť��� ����
		virtual void DeleteCubeMap(const char* cubeMapName) abstract;
		// ť��� ����Ʈ ��������
		virtual std::vector<std::string> GetCubeMapList() abstract;
		// ���� ������ �Ǵ� ���� ť��� ��������
		virtual void SetMainCubeMap(std::string cubeMapName) abstract;

	public:
		// ī�޶� ���� �� ��ȯ
		virtual GInterface::GraphicsCamera* CreateCamera(KunrealEngine::KunrealMath::Float3 cameraPosition = { 0.0f, 0.0f, 0.0f }, KunrealEngine::KunrealMath::Float3 targetPosition = { 0.0f, 0.0f, 0.0f }) abstract;
		// ī�޶� ��ü�� ����
		virtual void DeleteCamera(GInterface::GraphicsCamera* camera) abstract;
		// ���� �������ϰ� �ִ� ���� ī�޶� ��������
		virtual GInterface::GraphicsCamera* GetMainCamera() abstract;
		// �Ű����ڷ� ���� ī�޶� ���� ī�޶�� ����
		virtual void SetMainCamera(GInterface::GraphicsCamera* camera) abstract;

	public:
		// Directional Light ����
		virtual GInterface::GraphicsDirLight* CreateDirectionalLight(KunrealEngine::KunrealMath::Float4 ambient, KunrealEngine::KunrealMath::Float4 diffuse, KunrealEngine::KunrealMath::Float4 specular, KunrealEngine::KunrealMath::Float3 direction) abstract;
		virtual GInterface::GraphicsPointLight* CreatePointLight(KunrealEngine::KunrealMath::Float4 ambient, KunrealEngine::KunrealMath::Float4 diffuse, KunrealEngine::KunrealMath::Float4 specular, KunrealEngine::KunrealMath::Float3 position, float range) abstract;


	public:
		// End Ű�� ���� �ؽ�Ʈ ��� Ȱ��ȭ / ��Ȱ��ȭ
		virtual void DrawDebugText(int posX, int posY, const char* text, ...) abstract;

	public:
		// IMGUI���� ��������Ű�� ���� �̹����� �Ѱ���
		virtual void* GetRenderingImage() abstract;

		// IMGUI�� ����̽��� �Ѱ��ֱ� ����
		virtual void* GetDevice() abstract;

		// IMGUI�� ����̽� ���ؽ�Ʈ�� �Ѱ��ֱ� ����
		virtual void* GetDeviceContext() abstract;

		// IMGUI�� RenderTargetView�� �Ѱ��ֱ� ����
		virtual void* GetRenderTargetView() abstract;

		// IMGUI�� �׸� �� �ִ� �Ľ̵� ������Ʈ�� ���ϸ��� �Ѱ��ֱ� ����
		virtual const std::vector<std::string> GetRenderableNameList() abstract;

		// IMGUI�� �׸� �� �ִ� �Ľ̵� �ؽ����� ���ϸ��� �Ѱ��ֱ� ����
		virtual const std::vector<std::string> GetTextureNameList() abstract;
	};

	//public:
	//	//�׷��Ƚ� ���� �ʱ�ȭ
	//	virtual void Initialize(long long hWnd, int width, int height) abstract;
	//	//�������� ������ �ʱ�ȭ
	//	//ī�޶� ���� ������ �ٸ���?
	//	virtual void Initialize(long long hWnd, int width, int height, float bgColor[4]) abstract;
	//
	//	//�׸���
	//	virtual void BeginRender() abstract;
	//	virtual void EndRender() abstract;
	//
	//	//�׷��Ƚ� ������ �ڷḦ �Ѱ�����Ѵ� �Ű������� �ӽ�
	//	//������ �ִ� ������Ʈ�ε��� ��� �Ѱ��ش�? ��ȿ�� �̸� ���Ѱ� �ϳ� �� ������
	//	//�Ѱ��� ����ü�� �����غ��� ���߿� �и��ϴ���
	//	virtual void SetMeshData(DirectX::XMMATRIX worldTM) abstract;
	//	virtual void SetSkinnedMeshData(DirectX::XMMATRIX worldTM) abstract;
	//
	//	virtual void �׷���() abstract;
	//
	//	//ť�� �׷���
	//	//�̷��� �ϳ��ϳ� �� �������� �Ѱ��� ����ü�� �����ؼ� �׿� �°� ������ �ǵ���
	//	//�ϴ� �׷�����
	//	virtual void DrawMesh(DirectX::XMMATRIX worldTM, float �����ΰ������Դϴ�) abstract;
	//
	//	//MeshObject
	//	//���� �̸��� ���°� �ƴ϶� ������ �ִ� ���� ��θ� �Է�
	//	//??
	//	virtual ArkEngine::IRenderable* CreateRenderable(const char* fbxPath) abstract;
	//
	//	virtual void Finalize() abstract;
};

	// �׷��Ƚ� ������ ������ �� ��ȯ
	extern "C" __declspec(dllexport) GInterface::GraphicsInterface* CreateGraphicsEngine();

	// �׷��Ƚ� ������ ����
	extern "C" __declspec(dllexport) void ReleaseGraphicsEngine(GInterface::GraphicsInterface* instance);