#pragma once

#include <DirectXMath.h>
#include "Component.h"

namespace GInterface
{
	class GraphicsCamera;
}

#pragma warning(disable: 4251)

namespace KunrealEngine
{
	class Transform;

	class _DECLSPEC Camera : public Component
	{
		friend class GameObject;
	private:
		Camera();
	public:
		~Camera();

		virtual void Initialize() override;
		virtual void Release() override;

		virtual void FixedUpdate() override;
		virtual void Update() override;
		virtual void LateUpdate() override;

		virtual void OnTriggerEnter() override;
		virtual void OnTriggerStay() override;
		virtual void OnTriggerExit() override;

		virtual void SetActive(bool active) override;

	private:
		// ī�޶� ��ü
		GInterface::GraphicsCamera* _camera;

		// �θ� ������Ʈ�� Ʈ������ ��ü
		Transform* _transform;

		// Ÿ�� ������
		DirectX::XMFLOAT3 _targetPosition;

		// �׷��Ƚ� ���� ī�޶��� strafe, walk���� �������� ���� ����
		DirectX::XMFLOAT3 _prevPosition;

		// �׷��Ƚ� ���� ī�޶��� Rotate�� ���� ����
        DirectX::XMFLOAT3 _prevRotation;

		// ī�޶� �����̴� ���
		// Ÿ���� ��� �ٶ� ���̳� �����Ǹ� ������ ���̳�
		bool _fixTarget;

	public:
		// ������Ʈ�� �����ϰ� �ִ� ī�޶� ��ȯ
		GInterface::GraphicsCamera* GetCamera();
		
		// ī�޶��� ��ġ ����
		void SetCameraPosition(float posX, float posY, float posZ);

		// ī�޶��� Ÿ�� ��ġ ����
		void SetTargetPosition(float posX, float posY, float posZ);

		// ī�޶��� ���� ��ȯ
		void RotateCamera(float pitch, float yaw);

		// ī�޶� �¿� �̵�
		void CameraStrafe(float deltaTime);

		// ī�޶� �յ� �̵�
		void CameraWalk(float deltaTime);

		// ī�޶� ���� �̵�
		void CameraUpDown(float deltaTime);

	public:
		/// ���� ��Ŀ� ���� ī�޶��� ������
		// Ÿ���� �������� ����
		void MoveCamera();

		// Ÿ���� ������
		void MoveFixedCamera();

		// Ÿ���� �������� �ʵ��� ����
		void UnfixCamera();

		// Ÿ���� �����ϵ��� ����
		// �̰� �����ϸ� �� ������Ʈ�� ����� Ÿ���� ���������� �ٶ󺸵��� ������
		// �ٸ� ��ġ�� �ٶ󺸰� �ϰ� ������ SetTargetPosition���� Ÿ�� �������� �缳�� �������
		void FixCamera();

	public:
		// �� ������Ʈ�� ����ִ� ī�޶� ���� ī�޶�� ������
		void SetMainCamera();

		/// ������ ī�޶� ������ �Լ� ��������������� �������
		void MoveToDebug();
	};
}

#pragma warning(default : 4251)
