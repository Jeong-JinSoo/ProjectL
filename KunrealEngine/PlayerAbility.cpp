#include "PlayerAbility.h"
#include "InputSystem.h"
#include "GraphicsSystem.h"
#include "MeshRenderer.h"
#include "BoxCollider.h"
#include "Particle.h"
#include "TimeManager.h"
#include "ToolBox.h"
#include "Animator.h"
#include "Transform.h"

#include "PlayerMove.h"
#include "Boss.h"
#include "Kamen.h"

#include "SceneManager.h"
#include "EventManager.h"
#include "Scene.h"

KunrealEngine::PlayerAbility::PlayerAbility()
	:_playerComp(nullptr), _meteor(nullptr), _shot(nullptr), _ice(nullptr), _area(nullptr)
	, _isIceReady(true), _destroyIce(false), _isShotReady(true), _isMeteorReady(true), _isAreaReady(true)
	, _isShotHit(false), _isIceHit(false), _isAreaHit(false), _isMeteorHit(false)
	, _currentBoss(nullptr), _currentDamage(0.0f)
	, _isShotDetected(false), _isIceDetected(false), _isAreaDetected(false), _isMeteorDetected(false), _isShotEnded(false) ,
	_shotParticleTimer(0), _isMeteorEnded(false), _meteorParticleTimer(0), _isIceEnded(false), _iceParticleTimer(0), _isLazerEnded(false), _lazerParticleTimer(0), _isLazerStarted(false)
{

}

KunrealEngine::PlayerAbility::~PlayerAbility()
{

}

