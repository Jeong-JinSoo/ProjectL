#pragma once

#include <DirectXMath.h>
#include "CommonHeader.h"
#include "Component.h"
#include "Transform.h"
#include "GraphicsSystem.h"

namespace KunrealEngine
{
	class _DECLSPEC BoxCollider : public Component
	{
		friend class PhysicsSystem;
		friend class GameObject;
	private:
		BoxCollider();
	public:
		~BoxCollider();

		void Initialize() override;
		void Release() override;

		void FixedUpdate() override;
		void Update() override;
		void LateUpdate() override;

		void OnTriggerEnter() override;
		void OnTriggerStay() override;
		void OnTriggerExit() override;

		void SetActive(bool active) override;

	private:
		Transform* _transform;

		// public���� ����..
		// �����̴� ��ü���� ������ �ִ� ��ü����		// default�� false->�����̴� ��ü
		bool _isStatic;

		// collider ũ��
		DirectX::XMFLOAT3 _boxSize;

		// PhysX�� �Ѱ��� Transform
		DirectX::XMFLOAT3 _position;

		// ������Ʈ�� Transform���κ��� �󸶳� ���̰� �� ������
		DirectX::XMFLOAT3 _offset;

		// wireframe���� collider�� ũ�⸦ ǥ������
		GInterface::GraphicsDebug* _debugObject;

		// �浹�ߴ���
		bool _isCollided;

		// � ������Ʈ�� �浹�ߴ���
		GameObject* _targetObj;

	public:
		// �浹���� ��ȯ
		bool IsCollided();

		// �浹�� ����� ������Ʈ
		GameObject* GetTargetObject();

		// ������Ʈ�� Transform���κ��� �󸶳� ���̰� �� ������ ����
		void SetOffset(float x, float y, float z);

		// offset ��ġ�� ��ȯ	// default�� 0 0 0
		DirectX::XMFLOAT3 GetOffset();

		// Collider�� �ڽ� ũ�⸦ ����
		void SetBoxSize(float x, float y, float z);

		// Collider�� �ڽ� ũ�⸦ ��ȯ	// default�� 1 1 1
		DirectX::XMFLOAT3 GetBoxSize();

		// Collider�� ������ ��ȯ
		DirectX::XMFLOAT3 GetColliderPos();

		// Debug Object ����
		void SetDebugMeshData();

	public:
		// �������� �ʴ� ��ü�� ����
		void SetStatic();

		// �����̴� ��ü�� ����
		void SetDynamic();
	};
}


