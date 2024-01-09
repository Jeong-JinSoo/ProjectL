#pragma once
#include <iostream>
#include <vector>
#include "../FbxLoader/Types.h"
#include "../FbxLoader/VertexDatas.h"
#include "Geometry.h"
#include "DirectXMath.h"

struct ModelBone
{
	std::wstring name = L"";
	int index = 0;
	int parentIndex = 0;
	std::shared_ptr<ModelBone> parent = nullptr;	// Cache

	Matrix transform = {};
	std::vector<std::shared_ptr<ModelBone>> children = {0};	// Cache

	Matrix boneOffsetTM = {};

	//DirectX::XMFLOAT4X4 OriginWorldTM;
	//DirectX::XMFLOAT4X4 LocalTM;
	//DirectX::XMFLOAT4X4 NewWorldTM;

	//DirectX::XMFLOAT4X4 OriginLocalTM;

	//DirectX::XMFLOAT4X4 SlerpTM;

};

struct ModelMesh
{
	void CreateBuffers();

	std::wstring name;

	// Mesh
	std::shared_ptr<Geometry<ModelVertexType>> _geometry = std::make_shared<Geometry<ModelVertexType>>();

	unsigned int vertexNum = 0;
	unsigned int indexNum = 0;

	std::vector<ModelVertexType> vertices;
	std::vector<unsigned int> indices;

	// Material 
	std::wstring materialName = L"";
	//std::shared_ptr<Material> material;	// Cache

	// Bones
	int boneIndex;
	std::shared_ptr<ModelBone> bone; // Cache

	bool isBone;

	// ���� ����
	std::shared_ptr<ModelMesh> parentMesh; // ���� �������� �θ� �޽�
	std::vector<std::shared_ptr<ModelMesh>> childMeshes; // ���� �������� �ڽ� �޽���
};


