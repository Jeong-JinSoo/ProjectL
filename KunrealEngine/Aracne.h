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
		void ChargeAttack();// ���� ����
		void DropWeb();
		void LeftAttack();	// �¼� ����
		void RightAttack();	// ��� ����
		void FrontAttack();	// ��� ���� ���
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
		BossPattern* _leftAttack;
		BossPattern* _righttAttack;
		BossPattern* _frontAttack;
		BossPattern* _tailAttack;

	private:	// ���� �ݶ��̴��� �־��
		GameObject* _colJumpAttack;
		GameObject* _colFrontAttack;
		GameObject* _colTailAttack;
		GameObject* _colLeftHand;
		GameObject* _colRightHand;

	private:	// �� ��
		const float jumpAttackRange = 10.0f;
		int call = 0;

		// ���� ���� ���޿�
		bool jumpAttack_end = false;
	private:	// �ڷ�ƾ
		Coroutine_Func(JumpAttackCo)
		{	// ������ ��������. ������ �����ϰ� ���Ƽ� �÷��̾�� �̵��� ���� �������°ǵ� 
			// ���߿� �̰� 3�������� �������� ���� ���� �ؼ� �������� �׸��鼭 ���ư��� �ٲܰ���
			// �ϴ� ���� �ϳ��� �ð� �ʹ� ���°� ���Ƽ� ���������

			Aracne* some = this;
			auto animator = _boss->GetComponent<Animator>();
			DirectX::XMFLOAT3 target = some->_playerTransform->GetPosition();	// ���� ���� ����
			DirectX::XMFLOAT3 start = some->_bossTransform->GetPosition();	// ���� ������ġ
			some->jumpAttack_end = true;

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

			some->_colJumpAttack->SetActive(true);
			while (true)
			{
				if (!(animator->Play("Anim_Land", 30.0f)))
				{
					some->_colJumpAttack->SetActive(false);
			 		animator->Stop();
			 		break;
				}
				DirectX::XMFLOAT3 rot = some->_bossTransform->GetRotation();
				some->_colJumpAttack->GetComponent<Transform>()->SetPosition(some->_bossTransform->GetPosition());
				some->_colJumpAttack->GetComponent<Transform>()->SetRotation(DirectX::XMFLOAT3(0,rot.y,0));
				Return_null;
			}

			some->jumpAttack_end = false;
		};
	};
}