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
	class GraphicsDebug;
	class GraphicsImage;

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
		virtual GInterface::GraphicsRenderable* CreateRenderable(const char* fileName, bool isSold = true) abstract;
		// �������� ������Ʈ ����
		virtual void DeleteRenderable(GInterface::GraphicsRenderable* renderable) abstract;
		// ��ŷ�� �������� ������Ʈ ��ȯ
		virtual GInterface::GraphicsRenderable* GetPickedRenderable(int mouseX, int mouseY) abstract;

	public:
		// ����� ť�� ���� �� ��ȯ ** �� ���� ������Ʈ ����� �̸� �����!**
		virtual GInterface::GraphicsDebug* CreateDebugCube(const char* objectName, float width, float height, float depth) abstract;
		virtual GInterface::GraphicsDebug* CreateDebugSphere(const char* objectName, float radius) abstract;
		
		virtual void DeleteDebugObject(GInterface::GraphicsDebug* debugObject) abstract;

	public:
		// �� ���ؽ��� ���ϴ� �÷��� ������ ����
		virtual void CreateDebugLine(const DirectX::XMFLOAT3& vertex1, const DirectX::XMFLOAT3& vertex2, const DirectX::XMFLOAT4& color) abstract;
		// �������� ������� index�� �ο�, �� index�� ���� ������ ����
		virtual void DeleteLine(int index) abstract;
		// �������� ���ΰ� ��ġ�ϴ� ���ؽ��� ���� ������ ����
		virtual void DeleteLine(const DirectX::XMFLOAT3& vertex1, const DirectX::XMFLOAT3& vertex2) abstract;
		// ��� ������ ����
		virtual void DeleteAllLine() abstract;

	public:
		// UI Image ������Ʈ ���� �� ��ȯ
		virtual GInterface::GraphicsImage* CreateImage(const char* imageName) abstract;
		// UI Image ������Ʈ ����
		virtual void DeleteImage(GInterface::GraphicsImage* image) abstract;
		// ��ŷ�� UI Image ������Ʈ ��ȯ
		virtual GInterface::GraphicsImage* GetPickedImage(int mouseX, int mouseY) abstract;

	public:
		// ť��� ����
		virtual void CreateCubeMap(const char* cubeMapName, const char* textureName="", bool isCube = false) abstract;
		// ť��� ����
		virtual void DeleteCubeMap(const char* cubeMapName) abstract;
		// ť��� ����Ʈ ��������
		virtual const std::vector<std::string>& GetCubeMapList() abstract;
		// ���� ������ �Ǵ� ���� ť��� ��������
		virtual void SetMainCubeMap(std::string cubeMapName) abstract;

	public:
		// ī�޶� ���� �� ��ȯ
		virtual GInterface::GraphicsCamera* CreateCamera(const DirectX::XMFLOAT3& cameraPosition = { 0.0f, 0.0f, 0.0f }, const DirectX::XMFLOAT3& targetPosition = { 0.0f, 0.0f, 0.0f }) abstract;
		// ī�޶� ��ü�� ����
		virtual void DeleteCamera(GInterface::GraphicsCamera* camera) abstract;
		// ���� �������ϰ� �ִ� ���� ī�޶� ��������
		virtual GInterface::GraphicsCamera* GetMainCamera() abstract;
		// �Ű����ڷ� ���� ī�޶� ���� ī�޶�� ����
		virtual void SetMainCamera(GInterface::GraphicsCamera* camera) abstract;

	public:
		// Directional Light ����
		virtual GInterface::GraphicsDirLight* CreateDirectionalLight(const DirectX::XMFLOAT4& ambient, const DirectX::XMFLOAT4& diffuse, const DirectX::XMFLOAT4& specular, const DirectX::XMFLOAT3& direction) abstract;
		virtual GInterface::GraphicsPointLight* CreatePointLight(const DirectX::XMFLOAT4& ambient, const DirectX::XMFLOAT4& diffuse, const DirectX::XMFLOAT4& specular, const DirectX::XMFLOAT3& position, float range) abstract;


	public:
		// End Ű�� ���� �ؽ�Ʈ ��� Ȱ��ȭ / ��Ȱ��ȭ
		virtual void DrawDebugText(int posX, int posY, int fontSize, const char* text, ...) abstract;
		// ������ ������ �ؽ�Ʈ ��� (RGBA 0~255)
		virtual void DrawColorText(int posX, int posY, int fontSize, const DirectX::XMFLOAT4& color, const char* text, ...) abstract;

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
		virtual const std::vector<std::string>& GetTextureNameList() abstract;

		// screen ���콺 ��ǥ�� world ��ǥ�� ��ȯ
		virtual const DirectX::XMFLOAT3 ScreenToWorldPoint(int mouseX, int mouseY) abstract;

		// IMGUIZMO�� ����ϱ� ���� ���� ī�޶��� View����� ��ȯ
		virtual const DirectX::XMFLOAT4X4& GetViewMatrix() abstract;

		// IMGUIZMO�� ����ϱ� ���� ���� ī�޶��� Proj����� ��ȯ
		virtual const DirectX::XMFLOAT4X4& GetProjMatrix() abstract;


		// Ư�� ���� ���󰡵��� �ϴ� Transform ��� ��ȯ
		virtual const DirectX::XMFLOAT4X4& GetTransform(GInterface::GraphicsRenderable* renderable, const std::string& boneName) abstract;

		// �׺� �޽��� ���� ��� �޽����� ���ؽ��� ������ǥ ��ȯ
		virtual const std::vector<std::vector<std::vector<DirectX::XMFLOAT3>>>& GetAllMeshVertex() abstract;
		// �׺� �޽��� ���� ��� �ε������� ���ؽ��� ������ǥ ��ȯ
		virtual const std::vector<std::vector<std::vector<unsigned int>>>& GetAllMeshIndex() abstract;
	};
};

	// �׷��Ƚ� ������ ������ �� ��ȯ
	extern "C" __declspec(dllexport) GInterface::GraphicsInterface* CreateGraphicsEngine();

	// �׷��Ƚ� ������ ����
	extern "C" __declspec(dllexport) void ReleaseGraphicsEngine(GInterface::GraphicsInterface* instance);