#define NOMINMAX

#include <filesystem>
#include <algorithm>
#include "ResourceManager.h"
#include "ArkEffect.h"
#include "d3dx11effect.h"
#include "Camera.h"
#include "ArkDevice.h"
#include "ArkTexture.h"
#include "ArkBuffer.h"
#include "Transform.h"
#include "MathConverter.h"
#include "DirectionalLight.h"
#include "LightManager.h"
#include "GeometryGenerator.h"
#include "DebugObject.h"
#include "ModelAnimation.h"
#include "../FbxLoader/AssimpTool.h"
#include "../FbxLoader/FileUtils.h"
#include "../FbxLoader/Utils.h"
#include "FBXMesh.h"

ArkEngine::ArkDX11::FBXMesh::FBXMesh(const std::string& fileName, const std::string& textureName, bool isSolid)
	: _fileName(fileName), _effectName("Resources/FX/BasicTexDeferred.fx"), _textureName(textureName), _transform(),
	_effect(nullptr), _tech(nullptr),
	_fxWorld(nullptr), _fxWorldInvTranspose(nullptr), _fxWorldViewProj(nullptr), _fxTexTransform(nullptr), _fxMaterial(nullptr),
	_fxBoneTransforms(nullptr),
	_diffuseMap(nullptr), _diffuseMapSRV(nullptr), _normalMap(nullptr), _normalMapSRV(nullptr),
	_cubeMap(nullptr), _boneTMList(), _world(), _view(), _proj(), _vertexBuffer(nullptr), _indexBuffer(nullptr),
	_material(), _eyePosW(),
	_arkDevice(nullptr), _arkEffect(nullptr), _totalVertexCount(0), _totalIndexCount(0), _meshTransform(nullptr),
	_isStaticMesh(true), _isRendering(true), _isSolid(isSolid), _myIndex(0),
	_animTransforms(0), _bones(0), _animations(0),
	_newVertexVector(0), _newIndexVector(0), _boneTransforms(0),
	_currentTime(0.0f), _animationSpeed(0.0f), _utils(nullptr),
	_modelPath(L"Resources/Models/"), _texturePath(L"Resources/Textures/"),
	_invGlobal(), _anim(nullptr),
	_meshCount(0), _boneIndexNum(0),
	_debugObject(nullptr)

{
	Initialize();

	for (int i = 0; i < 96; i++)
	{
		_boneTMList.emplace_back();
	}
}


ArkEngine::ArkDX11::FBXMesh::~FBXMesh()
{
	Finalize();
}

void ArkEngine::ArkDX11::FBXMesh::Initialize()
{
	// ���ʿ��� ���� �� ������ �����ش�
	BuildGeometryBuffersFBX(_utils->ToWString(_fileName));

	ReadFBXAnimation(L"Kachujin/Idle");
	ReadFBXAnimation(L"Kachujin/Run");
	ReadFBXAnimation(L"Kachujin/Slash");


	if (_isStaticMesh == false)
	{
		_effectName = "Resources/FX/SkinningDeferred.fx";
	}

	_arkEffect = ResourceManager::GetInstance()->GetResource<ArkEngine::ArkDX11::ArkEffect>(_effectName);
	_effect = _arkEffect->GetEffect();

	_arkDevice = ResourceManager::GetInstance()->GetResource<ArkEngine::ArkDX11::ArkDevice>("Device");

	_meshTransform = new Transform();

	SetEffect();

	SetDiffuseTexture(_textureName.c_str());

	SetBasicMaterial();

	SetLight();

	_debugObject = new DebugObject(_fileName, DebugObject::eDebugType::Box);
}

void ArkEngine::ArkDX11::FBXMesh::Update(ArkEngine::ICamera* p_Camera)
{
	auto camera = static_cast<ArkEngine::ArkDX11::Camera*>(p_Camera);

	DirectX::XMStoreFloat4x4(&_world, DirectX::XMMatrixIdentity());
	DirectX::XMStoreFloat4x4(&_view, camera->GetViewMatrix());
	DirectX::XMStoreFloat4x4(&_proj, camera->GetProjMatrix());

	_eyePosW = DirectX::XMFLOAT3(camera->GetCameraPosition().x, camera->GetCameraPosition().y, camera->GetCameraPosition().z);

}

