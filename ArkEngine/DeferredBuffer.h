/// <summary>
/// ���۵� �������� ���� 
/// ���۵� ���۸� �����ϱ� ���� Ŭ����
/// 
/// ����ȭ
/// </summary>

#pragma once
struct ID3D11Texture2D;
struct ID3D11RenderTargetView;
struct ID3D11ShaderResourceView;
struct ID3D11DepthStencilView;


// GBuffer�� �� ��ҵ�
enum class eBUFFERTYPE
{
	GBUFFER_POSITION,
	GBUFFER_DIFFUSE,
	GBUFFER_NORMALMAP,
	GBUFFER_EMISSIONMAP,
	GBUFFER_MATERIAL,
	GBUFFER_ADDITIONALINFO,
	GBUFFER_COLOR,
	// G���� ����� �� ����
	GBUFFER_COUNT		
};

namespace ArkEngine
{
	namespace ArkDX11
	{
		class ArkDevice;

		class deferredBuffer
		{
		public:
			deferredBuffer(int width, int height);
			~deferredBuffer();

		public:
			void Initialize();
			void Finalize();

		public:
			void SetRenderTargets();
			void SetRenderTargets(ID3D11DepthStencilView* dsv);
			void ClearRenderTargets(float color[4]);
			void ClearRenderTargets(int index, float color[4]);

		public:
			void SetRenderTargetsForFinal();
			void ClearRenderTargetsForFinal(float color[4]);

		public:
			void SetRenderTargetsForBloom();
			void ClearRenderTargetsForBloom(float color[4]);

		public:
			ID3D11ShaderResourceView* GetSRV(int index);
			ID3D11Texture2D* GetTextrue(int index);
			ID3D11RenderTargetView* GetRenderTargetView(int index);

		public:
			ID3D11ShaderResourceView* GetSRVForFinal(int index);
			ID3D11Texture2D* GetTextrueForFinal(int index);
			ID3D11RenderTargetView* GetRenderTargetViewForFinal(int index);

		public:
			ID3D11ShaderResourceView* GetSRVForBloom(int index);
			ID3D11Texture2D* GetTextrueForBloom(int index);
			ID3D11RenderTargetView* GetRenderTargetViewForBloom(int index);

		private:
			void CreateRenderTargetTexture();
			void CreateRenderTargetView();
			void CreateShaderResourceView();

		private:
			void CreateRenderTargetTextureForFinal();
			void CreateRenderTargetViewForFinal();
			void CreateShaderResourceViewForFinal();

		private:
			void CreateRenderTargetTextureForBloom();
			void CreateRenderTargetViewForBloom();
			void CreateShaderResourceViewForBloom();

		private:
			ArkDevice* _arkDevice;
			int _textureWidth;
			int _textureHeight;

			// texture�� �� ����� ���� ������ srv���� ���� �Ŀ� release ��Ű�� ����ε� ������ ���� ���� ����
			ID3D11Texture2D* _renderTargetTextureArray[static_cast<int>(eBUFFERTYPE::GBUFFER_COUNT)];
			ID3D11RenderTargetView* _renderTargetViewArray[static_cast<int>(eBUFFERTYPE::GBUFFER_COUNT)];
			ID3D11ShaderResourceView* _shaderResourceViewArray[static_cast<int>(eBUFFERTYPE::GBUFFER_COUNT)];

		private:
			ID3D11Texture2D* _renderTargetTextureArrayForFinal;
			ID3D11RenderTargetView* _renderTargetViewArrayForFinal;
			ID3D11ShaderResourceView* _shaderResourceViewArrayForFinal;

		private:
			ID3D11Texture2D* _renderTargetTextureArrayForBloom;
			ID3D11RenderTargetView* _renderTargetViewArrayForBloom;
			ID3D11ShaderResourceView* _shaderResourceViewArrayForBloom;
		};
	}
}