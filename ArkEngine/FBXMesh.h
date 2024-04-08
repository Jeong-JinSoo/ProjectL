/// <summary>
/// 
/// �������� FBX �޽�
/// 
/// ������
/// </summary>

#pragma once
#include <string>
#include <array>
// Buffer����ϱ�����
#include "d3d11.h"
#include "CommonStruct.h"
#include "GraphicsRenderable.h"
#include "IRenderable.h"

#include "tinyxml2.h"
#include "ParsingStructs.h"

namespace DirectX
{
	struct XMFLOAT4;
	struct XMFLOAT4X4;
}

namespace ArkEngine
{
	class ICamera;
	class IDebugObject;

	namespace ArkDX11
	{
		class ArkEffect;
		class ArkDevice;
		class Transform;
		class Animator;

		struct Material;

		class FBXAnimator;
		//class DirectionalLight;
	}
}


struct ID3DX11Effect;
struct ID3DX11EffectTechnique;

struct ID3DX11EffectVariable;
struct ID3DX11EffectScalarVariable;
struct ID3DX11EffectMatrixVariable;
struct ID3DX11EffectVectorVariable;
struct ID3DX11EffectShaderResourceVariable;

struct ID3D11ShaderResourceView;

/// ������ �߰�
struct ModelMesh;

namespace ArkEngine
{
	namespace ArkDX11
	{
		class FBXMesh : public ArkEngine::IRenderable, public GInterface::GraphicsRenderable
		{
		public:
			FBXMesh(const std::string& fileName, bool isSolid = true);
			~FBXMesh();

		public:
			virtual void Initialize() override;
			virtual void Update(ArkEngine::ICamera* p_Camera) override;
			virtual void Render() override;
			virtual void Render(int passIndex) override;
			virtual void Finalize() override;

		public:
			virtual void Delete() override;

		public:
			virtual bool GetRenderingState() override;
			virtual void SetRenderingState(bool tf) override;
			
		public:
			virtual void SetShadowState(bool tf) override;
			virtual bool GetShadowState() override;

		public:
			virtual void SetTransform(DirectX::XMFLOAT4X4 matrix) override;
			virtual void SetPosition(float x = 0.0f, float y = 0.0f, float z = 0.0f) override;
			virtual void SetRotation(float x = 0.0f, float y = 0.0f, float z = 0.0f) override;
			virtual void SetScale(float x = 1.0f, float y = 1.0f, float z = 1.0f) override;
			virtual void SetModel(const char* fileName) override;
			virtual void SetDiffuseTexture(int index, const char* textureName) override;
			virtual void SetNormalTexture(int index, const char* textureName) override;
			virtual void SetEmissiveTexture(int index, const char* textureName) override;
			virtual void SetAnimator() override;
			virtual const GInterface::Material GetMaterial() override;
			virtual void SetMaterial(GInterface::Material material) override;
			virtual void SetReflect(DirectX::XMFLOAT4 reflect) override;

		public:
			virtual const std::vector<std::string> GetDiffuseTextureList() override;
			virtual const std::vector<std::string> GetNormalTextureList() override;

		public:
			virtual void PlayAnimation(float deltaTime, bool continiousPlay) override;
			virtual void StopAnimation() override;
			virtual bool PlayAnimation(float speed, float deltaTime, int animIndex, bool continuousPlay) override;
			virtual bool PlayAnimation(float speed, float deltaTime, std::string animName, bool continuousPlay) override;
			virtual const std::vector<std::string>& GetClipNames() override;
			virtual void PauseAnimation() override;
			virtual void ReplayAnimation() override;
			virtual float GetCurrentFrame() override;
			virtual float GetMaxFrame() override;
			virtual bool GetIsPlaying() override;
			virtual const std::string& GetNowAnimationName() override;
			virtual void SetParentBone(GInterface::GraphicsRenderable* model, const std::string& boneName) override;
			virtual void DeleteParentBone() override;
			virtual DirectX::XMFLOAT4X4 GetTransformEffectedByBone() override;
			virtual DirectX::XMFLOAT4X4 GetBoneTransform(std::string boneName) override;

		public:
			virtual bool GetPickable() override;
			virtual void SetPickable(bool tf) override;

