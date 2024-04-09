/// <summary>
/// IRenderer �������̽��� ��ӹ��� ������
/// DX11�� �̿��� ������
/// 
/// ����ȭ
/// </summary>

#pragma once
#include <memory>
#include <wrl/client.h>
#include "IRenderer.h"
#include "GraphicsInterface.h"

enum D3D_FEATURE_LEVEL;
struct ID3D11Device;
struct ID3D11DeviceContext;
struct IDXGISwapChain;
struct ID3D11RenderTargetView;
struct ID3D11DepthStencilView;
struct ID3D11RasterizerState;
struct ID3D11DepthStencilState;
struct DXGI_SWAP_CHAIN_DESC;

namespace GInterface
{
	class GraphicsRenderable;
}

namespace ArkEngine
{
	class ICamera;
	class ICubeMap;
	class IdirLight;

	namespace ArkDX11
	{
		class Camera;
		class DeferredRenderer;
		class DWFont;

		// �߰�
		class ParticleSystem;
	}

	namespace FBXLoader
	{
		class AssimpTool;
	}
}

namespace DirectX
{
	struct XMFLOAT3;
}

namespace KunrealEngine
{
	namespace KunrealMath
	{
		struct Float3;
	}
}

namespace ArkEngine
{
	namespace ArkDX11
	{
		class DX11Renderer : public GInterface::GraphicsInterface
		{
		public:
			DX11Renderer();
			~DX11Renderer();

		public:
			virtual void Initialize(long long hwnd, int clientWidth, int clientHeight) override;

			virtual void Initialize(long long hwnd, int clientWidth, int clientHeight, float backGroundColor[4]) override;

			virtual void Update() override;

			virtual void Render() override;

			virtual void Finalize() override;

		public:
			virtual GInterface::GraphicsRenderable* CreateRenderable(const char* fileName, bool isSold) override;
			virtual void DeleteRenderable(GInterface::GraphicsRenderable* renderable) override;
			virtual GInterface::GraphicsRenderable* GetPickedRenderable(int mouseX, int mouseY) override;

		public:
			virtual GInterface::GraphicsDebug* CreateDebugCube(const char* objectName, float width, float height, float depth) override;
			virtual GInterface::GraphicsDebug* CreateDebugSphere(const char* objectName, float radius) override;

			virtual void DeleteDebugObject(GInterface::GraphicsDebug* debugObject) override;

		public:
			virtual void CreateDebugLine(DirectX::XMFLOAT3 vertex1, DirectX::XMFLOAT3 vertex2, DirectX::XMFLOAT4 color) override;
			virtual void DeleteLine(int index) override;
			virtual void DeleteLine(DirectX::XMFLOAT3 vertex1, DirectX::XMFLOAT3 vertex2) override;
			virtual void DeleteAllLine() override;

		public:
			virtual void CreateCubeMap(const char* cubeMapName, const char* textureName, bool isCube) override;
			virtual void DeleteCubeMap(const char* cubeMapName) override;
			virtual std::vector<std::string> GetCubeMapList() override;
			virtual void SetMainCubeMap(std::string cubeMapName) override;

		public:
			virtual GInterface::GraphicsImage* CreateImage(const char* imageName) override;
			virtual void DeleteImage(GInterface::GraphicsImage* image) override;
			virtual GInterface::GraphicsImage* GetPickedImage(int mouseX, int mouseY) override;

		public:
			virtual GInterface::GraphicsCamera* CreateCamera(DirectX::XMFLOAT3 cameraPosition, DirectX::XMFLOAT3 targetPosition) override;
			virtual void DeleteCamera(GInterface::GraphicsCamera* camera) override;
			virtual GInterface::GraphicsCamera* GetMainCamera() override;
			virtual void SetMainCamera(GInterface::GraphicsCamera* camera) override;

		public:
			virtual GInterface::GraphicsDirLight* CreateDirectionalLight(DirectX::XMFLOAT4 ambient, DirectX::XMFLOAT4 diffuse, DirectX::XMFLOAT4 specular, DirectX::XMFLOAT3 direction) override;
			virtual GInterface::GraphicsPointLight* CreatePointLight(DirectX::XMFLOAT4 ambient, DirectX::XMFLOAT4 diffuse, DirectX::XMFLOAT4 specular, DirectX::XMFLOAT3 position, float range) override;

		public:
			// End Ű�� �������� Ȱ��ȭ�Ǵ� �ؽ�Ʈ
			virtual void DrawDebugText(int posX, int posY, int fontSize, const char* text, ...) override;
			virtual void DrawColorText(int posX, int posY, int fontSize, DirectX::XMFLOAT4 color, const char* text, ...) override;

		public:
			virtual void* GetRenderingImage() override;

			virtual void* GetDevice() override;

			virtual void* GetDeviceContext() override;

			virtual void* GetRenderTargetView() override;

			virtual const std::vector<std::string> GetRenderableNameList() override;

			virtual const std::vector<std::string> GetTextureNameList() override;

		public:
			virtual DirectX::XMFLOAT3 ScreenToWorldPoint(int mouseX, int mouseY) override;

		public:
			// IMGUIZMO�� ����ϱ� ���� ���� ī�޶��� View����� ��ȯ
			virtual const DirectX::XMFLOAT4X4& GetViewMatrix() override;

			// IMGUIZMO�� ����ϱ� ���� ���� ī�޶��� Proj����� ��ȯ
			virtual const DirectX::XMFLOAT4X4& GetProjMatrix() override;

			// Ư�� ���� ���󰡵��� �ϴ� Transform ��� ��ȯ
			virtual const DirectX::XMFLOAT4X4& GetTransform(GInterface::GraphicsRenderable* renderable, const std::string& boneName) override;

