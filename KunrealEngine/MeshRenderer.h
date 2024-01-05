#pragma once
#include "Component.h"
#include "GraphicsSystem.h"
#include "../KunrealMath/MathHeaders.h"


namespace GInterface
{
	class GraphicsRenderable;
	struct Material;
}

#pragma warning (disable: 4251)

namespace KunrealEngine
{
	class Transform;

	class _DECLSPEC MeshRenderer : public Component
	{
	public:
		MeshRenderer();
		~MeshRenderer();

	public:
		virtual void Initialize() override;
		virtual void Finalize() override;

		virtual void FixedUpdate() override;
		virtual void Update() override;
		virtual void LateUpdate() override;

		virtual void OnTriggerEnter() override;
		virtual void OnTriggerStay() override;
		virtual void OnTriggerExit() override;

		virtual void SetActive(bool active) override;

	private:
		GInterface::GraphicsRenderable* _mesh;
		Transform* _transform;

		// �׸��� �׸��� ���� ������
		// �����Ϳ� ��ġ�� ����ֱ� ���� �����͸� ����� ������ ����
	private:
		std::string _meshFileName;
		std::string _textureName;
		std::string _normalName;
		KunrealMath::Float4 _reflect;

	public:
		// ó�� ������Ʈ�� ������ �� �����������
		// �Ű������� mesh ���� �̸�, �ؽ�ó �̸� -> �ؽ�ó�� default �Ű������� nullptr ������ ����
		void SetMeshObject(const char* fileName, const char* textureName = nullptr);
		std::string GetMeshName();

		// �̹� mesh�� ���� ������ ���� �� ��ü�ϴ� �Լ�
		void ChangeMeshObject(const char* fileName);

		// �׸��� �׸� �� ���� ����
	private:
		void SetRenderingState(bool flag);

	public:
		bool GetRenderingState();

		// �ؽ�ó
		// ó�� ���� �� ���� �־������� ���ص� ��		//��ü�����ε� ��� ����
		void SetDiffuseTexture(const char* textureName);
		std::string GetTextureName();

		// �븻
		void SetNormalTexture(const char* textureName);
		std::string GetNormalName();

		// ���͸���
		void SetMaterial(GInterface::Material material);
		GInterface::Material GetMaterial();

		// ���÷�Ʈ		// �ݻ���
		void SetReflect(float x, float y, float z, float w);
		KunrealMath::Float4 GetReflect();

		// _mesh�� null�϶�, �� �ƹ��͵� �� ������� �� false �ƴϸ� true
		// �� ������Ʈ�� ó���� SetMeshObject�� �����ؼ� mesh ������ �־�����Ѵ�
		bool GetMeshStatus();
	};
}

#pragma warning (default: 4251)