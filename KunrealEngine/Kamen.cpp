#include <functional>
#include "SceneManager.h"
#include "Kamen.h"

KunrealEngine::Kamen::Kamen()
	: Boss()
{
	BossBasicInfo info;

	info.SetHp(10000).SetPhase(3).SetArmor(10).SetDamage(100).SetHp(10000).SetMoveSpeed(20.0f).SetsStaggeredGauge(100.0f);
	info.SetAttackRange(5.0f);

	SetInfo(info);

	CreateTestPattern1();
}

KunrealEngine::Kamen::~Kamen()
{
	
}

void KunrealEngine::Kamen::Initialize()
{
	// �ݵ�� �ؾ���
	Boss::Initialize(this->GetOwner());
}

void KunrealEngine::Kamen::Release()
{

}

void KunrealEngine::Kamen::FixedUpdate()
{

}

void KunrealEngine::Kamen::Update()
{
	// �ݵ�� �ؾ���
	Boss::Update();
}

void KunrealEngine::Kamen::LateUpdate()
{

}

void KunrealEngine::Kamen::OnTriggerEnter()
{

}

void KunrealEngine::Kamen::OnTriggerStay()
{

}

void KunrealEngine::Kamen::OnTriggerExit()
{

}

void KunrealEngine::Kamen::SetActive(bool active)
{

}

void KunrealEngine::Kamen::CreateSubObject()
{
	auto weapon = SceneManager::GetInstance().GetCurrentScene()->CreateObject("Weapon");
	_subObjectList.emplace_back(weapon);

	auto leftHand = SceneManager::GetInstance().GetCurrentScene()->CreateObject("leftHand");
	_subObjectList.emplace_back(leftHand);

	auto testSub = SceneManager::GetInstance().GetCurrentScene()->CreateObject("testSub");
	_subObjectList.emplace_back(testSub);
}

void KunrealEngine::Kamen::SetMesh()
{
	_boss->AddComponent<MeshRenderer>();
	_boss->GetComponent<MeshRenderer>()->SetMeshObject("alien/alien");
	
	_subObjectList[0]->AddComponent<MeshRenderer>();
	_subObjectList[0]->GetComponent<MeshRenderer>()->SetMeshObject("Sword/Sword");
	_subObjectList[0]->GetComponent<MeshRenderer>()->SetParentBone(_boss, "mixamorig:RightHand");
}

void KunrealEngine::Kamen::SetCollider()
{
	// ������ �ݶ��̴�, �÷��̾��� �̵� üũ�� ���� ������ ũ�⸸ŭ ����
	_boss->AddComponent<BoxCollider>();
	_boss->GetComponent<BoxCollider>()->SetBoxSize(5.0f, 10.0f, 5.0f);
	_boss->GetComponent<BoxCollider>()->SetTransform(_boss, "mixamorig:Spine");

	// ���������� ���� �ݶ��̴��� ���� ������Ʈ���� �ݶ��̴� ����
	_subObjectList[0]->AddComponent<BoxCollider>();
	_subObjectList[0]->GetComponent<BoxCollider>()->SetBoxSize(2.0f, _info._attackRange, 2.0f);

	_subObjectList[1]->AddComponent<BoxCollider>();
	_subObjectList[1]->GetComponent<BoxCollider>()->SetTransform(_boss, "mixamorig:LeftHand");
	//
	//_subObjectList[2]->AddComponent<BoxCollider>();
}

void KunrealEngine::Kamen::SetBossTransform()
{
	_boss->GetComponent<Transform>()->SetPosition(5.0f, 0.0f, -20.0f);
	_boss->GetComponent<Transform>()->SetScale(0.05f, 0.05f, 0.05f);
	_boss->GetComponent<Transform>()->SetRotation(0.f, 0.f, 0.f);
}

void KunrealEngine::Kamen::CreateTestPattern1()
{
	BossPattern* pattern1 = new BossPattern();

	pattern1->SetAnimName("Punch").SetDamage(100.0f).SetSpeed(20.0f).SetRange(_info._attackRange).SetAfterDelay(5.0f);
	pattern1->SetIsWarning(false).SetWarningName("").SetTriggerHp(20.0f);

	std::function<bool()> logic = [pattern1, this]()
	{
		auto animator = _boss->GetComponent<Animator>();
		auto isAnimationPlaying = animator->Play(pattern1->_animName, pattern1->_speed, false);

		// ���� �������� �Ѿ�� ������ üũ�� �ݶ��̴��� Ű��
		if (_maxColliderOnCount > 0)
		{
			if (animator->GetCurrentFrame() >= 10)
			{
				SetColliderState(true);
			}
		}

		if (isAnimationPlaying == false)
		{
			return false;
		}

		return true;
	};

	pattern1->SetLogic(logic);

	_basicPattern.emplace_back(pattern1);
}