void ArkEngine::ArkDX11::FBXMesh::Render()
{
	auto deviceContext = _arkDevice->GetDeviceContext();

	deviceContext->IASetInputLayout(_arkEffect->GetInputLayOut());
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	if (_isSolid)
	{
		deviceContext->RSSetState(_arkDevice->GetSolidRS());
	}
	else
	{
		deviceContext->RSSetState(_arkDevice->GetWireRS());
	}

	UINT stride = sizeof(ArkEngine::ArkDX11::Vertex);
	UINT offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, &_vertexBuffer, &stride, &offset);
	deviceContext->IASetIndexBuffer(_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	DirectX::XMMATRIX texTransform = DirectX::XMMatrixIdentity();
	_fxTexTransform->SetMatrix(reinterpret_cast<float*>(&texTransform));

	_fxMaterial->SetRawValue(&_material, 0, sizeof(Material));

	_diffuseMap->SetResource(_diffuseMapSRV);

	_normalMap->SetResource(_normalMapSRV);


	auto nowCubeMap = ResourceManager::GetInstance()->GetNowCubeMap();

	if (nowCubeMap != nullptr)
	{
		auto cubeTexture = ResourceManager::GetInstance()->GetResource<ArkTexture>(nowCubeMap->GetTextureName());
		_cubeMap->SetResource(cubeTexture->GetDiffuseMapSRV());
	}



	D3DX11_TECHNIQUE_DESC techDesc;
	_tech->GetDesc(&techDesc);
	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		unsigned sumIndexCount = 0;
		unsigned sumVertexCount = 0;


		_tech->GetPassByIndex(p)->Apply(0, deviceContext);

		for (auto nowMesh : _meshes)
		{
			{
				DirectX::XMMATRIX world = XMLoadFloat4x4(&_world);
				world = XMMatrixMultiply(world, _meshTransform->GetTransformMatrix());

				DirectX::XMMATRIX view = XMLoadFloat4x4(&_view);
				DirectX::XMMATRIX proj = XMLoadFloat4x4(&_proj);
				DirectX::XMMATRIX WorldViewProj = world * view * proj;

				DirectX::XMMATRIX worldCopy = world;
				worldCopy.r[3] = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
				DirectX::XMMATRIX worldInv = XMMatrixInverse(nullptr, worldCopy);
				DirectX::XMMATRIX worldInvTranspose = DirectX::XMMatrixTranspose(worldInv);

				_fxWorld->SetMatrix(reinterpret_cast<float*>(&world));
				_fxWorldInvTranspose->SetMatrix(reinterpret_cast<float*>(&worldInvTranspose));
				_fxWorldViewProj->SetMatrix(reinterpret_cast<float*>(&WorldViewProj));

				static float testNum = 0.0f;
				static float slowman = 0.0f;
				/// <summary>
				/// �������� mesh �ؾ���
				/// </summary>
					/// Animation �κ�
				if (_isStaticMesh == false)
				{
					UpdateAnimationTransforms(2, testNum);

					_fxBoneTransforms->SetMatrixArray(reinterpret_cast<float*>(&_boneTMList[0]), 0, static_cast<uint32_t>(_boneTMList.size()));
					//testNum++;

					slowman++;
					if (slowman > 5.f)
					{
						//testNum++;
						slowman = 0.f;
					}

					if (testNum > _anim->duration)
					{
						testNum = 0.f;
					}

				}

				deviceContext->DrawIndexed(nowMesh->indexNum, sumIndexCount, sumVertexCount);

				sumVertexCount += nowMesh->vertexNum;
				sumIndexCount += nowMesh->indexNum;
			}
		}
	}
}

void ArkEngine::ArkDX11::FBXMesh::Finalize()
{
	delete _debugObject;

	_indexBufferSize.clear();

	for (auto index : _bones)
	{
		index.reset();
	}

	for (auto ani : _animations)
	{
		ani.reset();
	}

	_tech = nullptr;
	_effect = nullptr;

	_vertexBuffer = nullptr;
	_indexBuffer = nullptr;

	_boneTMList.clear();

	_normalMapSRV = nullptr;
	_diffuseMapSRV = nullptr;

	_normalMap->Release();
	_diffuseMap->Release();

	_fxMaterial->Release();
	_fxTexTransform->Release();
	_fxWorldViewProj->Release();
	_fxWorldInvTranspose->Release();
	_fxWorld->Release();

	delete _meshTransform;

	_arkEffect = nullptr;
	_arkDevice = nullptr;

	if (_isStaticMesh == false)
	{
		_fxBoneTransforms->Release();
	}
}

