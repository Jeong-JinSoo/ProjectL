#pragma once
#include "KunrealAPI.h" // ���⿡ �߰��������
#include "Boss.h"
#include "Coroutine.h"

namespace KunrealEngine
{
	class _DECLSPEC Ent : public Component, public Boss
	{
	public:
		Ent();
		virtual ~Ent();

	public:
		void Initialize() override;
		void Release() override;

		void FixedUpdate() override;
		void Update() override;
		void LateUpdate() override;

		void OnTriggerEnter() override;
		void OnTriggerStay() override;
		void OnTriggerExit() override;

		void SetActive(bool active) override;

		virtual void SetMesh() override;
		virtual void SetTexture() override;
		virtual void SetBossTransform() override;
		virtual void SetBossCollider() override;

	public:
		virtual void CreatePattern() override;


	private:
		void CreateSubObject();

	private:
		// �Ϲ� ��������
		void CreateLeftAttack();
		void CreateRightAttack();
		// �����ٱ� ���Ÿ� ��ô
		void CreateLeftRootShot();
		void CreateRightRootShot();
		// ���� ���� ����
		void CreateRandomRootAttack();
		// ���� ���� ����
		void CreateJumpAttack();

		// ������ ��
		void CreateEyeLight();

		// �Ϲ� ������
		void CreateSwiping();

		// ������ 2

		// ȭ�� ���Ÿ� ��ô (��ġ�� call�� ������ ����)
		void CreateLeftFireShot();
		void CreateRightFireShot();
		// ���� ���� ���� (����Ʈ�� ȭ������)
		void CreateRandomFireAttack();



	private:
		// �ھ�����, ��� ����

		void CreateCorePatternFirst();

		void CorePatternObjectFirst();

	private:
		float GetRandomRange(float center, float range);
		bool Move(DirectX::XMFLOAT3& startPos, DirectX::XMFLOAT3& targetPos, float speed);

	private:
		BossPattern* _leftAttack;
		BossPattern* _rightAttack;
		BossPattern* _bigRootShot;
		// �ش� ���� ������� �����
		 
	private:
		// ���Ͽ� �ʿ��� subObject��
		GameObject* _leftHand;
		GameObject* _rightHand;
		GameObject* _leftRoot;
		GameObject* _rightRoot;

		std::vector<GameObject*> _smallRootVector;

		std::vector<GameObject*> _treeObject;

		GameObject* _treeObjectReal;

		GameObject* _eyeLight;

		//GameObject* _eyeLightCollider;

	private:
		// Call �Ÿ� üũ��
		float _callMoveDistance;
		bool _isRotateFinish;

	private:
		bool _isCoreStart;  
		bool _isRandomStart;

		// logic
	private:
		std::function<void()> _callInitLogic;

	private:
		GameObject* _bigRootR;
		GameObject* _bigRootL;

		GameObject* _rootShot;

		GameObject* _colJumpAttack;

	private:
		bool _bigRootShotStart;

		bool _isIdleHealing;

		float _rootY;

		float randomX;

		float randomZ;

		bool isEyesLightStart;


	private:
		// �ھ����Ͽ��� ����� ������

		int _successCountCoreFirst;

		std::vector<std::string> _objectOrderCoreFirst;


	private:	// �ڷ�ƾ �������� ���� �߹�
		Coroutine_Func(JumpAttackCo)
		{	
			Ent* some = this;
			auto animator = _boss->GetComponent<Animator>();
			DirectX::XMFLOAT3 target = some->_playerTransform->GetPosition();	// ���� ���� ����
			DirectX::XMFLOAT3 start = some->_bossTransform->GetPosition();	// ���� ������ġ

			while (true)
			{
				if (!(animator->Play("Anim_Jump_Start", 20.0f)))
				{
					animator->Stop();
					break;
				}

				if (25 < animator->GetCurrentFrame())
				{
					DirectX::XMFLOAT3 mine = some->_bossTransform->GetPosition();
					some->Move(mine, target, 10.0f);
				}
				Return_null;
			}

			while (true)
			{
				DirectX::XMFLOAT3 mine = some->_bossTransform->GetPosition();
				if (!(some->Move(mine, target, 30.0f)))
				{
					animator->Stop();
					break;
				}
				animator->Play("Anim_Fall", 30.0f, true);
				Return_null;
			}

			some->_colJumpAttack->SetActive(true);
			while (true)
			{
				if (!(animator->Play("Anim_Jump_End", 30.0f)))
				{
					some->_colJumpAttack->SetActive(false);
					animator->Stop();
					break;
				}
				DirectX::XMFLOAT3 rot = some->_bossTransform->GetRotation();
				some->_colJumpAttack->GetComponent<Transform>()->SetPosition(some->_bossTransform->GetPosition());
				some->_colJumpAttack->GetComponent<Transform>()->SetRotation(DirectX::XMFLOAT3(0, rot.y, 0));
				Return_null;
			}

			some->_status = BossStatus::IDLE;
		};
	};
}