			virtual unsigned int GetHashID() override;

		public:
			virtual bool GetInsideFrustumState() override;

		private:
			void SetEffect();

		public:

		private:
			void SetHashValue(unsigned int index);

		private:
			void ConvertHashToRGBA(int hashValue);

		public:
			const DirectX::XMMATRIX GetWorldTransform();
			const std::string& GetFileName();

		public:
			virtual void SetMainCamera(ArkEngine::ICamera* camera) override;

		private:
			std::string _simpleModelName;
			std::string _fileName;
			std::string _effectName;
			std::vector<std::string> _diffuseTextureName;
			std::vector<std::string> _normalTextureName;
			std::vector<std::string> _emssiveTextureName;


			ID3DX11Effect* _effect;
			ID3DX11EffectTechnique* _tech;

			// cbPerObject
			ID3DX11EffectMatrixVariable* _fxWorld;
			ID3DX11EffectMatrixVariable* _fxWorldInvTranspose;
			ID3DX11EffectMatrixVariable* _fxWorldViewProj;
			ID3DX11EffectMatrixVariable* _fxTexTransform;
			ID3DX11EffectVariable* _fxMaterial;

			// cbSkinned
			ID3DX11EffectMatrixVariable* _fxBoneTransforms;

			ID3DX11EffectShaderResourceVariable* _diffuseMap;

			std::vector<ID3D11ShaderResourceView*> _diffuseMapSRV;

			ID3DX11EffectShaderResourceVariable* _normalMap;

			std::vector<ID3D11ShaderResourceView*> _normalMapSRV;

			ID3DX11EffectShaderResourceVariable* _emissionMap;

			std::vector<ID3D11ShaderResourceView*> _emissionMapSRV;

			ID3DX11EffectShaderResourceVariable* _cubeMap;

			ID3DX11EffectVectorVariable* _fxColor;

			std::vector<DirectX::XMFLOAT4X4> _boneTMList;

			DirectX::XMFLOAT4X4 _world;
			DirectX::XMFLOAT4X4 _view;
			DirectX::XMFLOAT4X4 _proj;

			std::vector<ID3D11Buffer*> _vertexBuffer;
			std::vector<ID3D11Buffer*> _indexBuffer;

		private:
			std::vector<ArkEngine::ArkDX11::Material> _material;

		private:
			ArkEngine::ArkDX11::ArkDevice* _arkDevice;
			ArkEngine::ArkDX11::ArkEffect* _arkEffect;
			UINT _totalVertexCount;
			UINT _totalIndexCount;
			ArkEngine::ArkDX11::Transform* _meshTransform;

		private:
			IDebugObject* _debugObject;

		private:
			ArkEngine::ICamera* _mainCamera;

		private:
			bool _havePlayedAnimation;
			bool _isAnimationPlaying;
			bool _isRendering;

			bool _isSolid;

		private:
			bool _isPickable;
			UINT _hashValue;
			unsigned int _objectIndex;

			float _color[4];
			bool _haveShadow;

			/// <summary>
			/// �����簡 �߰��� �͵�
			/// </summary>
		private:
			/// FBX �߰� ������
			void BuildGeometryBuffersFBX(std::wstring fileName);
			void BindCacheInfo();

			/// Materials
			void ReadMaterial(std::wstring fileName);

			// .mesh, .clip ��� 
			std::wstring _modelPath;
			// xml ���
			std::wstring _texturePath;

		private:
			std::vector<ModelMesh*> _meshes;

			// Material
			std::vector<ModelMaterial*> _materials;
		private:
			UINT _meshCount;

			UINT _boneIndexNum;

			std::vector<std::vector<Vertex>> _newVertexVector;
			std::vector<std::vector<unsigned int>> _newIndexVector;

			std::vector<Matrix> _boneTransforms;

			/// Animator
			std::unique_ptr<FBXAnimator> _animator;

		private:
			FBXMesh* _parentMesh;
			DirectX::XMFLOAT4X4 _parentBoneTrasnform;
			int _parentBoneIndex;
			DirectX::XMFLOAT4X4 _transformEffectedByParent;
		};
	}
}