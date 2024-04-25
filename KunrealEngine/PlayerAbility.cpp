#include "PlayerAbility.h"
#include "InputSystem.h"
#include "GraphicsSystem.h"
#include "MeshRenderer.h"
#include "BoxCollider.h"
#include "Ability.h"
#include "Projectile.h"
#include "Particle.h"
#include "TimeManager.h"

KunrealEngine::PlayerAbility::PlayerAbility()
	:_playerComp(nullptr), _meteor(nullptr)
{

}

KunrealEngine::PlayerAbility::~PlayerAbility()
{

}

void KunrealEngine::PlayerAbility::Initialize()
{
	_playerComp = GetOwner()->GetComponent<Player>();

	CreateAbility1();
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
	if (InputSystem::GetInstance()->KeyDown(KEY::R))
	{
		ResetMeteorPos();
		_meteor->SetActive(true);
	}

	_abilityContainer[0]->_abilityLogic();
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
	/// �̰� �ƴ� ��������� ���а� ���߿� ���ľ���
	Ability* shot = new Ability();
	shot->Initialize("Shot");

	shot->SetTotalData(
		"Meteor",			// �̸�
		20.0f,			// ������
		10.0f,			// ����
		5.0f,			// ����ȭ ���ط�
		6.0f,			// ��Ÿ��
		15.0f			// ��Ÿ�
	);

	// ��ü ����
	_shot = shot->_projectile;

	// ����ü ������Ʈ �߰�
	_shot->AddComponent<Projectile>();
	Projectile* shotProj = _shot->GetComponent<Projectile>();

	shotProj->SetMeshObject("Meteor/Meteor");
	shotProj->GetCollider()->SetBoxSize(2.0f, 2.0f, 2.0f);
	shotProj->SetDestoryCondition([shotProj, this]()->bool
		{
			if (shotProj->GetCollider()->IsCollided() && shotProj->GetCollider()->GetTargetObject() != this->GetOwner())
			{
				return true;
			}
			else
			{
				return false;
			}
		});

	_shot->SetActive(false);

	shot->SetLogic([shot, shotProj, this]()
		{
			shot->_projectile->SetActive(true);

			shotProj->SetDirection(GetOwner());
		});

	AddToContanier(shot);
}

void KunrealEngine::PlayerAbility::ResetMeteorPos()
{
	// ���콺 3D��ǥ�κ��� 20.0f ���� ��ġ��Ű����
	_meteor->GetComponent<Transform>()->SetPosition(GRAPHICS->ScreenToWorldPoint(InputSystem::GetInstance()->GetEditorMousePos().x, InputSystem::GetInstance()->GetEditorMousePos().y));
	_meteor->GetComponent<Transform>()->SetPosition
	(
		_meteor->GetComponent<Transform>()->GetPosition().x,
		_meteor->GetComponent<Transform>()->GetPosition().y + 40.0f,
		_meteor->GetComponent<Transform>()->GetPosition().z
	);

	// �츮 �������� ������ �̻ڴ�
	_meteor->GetComponent<Transform>()->SetRotation(0.0f, 0.0f, -90.0f);
}

void KunrealEngine::PlayerAbility::CreateAbility4()
{
	Ability* meteor = new Ability();
	meteor->Initialize("Meteor");

	meteor->SetTotalData(
		"Meteor",			// �̸�
		50.0f,			// ������
		10.0f,			// ����
		15.0f,			// ����ȭ ���ط�
		20.0f,			// ��Ÿ��
		15.0f			// ��Ÿ�
	);

	// ��ü ����
	_meteor = meteor->_projectile;

	// ����ü ������Ʈ �߰�
	_meteor->AddComponent<Projectile>();
	Projectile* meteorProj = _meteor->GetComponent<Projectile>();

	meteorProj->SetMeshObject("Meteor/Meteor");
	meteorProj->GetCollider()->SetBoxSize(2.0f, 2.0f, 2.0f);

	// ��ƼŬ �߰�	// ��Ÿ������
	Particle* meteorParticle = _meteor->AddComponent<Particle>();
	//Particle* meteorParticle = _meteor->GetComponent<Particle>();
	meteorParticle->SetParticleEffect("Fire", "Resources/Textures/Particles/flare.dds", 1000);
	meteorParticle->SetParticleSize(40.0f, 20.0f);
	meteorParticle->SetParticleVelocity(20.0f, true);
	meteorParticle->SetParticleDuration(2.0f, 2.0f);
	//meteorParticle->AddParticleColor(1.0f, 0.0f, 0.0f);
	meteorParticle->SetParticleDirection(0.0f, 20.0f, 0.0f);
	meteorParticle->SetActive(true);

	//Particle* meteorParticle2 = _meteor->AddComponent<Particle>();
	//meteorParticle2->SetParticleEffect("Fire2", "Resources/Textures/Particles/flare.dds", 1000);
	//meteorParticle2->SetParticleSize(20.0f, 1.0f);
	//meteorParticle2->SetParticleVelocity(50.0f, true);
	//meteorParticle2->SetParticleDuration(1.0f, 1.0f);
	////meteorParticle2->AddParticleColor(1.0f, 0.0f, 0.0f);
	//meteorParticle2->SetParticleDirection(0.0f, 20.0f, 0.0f);
	//meteorParticle2->SetActive(true);

	// ��Ȱ��ȭ ����		// �ΰ��ӿ����� �Ҹ�ó��
	meteorProj->SetDestoryCondition([meteorProj, this]()->bool
		{
			if (_meteor->GetComponent<Transform>()->GetPosition().y <= 2.0f)
			{
				return true;
			}
			else
			{
				return false;
			}
		});

	// ��Ȱ��ȭ �� ���·� ����
	_meteor->SetActive(false);

	// ���� ����
	meteor->SetLogic([meteor, meteorProj, this]()
		{
			if (_meteor->GetActivated())
			{
				_meteor->GetComponent<Transform>()->SetPosition
				(
					_meteor->GetComponent<Transform>()->GetPosition().x,
					_meteor->GetComponent<Transform>()->GetPosition().y - 20.0f * TimeManager::GetInstance().GetDeltaTime(),
					_meteor->GetComponent<Transform>()->GetPosition().z
				);
			}
		});

	// �� ��������� �����̳ʿ� �߰�
	AddToContanier(meteor);
}

void KunrealEngine::PlayerAbility::AddToContanier(Ability* abil)
{
	_abilityContainer.emplace_back(abil);
}
