/// <summary>
/// ������ ��ӹ��� Ŭ����
/// ������ �⺻������ ���� ������ ��� ����
/// 
/// ����ȭ ������
/// </summary>

#pragma once
#include <vector>
#include "BossStruct.hpp"

namespace DirectX
{
	struct XMFLOAT3;
}

namespace KunrealEngine
{ 
	class GameObject;

	class Boss
	{
	public:
		Boss();
		virtual ~Boss();

	public:
		// �ݵ�� ȣ��
		void Initialize(GameObject* boss);
		// �ݵ�� ȣ��
		void Update();
		

	// ���¿� ���� �Լ���
	public:
		virtual void Idle();
		virtual void Chase();
		virtual void Hit();
		virtual void Staggred();
		virtual void Dead();
		virtual void BasicAttack();
		virtual void CoreAttack();
		virtual void PatternEnd();

	public:
		void SetInfo(BossBasicInfo info);
		// �⺻ ���� ���� ����� �ֱ�
		void SetBasicPatternList(std::vector<BossPattern*>* basicPatternList);
		// �ھ� ���� ���� ����� �ֱ�
		void SetCorePatternList(std::vector<BossPattern*>* corePatternList);

	private:
		static bool CompareCorePattern(const BossPattern* pattern1, const BossPattern* pattern2);

		void PutCorePatternInQueue();

	private:
		float CaculateDistance(const DirectX::XMFLOAT3& bossPosition, const DirectX::XMFLOAT3& playerPosition);

		int GetRandomNumber(int maxNum);

		bool MoveToPlayer(DirectX::XMFLOAT3 targetPos, float speed, float patternRange);

		void TeleportToPlayer();

		void LookAtPlayer();

	protected:
		BossStatus _status;
		BossBasicInfo _info;

		GameObject* _boss;

		GameObject* _player;

		std::vector<BossPattern*>* _basicPattern;
		std::vector<BossPattern*>* _corePattern;

		int _patternIndex;

		float _distance;

	private:
		bool _isCorePattern;
	};
}