void ArkEngine::ArkDX11::FBXMesh::Delete()
{
	ResourceManager::GetInstance()->DeleteRenderable(this);
}

bool ArkEngine::ArkDX11::FBXMesh::GetRenderingState()
{
	return _isRendering;
}

void ArkEngine::ArkDX11::FBXMesh::SetRenderingState(bool tf)
{
	_isRendering = tf;

	_debugObject->SetRenderingState(tf);
}

void ArkEngine::ArkDX11::FBXMesh::SetTransform(KunrealEngine::KunrealMath::Matrix4x4 matrix)
{
	DirectX::XMFLOAT4X4 transformMat = ArkEngine::ArkDX11::ConvertMatrix4x4(matrix);
	_meshTransform->SetTransformMatrix(transformMat);

	_debugObject->SetTransformMatrix(transformMat);
}

void ArkEngine::ArkDX11::FBXMesh::SetPosition(float x, float y, float z)
{
	_meshTransform->SetTranslationMatrix(x, y, z);

	_debugObject->SetPosition(x, y, z);
}

void ArkEngine::ArkDX11::FBXMesh::SetRotation(float x, float y, float z)
{
	_meshTransform->SetRotationMatrix(x, y, z);

	_debugObject->SetRotation(x, y, z);
}

void ArkEngine::ArkDX11::FBXMesh::SetScale(float x, float y, float z)
{
	_meshTransform->SetScaleMatrix(x, y, z);

	_debugObject->SetScale(x, y, z);
}

void ArkEngine::ArkDX11::FBXMesh::SetModel(const char* fileName)
{

}

void ArkEngine::ArkDX11::FBXMesh::SetDiffuseTexture(const char* textureName)
{
	auto texture = ResourceManager::GetInstance()->GetResource<ArkTexture>(textureName);

	if (texture != nullptr)
	{
		_diffuseMapSRV = texture->GetDiffuseMapSRV();
		_tech = _effect->GetTechniqueByName("LightTexReflect");
	}
	else
	{
		texture = nullptr;
		_diffuseMapSRV = nullptr;
	}
}

void ArkEngine::ArkDX11::FBXMesh::SetNormalTexture(const char* textureName)
{
	auto texture = ResourceManager::GetInstance()->GetResource<ArkTexture>(textureName);

	if (texture != nullptr)
	{
		_normalMapSRV = texture->GetDiffuseMapSRV();
	}
	else
	{
		texture = nullptr;
		_normalMapSRV = nullptr;
	}
}

void ArkEngine::ArkDX11::FBXMesh::SetAnimator()
{

}

const GInterface::Material ArkEngine::ArkDX11::FBXMesh::GetMaterial()
{
	GInterface::Material material;

	material.ambient.x = _material.ambient.x;
	material.ambient.y = _material.ambient.y;
	material.ambient.z = _material.ambient.z;
	material.ambient.w = _material.ambient.w;

	material.diffuse.x = _material.diffuse.x;
	material.diffuse.y = _material.diffuse.y;
	material.diffuse.z = _material.diffuse.z;
	material.diffuse.w = _material.diffuse.w;

	material.specular.x = _material.specular.x;
	material.specular.y = _material.specular.y;
	material.specular.z = _material.specular.z;
	material.specular.w = _material.specular.w;

	material.reflect.x = _material.reflect.x;
	material.reflect.y = _material.reflect.y;
	material.reflect.z = _material.reflect.z;
	material.reflect.w = _material.reflect.w;

	return material;
}

void ArkEngine::ArkDX11::FBXMesh::SetMaterial(GInterface::Material material)
{
	_material.ambient = ArkEngine::ArkDX11::ConvertFloat4(material.ambient);
	_material.diffuse = ArkEngine::ArkDX11::ConvertFloat4(material.diffuse);
	_material.diffuse = ArkEngine::ArkDX11::ConvertFloat4(material.diffuse);
	_material.reflect = ArkEngine::ArkDX11::ConvertFloat4(material.reflect);
}

