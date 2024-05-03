#include "PlayerAbility.h"
#include "InputSystem.h"
#include "GraphicsSystem.h"
#include "MeshRenderer.h"
#include "BoxCollider.h"
#include "Ability.h"
#include "Projectile.h"
#include "Particle.h"
#include "TimeManager.h"
#include "ToolBox.h"
#include "Animator.h"

KunrealEngine::PlayerAbility::PlayerAbility()
	:_playerComp(nullptr), _meteor(nullptr), _shot(nullptr)
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
	/// �Լ��� ������ ���߿�
	/// ��Ÿ�� ���� �ʿ�
	if (InputSystem::GetInstance()->KeyDown(KEY::R))
	{
		ResetMeteorPos();
		_playerComp->_playerStatus = Player::Status::ABILITY;
		_playerComp->_abilityAnimationIndex = 3;
	}

	if (InputSystem::GetInstance()->KeyDown(KEY::Q))
	{
		_shot->SetActive(true);
		_shot->GetComponent<Projectile>()->SetActive(true);
		_shot->GetComponent<Projectile>()->ResetCondition();
		ResetShotPos();
	}

	if (_playerComp->_playerStatus == Player::Status::ABILITY && _playerComp->_abilityAnimationIndex == 3 && GetOwner()->GetComponent<Animator>()->GetCurrentFrame() >= GetOwner()->GetComponent<Animator>()->GetMaxFrame())
	{
		_meteor->SetActive(true);
		_meteor->GetComponent<Particle>()->SetActive(true);
		_playerComp->_playerStatus = Player::Status::IDLE;
	}

	_abilityContainer[0]->_abilityLogic();
	_abilityContainer[1]->_abilityLogic();
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
	this->_isActivated = active;
}


void KunrealEngine::PlayerAbility::ResetShotPos()
{
	// �÷��̾� ��ġ���� �߻�ǵ���
	_shot->GetComponent<Transform>()->SetPosition(this->GetOwner()->GetComponent<Transform>()->GetPosition());
	_shot->GetComponent<Transform>()->SetRotation(
		this->GetOwner()->GetComponent<Transform>()->GetRotation().x, 
		this->GetOwner()->GetComponent<Transform>()->GetRotation().y,
		this->GetOwner()->GetComponent<Transform>()->GetRotation().z
	);

	// ���⺤��
	DirectX::XMFLOAT3 currentPoint = _shot->GetComponent<Transform>()->GetPosition();
	DirectX::XMFLOAT3 targetPoint = GRAPHICS->ScreenToWorldPoint(InputSystem::GetInstance()->GetEditorMousePos().x, InputSystem::GetInstance()->GetEditorMousePos().y);

	DirectX::XMVECTOR direction = ToolBox::GetDirectionVec(currentPoint, targetPoint);

	_shot->GetComponent<Projectile>()->SetDirection(direction);
	_shot->GetComponent<Projectile>()->_movedRange = 0.0f;
}

void KunrealEngine::PlayerAbility::CreateAbility1()
{
	Ability* shot = new Ability();
	shot->Initialize("Shot");

	shot->SetTotalData(
		"Shot",			// �̸�
		10.0f,			// ������
		10.0f,			// ����
		5.0f,			// ����ȭ ���ط�
		6.0f,			// ��Ÿ��
		10.0f			// ��Ÿ�
	);

	// ��ü ����
	_shot = shot->_projectile;

	// ũ�� ����
	_shot->GetComponent<Transform>()->SetScale(0.3f, 0.3, 0.3f);

	// ����ü ������Ʈ �߰�
	_shot->AddComponent<Projectile>();
	Projectile* shotProj = _shot->GetComponent<Projectile>();

	shotProj->SetMeshObject("Meteor/Meteor");
	shotProj->GetCollider()->SetBoxSize(3.0f, 3.0f, 3.0f);
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
			if (_shot->GetActivated())
			{
				DirectX::XMFLOAT3 currentPoint = _shot->GetComponent<Transform>()->GetPosition();

				DirectX::XMVECTOR currentPosVec = DirectX::XMLoadFloat3(&currentPoint);
				DirectX::XMVECTOR newPosition = DirectX::XMVectorAdd(currentPosVec, DirectX::XMVectorScale(shotProj->GetDirection(), 40.0f * TimeManager::GetInstance().GetDeltaTime()));

				_shot->GetComponent<Transform>()->SetPosition(newPosition.m128_f32[0], 5.0f, newPosition.m128_f32[2]);
				_shot->GetComponent<Transform>()->SetRotation(0.0f, _shot->GetComponent<Transform>()->GetRotation().y + 50.0f, 0.0f);
				shotProj->_movedRange += DirectX::XMVectorGetX(DirectX::XMVector3Length(DirectX::XMVectorSubtract(newPosition, currentPosVec)));

			}
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
	meteorProj->GetCollider()->SetBoxSize(5.0f, 20.0f, 20.0f);

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
