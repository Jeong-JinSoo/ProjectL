#pragma once
#include "CommonHeader.h"

struct BossBasicInfo;

namespace KunrealEngine
{
	class GameObject;
	class Component;
	class PlayerAbility;
	
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

		// �̱���
	public:
		static EventManager& GetInstance()
		{
			static EventManager _instance;
			return _instance;
		}

	private:
		GameObject* _player;
		GameObject* _boss;

	private:
		Player* _playerComp;
		Kamen* _bossComp;

		PlayerAbility* _playerAbill;

		bool _eventStart;

	private:
		// �÷��̾ �������� �ִ� ������ ���
		void CalculateDamageToBoss();

		// ������ �÷��̾�� �ִ� ������ ���
		void CalculateDamageToPlayer();

	private:
		// ���������� �ٲ���� �� ������ �������� �˷��ִ� �Լ�
		void SetBossObject();
	};
}