void ArkEngine::ArkDX11::FBXMesh::SetReflect(KunrealEngine::KunrealMath::Float4 reflect)
{
	_material.reflect = ArkEngine::ArkDX11::ConvertFloat4(reflect);
}

void ArkEngine::ArkDX11::FBXMesh::PlayAnimation(float deltaTime, bool continiousPlay)
{
}

void ArkEngine::ArkDX11::FBXMesh::StopAnimation()
{
}

void ArkEngine::ArkDX11::FBXMesh::PlayFBXAnimation(float speed, int animIndex, bool continuousPlay)
{

}

void ArkEngine::ArkDX11::FBXMesh::SetEffect()
{
	_tech = _effect->GetTechniqueByIndex(0);

	if (_isStaticMesh == false)
	{
		_fxBoneTransforms = _effect->GetVariableByName("gBoneTransforms")->AsMatrix();
	}

	_fxWorld = _effect->GetVariableByName("gWorld")->AsMatrix();
	_fxWorldInvTranspose = _effect->GetVariableByName("gWorldInvTranspose")->AsMatrix();
	_fxWorldViewProj = _effect->GetVariableByName("gWorldViewProj")->AsMatrix();
	_fxTexTransform = _effect->GetVariableByName("gTexTransform")->AsMatrix();
	_fxMaterial = _effect->GetVariableByName("gMaterial");

	_diffuseMap = _effect->GetVariableByName("gDiffuseMap")->AsShaderResource();
	_normalMap = _effect->GetVariableByName("gNormalMap")->AsShaderResource();

	_cubeMap = _effect->GetVariableByName("gCubeMap")->AsShaderResource();
}

void ArkEngine::ArkDX11::FBXMesh::SetBasicMaterial()
{
	_material.ambient = { 1.0f, 1.0f, 1.0f, 1.0f };
	_material.diffuse = { 0.3f, 0.3f, 0.3f, 0.3f };
	_material.specular = { 0.3f, 0.3f, 0.3f, 0.3f };
}

void ArkEngine::ArkDX11::FBXMesh::SetLight()
{

}

