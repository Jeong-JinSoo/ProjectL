/// <summary>
/// ������ ��ӹ��� Ŭ����
/// ������ �⺻������ ���� ������ ��� ����
/// 
/// ����ȭ ������
/// </summary>

#pragma once
#include <vector>
#include "BossStruct.hpp"
#include "Coroutine.h"
#include "CommonHeader.h"

namespace DirectX
{
	struct XMFLOAT3;
}

namespace KunrealEngine
{ 
	class GameObject;
	class BoxCollider;

	class _DECLSPEC Boss
	{
	public:
		Boss();
		virtual ~Boss();

	public:
		// �ݵ�� ȣ��
		void Initialize(GameObject* boss);
		// �ݵ�� ȣ��
		void Update();

	public:
		// ���� ������Ʈ�� ����
		virtual void CreateSubObject() abstract;
		// ��� �𵨷� ������ �� �������� ����
		virtual void SetMesh() abstract;
		// �ݶ��̴��� ��� �������� ����
		virtual void SetCollider() abstract;

	public:
		// ��� �ؽ��ĸ� ���� �������� ����
		virtual void SetTexture();

	public:
		// ������ ������ ����
		virtual void SetBossTransform();

	// ���¿� ���� �Լ���
	public:
		virtual void Enter();
		virtual void Idle();
		virtual void Chase();
		virtual void Hit();
		virtual void Attack();
		virtual void Staggred();
		virtual void OffStaggred();
		virtual void Dead();
		virtual void BasicAttack();
		virtual void CoreAttack();
		virtual void PatternEnd();

	public:
		// ������ ���� �ֱ�
		void SetInfo(BossBasicInfo& info);

		// �⺻ ���� ���� ����� �ֱ�
		void SetBasicPatternList(std::vector<BossPattern*>* basicPatternList);
		
		// �ھ� ���� ���� ����� �ֱ�
		void SetCorePatternList(std::vector<BossPattern*>* corePatternList);

		// ���� ������ ���� ��������
		const BossStatus& GetStatus();

		// ������ üũ�� �ݶ��̴� Ȱ��ȭ ����
		void SetColliderState(bool tf);

	private:
		void SetParentToSubObject();

		static bool CompareCorePattern(const BossPattern* pattern1, const BossPattern* pattern2);

		void SortCorePattern();

	private:
		float CalculateAngle(const DirectX::XMFLOAT3& bossPosition, const DirectX::XMFLOAT3& playerPosition);

		bool MoveToPlayer(DirectX::XMFLOAT3 targetPos, float speed, float patternRange);

		bool MoveToPlayer(DirectX::XMFLOAT3 startPos, DirectX::XMFLOAT3 targetPos, float speed, float patternRange);

		void TeleportToPlayer();

		bool LookAtPlayer(float agnle, float rotateSpeed);

		void RegisterCollider();

		void UpdateMoveNode();

		void UpdateMoveNode(DirectX::XMFLOAT3 targetPos);

	protected:
		BossStatus _status;
		BossBasicInfo _info;

		GameObject* _boss;

		GameObject* _player;

		std::vector<BossPattern*> _basicPattern;
		std::vector<BossPattern*> _corePattern;

		std::vector<KunrealEngine::BoxCollider*> _subColliderList;

		int _patternIndex;

		float _distance;

		// ����, ��ű� �� �޽��� �ݶ��̴� ó���� �͵�
		std::vector<GameObject*> _subObjectList;

		// �� ���� ���� �ݶ��̴��� ������ �� 
		unsigned int _maxColliderOnCount;

	private:
		bool _isCorePattern;

	private:
		bool _isStart;
		bool _isHit;

		private:
		Transform* _bossTransform;
		Transform* _playerTransform;

		std::vector<std::pair<DirectX::XMFLOAT3, DirectX::XMFLOAT3>> _stopover;
		int _nodeCount;

	private:
		Coroutine_Func(patternEnd)
		{
			Boss* boss = this;

			Waitforsecond(1.0);

			if (boss->_isCorePattern)
			{
				(boss->_corePattern).pop_back();

				boss->_isCorePattern = false;
			}

			boss->_status = BossStatus::IDLE;

			boss->_patternIndex = -1;
		};
	};
}