/// <summary>
/// UI�� ����
/// ��ŷ�� �Ǵ� �̹��� ������Ʈ
/// 
/// ����ȭ
/// </summary>

#pragma once
#include "IUIImage.h"
#include "GraphicsImage.h"

struct ID3D11Buffer;
struct ID3DX11Effect;
struct ID3DX11EffectTechnique;
struct ID3DX11EffectMatrixVariable;
struct ID3D11ShaderResourceView;
struct ID3DX11EffectShaderResourceVariable;

namespace ArkEngine
{
	class ICamera;

	namespace ArkDX11
	{
		class ArkDevice;
		class ArkBuffer;
		class ArkEffect;
		class Transform;

		class UIImage : public ArkEngine::IUIImage, public GInterface::GraphicsImage
		{
		public:
			UIImage(const std::string& imageName, unsigned int screenWidth, unsigned int screenHeight);
			~UIImage();

		public:
			virtual void Render(bool isFinal) override;

		public:
			virtual bool GetRenderingState() override;
			virtual void SetRenderingState(bool tf) override;

		public:
			virtual const std::string& GetImageName() override;
			// �̹��� ��ü�� Ʈ������ ����	
			virtual void SetTransform(DirectX::XMFLOAT4X4 matrix) override;
			// �̹��� ��ü�� ��ġ�� ���� (�»�� 0,0)
			virtual void SetPosition(float x = 0.0f, float y = 0.0f) override;
			// �̹��� ��ü�� ȸ���� ����
			virtual void SetRotation(float angle) override;
			// �̹��� ��ü�� ũ�Ⱚ ����
			virtual void SetScale(float x = 1.0f, float y = 1.0f) override;

			// �̹��� ��ü ����
			virtual void Delete() override;

			// ��ŷ�� ���� �ؽ��� ��������
			virtual unsigned int GetHashID() override;

		private:
			void Initialize();
			
		private:
			void SetEffect();
			void BuildGeometryBuffers();
			void ConvertHashToRGBA(int hashValue);
			void SetHashValue(unsigned int index);
			void SetTexture();

		private:
			float NomalizeToScreenX(float num);
			float NomalizeToScreenY(float num);

		private:
			std::string _imageName;

		private:
			ID3DX11Effect* _effect;
			ID3DX11EffectTechnique* _tech;

		private:
			ID3DX11EffectMatrixVariable* _fxWorldViewProj;

		private:
			ID3D11Buffer* _vertexBuffer;
			ID3D11Buffer* _indexBuffer;

		private:
			ID3DX11EffectShaderResourceVariable* _diffuseMap;
			ID3D11ShaderResourceView* _diffuseMapSRV;

			ID3DX11EffectVectorVariable* _fxColor;

		private:
			ArkEngine::ArkDX11::ArkDevice* _arkDevice;
			ArkEngine::ArkDX11::ArkBuffer* _arkBuffer;
			ArkEngine::ArkDX11::ArkEffect* _arkEffect;
			ArkEngine::ArkDX11::Transform* _meshTransform;


		private:
			float _color[4];
			unsigned int _hashValue;
			unsigned int _objectIndex;

		private:
			bool _isRendering;

		private:
			int _halfScreenWidth;
			int _halfScreenHeight;

			int _imageHalfWidth;
			int _imageHalfHeight;
		};
	}
}


