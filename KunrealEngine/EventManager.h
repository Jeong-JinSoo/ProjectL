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

	public:
		GameObject* _player;
		GameObject* _boss;

	public:
		Player* _playerComp;
		Kamen* _bossComp;

		PlayerAbility* _playerAbill;

		bool _eventStart;

	private:
		unsigned int _insideSafeCount;

	private:
		// �÷��̾ �������� �ִ� ������ ���
		void CalculateDamageToBoss();

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
	};
}