void KunrealEngine::PlayerAbility::Initialize()
{
	_playerComp = GetOwner()->GetComponent<Player>();

	CreateAbility1();
	CreateAbility2();
	CreateAbility3();
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

	if (InputSystem::GetInstance()->KeyDown(KEY::Q) && this->_isShotReady)
	{
		ResetShotPos();
		Startcoroutine(shotCoolDown);
		_isShotDetected = true;
		_isShotReady = false;
		_isShotHit = true;
		_shot->SetActive(true);
		_shot->GetComponent<Projectile>()->SetActive(true);
		_shotParticle2->GetComponent<Particle>()->SetActive(true);
		_shotParticle3->GetComponent<Particle>()->SetActive(true);
		_shotParticle4->GetComponent<Particle>()->SetActive(true);
		_shot->GetComponent<Projectile>()->ResetCondition();

		_playerComp->_playerStatus = Player::Status::ABILITY;
		_playerComp->_abilityAnimationIndex = 1;				// ��ü ��ô �ִϸ��̼�
	}

	if (InputSystem::GetInstance()->KeyDown(KEY::W) && this->_isIceReady)
	{
		ResetIcePos();											// ����ü ��ġ ����
		_isIceDetected = true;
		_isIceHit = true;
		Startcoroutine(iceCoolDown);
		Startcoroutine(iceStandby);								// ���� ���� ���	
		
		_iceParticle1->GetComponent<Particle>()->SetActive(true);
		_iceParticle2->GetComponent<Particle>()->SetActive(true);
		_iceParticle3->GetComponent<Particle>()->SetActive(true);
		_iceParticleHit1->GetComponent<Transform>()->SetPosition(_ice->GetComponent<Transform>()->GetPosition());
		_iceParticleHit2->GetComponent<Transform>()->SetPosition(_ice->GetComponent<Transform>()->GetPosition());

		_iceParticleHit1->GetComponent<Particle>()->Update();
		_iceParticleHit2->GetComponent<Particle>()->Update();
		_iceParticle1->GetComponent<Particle>()->Update();
		_iceParticle2->GetComponent<Particle>()->Update();
		_iceParticle3->GetComponent<Particle>()->Update();

		_playerComp->_playerStatus = Player::Status::ABILITY;
		_playerComp->_abilityAnimationIndex = 2;				// ���� ��ȯ �ִϸ��̼�	
		Startcoroutine(iceDestroy);
	}

	if (InputSystem::GetInstance()->KeyDown(KEY::E) && this->_isAreaReady)
	{
		ResetAreaPos();
		_isAreaDetected = true;
		_isAreaHit = true;
		_isLazerStarted = true;
		Startcoroutine(AreaCoolDown);
		Startcoroutine(AreaStandby);
		//_area->GetComponent<BoxCollider>()->SetActive(true);
		//_playerComp->_playerStatus = Player::Status::ABILITY;
		//_playerComp->_abilityAnimationIndex = 3;				// ���� ���� �ִϸ��̼�


		Coroutine_Func(AreaDestroy)
		{
			auto* ability = this;
			Waitforsecond(3.0f);
			//_lazerParticle1->GetComponent<Particle>()->SetActive(false);
			//_lazerParticle2->GetComponent<Particle>()->SetActive(false);

			float delta = 0;
			while (true)
			{
				delta += TimeManager::GetInstance().GetDeltaTime();
				ability->_lazerParticle1->GetComponent<Particle>()->SetParticleSize((50 - (delta * 25)) * ToolBox::GetRandomFloat(0.8f, 1.0f), (50 - (delta * 25)) * ToolBox::GetRandomFloat(0.8f, 1.0f));
				ability->_lazerParticle2->GetComponent<Particle>()->SetParticleSize((20 - (delta * 10)) * ToolBox::GetRandomFloat(0.8f, 1.0f), (20 - (delta * 10)) * ToolBox::GetRandomFloat(0.8f, 1.0f));
				ability->_lazerParticle3->GetComponent<Particle>()->SetParticleSize((50 - (delta * 25)) * ToolBox::GetRandomFloat(0.8f, 1.0f), (50 - (delta * 25)) * ToolBox::GetRandomFloat(0.8f, 1.0f));
				ability->_lazerParticle4->GetComponent<Particle>()->SetParticleSize((50 - (delta * 25)) * ToolBox::GetRandomFloat(0.8f, 1.0f), (50 - (delta * 25)) * ToolBox::GetRandomFloat(0.8f, 1.0f));

				if (delta > 2) break;
				Return_null;
			}
			ability->_lazerParticle1->SetActive(false);
			ability->_lazerParticle2->SetActive(false);
			ability->_lazerParticle3->SetActive(false);
			ability->_lazerParticle4->SetActive(false);
			ability->_destroyArea = true;
			ability->_isAreaReady = false;
			ability->_isLazerStarted = false;

		};
		Startcoroutine(AreaDestroy);
	}

	if (InputSystem::GetInstance()->KeyDown(KEY::R) && this->_isMeteorReady)
	{
		ResetMeteorPos();
		_isMeteorDetected = true;
		_isMeteorHit = true;
		_isMeteorReady = false;
		Startcoroutine(meteorCoolDown);
		_playerComp->_playerStatus = Player::Status::ABILITY;
		_playerComp->_abilityAnimationIndex = 4;
	}

	/// �������� �ʿ�
	if (_playerComp->_playerStatus == Player::Status::ABILITY && _playerComp->_abilityAnimationIndex == 1 && GetOwner()->GetComponent<Animator>()->GetCurrentFrame() >= GetOwner()->GetComponent<Animator>()->GetMaxFrame())
	{
		_playerComp->_playerStatus = Player::Status::IDLE;
	}

	if (_playerComp->_playerStatus == Player::Status::ABILITY && _playerComp->_abilityAnimationIndex == 2 && GetOwner()->GetComponent<Animator>()->GetCurrentFrame() >= GetOwner()->GetComponent<Animator>()->GetMaxFrame())
	{
		_playerComp->_playerStatus = Player::Status::IDLE;
	}

	if (_playerComp->_playerStatus == Player::Status::ABILITY && _playerComp->_abilityAnimationIndex == 3 && GetOwner()->GetComponent<Animator>()->GetCurrentFrame() >= GetOwner()->GetComponent<Animator>()->GetMaxFrame())
	{
		_playerComp->_playerStatus = Player::Status::IDLE;
		this->_isAreaReady = false;
	}

	if (_playerComp->_playerStatus == Player::Status::ABILITY && _playerComp->_abilityAnimationIndex == 4 && GetOwner()->GetComponent<Animator>()->GetCurrentFrame() >= GetOwner()->GetComponent<Animator>()->GetMaxFrame())
	{
		_meteor->SetActive(true);
		_meteor->GetComponent<Particle>()->SetActive(true);
		_meteorParticle2->GetComponent<Particle>()->SetActive(true);
		_meteorParticle3->GetComponent<Particle>()->SetActive(true);
		_meteorParticle4->GetComponent<Particle>()->SetActive(true);
		_playerComp->_playerStatus = Player::Status::IDLE;
	}

	_abilityContainer[0]->_abilityLogic();
	_abilityContainer[1]->_abilityLogic();
	_abilityContainer[2]->_abilityLogic();
	_abilityContainer[3]->_abilityLogic();

	SetCurrentBossObject();

	if (this->_isShotReady)
	{
		GRAPHICS->DrawDebugText(100, 500, 40, "Q Ready");
	}
	else
	{
		GRAPHICS->DrawDebugText(100, 500, 40, "Q On CoolDown");
	}

	if (this->_isIceReady)
	{
		GRAPHICS->DrawDebugText(100, 600, 40, "W Ready");
	}
	else
	{
		GRAPHICS->DrawDebugText(100, 600, 40, "W On CoolDown");
	}

	if (this->_isAreaReady)
	{
		GRAPHICS->DrawDebugText(100, 700, 40, "E Ready");
	}
	else
	{
		GRAPHICS->DrawDebugText(100, 700, 40, "E On CoolDown");
	}

	if (this->_isMeteorReady)
	{
		GRAPHICS->DrawDebugText(100, 800, 40, "R Ready");
	}
	else
	{
		GRAPHICS->DrawDebugText(100, 800, 40, "R On CoolDown");
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

	_shot->GetComponent<BoxCollider>()->FixedUpdate();
	_shot->GetComponent<MeshRenderer>()->Update();
	_shot->GetComponent<Particle>()->Update();

	// ���⺤��
	DirectX::XMFLOAT3 currentPoint = _shot->GetComponent<Transform>()->GetPosition();
	DirectX::XMFLOAT3 targetPoint = GRAPHICS->ScreenToWorldPoint(InputSystem::GetInstance()->GetEditorMousePos().x, InputSystem::GetInstance()->GetEditorMousePos().y);

	DirectX::XMVECTOR direction = ToolBox::GetDirectionVec(currentPoint, targetPoint);

	DirectX::XMVECTOR currentPosVec = DirectX::XMLoadFloat3(&currentPoint);

	//�÷��̾� ���� ������
	DirectX::XMFLOAT3 playerPos = this->GetOwner()->GetComponent<Transform>()->GetPosition();
	DirectX::XMFLOAT3 playerRot = this->GetOwner()->GetComponent<Transform>()->GetRotation();
	DirectX::XMVECTOR playerPosVec = DirectX::XMLoadFloat3(&playerPos);
	DirectX::XMVECTOR rotPosVec = DirectX::XMLoadFloat3(&targetPoint);

	DirectX::XMVECTOR playerForward = DirectX::XMVectorSet(0.0f, playerRot.y, -1.0f, 0.0f);
	DirectX::XMVECTOR rotDirection = DirectX::XMVectorSubtract(rotPosVec, playerPosVec);
	rotDirection.m128_f32[1] = 0.0f;
	rotDirection = DirectX::XMVector3Normalize(rotDirection);

	DirectX::XMVECTOR dotResult = DirectX::XMVector3Dot(playerForward, rotDirection);
	float dotProduct = DirectX::XMVectorGetX(dotResult);

	float angle = acos(dotProduct);
	angle = DirectX::XMConvertToDegrees(angle);

	if (rotPosVec.m128_f32[0] > playerPosVec.m128_f32[0])
	{
		angle *= -1;
	}

	this->GetOwner()->GetComponent<Transform>()->SetRotation(0.0f, angle, 0.0f);

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
		2.0f,			// ��Ÿ��
		10.0f			// ��Ÿ�
	);

	// ��ü ����
	_shot = shot->_projectile;

	// ũ�� ����
	_shot->GetComponent<Transform>()->SetScale(0.3f, 0.3, 0.3f);

	// ����ü ������Ʈ �߰�
	_shot->AddComponent<Projectile>();
	Projectile* shotProj = _shot->GetComponent<Projectile>();

	// ��ƼŬ �߰�	// ������
	Particle* shotParticle = _shot->AddComponent<Particle>();
	shotParticle->SetParticleEffect("BlastWave2", "Resources/Textures/Particles/fx_BlastWave2.dds", 1000);
	shotParticle->SetParticleSize(7.f, 7.0f);
	shotParticle->SetParticleVelocity(3.0f, true);
	shotParticle->SetParticleDirection(0.0f, 0.0f, 0.0f);
	shotParticle->AddParticleColor(0.5f, 0.5f, 2.0f);
	//shotParticle->SetParticleRotation(90.0f, _shot->GetComponent<Transform>()->GetRotation().y, 0.0f);
	shotParticle->SetParticleDuration(0.7f, 0.05f);
	shotParticle->SetActive(true);

	_shotParticle2 = SceneManager::GetInstance().GetCurrentScene()->CreateObject("PlayerQ2");
	_shotParticle2->AddComponent<Particle>();
	_shotParticle2->GetComponent<Particle>()->SetParticleEffect("BlastWave3", "Resources/Textures/Particles/fx_BlastWave3.dds", 1000);
	_shotParticle2->GetComponent<Particle>()->SetParticleDuration(1.0f, 0.05f);
	_shotParticle2->GetComponent<Particle>()->SetParticleVelocity(7.0f, true);
	_shotParticle2->GetComponent<Particle>()->SetParticleSize(10.f, 10.0f);
	_shotParticle2->GetComponent<Particle>()->AddParticleColor(0.0f, 1.5f, 10.f);
	_shotParticle2->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 0.0f);
	_shotParticle2->GetComponent<Particle>()->SetActive(true);
	_shotParticle2->SetActive(true);
	_shotParticle2->SetParent(_shot);

	_shotParticle3 = SceneManager::GetInstance().GetCurrentScene()->CreateObject("PlayerQ3");
	_shotParticle3->AddComponent<Particle>();
	_shotParticle3->GetComponent<Particle>()->SetParticleEffect("Cracks1", "Resources/Textures/Particles/fx_Cracks1.dds", 1000);
	_shotParticle3->GetComponent<Particle>()->SetParticleDuration(1.0f, 0.05f);
	_shotParticle3->GetComponent<Particle>()->SetParticleVelocity(200.0f, true);
	_shotParticle3->GetComponent<Particle>()->SetParticleSize(4.0f, 4.0f);
	_shotParticle3->GetComponent<Particle>()->AddParticleColor(0.0f, 1.5f, 10.f);
	_shotParticle3->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 0.0f);
	_shotParticle3->GetComponent<Particle>()->SetActive(true);
	_shotParticle3->SetActive(true);
	_shotParticle3->SetParent(_shot);

	_shotParticle4 = SceneManager::GetInstance().GetCurrentScene()->CreateObject("PlayerQ4");
	_shotParticle4->AddComponent<Particle>();
	_shotParticle4->GetComponent<Particle>()->SetParticleEffect("EnergyBolt1", "Resources/Textures/Particles/fx_EnergyBolt1.dds", 1000);
	_shotParticle4->GetComponent<Particle>()->SetParticleDuration(1.0f, 0.05f);
	_shotParticle4->GetComponent<Particle>()->SetParticleVelocity(0.0f, true);
	_shotParticle4->GetComponent<Particle>()->SetParticleSize(1.0f, 1.0f);
	_shotParticle4->GetComponent<Particle>()->AddParticleColor(0.0f, 1.5f, 10.f);
	_shotParticle4->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 0.0f);
	_shotParticle4->GetComponent<Particle>()->SetActive(true);
	_shotParticle4->SetActive(true);
	_shotParticle4->SetParent(_shot);

	_shotParticleHit1 = SceneManager::GetInstance().GetCurrentScene()->CreateObject("ShotParticleHit1");
	_shotParticleHit1->AddComponent<Particle>();
	_shotParticleHit1->GetComponent<Particle>()->SetParticleEffect("BlastWave1", "Resources/Textures/Particles/fx_BlastWave1.dds", 1000);
	_shotParticleHit1->GetComponent<Particle>()->SetParticleDuration(0.1f, 0.1f);
	_shotParticleHit1->GetComponent<Particle>()->SetParticleVelocity(100.0f, true);
	_shotParticleHit1->GetComponent<Particle>()->SetParticleSize(5.f, 5.0f);
	_shotParticleHit1->GetComponent<Particle>()->AddParticleColor(0.0f, 0.5f, 1.0f);
	_shotParticleHit1->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 0.0f);
	_shotParticleHit1->GetComponent<Particle>()->SetActive(false);
	_shotParticleHit1->SetActive(true);
	//_shotParticleHit1->SetParent(_shot);

	_shotParticleHit2 = SceneManager::GetInstance().GetCurrentScene()->CreateObject("ShotParticleHit2");
	_shotParticleHit2->AddComponent<Particle>();
	_shotParticleHit2->GetComponent<Particle>()->SetParticleEffect("Cracks1", "Resources/Textures/Particles/fx_Cracks1.dds", 1000);
	_shotParticleHit2->GetComponent<Particle>()->SetParticleDuration(1.0f, 0.1f);
	_shotParticleHit2->GetComponent<Particle>()->SetParticleVelocity(150.0f, true);
	_shotParticleHit2->GetComponent<Particle>()->SetParticleSize(3.0f, 3.0f);
	_shotParticleHit2->GetComponent<Particle>()->AddParticleColor(0.0f, 0.5f, 2.0f);
	_shotParticleHit2->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 0.0f);
	_shotParticleHit2->GetComponent<Particle>()->SetActive(false);
	_shotParticleHit2->SetActive(true);
	//_shotParticleHit2->SetParent(_shot);

	_shotParticleHit3 = SceneManager::GetInstance().GetCurrentScene()->CreateObject("ShotParticleHit3");
	_shotParticleHit3->AddComponent<Particle>();
	_shotParticleHit3->GetComponent<Particle>()->SetParticleEffect("Halo2", "Resources/Textures/Particles/fx_Halo2.dds", 1000);
	_shotParticleHit3->GetComponent<Particle>()->SetParticleDuration(0.1f, 0.1f);
	_shotParticleHit3->GetComponent<Particle>()->SetParticleVelocity(100.0f, true);
	_shotParticleHit3->GetComponent<Particle>()->SetParticleSize(15.f, 15.0f);
	_shotParticleHit3->GetComponent<Particle>()->AddParticleColor(0.0f, 1.0f, 5.0f);
	_shotParticleHit3->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 0.0f);
	_shotParticleHit3->GetComponent<Particle>()->SetActive(false);
	_shotParticleHit3->SetActive(true);
	//_shotParticleHit3->SetParent(_shot);


	shotProj->SetMeshObject("Meteor/Meteor");

	// Q ��ų�� �޽��� �� ���̰�
	_shot->GetComponent<MeshRenderer>()->SetAlpha(0.0f);

	shotProj->GetCollider()->SetColliderScale(5.0f, 5.0f, 5.0f);
	shotProj->SetDestoryCondition([shot, shotProj, this]()->bool
		{
			if (shotProj->GetCollider()->IsCollided() && shotProj->GetCollider()->GetTargetObject()->GetTag() == "Boss")
			{
				//if (_isShotHit)
				//{
				//	_currentBoss->_info._hp -= 10.0f;
				//	_isShotHit = false;
				//}

				return true;
			}
			else
			{
				return false;
			}
		});

	_shot->SetActive(false);

	shot->SetLogic([shot, shotProj, this]()  // �θ� ��Ȱ��ȭ �Ǳ����� ��ƼŬ�� ����� ��ġ�� ���� �� ����
		{
			if (_shot->GetActivated())
			{
				DirectX::XMFLOAT3 currentPoint = _shot->GetComponent<Transform>()->GetPosition();

				DirectX::XMVECTOR currentPosVec = DirectX::XMLoadFloat3(&currentPoint);
				DirectX::XMVECTOR newPosition = DirectX::XMVectorAdd(currentPosVec, DirectX::XMVectorScale(shotProj->GetDirection(), 50.0f * TimeManager::GetInstance().GetDeltaTime()));

				_shot->GetComponent<Transform>()->SetPosition(newPosition.m128_f32[0], 5.0f, newPosition.m128_f32[2]);
				shotProj->_movedRange += DirectX::XMVectorGetX(DirectX::XMVector3Length(DirectX::XMVectorSubtract(newPosition, currentPosVec))); 
				
				if (shotProj->GetCollider()->IsCollided() && shotProj->GetCollider()->GetTargetObject()->GetTag() == "Boss")
				{
					//_shotParticleHit1->GetComponent<Particle>()->SetActive(true);
					//_shotParticleHit2->GetComponent<Particle>()->SetActive(true);
					//_shotParticleHit3->GetComponent<Particle>()->SetActive(true);
					_isShotEnded = true;
				}
			}
			
			
			if(!(_shot->GetActivated()) && _isShotEnded == true)
			{
				_shotParticleTimer += TimeManager::GetInstance().GetDeltaTime();
				_shotParticleHit1->GetComponent<Particle>()->SetActive(true);
				_shotParticleHit2->GetComponent<Particle>()->SetActive(true);
				_shotParticleHit3->GetComponent<Particle>()->SetActive(true);
				_shotParticleHit1->GetComponent<Transform>()->SetPosition(_shot->GetComponent<Transform>()->GetPosition());
				_shotParticleHit2->GetComponent<Transform>()->SetPosition(_shot->GetComponent<Transform>()->GetPosition());
				_shotParticleHit3->GetComponent<Transform>()->SetPosition(_shot->GetComponent<Transform>()->GetPosition());

				_shotParticleHit1->GetComponent<Particle>()->SetParticleSize(_shotParticleTimer * 40, _shotParticleTimer * 40);
				_shotParticleHit3->GetComponent<Particle>()->SetParticleSize(_shotParticleTimer * 120, _shotParticleTimer * 120);

				if (_shotParticleTimer > 0.2f)
				{
					_shotParticleHit1->GetComponent<Particle>()->SetActive(false);
					_shotParticleHit2->GetComponent<Particle>()->SetActive(false);
					_shotParticleHit3->GetComponent<Particle>()->SetActive(false);
					_shotParticleTimer = 0.0f;
					_isShotEnded = false;
				}
			}
		});

	AddToContanier(shot);
}