/// <summary>
/// FBX�� �߰��Ѱ�
/// </summary>
void ArkEngine::ArkDX11::FBXMesh::BuildGeometryBuffersFBX(std::wstring fileName)
{
	auto buffer = ResourceManager::GetInstance()->GetResource<ArkBuffer>(_fileName);

	DirectX::XMFLOAT3 minPos = { FLT_MAX, FLT_MAX, FLT_MAX };
	DirectX::XMFLOAT3 maxPos = { -FLT_MAX, -FLT_MAX, -FLT_MAX };

	if (buffer == nullptr)
	{
		//std::vector<ModelVertexType> vertices;
		//std::vector<unsigned int> indices;

		std::wstring fullPath = _modelPath + fileName + L".mesh";

		std::shared_ptr<ArkEngine::FileUtils> file = std::make_shared<ArkEngine::FileUtils>();

		_utils = std::make_unique<ArkEngine::Utils>();

		std::shared_ptr<Geometry<ModelVertexType>> geometry = std::make_shared<Geometry<ModelVertexType>>();
		file->Open(fullPath, FileMode::Read);

		//Bones
		{
			const UINT count = file->Read<UINT>();

			for (UINT i = 0; i < count; i++)
			{
				_root = std::make_shared<ModelBone>();
				_root->index = file->Read<int>();
				_root->name = _utils->ToWString(file->Read<std::string>());
				_root->parentIndex = file->Read<int>();
				_root->transform = file->Read<Matrix>();

				// offsetTM �߰� ������ �� ���� �����Ѵ�
				_root->boneOffsetTM = file->Read<Matrix>();

				_bones.push_back(_root);

				_boneTransforms.push_back(_root->transform);

			}
		}




		// Mesh
		{
			_meshCount = file->Read<UINT>();

			for (UINT i = 0; i < _meshCount; i++)
			{
				_modelMesh = std::make_shared<ModelMesh>();

				_modelMesh->name = _utils->ToWString(file->Read<std::string>());
				_modelMesh->boneIndex = file->Read<UINT>();

				// Material
				_modelMesh->materialName = _utils->ToWString(file->Read<std::string>());

				/// Vertex Data
				_modelMesh->vertexNum = file->Read<UINT>();

				// vertex�� ������ ���� vector�� ũ�⸦ ����
				_modelMesh->vertices.resize(_modelMesh->vertexNum);

				// �����̳ʿ� ��� ù��° ��Ҹ� ����Ŵ
				void* vertexData = _modelMesh->vertices.data();

				// �����͵��� �о�´�
				file->Read(&vertexData, sizeof(ModelVertexType) * _modelMesh->vertexNum);

				// vertices�� ���Ե� ��ҵ��� _vertices����� �ٸ� ���Ϳ� �߰��ϴ� ������ �Ѵ�
				geometry->AddVertices(_modelMesh->vertices);


				/// Index Data
				_modelMesh->indexNum = file->Read<UINT>();

				_modelMesh->indices.resize(_modelMesh->indexNum);

				void* indexData = _modelMesh->indices.data();
				file->Read(&indexData, sizeof(UINT) * _modelMesh->indexNum);

				geometry->AddIndices(_modelMesh->indices);

				/// �ϴ� buffer �ߺ��ǰ� ������ �ʱ� ���� �ӽ÷� �̷��� �س��Ҵµ� 
				/// ���� _mesh�� ������ ��� ����ִ� ������ ��ȿ����
				/// ���ҽ� �Ŵ����� �־���� ���� ������ �ؾ� ��
				if (buffer == nullptr)
				{
					// FBXLoader���� ����ϴ� ����ü�� ��ȭ�׷��Ƚ������� ����ü�� �°� ���� �־� �ش�
					for (UINT j = 0; j < _modelMesh->vertexNum; j++)
					{
						_newVertexVector.emplace_back();
						_newVertexVector.back().pos = _modelMesh->vertices[j].position;

						_newVertexVector.back().norm.x = _modelMesh->vertices[j].normal.x;
						_newVertexVector.back().norm.y = _modelMesh->vertices[j].normal.y;
						_newVertexVector.back().norm.z = _modelMesh->vertices[j].normal.z;

						_newVertexVector.back().tex.x = _modelMesh->vertices[j].uv.x;
						_newVertexVector.back().tex.y = -1 * _modelMesh->vertices[j].uv.y;

						_newVertexVector.back().tangent.x = _modelMesh->vertices[j].tangent.x;
						_newVertexVector.back().tangent.y = _modelMesh->vertices[j].tangent.y;
						_newVertexVector.back().tangent.z = _modelMesh->vertices[j].tangent.z;

						_newVertexVector.back().weight = _modelMesh->vertices[j].weight;
						_newVertexVector.back().boneIndicle = _modelMesh->vertices[j].boneIndices;
					}

					for (UINT j = 0; j < _modelMesh->indexNum; j++)
					{
						_newIndexVector.emplace_back();
						_newIndexVector.back() = _modelMesh->indices[j];
					}
				}

				_meshes.push_back(_modelMesh);


				_meshes[i]->vertexNum = _modelMesh->vertexNum;
				_meshes[i]->indexNum = _modelMesh->indexNum;

				_totalVertexCount += _modelMesh->vertexNum;
				_totalIndexCount += _modelMesh->indexNum;

				// �ּ�/�ִ� ��ġ ã��
				for (const auto& vertex : _modelMesh->vertices)
				{
					minPos.x = std::min(minPos.x, vertex.position.x);
					minPos.y = std::min(minPos.y, vertex.position.y);
					minPos.z = std::min(minPos.z, vertex.position.z);

					maxPos.x = std::max(maxPos.x, vertex.position.x);
					maxPos.y = std::max(maxPos.y, vertex.position.y);
					maxPos.z = std::max(maxPos.z, vertex.position.z);
				}
			}


			// ����ȯ
			std::string charFileName = _utils->ToString(fileName);

			// Buffer ����
			if (buffer == nullptr)
			{
				ArkBuffer* newBuffer = new ArkBuffer(charFileName.c_str(), _totalVertexCount, _newVertexVector, _totalIndexCount, _newIndexVector);
				newBuffer->SetSize(minPos, maxPos);
			}

			buffer = ResourceManager::GetInstance()->GetResource<ArkBuffer>(charFileName);

			_vertexBuffer = buffer->GetVertexBuffer();
			_indexBuffer = buffer->GetIndexBuffer();
		}

		if (_modelMesh->vertices[0].boneIndices.x != 0)
		{
			_isStaticMesh = false;
		}

		BindCacheInfo();
	}
	else
	{
		GetMeshByName(fileName);
		_vertexBuffer = buffer->GetVertexBuffer();
		_indexBuffer = buffer->GetIndexBuffer();
	}
}

