#pragma once

#include <DirectXMath.h>
#include "CommonHeader.h"
#include "Component.h"
#include "GameObject.h"
#include "Player.h"
#include "Ability.h"
#include "Projectile.h"
#include "BoxCollider.h"
#include "Coroutine.h"

namespace KunrealEngine
{
	class Boss;

	class _DECLSPEC PlayerAbility : public Component
	{
		friend class Player;
		friend class BattleUIManager;
	public:
		PlayerAbility();
		~PlayerAbility();

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
		std::vector<Ability*> _abilityContainer;
		Player* _playerComp;

		GameObject* _shot;			// Q ��ų ����ü ��ü
		GameObject* _ice;			// W ��ų ��ü
		GameObject* _area;			// E ��ų ��ü
		GameObject* _meteor;		// R ��ų � ��ü

		bool _isShotHit;			// �� ���� ������ �޵���
		bool _isIceHit;
		bool _isAreaHit;
		bool _isMeteorHit;

		/// coroutine�� Ȱ���� Ÿ�̸� ������
		bool _isShotReady;			// Q ��Ÿ�� ����

		bool _isIceReady;			// W ��Ÿ�� ����
		bool _destroyIce;			// W �Ҹ� ������ ���� ����
		
		bool _isAreaReady;			// E ��ų ��Ÿ�� ����

		bool _isMeteorReady;		// R ��Ÿ�� ����

		/// BattleUIManager�� �Ѱ��� ��Ÿ�� üũ ������
		bool _isShotDetected;
		bool _isIceDetected;
		bool _isAreaDetected;
		bool _isMeteorDetected;

	private:
		Boss* _currentBoss;			// ���� ���� ����
		float _currentDamage;		// ������ ���� ������

	private:
		void ResetShotPos();
		void CreateAbility1();

		void ResetIcePos();
		void CreateAbility2();
		
		void ResetAreaPos();
		void CreateAbility3();

		void ResetMeteorPos();
		void CreateAbility4();

	public:
		void AddToContanier(Ability* abil);

		// ���� �ٲ���� �� ���� ������
		void SetCurrentBossObject();

		float GetDamage();

	private:
		/// �ڷ�ƾ Ÿ�̸� �Լ���

		// Q��ų ��Ÿ��
		Coroutine_Func(shotCoolDown)
		{
			auto* ability = this;
			Waitforsecond(ability->_abilityContainer[0]->_cooldown);
			ability->_isShotReady = true;
		};

		// W��ų ��Ÿ�� 
		Coroutine_Func(iceCoolDown)
		{
			auto* ability = this;
			Waitforsecond(ability->_abilityContainer[1]->_cooldown);
			ability->_isIceReady = true;
		};

		// W��ų �ߵ����
		Coroutine_Func(iceStandby)
		{
			auto* ability = this;
			Waitforsecond(0.8f);		// 2�� �� ����

			ability->_destroyIce = false;	// �Ҹ� ���� �ʱ�ȭ
			ability->_ice->SetActive(true);
			ability->_ice->GetComponent<Projectile>()->SetActive(true);
			ability->_ice->GetComponent<Projectile>()->ResetCondition();
		};

		// W��ų �Ҹ�
		Coroutine_Func(iceDestroy)
		{
			auto* ability = this;
			Waitforsecond(3.0f);
			ability->_destroyIce = true;
			ability->_isIceReady = false;
		};

		// E��ų ��Ÿ��
		Coroutine_Func(AreaCoolDown)
		{
			auto* ability = this;
			Waitforsecond(ability->_abilityContainer[2]->_cooldown);
			ability->_isAreaReady = true;
		};

		// R��ų ��Ÿ��
		Coroutine_Func(meteorCoolDown)
		{
			auto* ability = this;
			Waitforsecond(ability->_abilityContainer[3]->_cooldown);
			ability->_isMeteorReady = true;
		};
	};
}

