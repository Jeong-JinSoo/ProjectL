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

		// ������ 2

		// ȭ�� ���Ÿ� ��ô (��ġ�� call�� ������ ����)
		void CreateLeftFireShot();
		void CreateRightFireShot();
		// ���� ���� ���� (����Ʈ�� ȭ������)
		void CreateRandomFireAttack();


	private:
		BossPattern* _leftAttack;
		BossPattern* _rightAttack;
		BossPattern* _turn180;

		BossPattern* _spellAttack;
		BossPattern* _callAttack;
		BossPattern* _backStep;

	private:
		// ���Ͽ� �ʿ��� subObject��
		GameObject* _leftHand;
		GameObject* _rightHand;
		GameObject* _leftRoot;
		GameObject* _rightRoot;

		GameObject* _smallRoot;

		std::vector<GameObject*> _fakeBoss;

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
	};
}
