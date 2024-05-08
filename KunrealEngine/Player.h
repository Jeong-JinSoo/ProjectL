#pragma once

#include <DirectXMath.h>
#include "CommonHeader.h"
#include "Component.h"

namespace KunrealEngine
{
	class Transform;

	class _DECLSPEC Player : public Component
	{
		friend class PlayerMove;
		friend class PlayerAbility;
	public:
		// �÷��̾��� ����
		enum class Status
		{
			IDLE,				// ����
			WALK,				// �̵�
			DASH,				// ���
			ABILITY,			// ��ų ���		// ��ų ������ ���� ���� ����
			STAGGERED,			// �÷��̾ ����ȭ �Ǿ��� ��		// �����̻� ��
			PARALYSIS,			// ����
			SWEEP,				// ���ư�����
			DEAD				// ���
		};

		// �÷��̾��� ���� ����
		struct PlayerInfo
		{
			float _hp;				// ü��
			float _stamina;			// ���

			float _moveSpeed;		// �̵� �ӵ�
			float _dashSpeed;		// ��� �ӵ�
			float _dashRange;		// ��� �Ÿ�
			float _dashCooldown;	// ��� ��Ÿ��

			float _spellPower;		// ��ų ��� �� ������ ����
			float _damageReduce;	// ���� ����

			float _speedScale;		// ��ü���� �ӵ��� ����

			PlayerInfo(float hp, float stamina, float moveSpeed, float dashSpeed, float dashRange, float dashCooldown, float spellpower, float damageReduce, float speedScale)
				:_hp(hp), _stamina(stamina), _moveSpeed(moveSpeed), _dashSpeed(dashSpeed), _dashRange(dashRange), _dashCooldown(dashCooldown), _spellPower(spellpower), _damageReduce(damageReduce), _speedScale(speedScale)
			{}
		};

	public:
		Player();
		~Player();

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
		Transform* _transform;					// �÷��̾��� transform
		Status _playerStatus;					// �÷��̾��� ���� �̵� ���� ��
		Status _tempStatus;						// �ִϸ��̼� ��ȯ�ϸ� Stop �ҷ��� �뵵
		PlayerInfo _playerInfo;					// �÷��̾� �⺻ ���� ���� ��
		DirectX::XMVECTOR _directionVector;		// �÷��̾ �ٶ󺸴� ���� ����

		int _abilityAnimationIndex;				// � ��ų �ִϸ��̼�

	private:
		// �÷��̾��� ���¿� ���� �ִϸ��̼� ���
		void AnimateByStatus();

		// �ǰ� ���� ó��
		void AfterHit();

	public:
		// �÷��̾ �ٶ󺸴� ���⺤��
		const DirectX::XMVECTOR GetDirectionVector();

		// �÷��̾��� ���� ��ȯ
		const Status GetPlayerStatus();

		// �÷��̾��� ���� ��ȯ
		PlayerInfo& GetPlayerData();

		// �÷��̾� �ǰ�ó��
		/// �����ʿ��� Ÿ���� ENUM���� �ٲٸ� �Ű������� ���濹��
		void SetHitState(int patternType);
	};
}