			// �׺� �޽��� ���� ��� �޽����� ���ؽ��� ������ǥ ��ȯ
			virtual const std::vector<std::vector<std::vector<DirectX::XMFLOAT3>>> GetAllMeshVertex() override;
			// �׺� �޽��� ���� ��� �ε������� ���ؽ��� ������ǥ ��ȯ
			virtual const std::vector<std::vector<std::vector<unsigned int>>> GetAllMeshIndex() override;

		public:
			void CreateShadowViewPort(int shadowWidth, int shadowHeight);

		public:
			void CreateShadowCamera(DirectX::XMFLOAT3 cameraPosition, DirectX::XMFLOAT3 targetPosition);

		private:
			void BeginShadowRender();
			void BeginRender();
			void FinalRender();
			void EndRender();

		private:
			void CreateDevice();			// ID3D11Device �������̽��� DeviceContext �������̽��� ����
			void CheckQualification();	// �ϵ��� 4X MSAA�� ���� ǰ�� ������ �����ϴ��� ����
			DXGI_SWAP_CHAIN_DESC GetSwapChainDesc(long long hwnd);// ������ ��ȯ �罽�� Ư���� �����ϴ� DXGI_SWAP_CHAIN_DEXC ����ü�� ä��
			void CreateSwapChain(long long hwnd);			// SWAP CHAIN�� ����
			void CreateRenderTargetView();	// SWAP CHAIN�� �ĸ� ���ۿ� ���� ���� ��� �並 ����
			void CreateDepthStencilView();		// ���� ���Ľ� ���ۿ� �׿� ����Ǵ� ���� ���Ľ� �並 ����
			void BindView();				// ����Ÿ�� ��� ���� ���Ľ� �並 ������ ������ ��� ���ձ� �ܰ迡 ����
			void SetViewport();
			void SetShadowViewport();				// ȭ�� ũ��� ����Ʈ ����
			void SetViewportWithDefaultCamera();				// ����Ʈ ����
			void CreateRenderState();		// ���� ���� ����
			void CreateDepthStecilState();	// ���� ���� ����

		private:
			void SetResourceManager();
			void CreateFont();
			void CreateEffect();
			void CreateTexture();
			void CreateNewTexture();
			void CreateASEParser();
			void CreateDefaultCamera(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 target, DirectX::XMFLOAT3 worldUp);

		private:
			void InitializeAssimpTool();

		private:
			void InitializeWIC();

		private:
			float GetAspectRatio();
			float GetShadowRatio();

		private:
			void CreateDirLight(DirectX::XMFLOAT4 ambient, DirectX::XMFLOAT4 diffuse, DirectX::XMFLOAT4 specular, DirectX::XMFLOAT3 direction);
			void CreatePoLight(DirectX::XMFLOAT4 ambient, DirectX::XMFLOAT4 diffuse, DirectX::XMFLOAT4 specular, DirectX::XMFLOAT3 position, float range);

		private:
			void SetPickingTexture();
			UINT Picking(int mouseX, int mouseY);

		private:
			DirectX::XMFLOAT3 GetMyPosition(DirectX::XMFLOAT3 direction, DirectX::XMFLOAT3 targetPos);

		private:
			bool Equal(const DirectX::XMFLOAT3& a, const DirectX::XMFLOAT3& b)
			{
				return a.x == b.x && a.y == b.y && a.z == b.z;
			}

		private:
			Microsoft::WRL::ComPtr<ID3D11Device> _device;
			Microsoft::WRL::ComPtr<ID3D11DeviceContext> _deviceContext;

			unsigned int _4xMsaaQuality;

			bool _enable4xMsaa;

			Microsoft::WRL::ComPtr<IDXGISwapChain> _swapChain;

			Microsoft::WRL::ComPtr<ID3D11RenderTargetView> _renderTargetView;

			Microsoft::WRL::ComPtr <ID3D11DepthStencilView> _depthStencilView;

			Microsoft::WRL::ComPtr<ID3D11RasterizerState> _solidRS;
			Microsoft::WRL::ComPtr<ID3D11RasterizerState> _wireRS;
			Microsoft::WRL::ComPtr<ID3D11RasterizerState> _shadowRS;

			Microsoft::WRL::ComPtr<ID3D11DepthStencilState> _depthStencilState;
			Microsoft::WRL::ComPtr<ID3D11DepthStencilState> _depthStencilStateDisable;


			float _backGroundColor[4];

		private:
			std::unique_ptr<ArkEngine::ArkDX11::DWFont> _font;

			ArkEngine::ICamera* _originMainCamera;
			ArkEngine::ICamera* _mainCamera;


			ArkEngine::ICubeMap* _mainCubeMap;

		private:
			int _clientWidth;
			int _clientHeight;

			int _shadowWidth;
			int _shadowHeight;

			bool _isDebugMode;

		private:
			std::vector<ArkEngine::IdirLight*> _dirLightList;

		private:
			ID3D11ShaderResourceView* _renderingImageView;

		private:
			std::unique_ptr<ArkEngine::ArkDX11::DeferredRenderer> _deferredRenderer;

			/// <summary>
			/// ������ �߰�
			/// </summary>
		private:
			std::unique_ptr<ArkEngine::FBXLoader::AssimpTool> _assimpTool;

		private:
			ID3D11Texture2D* _colorTexture;

		private:
			D3D11_VIEWPORT _viewPort;
			D3D11_VIEWPORT _shadowViewPort;

			//�߰�
		private:
			std::unique_ptr<ArkEngine::ArkDX11::ParticleSystem> _particle;

		};
	}
}