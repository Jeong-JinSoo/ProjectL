#include "PlayerAbility.h"
#include "MeshRenderer.h"
#include "BoxCollider.h"
#include "Ability.h"
#include "Projectile.h"
#include "InputSystem.h"
#include "GraphicsSystem.h"

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

	CreateAbility4();
}

void KunrealEngine::PlayerAbility::Release()
{
	for (auto abils : _abilityContainer)
	{
		abils->Release();
	}
}

void KunrealEngine::PlayerAbility::FixedUpdate()
{
	
}

void KunrealEngine::PlayerAbility::Update()
{
	/// ��Ÿ�� ���� �ʿ�
	if (InputSystem::GetInstance()->KeyInput(KEY::Q))
	{
		_abilityContainer[0]->_abilityLogic();
	}
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

void KunrealEngine::PlayerAbility::CreateAbility4()
{
	Ability* meteor = new Ability();
	meteor->Initialize("Meteor");

	meteor->SetTotalData(
		"Meteor",			// �̸�
		20.0f,			// ������
		10.0f,			// ����
		5.0f,			// ����ȭ ���ط�
		6.0f,			// ��Ÿ��
		15.0f			// ��Ÿ�
	);

	meteor->_projectile->AddComponent<Projectile>();
	Projectile* meteorProj = meteor->_projectile->GetComponent<Projectile>();

	meteorProj->SetMeshObject("Meteor/Meteor");
	meteorProj->GetCollider()->SetBoxSize(2.0f, 2.0f, 2.0f);
	meteorProj->SetDestoryCondition([meteorProj, this]()->bool
		{
			if (meteorProj->GetCollider()->IsCollided() && meteorProj->GetCollider()->GetTargetObject() != this->GetOwner())
			{
				return true;
			}
			else
			{
				return false;
			}
		});

	meteor->_projectile->SetActive(false);

	meteor->SetLogic([meteor, meteorProj, this]()
		{
			meteor->_projectile->SetActive(true);
			meteor->_projectile->GetComponent<Transform>()->SetPosition(
				this->GetOwner()->GetComponent<Transform>()->GetPosition().x,
				this->GetOwner()->GetComponent<Transform>()->GetPosition().y,
				this->GetOwner()->GetComponent<Transform>()->GetPosition().z
				);

			meteorProj->SetDirection(GetOwner());
		});

	AddToContanier(meteor);
}

void KunrealEngine::PlayerAbility::AddToContanier(Ability* abil)
{
	_abilityContainer.emplace_back(abil);
}