void KunrealEngine::PlayerAbility::ResetIcePos()
{
	// ���콺 3D��ǥ�ο� ��ġ��Ű����
	DirectX::XMFLOAT3 newPos = GRAPHICS->ScreenToWorldPoint(InputSystem::GetInstance()->GetEditorMousePos().x, InputSystem::GetInstance()->GetEditorMousePos().y);
	_ice->GetComponent<Transform>()->SetPosition
	(
		newPos.x,
		this->_playerComp->GetOwner()->GetComponent<Transform>()->GetPosition().y,
		newPos.z
	);

	//�÷��̾� ���� ������
	DirectX::XMFLOAT3 playerPos = this->GetOwner()->GetComponent<Transform>()->GetPosition();
	DirectX::XMFLOAT3 playerRot = this->GetOwner()->GetComponent<Transform>()->GetRotation();
	DirectX::XMVECTOR playerPosVec = DirectX::XMLoadFloat3(&playerPos);
	DirectX::XMVECTOR rotPosVec = DirectX::XMLoadFloat3(&newPos);

	DirectX::XMVECTOR playerForward = DirectX::XMVectorSet(0.0f, playerRot.y, -1.0f, 0.0f);
	DirectX::XMVECTOR rotDirection = DirectX::XMVectorSubtract(rotPosVec, playerPosVec);
	rotDirection.m128_f32[1] = 0.0f;
	rotDirection = DirectX::XMVector3Normalize(rotDirection);

	DirectX::XMVECTOR dotResult = DirectX::XMVector3Dot(playerForward, rotDirection);
	float dotProduct = DirectX::XMVectorGetX(dotResult);

	float angle = acos(dotProduct);
	angle = DirectX::XMConvertToDegrees(angle);

	if (rotPosVec.m128_f32[0] > playerPosVec.m128_f32[0])
	{
		angle *= -1;
	}

	this->GetOwner()->GetComponent<Transform>()->SetRotation(0.0f, angle, 0.0f);

	// ��ġ ����
	_ice->GetComponent<BoxCollider>()->Update();
	_ice->GetComponent<MeshRenderer>()->Update();
}

