/// <summary>
/// ��ũ�������� ����� ī�޶�
/// DirectXMath�� ������� ����
/// 
/// ����ȭ
/// </summary>

#pragma once
#include "ICamera.h"
#include "GraphicsCamera.h"

namespace DirectX
{
	struct XMFLOAT3;
	struct XMFLOAT4;
	struct XMFLOAT4X4;
	struct XMMATRIX;
}

namespace KunrealEngine
{
	namespace KunrealMath
	{
		struct Float2;
		struct Float3;
	}
}

namespace ArkEngine
{
	namespace ArkDX11
	{
		class Transform;

		class Camera : public ICamera, public GInterface::GraphicsCamera
		{
		public:
			Camera(const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& target, DirectX::XMFLOAT3& worldUp);
			~Camera();

		public:
			virtual bool GetMain() override;
			virtual void SetMain(bool tf) override;

		public:
			virtual void Delete() override;

		public:
			// ������� ����
			virtual void SetProjectionMatrix(float fieldOfView_Y, float aspect, float nearZ, float farZ, bool isPerspective) override;

			// ī�޶� �¿�� �̵�
			virtual void Strafe(float deltaTime) override;

			// ī�޶� �յڷ� �̵�
			virtual void Walk(float deltaTime) override;

			// ī�޶� ���Ʒ��� �̵�
			virtual void UpDown(float deltaTime) override;

		public:
			virtual void SetCameraPosition(KunrealEngine::KunrealMath::Float3 position) override;
			virtual void SetTargetPosition(KunrealEngine::KunrealMath::Float3 position) override;
			virtual void RotateCamera(KunrealEngine::KunrealMath::Float2 angle) override;


		public:
			// View ��� ��������
			const DirectX::XMMATRIX GetViewMatrix();

			// ���� ��� ��������
			const DirectX::XMMATRIX GetProjMatrix();

			// ī�޶��� ��ġ ��������
			const DirectX::XMFLOAT3 GetCameraPosition();

		private:
			// pos, target, worldUp�� ���� look, right, up ���͸� ���� �� view ��� ������Ʈ
			void LookAt(const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& target, DirectX::XMFLOAT3& worldUp);
			
			// ������ look, right, up ���͸� ����Ͽ� view ��� ������Ʈ
			void UpdateViewMatrix();
			
		private:
			DirectX::XMFLOAT3 _positionVector;
			DirectX::XMFLOAT3 _targetVector;

			DirectX::XMFLOAT3 _lookVector;
			DirectX::XMFLOAT3 _rightVector;
			DirectX::XMFLOAT3 _upVector;

			DirectX::XMFLOAT4X4 _viewMatrix;
			DirectX::XMFLOAT4X4 _projectionMatrix;

		private:
			bool _isMain;
		};
	}
}

