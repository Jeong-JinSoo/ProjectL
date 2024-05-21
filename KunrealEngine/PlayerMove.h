#pragma once
#include "KunrealAPI.h"
#include "Player.h"

namespace DirectX
{
	struct XMFLOAT3;
}

namespace KunrealEngine
{
	class _DECLSPEC PlayerMove : public Component
	{
	public:
		PlayerMove();
		~PlayerMove();

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
		Transform* _transform;					// �θ� ������Ʈ�� Transform
		DirectX::XMFLOAT3 _targetPos;			// �̵��� ��ġ
		Player* _playerComp;					// �÷��̾� ������Ʈ
		float _errorRange;						// �� ���� �������� �̳��� �����̸� �������� �Ϸ�� ������ �Ǵ�
		int _nodeCount;							// �̵��� ����� ��
		float _movedRange;						// ��÷� �̵��� �Ÿ� üũ�� ����
		float _posY;							// �ʸ��� �ٸ� ���̸� ������ ����

		// �̵��� ��ġ�� ���콺 ��Ŭ�� �� ������Ʈ
		void UpdateTargetPosition();

		// �̵� ��ɽ� ��� ������Ʈ
		void UpdateMoveNode();

		// ��� ��ɽ� ��� ������Ʈ
		void UpdateDashNode();

		// ����ϰ� �ִ� �����ΰ�
		bool _isDash;
		bool _isMoving;

		// �̵� �� ����ϴ� �ӽ� ������
		int _tempX;
		int _tempY;
		int _tempZ;

	public:
		// �÷��̾� �̵�
		void MoveToTarget(DirectX::XMFLOAT3 targetPos, float speed);

		// �÷��̾� ���
		void PlayerDash(DirectX::XMFLOAT3 targetPos, float speed);

		// �÷��̾��� Y ��ǥ�� ������Ű�� �Լ�
		void SetPlayerY(float y);
		float GetPlayerY();

		// Navigation�� �̿��� ������
		void NavigationMove(float speed);
		void NavigationDash(float speed);
		std::vector<std::pair<DirectX::XMFLOAT3, DirectX::XMFLOAT3>> _stopover;

		/// ������� ���� �Լ�
		void ShowPlayerInfo();
	};
}

