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
#include "Particle.h"
#include "Transform.h"

namespace KunrealEngine
{
	class Boss;
	class Particle;

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
		bool _destroyArea;			// E �Ҹ� ������ ���� ����

		bool _isMeteorReady;		// R ��Ÿ�� ����

		/// BattleUIManager�� �Ѱ��� ��Ÿ�� üũ ������
		bool _isShotDetected;
		bool _isIceDetected;
		bool _isAreaDetected;
		bool _isMeteorDetected;

	private:
		Boss* _currentBoss;			// ���� ���� ����
		float _currentDamage;		// ������ ���� ������

	/// ��ƼŬ ��ġ�� �������
	private:
		GameObject* _shotParticle2;
		GameObject* _shotParticle3;
		GameObject* _shotParticle4;

		GameObject* _shotParticleHit1;
		GameObject* _shotParticleHit2;
		GameObject* _shotParticleHit3;

		GameObject* _iceParticle1;
		GameObject* _iceParticle2;
		GameObject* _iceParticle3;
		GameObject* _iceParticleHit1;
		GameObject* _iceParticleHit2;

		GameObject* _lazerParticle1;
		GameObject* _lazerParticle2;

		GameObject* _meteorParticle2;
		GameObject* _meteorParticle3;
		GameObject* _meteorParticle4;

		GameObject* _meteorParticleHit1;
		GameObject* _meteorParticleHit2;
		GameObject* _meteorParticleHit3;
		GameObject* _meteorParticleHit4;

	private:
		// q ��ų üũ�� ����
		bool _isShotEnded;
		float _shotParticleTimer;

		// w ��ų üũ�� ����
		bool _isIceEnded;
		float _iceParticleTimer;

		// e ��ų üũ�� ����
		bool _isLazerStarted;
		bool _isLazerEnded;
		float _lazerParticleTimer;


		// r ��ų üũ�� ����
		bool _isMeteorEnded;
		float _meteorParticleTimer;
		 
		// ��Ȱ��ȭ �Ǿ��� ���
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
			_iceParticle1->GetComponent<Particle>()->SetActive(false);
			_iceParticle2->GetComponent<Particle>()->SetActive(false);
			_iceParticle3->GetComponent<Particle>()->SetActive(false);
			_iceParticleHit1->GetComponent<Particle>()->SetActive(true);
			_iceParticleHit2->GetComponent<Particle>()->SetActive(true);
			_iceParticleHit1->SetActive(true);
			_iceParticleHit2->SetActive(true);
			ability->_destroyIce = true;
			ability->_isIceReady = false;
			Waitforsecond(0.5f);
			_iceParticleHit1->GetComponent<Particle>()->SetActive(false);
			_iceParticleHit2->GetComponent<Particle>()->SetActive(false);
			_iceParticleHit1->SetActive(false);
			_iceParticleHit2->SetActive(false);
		};

		// E��ų ��Ÿ��
		Coroutine_Func(AreaCoolDown)
		{
			auto* ability = this;
			Waitforsecond(ability->_abilityContainer[2]->_cooldown);
			ability->_isAreaReady = true;
		};

		// E��ų �ߵ� ���
		Coroutine_Func(AreaStandby)
		{
			auto* ability = this;
			Waitforsecond(0.8f);		// 2�� �� ����

			//ability->_destroyIce = false;	// �Ҹ� ���� �ʱ�ȭ
			ability->_area->SetActive(true);
			//ability->_area->GetComponent<Projectile>()->SetActive(true);
			//ability->_area->GetComponent<Projectile>()->ResetCondition();
			_lazerParticle1->SetActive(true);
			_lazerParticle2->SetActive(true);
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

