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


// GBuffer�� �� ��ҵ�
enum class eBUFFERTYPE
{
	GBUFFER_POSITION,
	GBUFFER_DIFFUSE,
	GBUFFER_NORMALMAP,
	GBUFFER_DEPTH,

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
			void ClearRenderTargets(float color[4]);

		public:
			ID3D11ShaderResourceView* GetSRV(int index);

		private:
			void CreateRenderTargetTexture(ArkEngine::ArkDX11::ArkDevice* pDeivce);
			void CreateRenderTargetView(ArkEngine::ArkDX11::ArkDevice* pDeivce);
			void CreateShaderResourceView(ArkEngine::ArkDX11::ArkDevice* pDeivce);

		private:
			int _textureWidth;
			int _textureHeight;

			// texture�� �� ����� ���� ������ srv���� ���� �Ŀ� release ��Ű�� ����ε� ������ ���� ���� ����
			ID3D11Texture2D* _renderTargetTextureArray[static_cast<int>(eBUFFERTYPE::GBUFFER_COUNT)];
			ID3D11RenderTargetView* _renderTargetViewArray[static_cast<int>(eBUFFERTYPE::GBUFFER_COUNT)];
			ID3D11ShaderResourceView* _shaderResourceViewArray[static_cast<int>(eBUFFERTYPE::GBUFFER_COUNT)];
		};
	}
}