void KunrealEngine::PlayerAbility::CreateAbility2()
{
	Ability* ice = new Ability();
	ice->Initialize("Ice");
	
	ice->SetTotalData(
		"Ice",			// �̸�
		20.0f,			// ������
		15.0f,			// ����
		10.0f,			// ����ȭ ���ط�
		5.0f,			// ��Ÿ��
		12.0f			// ��Ÿ�
	);
	
	_ice = ice->_projectile;
	
	// ũ�� ����
	_ice->GetComponent<Transform>()->SetScale(30.0f, 30.0f, 30.0f);
	_ice->GetComponent<Transform>()->SetRotation(90.0f, 0.0f, 0.0f);
	
	_iceParticle1 = SceneManager::GetInstance().GetCurrentScene()->CreateObject("IceParticle1");
	_iceParticle1->AddComponent<Particle>();
	_iceParticle1->GetComponent<Particle>()->SetParticleEffect("Blast3", "Resources/Textures/Particles/fx_Blast3.dds", 1000);
	_iceParticle1->GetComponent<Particle>()->SetParticleDuration(3.0f, 2.0f);
	_iceParticle1->GetComponent<Particle>()->SetParticleVelocity(8.0f, true);
	_iceParticle1->GetComponent<Particle>()->SetParticleSize(5.f, 5.0f);
	_iceParticle1->GetComponent<Particle>()->AddParticleColor(0.5f, 3.0f, 5.0f);
	_iceParticle1->GetComponent<Particle>()->SetParticleDirection(0.0f, -1.0f, 0.0f);
	_iceParticle1->GetComponent<Particle>()->SetParticleAngle(339.0f, 0.0f, 30.0f);
	_iceParticle1->GetComponent<Particle>()->SetParticleCameraApply(true);
	_iceParticle1->GetComponent<Particle>()->SetActive(false);

	_iceParticle2 = SceneManager::GetInstance().GetCurrentScene()->CreateObject("IceParticle2");
	_iceParticle2->AddComponent<Particle>();
	_iceParticle2->GetComponent<Particle>()->SetParticleEffect("Dust3", "Resources/Textures/Particles/fx_Dust3.dds", 1000);
	_iceParticle2->GetComponent<Particle>()->SetParticleDuration(3.0f, 2.0f);
	_iceParticle2->GetComponent<Particle>()->SetParticleVelocity(8.0f, true);
	_iceParticle2->GetComponent<Particle>()->SetParticleSize(5.0f, 5.0f);
	_iceParticle2->GetComponent<Particle>()->AddParticleColor(0.5f, 3.0f, 5.0f);
	_iceParticle2->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 0.0f);
	_iceParticle2->GetComponent<Particle>()->SetParticleAngle(339.0f, 0.0f, 30.0f);
	_iceParticle2->GetComponent<Particle>()->SetParticleCameraApply(true);
	_iceParticle2->GetComponent<Particle>()->SetActive(false);

	_iceParticle3 = SceneManager::GetInstance().GetCurrentScene()->CreateObject("IceParticle3");
	_iceParticle3->AddComponent<Particle>();
	_iceParticle3->GetComponent<Particle>()->SetParticleEffect("Sparks1", "Resources/Textures/Particles/fx_Sparks1.dds", 1000);
	_iceParticle3->GetComponent<Particle>()->SetParticleDuration(1.0f, 0.1f);
	_iceParticle3->GetComponent<Particle>()->SetParticleVelocity(1.0f, true);
	_iceParticle3->GetComponent<Particle>()->SetParticleSize(30.f, 30.0f);
	_iceParticle3->GetComponent<Particle>()->AddParticleColor(0.0f, 1.0f, 2.0f);
	_iceParticle3->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 0.0f);
	_iceParticle3->GetComponent<Particle>()->SetParticleCameraApply(true);
	_iceParticle3->GetComponent<Particle>()->SetActive(false);

	_iceParticleHit1 = SceneManager::GetInstance().GetCurrentScene()->CreateObject("IceParticleHit1");
	_iceParticleHit1->AddComponent<Particle>();
	_iceParticleHit1->GetComponent<Particle>()->SetParticleEffect("Dust3", "Resources/Textures/Particles/fx_Dust3.dds", 100);
	_iceParticleHit1->GetComponent<Particle>()->SetParticleDuration(4.0f, 0.5f);
	_iceParticleHit1->GetComponent<Particle>()->SetParticleVelocity(10.0f, true);
	_iceParticleHit1->GetComponent<Particle>()->SetParticleSize(25.0f, 25.0f);
	_iceParticleHit1->GetComponent<Particle>()->AddParticleColor(0.5f, 3.0f, 5.0f);
	_iceParticleHit1->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 0.0f);
	_iceParticleHit1->GetComponent<Particle>()->SetParticleAngle(0.0f, 0.0f, 0.0f);
	_iceParticleHit1->GetComponent<Particle>()->SetActive(false);

	_iceParticleHit2 = SceneManager::GetInstance().GetCurrentScene()->CreateObject("IceParticleHit2");
	_iceParticleHit2->AddComponent<Particle>();
	_iceParticleHit2->GetComponent<Particle>()->SetParticleEffect("Sparks1", "Resources/Textures/Particles/fx_Sparks1.dds", 100);
	_iceParticleHit2->GetComponent<Particle>()->SetParticleDuration(1.0f, 0.1f);
	_iceParticleHit2->GetComponent<Particle>()->SetParticleVelocity(1.0f, true);
	_iceParticleHit2->GetComponent<Particle>()->SetParticleSize(30.f, 30.0f);
	_iceParticleHit2->GetComponent<Particle>()->AddParticleColor(0.0f, 1.0f, 2.0f);
	_iceParticleHit2->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 0.0f);
	_iceParticleHit2->GetComponent<Particle>()->SetParticleCameraApply(true);
	_iceParticleHit2->GetComponent<Particle>()->SetActive(false);

	// ����ü ������Ʈ �߰�
	_ice->AddComponent<Projectile>();
	Projectile* iceProj = _ice->GetComponent<Projectile>();
	
	iceProj->SetMeshObject("Ice/Ice", "Ice/Ice.png");
	iceProj->GetCollider()->SetColliderScale(15.0f, 15.0f, 15.0f);
	iceProj->SetDestoryCondition([iceProj, this]()->bool
		{
			if (this->_destroyIce)
			{
				return true;
			}
			else
			{
				return false;
			}
		});
	
	_ice->SetActive(false);
	
	ice->SetLogic([ice, iceProj, this]()
		{
			_iceParticle1->GetComponent<Transform>()->SetPosition(_ice->GetComponent<Transform>()->GetPosition());
			_iceParticle2->GetComponent<Transform>()->SetPosition(_ice->GetComponent<Transform>()->GetPosition());
			_iceParticle3->GetComponent<Transform>()->SetPosition(_ice->GetComponent<Transform>()->GetPosition());

			if (iceProj->GetCollider()->GetTargetObject() != nullptr && iceProj->GetCollider()->GetTargetObject()->GetTag() == "BOSS" && _isIceHit)
			{
				_currentBoss->_info._hp -= 30.0f;
				_isIceHit = false;
			}
		});
	
	AddToContanier(ice);
}


