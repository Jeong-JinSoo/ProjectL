#pragma once
#include "CommonHeader.h"
#include "KunrealAPI.h"

namespace KunrealEngine
{
	class EventManager;

	class BattleUIManager : public Component
	{
	public:
		BattleUIManager();
		~BattleUIManager();

		// Component�� �����Լ��� �������̵�
		void Initialize() override;
		void Release() override;

		void FixedUpdate() override;
		void Update() override;
		void LateUpdate() override;

		void OnTriggerEnter() override;
		void OnTriggerStay() override;
		void OnTriggerExit() override;

		void SetActive(bool active) override;

	public:
		void SetBossHpbar();
		void SetPlayerHpBar();

	private:
		KunrealEngine::GameObject* _battleuibox;	// �θ� �Ǿ��� ������Ʈ(Empty)

		KunrealEngine::GameObject* _ui_skill1;		// 1�� ��ų
		KunrealEngine::GameObject* _ui_skill1_cool;	// 1�� ��ų ��ٿ�
		KunrealEngine::GameObject* _ui_skill2;		// 2�� ��ų
		KunrealEngine::GameObject* _ui_skill2_cool;	// 2�� ��ų ��ٿ�
		KunrealEngine::GameObject* _ui_skill3;		// 3�� ��ų
		KunrealEngine::GameObject* _ui_skill3_cool;	// 3�� ��ų ��ٿ�
		KunrealEngine::GameObject* _ui_skill4;		// 4�� ��ų
		KunrealEngine::GameObject* _ui_skill4_cool;	// 4�� ��ų ��ٿ�

		KunrealEngine::GameObject* _potion;			// ���� �Ա�
		KunrealEngine::GameObject* _dash;			// �뽬(ȸ��?)

		KunrealEngine::GameObject* _playerhp_bar;		// �÷��̾� ü�¹�
		KunrealEngine::GameObject* _bosshp_bar;			// ���� ü�¹�

	private:
		EventManager* _eventmanager;
		const float _bosshpsize;
		const float _playerhpsize;
	};
}