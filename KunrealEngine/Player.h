#pragma once

#include <DirectXMath.h>
#include "CommonHeader.h"
#include "Component.h"
#include "Coroutine.h"

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
			DEAD,				// ���
			BEFORESTART,		// ���� ���� �� �����ִ� ����
		};

		// �÷��̾��� ���� ����
		struct PlayerInfo
		{
			float _hp;				// ü��
			float _maxhp;			// �ִ�ü��
			float _stamina;			// ���

			float _moveSpeed;		// �̵� �ӵ�
			float _dashSpeed;		// ��� �ӵ�
			float _dashRange;		// ��� �Ÿ�
			float _dashCooldown;	// ��� ��Ÿ��

			float _spellPower;		// ��ų ��� �� ������ ����
			float _damageReduce;	// ���� ����

			float _speedScale;		// ��ü���� �ӵ��� ����

			PlayerInfo(float hp, float maxhp, float stamina, float moveSpeed, float dashSpeed, float dashRange, float dashCooldown, float spellpower, float damageReduce, float speedScale)
				:_hp(hp), _maxhp(maxhp), _stamina(stamina), _moveSpeed(moveSpeed), _dashSpeed(dashSpeed), _dashRange(dashRange), _dashCooldown(dashCooldown), _spellPower(spellpower), _damageReduce(damageReduce), _speedScale(speedScale)
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
		GameObject* _owner;						// ������Ʈ�� ��� �ִ� ������Ʈ
		Status _playerStatus;					// �÷��̾��� ���� �̵� ���� ��
		Status _tempStatus;						// �ִϸ��̼� ��ȯ�ϸ� Stop �ҷ��� �뵵
		PlayerInfo _playerInfo;					// �÷��̾� �⺻ ���� ���� ��
		DirectX::XMVECTOR _directionVector;		// �÷��̾ �ٶ󺸴� ���� ����

		int _abilityAnimationIndex;				// � ��ų �ִϸ��̼�

		// �ǰ� �� ���ư��� ���� ����
		bool _isSweep;							// ���ư��� Ʈ����
		float _sweepRange;						// �󸶳� �ָ� ���ư����ΰ�
		float _movedRange;						// ��ŭ �̵��ߴ°� ��Ͽ�
		float _sweepDuration;					// �󸶳� ���� ü���� ���ΰ�
		float _sweepAnimationSpeed;				// ���ư��� �ִϸ��̼� �ӵ�
		float _gravity;							// �������� ������ �߷°��ӵ�
		std::vector<DirectX::XMFLOAT3> _sweepNode;	// ���ư��� ��ǥ���
		int _nodeCount;							// ��� üũ ��������

		GameObject* _deathParticle1;				// �÷��̾� ���� �� ��µ� ��ƼŬ
		GameObject* _deathParticle2;	
		GameObject* _deathParticle3;	
		GameObject* _deathParticle4;	
		GameObject* _deathParticle5;	
		GameObject* _deathParticle6;	

		std::vector<GameObject*> _deathParticleVector;	
		float _deathAnimationSpeed;

		float _playerStartX;					// ������ ���� x z ��ǥ
		float _playerStartZ;		

		int _knock_downSound;
		int _diedsound;
		int _staandup;
		int _hit;

	private:
		// �÷��̾��� ���¿� ���� �ִϸ��̼� ���
		void AnimateByStatus();

		// �÷��̾� ���ó��
		void CheckDeath();

		// �ǰ� ���� ó��
		void AfterHit();

		// ���� ���� �� �����ִ� �Ͼ��
		void BeforeStart();

		// �÷��̾� ���� �̻��� ó���� �ڷ�ƾ �Լ�
		Coroutine_Func(afterSweep)
		{
			auto* playerComp = this;
			Waitforsecond(playerComp->_sweepDuration + 1.0f);				// ���� ���� 1�� �� ���
			
			playerComp->_isSweep = false;
			playerComp->_playerStatus = Status::IDLE;
		};

		/// ����� ��忡���� ����� ���� �Լ�
		void DebugFunc();

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

		// �÷��̾� �ǰ� �� ���ư��� ��ǥ ���
		void CalculateSweep(DirectX::XMVECTOR direction);
		void CalculateSweep(DirectX::XMFLOAT3 direction);
		
		// �÷��̾� �ǰ� �� ���ư��� ����
		void PlayerSweep();

		// �÷��̾� scene �̵�
		void MoveToScene(std::string sceneName);

		// �÷��̾� ����, ü�� �ʱ�ȭ
		void ResetPlayerStatus();

		// �÷��̾ ��ų ���������� Flag
		bool _onCasting;
	};
}