void KunrealEngine::PlayerAbility::ResetAreaPos()
{
	this->_area->GetComponent<Transform>()->SetPosition(this->GetOwner()->GetComponent<Transform>()->GetPosition());
	_lazerParticle1->GetComponent<Particle>()->Update();
	_lazerParticle2->GetComponent<Particle>()->Update();
}

void KunrealEngine::PlayerAbility::CreateAbility3()
{
	Ability* area = new Ability();
	area->Initialize("Area");

	area->SetTotalData(
		"Area",			// �̸�
		40.0f,			// ������
		20.0f,			// ����
		15.0f,			// ����ȭ ���ط�
		7.0f,			// ��Ÿ��
		15.0f			// ��Ÿ�
	);

	_area = area->_projectile;

	// ũ�� ����			/// ���� ���� �ʿ�
	_area->GetComponent<Transform>()->SetScale(20.0f, 20.0f, 20.0f);
	_area->GetComponent<Transform>()->SetRotation(90.0f, 0.0f, 0.0f);

	// ����ü ������Ʈ �߰�
	_area->AddComponent<BoxCollider>();
	BoxCollider* areaCollider = _area->GetComponent<BoxCollider>();
	areaCollider->SetColliderScale(20.0f, 20.0f, 20.0f);
		
	_lazerParticle1 = SceneManager::GetInstance().GetCurrentScene()->CreateObject("PlayerE1");
	_lazerParticle1->AddComponent<Particle>();
	_lazerParticle1->GetComponent<Particle>()->SetParticleEffect("fx_SmokeyHalo1", "Resources/Textures/Particles/fx_SmokeyHalo1.dds", 1000);
	_lazerParticle1->GetComponent<Particle>()->SetParticleDuration(3.0f, 2.0f);
	_lazerParticle1->GetComponent<Particle>()->SetParticleVelocity(84.f, false);
	_lazerParticle1->GetComponent<Particle>()->SetParticleSize(50.f, 50.0f);
	_lazerParticle1->GetComponent<Particle>()->AddParticleColor(0.0f, 0.5f, 1.0f);
	_lazerParticle1->GetComponent<Particle>()->SetParticleRotation(90.0f, _playerComp->_transform->GetRotation().y, 0.0f);
	_lazerParticle1->GetComponent<Particle>()->SetParticleCameraApply(true);
	_lazerParticle1->GetComponent<Particle>()->SetParticleAngle(339, 0.0f, 30);
	_lazerParticle1->SetTotalComponentState(false);
	_lazerParticle1->SetActive(false);

	_lazerParticle2 = SceneManager::GetInstance().GetCurrentScene()->CreateObject("PlayerE2");
	_lazerParticle2->AddComponent<Particle>();
	_lazerParticle2->GetComponent<Particle>()->SetParticleEffect("Laser", "Resources/Textures/Particles/fx_Dust2.dds", 1000);
	_lazerParticle2->GetComponent<Particle>()->SetParticleDuration(3.0f, 2.0f);
	_lazerParticle2->GetComponent<Particle>()->SetParticleVelocity(84.f, false);
	_lazerParticle2->GetComponent<Particle>()->SetParticleSize(20.f, 20.f);
	_lazerParticle2->GetComponent<Particle>()->AddParticleColor(1.0f, 1.0f, 0.0f);
	_lazerParticle2->GetComponent<Particle>()->SetParticleRotation(90.0f, _playerComp->_transform->GetRotation().y, 0.0f);
	_lazerParticle2->GetComponent<Particle>()->SetParticleCameraApply(true);
	_lazerParticle2->GetComponent<Particle>()->SetParticleAngle(339, 0.0f, 30);
	_lazerParticle2->SetTotalComponentState(false);
	_lazerParticle2->SetActive(false);

	_lazerParticle3 = SceneManager::GetInstance().GetCurrentScene()->CreateObject("PlayerE3");
	_lazerParticle3->AddComponent<Particle>();
	_lazerParticle3->GetComponent<Particle>()->SetParticleEffect("fx_BlastWave5", "Resources/Textures/Particles/fx_BlastWave5.dds", 1000);
	_lazerParticle3->GetComponent<Particle>()->SetParticleDuration(3.0f, 2.0f);
	_lazerParticle3->GetComponent<Particle>()->SetParticleVelocity(84.f, false);
	_lazerParticle3->GetComponent<Particle>()->SetParticleSize(30.f, 30.f);
	_lazerParticle3->GetComponent<Particle>()->AddParticleColor(1.0f, 1.0f, 0.3f);
	_lazerParticle3->GetComponent<Particle>()->SetParticleRotation(90.0f, _playerComp->_transform->GetRotation().y, 0.0f);
	_lazerParticle3->GetComponent<Particle>()->SetParticleCameraApply(true);
	_lazerParticle3->GetComponent<Particle>()->SetParticleAngle(339, 0.0f, 30);
	_lazerParticle3->SetTotalComponentState(false);
	_lazerParticle3->SetActive(false);

	_lazerParticle4 = SceneManager::GetInstance().GetCurrentScene()->CreateObject("PlayerE4");
	_lazerParticle4->AddComponent<Particle>();
	_lazerParticle4->GetComponent<Particle>()->SetParticleEffect("fx_Flare7", "Resources/Textures/Particles/fx_Flare7.dds", 1000);
	_lazerParticle4->GetComponent<Particle>()->SetParticleDuration(3.0f, 0.1f);
	_lazerParticle4->GetComponent<Particle>()->SetParticleVelocity(0.f, true);
	_lazerParticle4->GetComponent<Particle>()->SetParticleSize(30.f, 30.f);
	_lazerParticle4->GetComponent<Particle>()->AddParticleColor(1.0f, 1.0f, 0.3f);
	_lazerParticle4->GetComponent<Particle>()->SetParticleRotation(90.0f, _playerComp->_transform->GetRotation().y, 0.0f);
	_lazerParticle4->GetComponent<Particle>()->SetParticleCameraApply(true);
	_lazerParticle4->GetComponent<Particle>()->SetParticleAngle(339, 0.0f, 30);
	_lazerParticle4->SetTotalComponentState(false);
	_lazerParticle4->SetActive(false);

	_area->SetActive(false);

	area->SetLogic([area, areaCollider, this]()
		{
			if (_isLazerStarted == true)
			{
				auto lazerPosOffset = 15.0f;
				auto lazerScaleOffset = 80.0f;

				auto direction = ToolBox::RotateVector(DirectX::XMFLOAT3(0, 0, -1.0f), _playerComp->_transform->GetRotation().y);

				DirectX::XMVECTOR dirVec = DirectX::XMLoadFloat3(&direction);

				auto lazerScaleDir = DirectX::XMVectorScale(dirVec, lazerPosOffset);
				_lazerParticle1->GetComponent<Transform>()->SetPosition(_playerComp->_transform->GetPosition().x + lazerScaleDir.m128_f32[0], 16.0f, _playerComp->_transform->GetPosition().z + lazerScaleDir.m128_f32[2]);
				_lazerParticle2->GetComponent<Transform>()->SetPosition(_playerComp->_transform->GetPosition().x + lazerScaleDir.m128_f32[0], 16.0f, _playerComp->_transform->GetPosition().z + lazerScaleDir.m128_f32[2]);
				_lazerParticle3->GetComponent<Transform>()->SetPosition(_playerComp->_transform->GetPosition().x + lazerScaleDir.m128_f32[0], 16.0f, _playerComp->_transform->GetPosition().z + lazerScaleDir.m128_f32[2]);
				_lazerParticle4->GetComponent<Transform>()->SetPosition(_playerComp->_transform->GetPosition().x + lazerScaleDir.m128_f32[0], 16.0f, _playerComp->_transform->GetPosition().z + lazerScaleDir.m128_f32[2]);
				
				_lazerParticle1->GetComponent<Particle>()->SetParticleRotation(90.0f, _playerComp->_transform->GetRotation().y, 0.0f);
				_lazerParticle2->GetComponent<Particle>()->SetParticleRotation(90.0f, _playerComp->_transform->GetRotation().y, 0.0f);
				_lazerParticle3->GetComponent<Particle>()->SetParticleRotation(90.0f, _playerComp->_transform->GetRotation().y, 0.0f);
				_lazerParticle4->GetComponent<Particle>()->SetParticleRotation(0.0f, _playerComp->_transform->GetRotation().y, 0.0f);
			
				_lazerParticle1->GetComponent<Particle>()->SetParticleSize(50.f * ToolBox::GetRandomFloat(0.8f, 1.0f), 50.f * ToolBox::GetRandomFloat(0.8f, 1.0f));
				_lazerParticle2->GetComponent<Particle>()->SetParticleSize(30.f * ToolBox::GetRandomFloat(0.8f, 1.0f), 30.f * ToolBox::GetRandomFloat(0.8f, 1.0f));
				_lazerParticle3->GetComponent<Particle>()->SetParticleSize(60.f * ToolBox::GetRandomFloat(0.8f, 1.0f), 60.f * ToolBox::GetRandomFloat(0.8f, 1.0f));
				_lazerParticle4->GetComponent<Particle>()->SetParticleSize(50.f * ToolBox::GetRandomFloat(0.8f, 1.0f), 50.f * ToolBox::GetRandomFloat(0.8f, 1.0f));

				_lazerParticle1->GetComponent<Particle>()->Update();
				_lazerParticle2->GetComponent<Particle>()->Update();
				_lazerParticle3->GetComponent<Particle>()->Update();
				_lazerParticle4->GetComponent<Particle>()->Update();

				_lazerParticle1->GetComponent<Particle>()->SetActive(true);
				_lazerParticle2->GetComponent<Particle>()->SetActive(true);
				_lazerParticle3->GetComponent<Particle>()->SetActive(true);
				_lazerParticle4->GetComponent<Particle>()->SetActive(true);
			}

			if (areaCollider->GetTargetObject() != nullptr && areaCollider->GetActivated() && areaCollider->IsCollided() && areaCollider->GetTargetObject()->GetTag() == "BOSS" && this->_isAreaHit)
			{
				_currentBoss->_info._hp -= 20.0f;

				this->_isAreaHit = false;
			}
		});

	AddToContanier(area);
}

