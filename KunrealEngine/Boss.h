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
		void DebugTest();

	public:
		// �ݵ�� ȣ��
		void Initialize(GameObject* boss);
		// �ݵ�� ȣ��
		void Update();

	public:
		// ��� �𵨷� ������ �� �������� ����
		virtual void SetMesh() abstract;

	public:
		// ���� ����
		virtual void CreatePattern() abstract;

	public:
		// ��� �ؽ��ĸ� ���� �������� ����
		virtual void SetTexture();

	public:
		// ������ ������ ����
		virtual void SetBossTransform();

	public:
		const std::pair<DirectX::XMFLOAT3, DirectX::XMFLOAT3> GetBossPosition();

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
		virtual void PatternReady();
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

		// ���� ������ �ٶ󺸴� ���� ���� ��������
		const DirectX::XMVECTOR GetDirection();

	public:
		// ���� ��Ʈ ������ �ݶ��̴� ����
		virtual void SetBossCollider();

		void SetSubObject(bool tf);

		/// ���� ���� �����Ҷ� ������ ������.
	public:
		// �÷��̾� �ٶ󺸵��� ȸ����Ű��
		bool RotateToTarget(const DirectX::XMFLOAT3& targetPos);
		
		// Ư�� ���������� �̵���Ű�� (raycast true�� ���𰡿� ������ �ű������ ã�ư�)
		bool Move(DirectX::XMFLOAT3& targetPos, float speed, bool roateToTarget, bool rayCast);

		std::function<bool()> CreateBackStepLogic(BossPattern* pattern, float moveSpeed, float distance);
		
		// �÷��̾ �ٶ󺸵��� �ڷ���Ʈ
		void TeleportToPlayer();

		// ������ ������ �ڷ���Ʈ
		void Teleport(const DirectX::XMFLOAT3 &targetPos, bool lookAtPlayer);

	private:
		static bool CompareCorePattern(const BossPattern* pattern1, const BossPattern* pattern2);

		void SortCorePattern();

		void SetSubObjectScene();

	private:
		bool LookAtPlayer(float angle, float rotateSpeed);

		float CalculateAngle(const DirectX::XMFLOAT3& bossPosition, const DirectX::XMFLOAT3& playerPosition);

		bool MoveToPlayer(DirectX::XMFLOAT3& targetPos, float speed, float patternRange);

		bool MoveToPlayer(DirectX::XMFLOAT3& startPos, DirectX::XMFLOAT3& targetPos, float speed, float patternRange);

		void UpdateMoveNode();

		void UpdateMoveNode(DirectX::XMFLOAT3& targetPos);

		void CalculateDirection();

		bool MoveToTarget(DirectX::XMFLOAT3& startPos, DirectX::XMFLOAT3& targetPos, float speed);

		// �齺�� �̵�
		bool BackStep(float speed, float distance);

	protected:
		BossStatus _status;
		BossBasicInfo _info;

		GameObject* _boss;

		GameObject* _player;

		std::vector<BossPattern*> _basicPattern;
		std::vector<BossPattern*> _corePattern;
		BossPattern* _nowPattern;

		int _patternIndex;
		int _exPatternIndex;

		float _distance;

		// �� ���� ���� �ݶ��̴��� ������ �� 
		unsigned int _maxColliderOnCount;

	private:
		bool _isCorePattern;

	private:
		bool _isStart;
		bool _isHit;
		bool _isRotateFinish;

		private:
		Transform* _bossTransform;
		Transform* _playerTransform;

		std::vector<std::pair<DirectX::XMFLOAT3, DirectX::XMFLOAT3>> _stopover;
		int _nodeCount;

		DirectX::XMFLOAT3 _direction;

		DirectX::XMFLOAT3 _prevPos;
		DirectX::XMFLOAT3 _backStepPos;

	private:
		bool _isMoving;
		bool _isRotate;
		bool _backStepReady;

	private:
		Coroutine_Func(patternEnd)
		{
			Boss* boss = this;

			auto delay = _nowPattern->_afterDelay;
			Waitforsecond(delay);

			// �ھ� �����̾��ٸ�
			if (boss->_isCorePattern)
			{
				// ������ �ھ� ������ ���Ϳ��� ����
				(boss->_corePattern).pop_back();
				
				boss->_isCorePattern = false;
			}

			// ���� �ʱ�ȭ�� �͵� �ʱ�ȭ
			boss->_nowPattern->Initialize();

			boss->_status = BossStatus::IDLE;

			// �⺻ ������ ������ �� �ֵ��� �ʱ�ȭ
			boss->_patternIndex = -1;

			boss->_nowPattern = nullptr;
		};
	};
}