#pragma once
#include "CommonHeader.h"
#include "Coroutine.h"
#include "ImageRenderer.h"

struct BossBasicInfo;

namespace KunrealEngine
{
	class GameObject;
	class Component;
	class PlayerAbility;
	class Ability;
	class Camera;
	
	class Player;
	class Kamen;

	class EventManager
	{
	private:
		EventManager();
		~EventManager();

	public:
		void Initialize();
		void Release();

		void Update();

		void SetCamera(std::string name);
		std::vector<DirectX::XMFLOAT2> CamShake(float radius, int numPoints);
		std::vector<float> CamShakeLinear(float sigma, int numPoints);
		void CamShake(float harder);

		// �̱���
	public:
		static EventManager& GetInstance()
		{
			static EventManager _instance;
			return _instance;
		}

	public:
		GameObject* _player;
		GameObject* _boss;
		GameObject* _mainCamera;

	public:
		Player* _playerComp;
		Kamen* _bossComp;


		PlayerAbility* _playerAbill;

		bool _eventStart;

		bool _iscamfollow;
		float _camshakex;
		float _camshakez;


		void CreateFadeObject();
	private:
		unsigned int _insideSafeCount;

		bool _fadeoutFlag;				// ȭ�� fadeout

		GameObject* _fadeObjectTitle;
		GameObject* _fadeObjectMain;

	public:
		// �÷��̾ �������� �ִ� ������ ���
		void CalculateDamageToBoss(Ability* abil);

	private:
		// ������ �÷��̾�� �ִ� ������ ���
		void CalculateDamageToPlayer();

		// ������ �÷��̾�� �ִ� ������ ���2
		void CalculateDamageToPlayer2();

	private:
		// ���������� �ٲ���� �� ������ �������� �˷��ִ� �Լ�
		void SetBossObject();

		// �ϴ� ����ٰ�... ��ġ �����
	private:
		const DirectX::XMVECTOR& SetWarningAttackDirection(GameObject* subObject);
		const DirectX::XMVECTOR& SetBossAttackDirection(GameObject* subObject);
		const DirectX::XMVECTOR& SetEgoAttackDirection(GameObject* subObject);

		_Coroutine(DashCamMove);

	public:
		// �÷��̾� ����� Ÿ��Ʋȭ������
		void MoveToTitleAfterDeath();

		// FadeOut 	
		void ActivateFadeOutTrigger();

		// FadeIn
		void ActivateFadeInTrigger();

	private:
		Coroutine_Func(fadeout)
		{
			EventManager* manager = this;
			GameObject* blackBG = manager->_fadeObjectMain;
			blackBG->SetActive(true);
			blackBG->GetComponent<ImageRenderer>()->SetAlpha(0.0f);

			while (blackBG->GetComponent<ImageRenderer>()->GetAlpha() < 1.0f)
			{
				blackBG->GetComponent<ImageRenderer>()->SetAlpha(blackBG->GetComponent<ImageRenderer>()->GetAlpha() + 0.005f);

				if (blackBG->GetComponent<ImageRenderer>()->GetAlpha() >= 1.0f)
				{
					blackBG->GetComponent<ImageRenderer>()->SetAlpha(1.0f);

					Waitforsecond(2.0f);
					blackBG->SetActive(false);
					manager->MoveToTitleAfterDeath();

					break;
				}

				Return_null;
			}		
		};

		Coroutine_Func(fadein)
		{
			EventManager* manager = this;
			GameObject* blackBG = manager->_fadeObjectTitle;
			blackBG->SetActive(true);
			blackBG->GetComponent<ImageRenderer>()->SetAlpha(1.0f);

			while (blackBG->GetComponent<ImageRenderer>()->GetAlpha() > 0.0f)
			{
				float pleaseman = blackBG->GetComponent<ImageRenderer>()->GetAlpha();

				blackBG->GetComponent<ImageRenderer>()->SetAlpha(blackBG->GetComponent<ImageRenderer>()->GetAlpha() - 0.005f);

				if (blackBG->GetComponent<ImageRenderer>()->GetAlpha() < 0.0f)
				{
					blackBG->GetComponent<ImageRenderer>()->SetAlpha(0.0f);
					blackBG->SetActive(false);

					break;
				}

				Return_null;
			}
		};
	};
}