void ArkEngine::ArkDX11::FBXMesh::UpdateAnimationTransforms(unsigned int animationIndex, float frameCount)
{
	// Ư�� �ִϸ��̼� �ε����� ���� ���� ��ȯ ����� ���
	std::vector<Matrix> tempAnimBoneTransforms(250, Matrix::Identity); // �ӽ� �ִϸ��̼� ���� ��� �迭

	//std::shared_ptr<ModelAnimation> animation = GetAnimationByIndex(animationIndex);
	_anim = GetAnimationByIndex(animationIndex);

	//_animations = GetAnimationByIndex(animationIndex);
	if (_animTransforms.size() <= animationIndex)
	{
		_animTransforms.resize(animationIndex + 1);
	}

	_anim->frameCount;
	/// �ִϸ��̼� �����Ӻ��� ���� ��ȯ ��� ���
		// ��
	for (unsigned int b = 0; b < GetBoneCount(); b++)
	{
		_modelMesh->bone = GetBoneByIndex(b);

		/// ���ʺ��� ������ ���� �������°� �س��� �Ѵ�
		// �߿�κ�
		// T����
		// toRootMatrix : ���� ������ ��Ʈ �������� ��ȯ���
		// boneOffsetInverse
		Matrix toRootMatrix = _modelMesh->bone->transform;


		// invGloval : ��Ʈ ������ ����������� �����
		// Gloval -> Relative
		//Matrix invGlobal = toRootMatrix.Invert();
		// offsetTM
		_invGlobal = toRootMatrix.Invert();



		// ���� ���� �θ� ���� ���� �ε���
		int parentIndex = _modelMesh->bone->parentIndex;

		// Animation
		// matParent : �θ� ���� ��ȯ ���
		Matrix matParent = Matrix::Identity;
		if (parentIndex >= 0)
			matParent = tempAnimBoneTransforms[parentIndex];

		Matrix matAnimation;

		if (b == 1)
		{
			_boneTMList[1] = toRootMatrix.Invert() * matParent;
			int test = 0;
		}

		frame = _anim->GetKeyframe(_modelMesh->bone->name);
		//std::shared_ptr<ModelKeyframe> frame = animation->GetKeyframe(_root->name);

		if (frame != nullptr)
		{
			// Ư�� ���� Ư�� �����ӿ� ���� SRT�� ������
			ModelKeyframeData& data = frame->transforms[frameCount];

			Matrix S = Matrix::CreateScale(data.scale.x, data.scale.y, data.scale.z);
			Matrix R = Matrix::CreateFromQuaternion(data.rotation);
			Matrix T = Matrix::CreateTranslation(data.translation.x, data.translation.y, data.translation.z);


			// SRT ����� ��ģ �ִϸ��̼� ��� ���
			// BoneTransform
			matAnimation = S * R * T;
		}
		else
		{
			// �����ӿ� �ش��ϴ� Ű�������� ������ ���� ��� ���
			matAnimation = Matrix::Identity;
		}

		// matAnimation : ���� �����ӿ����� �ִϸ��̼ǿ� ���� ��ȯ ���
		// tempAnimBoneTransforms : ���� ���� ���� �ִϸ��̼��� ������ ��ȯ ����� ����Ͽ� ����
		// ���뿡�� �۷ι��� ������ ��ȯ���
		tempAnimBoneTransforms[b] = matAnimation * matParent;

		// ���
		// _animTransforms[index].transforms[f][b] : �� �ִϸ��̼� ��� �迭�� ���� ���� ���� ���� ��ȯ ����� ����
		// ���������δ� ��Ʈ ������ ���� �������� ���� ��ȯ ����� _animTransforms�� �����Ѵ�
		_animTransforms[animationIndex]._transforms[frameCount][b] = _invGlobal * tempAnimBoneTransforms[b];

		DirectX::XMMATRIX finalBone = DirectX::XMMatrixMultiply(_invGlobal, tempAnimBoneTransforms[b]);
		if (_boneTMList.size() < GetBoneCount())
		{
			DirectX::XMFLOAT4X4 boneTM;
			DirectX::XMStoreFloat4x4(&boneTM, finalBone);

			_boneTMList.emplace_back(boneTM);
		}
		else
		{
			DirectX::XMStoreFloat4x4(&_boneTMList[b], finalBone);
		}
	}
}


