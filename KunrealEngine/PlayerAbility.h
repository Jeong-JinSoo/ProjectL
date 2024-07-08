#pragma once

#include "ToolBox.h"
#include "Component.h"
#include "GameObject.h"
#include "Player.h"
#include "Ability.h"
#include "Projectile.h"
#include "BoxCollider.h"
#include "Coroutine.h"
#include "Particle.h"
#include "Transform.h"
#include "TimeManager.h"

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
		GameObject* _laser;			// E ��ų ��ü
		GameObject* _meteor;		// R ��ų � ��ü

		bool _isShotHit;			// �� ���� ������ �޵���
		bool _isIceHit;
		bool _isLaserHit;
		bool _isMeteorHit;

		/// coroutine�� Ȱ���� Ÿ�̸� ������
		bool _isShotReady;			// Q ��Ÿ�� ����

		bool _isIceReady;			// W ��Ÿ�� ����
		bool _destroyIce;			// W �Ҹ� ������ ���� ����
		
		bool _isLaserReady;			// E ��ų ��Ÿ�� ����
		bool _destroyLaser;			// E �Ҹ� ������ ���� ����

		bool _isMeteorReady;		// R ��Ÿ�� ����

		/// BattleUIManager�� �Ѱ��� ��Ÿ�� üũ ������
		bool _isShotDetected;
		bool _isIceDetected;
		bool _isLaserDetected;
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

		GameObject* _laserParticle1;
		GameObject* _laserParticle2;
		GameObject* _laserParticle3;
		GameObject* _laserParticle4;

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
		bool _isLaserStarted;
		bool _isLaserEnded;
		float _laserParticleTimer;


		// r ��ų üũ�� ����
		bool _isMeteorEnded;
		float _meteorParticleTimer;
		 
		// ��Ȱ��ȭ �Ǿ��� ���
	private:
		void ResetShotPos();
		void CreateAbility1();

		void ResetIcePos();
		void CreateAbility2();
		
		void ResetLaserPos();
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
		Coroutine_Func(LaserCoolDown)
		{
			auto* ability = this;
			Waitforsecond(ability->_abilityContainer[2]->_cooldown);
			ability->_isLaserReady = true;
		};

		// E��ų �ߵ� ���
		Coroutine_Func(LaserStandby)
		{
			auto* ability = this;
			Waitforsecond(0.9f);		// 2�� �� ����

			ability->_laser->SetActive(true);
			_laserParticle1->SetActive(true);
			_laserParticle2->SetActive(true);
			//_laserParticle3->SetActive(true);
			_laserParticle4->SetActive(true);

			_laserParticle1->GetComponent<Particle>()->SetActive(true);
			_laserParticle2->GetComponent<Particle>()->SetActive(true);
			//_laserParticle3->GetComponent<Particle>()->SetActive(true);
			_laserParticle4->GetComponent<Particle>()->SetActive(true);
		};

		// E��ų �Ҹ�����
		Coroutine_Func(laserDestroy)
		{
			auto* ability = this;
			Waitforsecond(1.8f);

			float delta = 0;
			while (true)
			{
				delta += TimeManager::GetInstance().GetDeltaTime();
				ability->_laserParticle1->GetComponent<Particle>()->SetParticleSize((50 - (delta * 25)) * ToolBox::GetRandomFloat(0.8f, 1.0f), (50 - (delta * 25)) * ToolBox::GetRandomFloat(0.8f, 1.0f));
				ability->_laserParticle2->GetComponent<Particle>()->SetParticleSize((20 - (delta * 10)) * ToolBox::GetRandomFloat(0.8f, 1.0f), (20 - (delta * 10)) * ToolBox::GetRandomFloat(0.8f, 1.0f));
				ability->_laserParticle3->GetComponent<Particle>()->SetParticleSize((50 - (delta * 25)) * ToolBox::GetRandomFloat(0.8f, 1.0f), (50 - (delta * 25)) * ToolBox::GetRandomFloat(0.8f, 1.0f));
				ability->_laserParticle4->GetComponent<Particle>()->SetParticleSize((50 - (delta * 25)) * ToolBox::GetRandomFloat(0.8f, 1.0f), (50 - (delta * 25)) * ToolBox::GetRandomFloat(0.8f, 1.0f));

				if (delta > 2) break;
				Return_null;
			}
			ability->_laserParticle1->SetActive(false);
			ability->_laserParticle2->SetActive(false);
			ability->_laserParticle3->SetActive(false);
			ability->_laserParticle4->SetActive(false);
			ability->_destroyLaser = true;
			ability->_isLaserReady = false;
			ability->_isLaserStarted = false;

		};

		//E��ų ��� �� �ش� ũ��κ��� �پ�鵵��
		Coroutine_Func(LaserFadeOut)
		{
			auto* ability = this;
			Particle* laserP1 = ability->_laserParticle1->GetComponent<Particle>();
			Particle* laserP2 = ability->_laserParticle2->GetComponent<Particle>();
			Particle* laserP3 = ability->_laserParticle3->GetComponent<Particle>();
			Particle* laserP4 = ability->_laserParticle4->GetComponent<Particle>();

			float half1 = laserP1->GetParticleSize().x * 0.5f;
			float half2 = laserP2->GetParticleSize().x * 0.5f;
			float half3 = laserP3->GetParticleSize().x * 0.5f;
			float half4 = laserP4->GetParticleSize().x * 0.5f;

			float delta = 0;
			while (true)
			{
				/// �� �κ� ������� ���� �����
				delta += TimeManager::GetInstance().GetDeltaTime();

				if (laserP1->GetParticleSize().x > 0.0f)
				{
					laserP1->SetParticleSize((laserP1->GetParticleSize().x - (delta * half1)) * ToolBox::GetRandomFloat(0.8f, 1.0f), (laserP1->GetParticleSize().y - (delta * half1)) * ToolBox::GetRandomFloat(0.8f, 1.0f));
				}
				
				if (laserP2->GetParticleSize().x > 0.0f)
				{
					laserP2->SetParticleSize((laserP2->GetParticleSize().x - (delta * half2)) * ToolBox::GetRandomFloat(0.8f, 1.0f), (laserP2->GetParticleSize().y - (delta * half2)) * ToolBox::GetRandomFloat(0.8f, 1.0f));
				}
				
				if (laserP3->GetParticleSize().x > 0.0f)
				{
					laserP3->SetParticleSize((laserP3->GetParticleSize().x - (delta * half3)) * ToolBox::GetRandomFloat(0.8f, 1.0f), (laserP3->GetParticleSize().y - (delta * half3)) * ToolBox::GetRandomFloat(0.8f, 1.0f));
				}

				if (laserP4->GetParticleSize().x > 0.0f)
				{
					laserP4->SetParticleSize((laserP4->GetParticleSize().x - (delta * half4)) * ToolBox::GetRandomFloat(0.8f, 1.0f), (laserP4->GetParticleSize().x - (delta * half4)) * ToolBox::GetRandomFloat(0.8f, 1.0f));
				}

				if (delta > 2) break;
				Return_null;
			}
			ability->_laserParticle1->SetActive(false);
			ability->_laserParticle2->SetActive(false);
			ability->_laserParticle3->SetActive(false);
			ability->_laserParticle4->SetActive(false);
			ability->_destroyLaser = true;
			ability->_isLaserReady = false;
			ability->_isLaserStarted = false;
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

