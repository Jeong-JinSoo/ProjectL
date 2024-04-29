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

enum class BossStatus;
struct BossPattern;
struct BossBasicInfo;

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

		bool Rotate(float angle, float speed);

		// Ư�� ���������� �̵���Ű�� (raycast true�� ���𰡿� ������ �ű������ ã�ư�)
		bool Move(DirectX::XMFLOAT3& targetPos, float speed, bool roateToTarget, bool rayCast);

		std::function<bool()> CreateBackStepLogic(BossPattern* pattern, float moveSpeed, float distance);

		std::function<bool()> CreateBasicAttackLogic(BossPattern* pattern, GameObject* subObject, float activeColliderFrame);

		std::function<bool()> CreateBasicAttackLogic(BossPattern* pattern, const std::string& animName, GameObject* subObject, float activeColliderFrame);

		// �÷��̾ �ٶ󺸵��� �ڷ���Ʈ
		void TeleportToPlayer();

		// ������ ������ �ڷ���Ʈ
		bool Teleport(const DirectX::XMFLOAT3& targetPos, bool lookAtPlayer, float hideTime = 0.0f);

		// ������ ������ ���� �̵�
		void ForceMove(const DirectX::XMFLOAT3& targetPos);

	public:
		void SetMaxColliderOnCount(unsigned int index);

	private:
		static bool CompareCorePattern(const BossPattern* pattern1, const BossPattern* pattern2);

		void SortCorePattern();

		void SetSubObjectScene();

	private:
		bool LookAtPlayer(float angle, float rotateSpeed);

		float CalculateAngle(const DirectX::XMFLOAT3& bossPosition, const DirectX::XMFLOAT3& playerPosition);

		bool MoveToPlayer(DirectX::XMFLOAT3& startPos, DirectX::XMFLOAT3& targetPos, float speed);

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

		Transform* _bossTransform;
		Transform* _playerTransform;

	private:
		bool _isCorePattern;

	private:
		bool _isStart;
		bool _isHit;
		bool _isRotateFinish;

	private:
		std::vector<std::pair<DirectX::XMFLOAT3, DirectX::XMFLOAT3>> _stopover;
		int _nodeCount;

		DirectX::XMFLOAT3 _direction;

		DirectX::XMFLOAT3 _prevPos;
		DirectX::XMFLOAT3 _backStepPos;

	private:
		float _goalAngle;

	private:
		bool _isMoving;
		bool _isRotate;
		bool _backStepReady;

		bool _isHideFinish;

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

		Coroutine_Func(TeleportWithHide)
		{
			Boss* boss = this;

			boss->_boss->GetComponent<MeshRenderer>()->SetActive(false);
			boss->_boss->GetComponent<BoxCollider>()->SetActive(false);

			Waitforsecond(3.0f);

			boss->_boss->GetComponent<MeshRenderer>()->SetActive(true);
			boss->_boss->GetComponent<BoxCollider>()->SetActive(true);

			boss->_isHideFinish = true;
		};

	};
}