void ArkEngine::ArkDX11::FBXMesh::ReadFBXAnimation(std::wstring fileName)
{
	std::wstring fullPath = _modelPath + fileName + L".clip";

	std::shared_ptr<FileUtils> file = std::make_shared<FileUtils>();
	file->Open(fullPath, FileMode::Read);

	//std::shared_ptr<ModelAnimation> _anim = std::make_shared<ModelAnimation>();
	_anim = std::make_shared<ModelAnimation>();

	_anim->name = _utils->ToWString(file->Read<std::string>());
	_anim->duration = file->Read<float>();
	_anim->frameRate = file->Read<float>();
	_anim->frameCount = file->Read<unsigned int>();

	unsigned int keyframesCount = file->Read<unsigned int>();

	for (unsigned int i = 0; i < keyframesCount; i++)
	{
		std::shared_ptr<ModelKeyframe> keyframe = std::make_shared<ModelKeyframe>();
		keyframe->boneName = _utils->ToWString(file->Read<std::string>());

		unsigned int size = file->Read<unsigned int>();

		if (size > 0)
		{
			keyframe->transforms.resize(size);
			void* ptr = &keyframe->transforms[0];
			file->Read(&ptr, sizeof(ModelKeyframeData) * size);
		}

		_anim->keyframes[keyframe->boneName] = keyframe;
	}
	_animations.push_back(_anim);
}

unsigned int ArkEngine::ArkDX11::FBXMesh::GetAnimationCount()
{
	return _animations.size();
}

std::vector<std::shared_ptr<ModelAnimation>>& ArkEngine::ArkDX11::FBXMesh::GetAnimations()
{
	return _animations;
}

std::shared_ptr<ModelAnimation> ArkEngine::ArkDX11::FBXMesh::GetAnimationByIndex(unsigned int index)
{
	// �ε����� ��ȿ�� ���� ���� �ִ��� Ȯ���մϴ�.
	// ���� �ε����� ��ȿ���� ������ nullptr�� ��ȯ�մϴ�.
	return (index < 0 || index >= _animations.size()) ? nullptr : _animations[index];
}

std::shared_ptr<ModelAnimation> ArkEngine::ArkDX11::FBXMesh::GetAnimationByName(std::wstring name)
{
	for (auto& animation : _animations)
	{
		if (animation->name == name)
			return animation;
	}

	return nullptr;
}

std::shared_ptr<ModelMesh> ArkEngine::ArkDX11::FBXMesh::GetMeshByIndex(unsigned int index)
{
	// �ε����� ��ȿ�� ���� ���� �ִ��� Ȯ���մϴ�.
	// ���� �ε����� ��ȿ���� ������ nullptr�� ��ȯ�մϴ�.
	return (index < 0 || index >= _meshes.size()) ? nullptr : _meshes[index];
}

std::shared_ptr<ModelMesh> ArkEngine::ArkDX11::FBXMesh::GetMeshByName(std::wstring name)
{
	for (auto& mesh : _meshes)
	{
		if (mesh->name == name)
			return mesh;
	}

	return nullptr;
}

void ArkEngine::ArkDX11::FBXMesh::BindCacheInfo()
{
	// Mesh�� Material ĳ��
	//for (const auto& mesh : _meshes)
	//{
	//	// �̹� ã������ ��ŵ
	//	if (mesh->material != nullptr)
	//		continue;

	//	mesh->material = GetMaterialByName(mesh->materialName);
	//}

	// Mesh�� Bone ĳ��
	for (const auto& mesh : _meshes)
	{
		// �̹� ã������ ��ŵ
		if (mesh->bone != nullptr)
			continue;

		mesh->bone = GetBoneByIndex(mesh->boneIndex);
	}

	// Bone ���� ���� ä���
	if (_modelMesh->bone == nullptr && _bones.size() > 0)
	{
		_modelMesh->bone = _bones[0];

		for (const auto& bone : _bones)
		{
			if (bone->parentIndex >= 0)
			{
				bone->parent = _bones[bone->parentIndex];
				bone->parent->children.push_back(bone);
			}
			else
			{
				bone->parent = nullptr;
			}
		}
	}
}

