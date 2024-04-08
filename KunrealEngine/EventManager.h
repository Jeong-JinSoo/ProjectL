#pragma once
#include "CommonHeader.h"

namespace KunrealEngine
{
	class GameObject;
	class Component;
	class Ability;

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
		

		// ������ �÷��̾�� �ִ� ������ ���
		void CalculateDamageToPlayer();

	public:
		// ���������� �ٲ���� �� ������ �������� �˷��ִ� �Լ�
		void SetBossObject();

		// �÷��̾ �������� �ִ� ������ ���
		void CalculateDamageToBoss(Ability abil);
	};
}