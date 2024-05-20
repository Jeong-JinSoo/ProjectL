/// 2024.04.25 - ������(update 24.04.25)
/// ������ : �ƶ�ũ��
/// �Ź�Ÿ�� ����
///
#pragma once
#include "KunrealAPI.h"
#include "Boss.h"
#include "Coroutine.h"

namespace KunrealEngine
{
	class _DECLSPEC Aracne : public Component, public Boss
	{
	public:
		Aracne();
		virtual ~Aracne();

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

		void CreatesubObject();
	public:
		virtual void CreatePattern() override;


	private:
		// 1������
		void ChargeAttack();// ���� ���� - 2�� �����ϰ� �������
		void DropWeb();		// �ھ������� ���� �Ź��� ���
		void LeftAttack();	// �¼� ����
		void RightAttack();	// ��� ����
		void FrontAttack();	// ��� ���� ���

		//2������ ���� ����
		void PullAllWeb();	// ��Ƶ� �Ź��� ����ø���

		// 2������
		void JumpAttack();	// ���� - ������� ����
		void TailAttack();	// ���� ����
		void ShootingWeb();	// �Ź��� ��� ���� - ��ä�� �����
		void Casting();		// ���� ����(�� ���������� ������)

	private:
		bool Move(DirectX::XMFLOAT3& startPos, DirectX::XMFLOAT3& targetPos, float speed);
		float CalculateParabolaHeight(DirectX::XMFLOAT3 start, DirectX::XMFLOAT3 end, DirectX::XMFLOAT3 current);

	private:	// ���� ���ϸ� �־��
		BossPattern* _jumpAttack;
		BossPattern* _ChargeAttack;
		BossPattern* _leftAttack;
		BossPattern* _righttAttack;
		BossPattern* _frontAttack;
		BossPattern* _tailAttack;

	private:	// ���� �ݶ��̴��� �־��
		GameObject* _colbodyAttack;
		GameObject* _colFrontAttack;
		GameObject* _colTailAttack;
		GameObject* _colLeftHand;
		GameObject* _colRightHand;

	private:	// �� ��
		const float _jumpAttackRange = 10.0f;
		int _call = 0;

		// ���� ���� ���޿�
		bool _jumpAttack_end = false;
		bool _chargeAttack_end = false;

	private:	// �ڷ�ƾ
		Coroutine_Func(JumpAttackCo)
		{	// ������ ��������. ������ �����ϰ� ���Ƽ� �÷��̾�� �̵��� ���� �������°ǵ� 
			// ���߿� �̰� 3�������� �������� ���� ���� �ؼ� �������� �׸��鼭 ���ư��� �ٲܰ���
			// �ϴ� ���� �ϳ��� �ð� �ʹ� ���°� ���Ƽ� ���������

			Aracne* some = this;
			auto animator = _boss->GetComponent<Animator>();
			DirectX::XMFLOAT3 target = some->_playerTransform->GetPosition();	// ���� ���� ����
			DirectX::XMFLOAT3 start = some->_bossTransform->GetPosition();	// ���� ������ġ
			some->_jumpAttack_end = true;

			while (true)
			{
				if (!(animator->Play("Anim_Jump", 20.0f)))
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

			some->_colbodyAttack->SetActive(true);
			while (true)
			{
				if (!(animator->Play("Anim_Land", 30.0f)))
				{
					some->_colbodyAttack->SetActive(false);
					animator->Stop();
					break;
				}
				DirectX::XMFLOAT3 rot = some->_bossTransform->GetRotation();
				some->_colbodyAttack->GetComponent<Transform>()->SetPosition(some->_bossTransform->GetPosition());
				some->_colbodyAttack->GetComponent<Transform>()->SetRotation(DirectX::XMFLOAT3(0, rot.y, 0));
				Return_null;
			}

			some->_jumpAttack_end = false;
		};

		Coroutine_Func(ChargeAttackCo)
		{
			Aracne* some = this;
			auto animator = _boss->GetComponent<Animator>();
			DirectX::XMFLOAT3 target = some->_playerTransform->GetPosition();	// ���� ���� ����
			DirectX::XMFLOAT3 start = some->_bossTransform->GetPosition();	// ���� ������ġ
			some->_chargeAttack_end = true;

			some->_colbodyAttack->SetActive(true);
			while (true)
			{
				DirectX::XMFLOAT3 mine = some->_bossTransform->GetPosition();
				animator->Play("Run", 30.0f, true);

				DirectX::XMFLOAT3 rot = some->_bossTransform->GetRotation();
				some->_colbodyAttack->GetComponent<Transform>()->SetPosition(some->_bossTransform->GetPosition());
				some->_colbodyAttack->GetComponent<Transform>()->SetRotation(DirectX::XMFLOAT3(0, rot.y, 0));
				Return_null;
				
				if (!some->Move(mine, target, 30.0f))
				{
					animator->Stop();
					break;
				}
			}

			while (true)
			{
				if (!(animator->Play("Attak_Take_down", 30.0f)))
				{
					animator->Stop();
					break;
				}

				DirectX::XMFLOAT3 rot = some->_bossTransform->GetRotation();
				some->_colbodyAttack->GetComponent<Transform>()->SetPosition(some->_bossTransform->GetPosition());
				some->_colbodyAttack->GetComponent<Transform>()->SetRotation(DirectX::XMFLOAT3(0, rot.y, 0));
				Return_null;
			}

			some->_colbodyAttack->SetActive(false);
			some->_chargeAttack_end = false;
		};
	};
}