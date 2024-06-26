#pragma once
#include "CommonHeader.h"

struct BossBasicInfo;

namespace KunrealEngine
{
	class GameObject;
	class Component;
	class PlayerAbility;
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
		std::vector<DirectX::XMFLOAT2> KunrealEngine::EventManager::CamShake(float radius, int numPoints);

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