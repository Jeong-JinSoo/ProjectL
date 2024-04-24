#include "PlayerAbility.h"
#include "MeshRenderer.h"
#include "Ability.h"

KunrealEngine::PlayerAbility::PlayerAbility()
	:_playerComp(nullptr)
{

}

KunrealEngine::PlayerAbility::~PlayerAbility()
{

}

void KunrealEngine::PlayerAbility::Initialize()
{
	_playerComp = GetOwner()->GetComponent<Player>();

	CreateAbility1();
}

void KunrealEngine::PlayerAbility::Release()
{
	
}

void KunrealEngine::PlayerAbility::FixedUpdate()
{
	
}

void KunrealEngine::PlayerAbility::Update()
{
	
}

void KunrealEngine::PlayerAbility::LateUpdate()
{
	
}

void KunrealEngine::PlayerAbility::OnTriggerEnter()
{
	
}

void KunrealEngine::PlayerAbility::OnTriggerStay()
{
	
}

void KunrealEngine::PlayerAbility::OnTriggerExit()
{
	
}

void KunrealEngine::PlayerAbility::SetActive(bool active)
{
	
}

void KunrealEngine::PlayerAbility::CreateAbility1()
{
	Ability* shot = new Ability();
	shot->SetTotalData(
		"Shot",			// �̸�
		20.0f,			// ������
		10.0f,			// ����
		5.0f,			// ����ȭ ���ط�
		6.0f,			// ��Ÿ��
		15.0f			// ��Ÿ�
	);

	shot->SetLogic([]() 
		{
			//Projectile* projectile = new Projectile();
		});
}

void KunrealEngine::PlayerAbility::AddToContanier(Ability* abil)
{
	_abilityContainer.push_back(abil);
}