void KunrealEngine::PlayerAbility::ResetMeteorPos()
{
	// ���콺 3D��ǥ�κ��� 20.0f ���� ��ġ��Ű����
	DirectX::XMFLOAT3 newPos = GRAPHICS->ScreenToWorldPoint(InputSystem::GetInstance()->GetEditorMousePos().x, InputSystem::GetInstance()->GetEditorMousePos().y);

	_meteor->GetComponent<Transform>()->SetPosition(newPos);
	_meteor->GetComponent<Transform>()->SetPosition
	(
		_meteor->GetComponent<Transform>()->GetPosition().x,
		_meteor->GetComponent<Transform>()->GetPosition().y + 80.0f,
		_meteor->GetComponent<Transform>()->GetPosition().z
	);

	//�÷��̾� ���� ������
	DirectX::XMFLOAT3 playerPos = this->GetOwner()->GetComponent<Transform>()->GetPosition();
	DirectX::XMFLOAT3 playerRot = this->GetOwner()->GetComponent<Transform>()->GetRotation();
	DirectX::XMVECTOR playerPosVec = DirectX::XMLoadFloat3(&playerPos);
	DirectX::XMVECTOR rotPosVec = DirectX::XMLoadFloat3(&newPos);

	DirectX::XMVECTOR playerForward = DirectX::XMVectorSet(0.0f, playerRot.y, -1.0f, 0.0f);
	DirectX::XMVECTOR rotDirection = DirectX::XMVectorSubtract(rotPosVec, playerPosVec);
	rotDirection.m128_f32[1] = 0.0f;
	rotDirection = DirectX::XMVector3Normalize(rotDirection);

	DirectX::XMVECTOR dotResult = DirectX::XMVector3Dot(playerForward, rotDirection);
	float dotProduct = DirectX::XMVectorGetX(dotResult);

	float angle = acos(dotProduct);
	angle = DirectX::XMConvertToDegrees(angle);

	if (rotPosVec.m128_f32[0] > playerPosVec.m128_f32[0])
	{
		angle *= -1;
	}

	this->GetOwner()->GetComponent<Transform>()->SetRotation(0.0f, angle, 0.0f);
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
		0.2f,			// ��Ÿ��
		15.0f			// ��Ÿ�
	);

	// ��ü ����
	_meteor = meteor->_projectile;

	// �츮 �������� ������ �̻ڴ�
	_meteor->GetComponent<Transform>()->SetRotation(0.0f, 0.0f, 90.0f);

	// ����ü ������Ʈ �߰�
	_meteor->AddComponent<Projectile>();
	Projectile* meteorProj = _meteor->GetComponent<Projectile>();

	meteorProj->SetMeshObject("Meteor/Meteor");
	meteorProj->GetCollider()->SetColliderScale(5.0f, 20.0f, 20.0f);

	// ��ƼŬ �߰�	// ��Ÿ������
	Particle* meteorParticle = _meteor->AddComponent<Particle>();
	//Particle* meteorParticle = _meteor->GetComponent<Particle>();
	meteorParticle->SetParticleEffect("BlastWave2", "Resources/Textures/Particles/fx_BlastWave2.dds", 1000);
	meteorParticle->SetParticleSize(17.f, 20.0f);
	meteorParticle->SetParticleVelocity(3.0f, true);
	meteorParticle->SetParticleDuration(1.5f, 1.5f);
	meteorParticle->AddParticleColor(6.0f, 0.2f, 0.1f);
	meteorParticle->SetParticleDirection(0.0f, 50.0f, 0.0f);
	meteorParticle->SetActive(true);

	_meteorParticle2 = SceneManager::GetInstance().GetCurrentScene()->CreateObject("PlayerR2");
	_meteorParticle2->AddComponent<Particle>();
	_meteorParticle2->GetComponent<Particle>()->SetParticleEffect("BlastWave3", "Resources/Textures/Particles/fx_BlastWave3.dds", 1000);
	_meteorParticle2->GetComponent<Particle>()->SetParticleDuration(2.0f, 1.4f);
	_meteorParticle2->GetComponent<Particle>()->SetParticleVelocity(6.5f, true);
	_meteorParticle2->GetComponent<Particle>()->SetParticleSize(17.f, 20.0f);
	_meteorParticle2->GetComponent<Particle>()->AddParticleColor(2.0f, 1.0f, 0.0f);
	_meteorParticle2->GetComponent<Particle>()->SetParticleDirection(0.0f, 50.0f, 0.0f);
	_meteorParticle2->SetParent(_meteor);
	

	_meteorParticle3 = SceneManager::GetInstance().GetCurrentScene()->CreateObject("PlayerR3");
	_meteorParticle3->GetComponent<Transform>()->SetPosition(0, 10.0f, 0);
	_meteorParticle3->AddComponent<Particle>();
	_meteorParticle3->GetComponent<Particle>()->SetParticleEffect("Fire1", "Resources/Textures/Particles/fx_Fire1.dds", 1000);
	_meteorParticle3->GetComponent<Particle>()->SetParticleDuration(1.0f, 0.6f);
	_meteorParticle3->GetComponent<Particle>()->SetParticleVelocity(6.0f, true);
	_meteorParticle3->GetComponent<Particle>()->SetParticleSize(13.f, 13.0f);
	_meteorParticle3->GetComponent<Particle>()->AddParticleColor(1.0f, 0.1f, 0.1f);
	_meteorParticle3->GetComponent<Particle>()->SetParticleDirection(0.0f, 100.0f, 0.0f);
	_meteorParticle3->SetParent(_meteor);

	_meteorParticle4 = SceneManager::GetInstance().GetCurrentScene()->CreateObject("PlayerR4");
	_meteorParticle4->GetComponent<Transform>()->SetPosition(0, 10.f, 0.f);
	_meteorParticle4->AddComponent<Particle>();
	_meteorParticle4->GetComponent<Particle>()->SetParticleEffect("Fire1", "Resources/Textures/Particles/fx_Fire1.dds", 1000);
	_meteorParticle4->GetComponent<Particle>()->SetParticleDuration(1.0f, 0.6f);
	_meteorParticle4->GetComponent<Particle>()->SetParticleVelocity(6.0f, true);
	_meteorParticle4->GetComponent<Particle>()->SetParticleSize(1.f, 1.0f);
	_meteorParticle4->GetComponent<Particle>()->AddParticleColor(1.0f, 0.1f, 0.1f);
	_meteorParticle4->GetComponent<Particle>()->SetParticleDirection(0.0f, 100.0f, 0.0f);
	_meteorParticle4->SetParent(_meteor);

	// ��ź ��ƼŬ

	_meteorParticleHit1 = SceneManager::GetInstance().GetCurrentScene()->CreateObject("MeteorParticleHit1");
	_meteorParticleHit1->AddComponent<Particle>();
	_meteorParticleHit1->GetComponent<Particle>()->SetParticleEffect("Flare6", "Resources/Textures/Particles/fx_Twister3.dds", 1000);
	_meteorParticleHit1->GetComponent<Particle>()->SetParticleDuration(2.4f, 2.0f);
	_meteorParticleHit1->GetComponent<Particle>()->SetParticleVelocity(1.0f, true);
	_meteorParticleHit1->GetComponent<Particle>()->SetParticleSize(1.f, 1.f);
	_meteorParticleHit1->GetComponent<Particle>()->AddParticleColor(1.f, 1.f, 1.f);
	_meteorParticleHit1->GetComponent<Particle>()->SetParticleDirection(0.0f, 400.0f, 0.0f);
	_meteorParticleHit1->GetComponent<Particle>()->SetActive(false);
	_meteorParticleHit1->SetActive(true);
	//_meteorParticleHit1->SetParent(_meteor);

	_meteorParticleHit2 = SceneManager::GetInstance().GetCurrentScene()->CreateObject("MeteorParticleHit2");
	_meteorParticleHit2->AddComponent<Particle>();
	_meteorParticleHit2->GetComponent<Particle>()->SetParticleEffect("Halo1", "Resources/Textures/Particles/fx_Halo1.dds", 1000);
	_meteorParticleHit2->GetComponent<Particle>()->SetParticleDuration(8.0f, 0.5f);
	_meteorParticleHit2->GetComponent<Particle>()->SetParticleVelocity(10.0f, true);
	_meteorParticleHit2->GetComponent<Particle>()->SetParticleSize(1.f, 1.0f);
	_meteorParticleHit2->GetComponent<Particle>()->AddParticleColor(2.0f, 0.5f, 0.0f);
	_meteorParticleHit2->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 0.0f);
	_meteorParticleHit2->GetComponent<Particle>()->SetParticleCameraApply(true);
	_meteorParticleHit2->GetComponent<Particle>()->SetActive(false);
	_meteorParticleHit2->SetActive(true);
	//_meteorParticleHit2->SetParent(_meteor);

	_meteorParticleHit3 = SceneManager::GetInstance().GetCurrentScene()->CreateObject("MeteorParticleHit3");
	_meteorParticleHit3->AddComponent<Particle>();
	_meteorParticleHit3->GetComponent<Particle>()->SetParticleEffect("Flare6", "Resources/Textures/Particles/fx_Twister3.dds", 1000);
	_meteorParticleHit3->GetComponent<Particle>()->SetParticleDuration(2.4f, 2.0f);
	_meteorParticleHit3->GetComponent<Particle>()->SetParticleVelocity(1.0f, true);
	_meteorParticleHit3->GetComponent<Particle>()->SetParticleSize(1.f, 1.0f);
	_meteorParticleHit3->GetComponent<Particle>()->AddParticleColor(2.0f, 0.3f, 0.1f);
	_meteorParticleHit3->GetComponent<Particle>()->SetParticleDirection(0.0f, 600.0f, 0.0f);
	_meteorParticleHit3->GetComponent<Particle>()->SetParticleCameraApply(true);
	_meteorParticleHit3->GetComponent<Particle>()->SetActive(false);
	_meteorParticleHit3->SetActive(true);
	//_meteorParticleHit3->SetParent(_meteor);

	_meteorParticleHit4 = SceneManager::GetInstance().GetCurrentScene()->CreateObject("MeteorParticleHit4");
	_meteorParticleHit4->AddComponent<Particle>();
	_meteorParticleHit4->GetComponent<Particle>()->SetParticleEffect("BlastWave2", "Resources/Textures/Particles/fx_BlastWave2.dds", 1000);
	_meteorParticleHit4->GetComponent<Particle>()->SetParticleDuration(4.0f, 0.5f);
	_meteorParticleHit4->GetComponent<Particle>()->SetParticleVelocity(0.0f, true);
	_meteorParticleHit4->GetComponent<Particle>()->SetParticleSize(1.f, 1.0f);
	_meteorParticleHit4->GetComponent<Particle>()->AddParticleColor(1.0f, 0.0f, 0.0f);
	_meteorParticleHit4->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 0.0f);
	_meteorParticleHit4->GetComponent<Particle>()->SetParticleCameraApply(true);
	_meteorParticleHit4->GetComponent<Particle>()->SetActive(false);
	_meteorParticleHit4->SetActive(true);
	//_meteorParticleHit4->SetParent(_meteor);


	// ��Ȱ��ȭ ����		// �ΰ��ӿ����� �Ҹ�ó��
	meteorProj->SetDestoryCondition([meteorProj, this]()->bool
		{
			if (meteorProj->GetCollider()->GetTargetObject() != nullptr && meteorProj->GetCollider()->IsCollided() && meteorProj->GetCollider()->GetTargetObject()->GetTag() == "BOSS" && this->_isMeteorHit)
			{
				_currentBoss->_info._hp -= 100.0f;
				_isMeteorHit = false;
				
			}

			if (_meteor->GetComponent<Transform>()->GetPosition().y <= 2.0f)
			{
				_isMeteorEnded = true;
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
					_meteor->GetComponent<Transform>()->GetPosition().y - 40.0f * TimeManager::GetInstance().GetDeltaTime(),
					_meteor->GetComponent<Transform>()->GetPosition().z
				);

				// ������ ������ �ʾ����� ������ ���� X
				if (meteorProj->GetCollider()->IsCollided() && meteorProj->GetCollider()->GetTargetObject()->GetTag() == "Boss")
				{
					//_meteorParticleHit1->GetComponent<Particle>()->SetActive(true);
					//_meteorParticleHit2->GetComponent<Particle>()->SetActive(true);
					//_meteorParticleHit3->GetComponent<Particle>()->SetActive(true);
					//_meteorParticleHit4->GetComponent<Particle>()->SetActive(true);
					_isMeteorEnded = true;
				}
			}

			if (!(_meteor->GetActivated()) && _isMeteorEnded == true)
			{

				_meteorParticleTimer += TimeManager::GetInstance().GetDeltaTime();

				_meteorParticleHit1->GetComponent<Particle>()->SetActive(true);
				_meteorParticleHit2->GetComponent<Particle>()->SetActive(true);
				_meteorParticleHit3->GetComponent<Particle>()->SetActive(true);
				_meteorParticleHit4->GetComponent<Particle>()->SetActive(true);

				_meteorParticleHit1->GetComponent<Transform>()->SetPosition(_meteor->GetComponent<Transform>()->GetPosition());
				_meteorParticleHit2->GetComponent<Transform>()->SetPosition(_meteor->GetComponent<Transform>()->GetPosition());
				_meteorParticleHit3->GetComponent<Transform>()->SetPosition(_meteor->GetComponent<Transform>()->GetPosition());
				_meteorParticleHit4->GetComponent<Transform>()->SetPosition(_meteor->GetComponent<Transform>()->GetPosition());

				_meteorParticleHit1->GetComponent<Particle>()->SetParticleSize(30 - (_meteorParticleTimer * 40), 30 - (_meteorParticleTimer * 40));

				_meteorParticleHit3->GetComponent<Particle>()->SetParticleSize(120 - (_meteorParticleTimer * 150), 120 - (_meteorParticleTimer * 150));
				

				if (_meteorParticleTimer < 0.2f)
				{
					_meteorParticleHit2->GetComponent<Particle>()->SetParticleSize(_meteorParticleTimer * 300, _meteorParticleTimer * 300);
					_meteorParticleHit4->GetComponent<Particle>()->SetParticleSize(_meteorParticleTimer * 300, _meteorParticleTimer * 300);
				}
				else
				{
					_meteorParticleHit2->GetComponent<Particle>()->SetParticleSize(120 - (_meteorParticleTimer * 120), 120 - (_meteorParticleTimer * 120));
					_meteorParticleHit4->GetComponent<Particle>()->SetParticleSize(120 - (_meteorParticleTimer * 120), 120 - (_meteorParticleTimer * 120));
				}


				if (_meteorParticleTimer > 1.0f)
				{
					_meteorParticleTimer = 0.0f;
					_meteorParticleHit1->GetComponent<Particle>()->SetActive(false);
					_meteorParticleHit2->GetComponent<Particle>()->SetActive(false);
					_meteorParticleHit3->GetComponent<Particle>()->SetActive(false);
					_meteorParticleHit4->GetComponent<Particle>()->SetActive(false);
					_isMeteorEnded = false;
				}

				if (_meteorParticleTimer < 0.3f)
				{
					EventManager::GetInstance().CamShake();
				}
			}
		});

	// �� ��������� �����̳ʿ� �߰�
	AddToContanier(meteor);
}

void KunrealEngine::PlayerAbility::AddToContanier(Ability* abil)
{
	_abilityContainer.emplace_back(abil);
}

void KunrealEngine::PlayerAbility::SetCurrentBossObject()
{
	if (InputSystem::GetInstance()->KeyDown(KEY::I))
	{
		std::string sceneName = SceneManager::GetInstance().GetCurrentScene()->GetSceneName();

		if (sceneName == "mapTest4.json")
		{
			this->_currentBoss = SceneManager::GetInstance().GetCurrentScene()->GetObjectWithTag("BOSS")->GetComponent<Kamen>()->GetBoss();
		}
		else
		{
			int a = 10;
		}
	}
}

float KunrealEngine::PlayerAbility::GetDamage()
{
	return this->_currentDamage;
}
