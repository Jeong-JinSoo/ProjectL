/// <summary>
/// ����׿� ��ü
/// 
/// ����ȭ
/// </summary>

#pragma once
#include <string>
#include "IDebugObject.h"
#include "GraphicsDebug.h"

struct ID3DX11Effect;
struct ID3DX11EffectTechnique;
struct ID3DX11EffectMatrixVariable;
struct ID3DX11EffectVectorVariable;
struct ID3DX11EffectScalarVariable;
struct ID3D11Buffer;

namespace DirectX
{
	struct XMFLOAT4;
	struct XMFLOAT4X4;
}

namespace ArkEngine
{
	class ICamera;

	namespace ArkDX11
	{
		class ArkEffect;
		class ArkDevice;
		class Transform;
	}
}

namespace ArkEngine
{
	namespace ArkDX11
	{
		class DebugObject : public IDebugObject, public GInterface::GraphicsDebug
		{
		public:
			enum class eDebugType
			{
				Cube,
				Shpere,
				Capsule
			};

		public:
			// Ÿ�� �����ؼ� ���� �� �ִ� ������
			DebugObject(const std::string& objectName, eDebugType type, DirectX::XMFLOAT4 color = DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f));
			// DebugCube ������
			DebugObject(const std::string& objectName, float width, float height, float depth, DirectX::XMFLOAT4 color = DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f));
			// DebugSphere ������
			DebugObject(const std::string& objectName, float centerPos, float range, DirectX::XMFLOAT4 color = DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f));

			~DebugObject();

		public:
			void Initialize() override;
			void Update(ArkEngine::ICamera* p_Camera) override;
			void Render() override;
			void Finalize() override;

		public:
			virtual bool GetRenderingState() override;
			virtual void SetRenderingState(bool tf) override;

		public:
			virtual void SetTransform(DirectX::XMFLOAT4X4 matrix) override;
			virtual void SetTransformMatrix(DirectX::XMFLOAT4X4 matrix) override;
			virtual void SetPosition(float x = 0.0f, float y = 0.0f, float z = 0.0f) override;
			virtual void SetRotation(float x = 0.0f, float y = 0.0f, float z = 0.0f) override;
			virtual void SetScale(float x = 1.0f, float y = 1.0f, float z = 1.0f) override;
			virtual void Delete() override;

		public:
			virtual float GetRadius() override;

		private:
			void CheckFrustum(ArkEngine::ICamera* p_Camera);

		private:
			void BuildGeomtryBuffers();
			void SetEffect();

		private:
			std::string _objectName;
			std::string _effectName;

			ID3DX11Effect* _effect;
			ID3DX11EffectTechnique* _tech;

			ID3DX11EffectMatrixVariable* _fxWorld;
			ID3DX11EffectMatrixVariable* _fxWorldViewProj;
			ID3DX11EffectVectorVariable* _fxFrustum;

			DirectX::XMFLOAT4X4 _world;
			DirectX::XMFLOAT4X4 _view;
			DirectX::XMFLOAT4X4 _proj;

			ID3D11Buffer* _vertexBuffer;
			ID3D11Buffer* _indexBuffer;

			ArkEngine::ArkDX11::ArkDevice* _arkDevice;
			ArkEngine::ArkDX11::ArkEffect* _arkEffect;

			ArkEngine::ArkDX11::Transform* _meshTransform;

			DirectX::XMFLOAT4 _color;

		private:
			unsigned int _totalIndexCount;
			bool _isRendering;

		private:
			float _halfWidth;
			float _halfHeight;
			float _halfDepth;
			float _minPos;

			eDebugType _type;

		private:
			DirectX::XMFLOAT3 _centerPos;
		};
	}
}

