#include <functional>
#include "ToolBox.h"
#include "TimeManager.h"
#include "SceneManager.h"
#include "Kamen.h"

KunrealEngine::Kamen::Kamen()
	: Boss(), _leftHand(nullptr), _rightHand(nullptr), _call(nullptr), _call2(nullptr), _lazer(nullptr),
	_callMoveDistance(0.0f), _isRotateFinish(false), _isCoreStart(false), _isRandomStart(false),
	_leftAttack(nullptr), _rightAttack(nullptr), _spellAttack(nullptr), _callAttack(nullptr), _call2Attack(nullptr),
	_turn180(nullptr), _backStep(nullptr), _teleport(nullptr), _teleportWithLook(nullptr),
	_turnClockWise(nullptr), _turnAntiClockWise(nullptr),
	_emergence9Lich(nullptr), _targetIndex(0),
	_call2PrevStep(1), _lazerCollider(nullptr),
	_outsideAttack(nullptr), _insideWarning(nullptr), _outsideSafe(nullptr), _insideWarningTimer(0.0f),
	_basicSwordAttack(nullptr), _sword(nullptr), _swordDissolveTimer(0.0f), _swordHide(nullptr)
{
	BossBasicInfo info;

	info.SetHp(100).SetPhase(3).SetArmor(10).SetDamage(100).SetMoveSpeed(20.0f).SetsStaggeredGauge(100.0f);
	info.SetAttackRange(5.0f);

	SetInfo(info);

	_centerPos.x = 6.5f;
	_centerPos.y = 2.0f;
	_centerPos.z = -34.0f;
}

KunrealEngine::Kamen::~Kamen()
{

}

void KunrealEngine::Kamen::Initialize()
{
	// �ݵ�� �ؾ���
	Boss::Initialize(this->GetOwner());

	// ���� Ÿ�̸� ����
	SetStartTime(5.0f);
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
	this->_isActivated = active;
}

void KunrealEngine::Kamen::SetMesh()
{
	_boss->AddComponent<MeshRenderer>();
	_boss->GetComponent<MeshRenderer>()->SetMeshObject("Lich/Lich");
}

void KunrealEngine::Kamen::SetTexture()
{
	auto texSize = _boss->GetComponent<MeshRenderer>()->GetTextures().size();
	for (int i = 0; i < texSize; i++)
	{
		_boss->GetComponent<MeshRenderer>()->SetDiffuseTexture(i, "Lich/T_Lich_1_D.tga");
		_boss->GetComponent<MeshRenderer>()->SetNormalTexture(i, "Lich/T_Lich_N.tga");
		_boss->GetComponent<MeshRenderer>()->SetEmissiveTexture(i, "Lich/T_Lich_01_E.tga");
	}

	_boss->GetComponent<MeshRenderer>()->SetCartoonState(true);
}

void KunrealEngine::Kamen::SetBossTransform()
{
	_boss->GetComponent<Transform>()->SetPosition(_centerPos.x, _centerPos.y, _centerPos.z);
}


void KunrealEngine::Kamen::SetBossCollider()
{
	_boss->AddComponent<BoxCollider>();
	_boss->GetComponent<BoxCollider>()->SetTransform(_boss, "Spine1_M");
	_boss->GetComponent<BoxCollider>()->SetBoxSize(6.0f, 18.0f, 10.0f);
	_boss->GetComponent<BoxCollider>()->SetOffset(0.0f, -1.5f, 0.0f);
}

void KunrealEngine::Kamen::CreatePattern()
{
	CreateSubObject();

	CreateLeftAttack();
	CreateRightAttack();
	CreateTurn180();
	CreateSpellAttack();
	CreateSwordAttack();
	CreateSwordHide();

	CreateCallAttack();
	CreateCall2Attack();
	CreateBackStep();
	CreateTeleportToCenter();
	CreateTeleportToCenterWithLook();
	CreateTurnClockWise();
	CreateTurnAntiClockWise();
	CreateOutsideSafe();
	CreateEmergenceAttack();

	// ���� ������� ���ϵ� ��Ƴ���
	GamePattern();
}


void KunrealEngine::Kamen::GamePattern()
{
	BasicPattern();						// �⺻ spell, call
	
	LeftRightPattern();					// ���� ��, �� ����
	RightLeftPattern();					// ���� ��, �Ĺ� �� ����
	BackStepCallPattern();				// �齺�� �� �� ����
	TeleportSpellPattern();				// ���� �� spell	
	//TeleportTurnClockPattern();			// ���� �� �ð� -> ���� ����
	TeleportTurnAntiClockPattern();		// ���� �� �ݽð� -> �ܺ� ����
	BasicSwordAttackPattern();

	//CoreEmmergencePattern();
}

void KunrealEngine::Kamen::CreateSubObject()
{

}


void KunrealEngine::Kamen::LeftRightPattern()
{
	BossPattern* leftRightPattern = new BossPattern();

	leftRightPattern->SetNextPatternForcePlay(false);

	leftRightPattern->SetMaxColliderCount(0);

	leftRightPattern->SetRange(_info._attackRange);

	leftRightPattern->SetPattern(_leftAttack);

	leftRightPattern->SetPattern(_rightAttack);

	_basicPattern.emplace_back(leftRightPattern);
}


void KunrealEngine::Kamen::RightLeftPattern()
{
	BossPattern* rightLeftPattern = new BossPattern();

	rightLeftPattern->SetNextPatternForcePlay(true);

	rightLeftPattern->SetMaxColliderCount(0);

	rightLeftPattern->SetRange(_info._attackRange);

	rightLeftPattern->SetPattern(_rightAttack);

	rightLeftPattern->SetPattern(_turn180);

	rightLeftPattern->SetPattern(_leftAttack);

	_basicPattern.emplace_back(rightLeftPattern);
}


void KunrealEngine::Kamen::BasicPattern()
{
	_basicPattern.emplace_back(_spellAttack);
	_basicPattern.emplace_back(_callAttack);
}

void KunrealEngine::Kamen::CreateLeftAttack()
{
	// �޼�
	_leftHand = _boss->GetObjectScene()->CreateObject("LeftHand");
	_leftHand->AddComponent<BoxCollider>();
	_leftHand->GetComponent<BoxCollider>()->SetTransform(_boss, "MiddleFinger1_L");
	_leftHand->GetComponent<BoxCollider>()->SetBoxSize(2.0f, 3.0f, 2.0f);
	_leftHand->GetComponent<BoxCollider>()->SetActive(false);

	BossPattern* pattern = new BossPattern();

	pattern->SetPatternName("Left_Attack_Once");

	pattern->SetAnimName("Left_Attack").SetDamage(100.0f).SetSpeed(20.0f).SetRange(_info._attackRange).SetAfterDelay(0.5);
	pattern->SetIsWarning(false).SetWarningName("");
	pattern->SetAttackState(BossPattern::eAttackState::ePush).SetMaxColliderCount(1);
	pattern->SetSubObject(_leftHand);
	auto leftHandLogic = CreateBasicAttackLogic(pattern, _leftHand, 10);

	pattern->SetLogic(leftHandLogic);

	_leftAttack = pattern;
}

void KunrealEngine::Kamen::CreateRightAttack()
{
	// ������
	_rightHand = _boss->GetObjectScene()->CreateObject("rightHand");
	_rightHand->AddComponent<BoxCollider>();
	_rightHand->GetComponent<BoxCollider>()->SetTransform(_boss, "MiddleFinger1_R");
	_rightHand->GetComponent<BoxCollider>()->SetBoxSize(2.0f, 3.0f, 2.0f);
	_rightHand->GetComponent<BoxCollider>()->SetActive(false);

	BossPattern* pattern = new BossPattern();

	pattern->SetPatternName("Right_Attack_Once");

	pattern->SetAnimName("Right_Attack").SetDamage(100.0f).SetSpeed(20.0f).SetRange(_info._attackRange).SetAfterDelay(0.5f);
	pattern->SetIsWarning(false).SetWarningName("");
	pattern->SetAttackState(BossPattern::eAttackState::ePush).SetMaxColliderCount(1);
	pattern->SetSubObject(_rightHand);

	auto rightHandLogic = CreateBasicAttackLogic(pattern, _rightHand, 10);

	pattern->SetLogic(rightHandLogic);

	_rightAttack = pattern;
}


void KunrealEngine::Kamen::CreateTurn180()
{
	BossPattern* pattern = new BossPattern();

	pattern->SetPatternName("Turn180");

	pattern->SetAnimName("Idle").SetMaxColliderCount(0).SetSpeed(200.0f);

	auto turn180 = [pattern, this]()
		{
			auto animator = _boss->GetComponent<Animator>();

			animator->Play("Idle", pattern->_speed, false);

			// ȸ�� ��Ŵ
			auto rotateFinish = Rotate(180, pattern->_speed);

			// ȸ�� ������ �ʾҴٸ�
			if (rotateFinish == false)
			{
				// ��� ȸ����Ű�� ����
				return true;
			}
			else
			{
				return false;
			}
		};


	pattern->SetLogic(turn180);

	_turn180 = pattern;
}

void KunrealEngine::Kamen::CreateBackStep()
{
	BossPattern* pattern = new BossPattern();

	pattern->SetPatternName("BackStep");

	pattern->SetAnimName("Run").SetSpeed(50.0f).SetRange(30.0f).SetMaxColliderCount(0);

	// ���� �Լ� ���� �����ϵ��� �־��ֱ�
	pattern->SetLogic(CreateBackStepLogic(pattern, pattern->_speed, pattern->_range));

	_backStep = pattern;
}

void KunrealEngine::Kamen::CreateTeleportToCenter()
{
	BossPattern* pattern = new BossPattern();

	pattern->SetPatternName("Teleport");

	pattern->SetAnimName("Idle").SetRange(0.0f).SetMaxColliderCount(0);

	auto teleport = [pattern, this]()
		{
			auto isTeleportFinish = Teleport(DirectX::XMFLOAT3(_centerPos.x, _centerPos.y, _centerPos.z), true, 1.0f);

			if (isTeleportFinish)
			{
				RotateByAngle(0.0f);

				return false;
			}

			return true;
		};

	// ���� �Լ� ���� �����ϵ��� �־��ֱ�
	pattern->SetLogic(teleport);

	_teleport = pattern;
}


void KunrealEngine::Kamen::CreateTeleportToCenterWithLook()
{
	BossPattern* pattern = new BossPattern();

	pattern->SetPatternName("Teleport");

	pattern->SetAnimName("Idle").SetRange(0.0f).SetMaxColliderCount(0);

	auto teleport = [pattern, this]()
		{
			auto isTeleportFinish = Teleport(DirectX::XMFLOAT3(_centerPos.x, _centerPos.y, _centerPos.z), true, 1.0f);

			if (isTeleportFinish)
			{
				return false;
			}

			return true;
		};

	// ���� �Լ� ���� �����ϵ��� �־��ֱ�
	pattern->SetLogic(teleport);

	_teleportWithLook = pattern;
}

void KunrealEngine::Kamen::CreateTurnClockWise()
{
	BossPattern* pattern = new BossPattern();

	pattern->SetPatternName("TurnClockWise");

	pattern->SetAnimName("Idle").SetMaxColliderCount(0).SetSpeed(50.0f);

	auto initializeLogic = [pattern, this]()
		{
			_boss->GetComponent<BoxCollider>()->SetActive(true);
			_boss->GetComponent<MeshRenderer>()->SetActive(true);
		};

	pattern->SetInitializeLogic(initializeLogic);

	auto turn = [pattern, this]()
		{
			auto animator = _boss->GetComponent<Animator>();

			// ȸ�� ���� �ȵƴٸ�
			if (_isRotateFinish == false)
			{
				animator->Play("Idle", pattern->_speed, true);

				// ȸ�� ��Ŵ
				auto rotateFinish = RotateClockWise(4 * pattern->_speed, false);

				// ȸ�� ������ �ʾҴٸ�
				if (rotateFinish == false)
				{
					// ��� ȸ����Ű�� ����
					return true;
				}
				else
				{
					_isRotateFinish = false;
					return false;
				}
			}
		};

	pattern->SetLogic(turn);

	_turnClockWise = pattern;
}


void KunrealEngine::Kamen::CreateTurnAntiClockWise()
{
	BossPattern* pattern = new BossPattern();

	pattern->SetPatternName("TurnAntiClockWise");

	pattern->SetAnimName("Idle").SetMaxColliderCount(0).SetSpeed(50.0f);

	auto initializeLogic = [pattern, this]()
		{
			_boss->GetComponent<BoxCollider>()->SetActive(true);
			_boss->GetComponent<MeshRenderer>()->SetActive(true);
		};

	pattern->SetInitializeLogic(initializeLogic);

	auto turn = [pattern, this]()
		{
			auto animator = _boss->GetComponent<Animator>();

			// ȸ�� ���� �ȵƴٸ�
			if (_isRotateFinish == false)
			{
				animator->Play("Idle", pattern->_speed, true);

				// ȸ�� ��Ŵ
				auto rotateFinish = RotateClockWise(4 * pattern->_speed, true);

				// ȸ�� ������ �ʾҴٸ�
				if (rotateFinish == false)
				{
					// ��� ȸ����Ű�� ����
					return true;
				}
				else
				{
					_isRotateFinish = false;
					return false;
				}
			}
		};

	pattern->SetLogic(turn);

	_turnAntiClockWise = pattern;
}


void KunrealEngine::Kamen::CreateOutsideSafe()
{
	BossPattern* pattern = new BossPattern();

	pattern->SetPatternName("OutSideSafe");

	pattern->SetAnimName("Idle").SetRange(0.0f).SetMaxColliderCount(1).SetSpeed(20.0f);
	pattern->SetAttackState(BossPattern::eAttackState::ePush);

	_insideWarning = _boss->GetObjectScene()->CreateObject("OutsideSafe");
	_insideWarning->AddComponent<TransparentMesh>();
	_insideWarning->GetComponent<TransparentMesh>()->CreateTMesh("OutsideSafe", "Resources/Textures/Warning/Warning.dds", 0.6f, true);
	_insideWarning->GetComponent<Transform>()->SetScale(10.0f, 10.0f, 10.0f);
	_insideWarning->GetComponent<Transform>()->SetPosition(_bossTransform->GetPosition().x, _bossTransform->GetPosition().y, _bossTransform->GetPosition().z);

	_insideWarning->GetComponent<TransparentMesh>()->SetTimer(2.0f);

	pattern->SetSubObject(_insideWarning);

	_outsideAttack = _boss->GetObjectScene()->CreateObject("OutsideAttack");
	_outsideAttack->AddComponent<BoxCollider>();
	_outsideAttack->GetComponent<BoxCollider>()->SetBoxSize(30.0f, 30.0f, 30.0f);
	_outsideAttack->GetComponent<BoxCollider>()->SetActive(false);

	_outsideAttack->AddComponent<Particle>();
	_outsideAttack->GetComponent<Particle>()->SetParticleEffect("fire", "Resources/Textures/Particles/flare.dds", 1000);
	_outsideAttack->GetComponent<Particle>()->SetParticleDuration(1.5f, 2.0f);
	_outsideAttack->GetComponent<Particle>()->SetParticleVelocity(80.f, false);
	_outsideAttack->GetComponent<Particle>()->AddParticleColor(1.2f, 7.5f, 0.6f);

	pattern->SetSubObject(_outsideAttack);

	// ���� �������� �ʱ�ȭ, ���� ����
	auto initializeLogic = [pattern, this]()
		{
			_insideWarning->GetComponent<TransparentMesh>()->Reset();
			_insideWarning->GetComponent<TransparentMesh>()->SetActive(true);
			_insideWarning->GetComponent<Transform>()->SetPosition(_bossTransform->GetPosition().x, _bossTransform->GetPosition().y + 1.0f, _bossTransform->GetPosition().z);
			_outsideAttack->GetComponent<Transform>()->SetPosition(_bossTransform->GetPosition().x, _bossTransform->GetPosition().y + 1.0f, _bossTransform->GetPosition().z);
			_insideWarningTimer = 0.0f;

			_boss->GetComponent<BoxCollider>()->SetActive(false);
			_boss->GetComponent<MeshRenderer>()->SetActive(false);
		};

	pattern->SetInitializeLogic(initializeLogic);

	auto outsideSafe = [pattern, this]()
		{
			// ���� ����
			auto isPlayed = _insideWarning->GetComponent<TransparentMesh>()->PlayOnce();

			// ���� ������ �Ϸ�Ǹ�
			if (isPlayed)
			{
				// n�ʵ��� �ݶ��̴� ����
				_insideWarningTimer += TimeManager::GetInstance().GetDeltaTime();
				_outsideAttack->GetComponent<BoxCollider>()->SetActive(true);
				_outsideAttack->GetComponent<Particle>()->SetActive(true);

				_outsideAttack->GetComponent<Particle>()->SetParticleSize(100.f * ToolBox::GetRandomFloat(0.3f, 1.0f), 40.0f * ToolBox::GetRandomFloat(0.1f, 1.0f));

				if (_insideWarningTimer >= 2.0f)
				{
					_boss->GetComponent<MeshRenderer>()->SetActive(true);
					_boss->GetComponent<BoxCollider>()->SetActive(true);
					return false;
				}
			}

			return true;
		};

	// ���� �Լ� ���� �����ϵ��� �־��ֱ�
	pattern->SetLogic(outsideSafe);

	_outsideSafe = pattern;
}

void KunrealEngine::Kamen::CreateSpellAttack()
{
	_lazer = _boss->GetObjectScene()->CreateObject("lazer");
	_lazer->AddComponent<Particle>();
	_lazer->GetComponent<Particle>()->SetParticleEffect("Laser", "Resources/Textures/Particles/RailGun_64.dds", 1000);
	_lazer->GetComponent<Particle>()->SetParticleDuration(1.7f, 2.0f);
	_lazer->GetComponent<Particle>()->SetParticleVelocity(84.f, false);
	_lazer->GetComponent<Particle>()->SetParticleRotation(90.0f, _bossTransform->GetRotation().y, 0.0f);
	_lazer->GetComponent<Particle>()->AddParticleColor(0.4f, 1.0f, 0.0f);
	_lazer->GetComponent<Particle>()->SetActive(false);

	_lazerCollider = _boss->GetObjectScene()->CreateObject("lazerCollider");
	_lazerCollider->AddComponent<BoxCollider>();
	_lazerCollider->GetComponent<BoxCollider>()->SetBoxSize(140.0f, 10.0f, 10.0f);

	BossPattern* pattern = new BossPattern();

	pattern->SetPatternName("Spell");

	pattern->SetAnimName("Spell").SetDamage(100.0f).SetSpeed(20.0f).SetRange(_info._attackRange + 80.0f).SetAfterDelay(2.0f);
	pattern->SetIsWarning(true).SetWarningName("Spell").SetMaxColliderCount(1).SetAttackState(BossPattern::eAttackState::ePush);
	pattern->SetSubObject(_lazer);
	pattern->SetSubObject(_lazerCollider);

	auto initLogic = [pattern, this]()
		{
			_boss->GetComponent<BoxCollider>()->SetActive(true);
			_boss->GetComponent<MeshRenderer>()->SetActive(true);

			auto playerPos = _playerTransform->GetPosition();
			auto bossPos = _bossTransform->GetPosition();
			auto direction = DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&playerPos), DirectX::XMLoadFloat3(&bossPos));
			direction = DirectX::XMVector3Normalize(direction);
			auto scaleDir = DirectX::XMVectorScale(direction, 15.0f);
			_lazer->GetComponent<Transform>()->SetPosition(_bossTransform->GetPosition().x + scaleDir.m128_f32[0], 16.0f, _bossTransform->GetPosition().z + scaleDir.m128_f32[2]);
			_lazer->GetComponent<Particle>()->SetParticleRotation(90.0f, _bossTransform->GetRotation().y, 0.0f);
			pattern->SetSpeed(20.0f);

			auto scaleDir2 = DirectX::XMVectorScale(direction, 80.0f);

			_lazerCollider->GetComponent<Transform>()->SetPosition(_bossTransform->GetPosition().x + scaleDir2.m128_f32[0], 16.0f, _bossTransform->GetPosition().z + scaleDir2.m128_f32[2]);

			_lazerCollider->GetComponent<Transform>()->SetRotation(_bossTransform->GetRotation().x, _bossTransform->GetRotation().y + 90.0f, _bossTransform->GetRotation().z);
		};

	pattern->SetInitializeLogic(initLogic);
	auto spellLogic = [pattern, this]()
		{
			auto animator = _boss->GetComponent<Animator>();
			auto isAnimationPlaying = animator->Play(pattern->_animName, pattern->_speed, false);

			// ���� �������� �Ѿ�� ������ üũ�� �ݶ��̴��� Ű��

			if (animator->GetCurrentFrame() >= 30.0f)
			{
				if (animator->GetCurrentFrame() >= 55.0f)
				{
					_lazer->GetComponent<Particle>()->SetActive(false);
					if (pattern->_colliderOnCount > 0)
					{
						_lazerCollider->GetComponent<BoxCollider>()->SetActive(false);
					}
				}
				else
				{
					pattern->SetSpeed(10.0f);
					_lazer->GetComponent<Particle>()->SetActive(true);
					_lazer->GetComponent<Particle>()->SetParticleSize(40.f * ToolBox::GetRandomFloat(0.3f, 1.0f), 30.0f * ToolBox::GetRandomFloat(0.1f, 1.0f));
					if (pattern->_colliderOnCount > 0)
					{
						_lazerCollider->GetComponent<BoxCollider>()->SetActive(true);
					}
				}
			}

			if (isAnimationPlaying == false)
			{
				return false;
			}

			return true;
		};


	pattern->SetLogic(spellLogic);

	_spellAttack = pattern;
}


void KunrealEngine::Kamen::CreateCallAttack()
{
	// call ����ü
	_call = _boss->GetObjectScene()->CreateObject("call");
	_call->AddComponent<BoxCollider>();
	_call->GetComponent<BoxCollider>()->SetBoxSize(10.0f, 10.0f, 10.0f);
	_call->GetComponent<BoxCollider>()->SetActive(false);
	_call->AddComponent<Particle>();
	_call->GetComponent<Particle>()->SetParticleEffect("Flame", "Resources/Textures/Particles/flare.dds", 1000);
	_call->GetComponent<Particle>()->SetParticleDuration(2.0f, 2.0f);
	_call->GetComponent<Particle>()->SetParticleVelocity(3.f, true);
	_call->GetComponent<Particle>()->SetParticleSize(10.f, 30.0f);
	_call->GetComponent<Particle>()->SetParticleDirection(0.0f, 7.0f, 0.0f);
	_call->GetComponent<Particle>()->AddParticleColor(1.2f, 7.5f, 0.6f);
	_call->GetComponent<Particle>()->SetActive(false);

	DirectX::XMFLOAT4 diffuse = { 0.0f, 0.6f, 0.0f, 1.0f };
	DirectX::XMFLOAT4 ambient = { 0.0f, 0.15f, 0.0f, 1.0f };
	DirectX::XMFLOAT4 specular = { 0.0f, 0.15f, 0.0f, 1.0f };

	_call->AddComponent<Light>();
	_call->GetComponent<Light>()->CreatePointLight(ambient, diffuse, specular, 1.0f);

	BossPattern* pattern = new BossPattern();

	pattern->SetPatternName("Call");

	pattern->SetAnimName("Call").SetDamage(100.0f).SetSpeed(20.0f).SetRange(_info._attackRange + 50.0f).SetAfterDelay(1.0f);
	pattern->SetIsWarning(true).SetWarningName("Call").SetMaxColliderCount(1).SetAttackState(BossPattern::eAttackState::ePush);
	pattern->SetSubObject(_call);

	auto callLogic = [pattern, this]()
		{
			auto animator = _boss->GetComponent<Animator>();
			auto isAnimationPlaying = animator->Play(pattern->_animName, pattern->_speed, false);

			// ���� �������� �Ѿ�� ������ üũ�� �ݶ��̴��� Ű��
			if (animator->GetCurrentFrame() >= 20)
			{
				if (pattern->_colliderOnCount > 0)
				{
					// �ݶ��̴� Ű��
					_call->GetComponent<BoxCollider>()->SetActive(true);
					// ��ƼŬ Ű��
					_call->GetComponent<Particle>()->SetActive(true);
					// ����Ʈ Ű��
					_call->GetComponent<Light>()->SetActive(true);
				}

				// ������ �ٶ󺸴� ���� ������
				auto direction = GetDirection();

				// ���� ������ ������
				auto nowPos = _boss->GetComponent<Transform>()->GetPosition();

				auto nowPosVec = DirectX::XMLoadFloat3(&nowPos);

				auto callNowPos = _call->GetComponent<Transform>()->GetPosition();
				auto distance = ToolBox::GetDistance(nowPos, callNowPos);

				if (distance < pattern->_range)
				{
					_callMoveDistance += (pattern->_speed * 1.5) * TimeManager::GetInstance().GetDeltaTime();
				}
				else
				{
					animator->Stop();
					isAnimationPlaying = false;
				}


				// ���� ������ �����ǿ��� �ٶ󺸴� ���� �������� ������
				DirectX::XMVECTOR newPosition = DirectX::XMVectorAdd(nowPosVec, DirectX::XMVectorScale(direction, _callMoveDistance));

				_call->GetComponent<Transform>()->SetPosition(newPosition.m128_f32[0], newPosition.m128_f32[1], newPosition.m128_f32[2]);
			}

			if (isAnimationPlaying == false)
			{
				return false;
			}

			return true;
		};

	pattern->SetLogic(callLogic, false);

	_callInitLogic = [pattern, this]()
		{
			_call->GetComponent<Transform>()->SetPosition(_boss->GetComponent<Transform>()->GetPosition().x, _boss->GetComponent<Transform>()->GetPosition().y, _boss->GetComponent<Transform>()->GetPosition().z);
			_callMoveDistance = 0.0f;
		};

	// �̴ϼȶ����� ���� �Լ� �־��ֱ�
	pattern->SetInitializeLogic(_callInitLogic);

	_callAttack = pattern;
}


void KunrealEngine::Kamen::CreateCall2Attack()
{
	_call2 = _boss->GetObjectScene()->CreateObject("call2");
	_call2->AddComponent<BoxCollider>();
	_call2->GetComponent<BoxCollider>()->SetBoxSize(15.0f, 30.0f, 15.0f);
	_call2->GetComponent<BoxCollider>()->SetActive(false);
	_call2->AddComponent<Particle>();
	_call2->GetComponent<Particle>()->SetParticleEffect("Flame", "Resources/Textures/Particles/flare.dds", 1000);
	_call2->GetComponent<Particle>()->SetParticleDuration(2.7f, 3.1f);
	_call2->GetComponent<Particle>()->SetParticleVelocity(10.f, true);
	_call2->GetComponent<Particle>()->SetParticleSize(30.f, 30.0f);
	_call2->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.5f, 0.0f);
	_call2->GetComponent<Particle>()->AddParticleColor(1.2f, 7.5f, 0.6f);
	_call2->GetComponent<Particle>()->SetActive(false);

	DirectX::XMFLOAT4 diffuse = { 0.0f, 0.6f, 0.0f, 1.0f };
	DirectX::XMFLOAT4 ambient = { 0.0f, 0.15f, 0.0f, 1.0f };
	DirectX::XMFLOAT4 specular = { 0.0f, 0.15f, 0.0f, 1.0f };

	_call2->AddComponent<Light>();
	_call2->GetComponent<Light>()->CreatePointLight(ambient, diffuse, specular, 1.0f);

	BossPattern* pattern = new BossPattern();

	pattern->SetPatternName("Call2");

	pattern->SetAnimName("Call").SetDamage(100.0f).SetSpeed(20.0f).SetRange(_info._attackRange + 50.0f).SetAfterDelay(1.0f);
	pattern->SetIsWarning(true).SetWarningName("Call2").SetMaxColliderCount(1).SetAttackState(BossPattern::eAttackState::eParalysis);

	pattern->SetSubObject(_call2);

	auto callLogic = [pattern, this]()
		{
			auto animator = _boss->GetComponent<Animator>();
			auto isAnimationPlaying = animator->Play(pattern->_animName, pattern->_speed, false);

			// ���� �������� �Ѿ�� ������ üũ�� �ݶ��̴��� Ű��
			if (animator->GetCurrentFrame() >= 20)
			{
				if (pattern->_colliderOnCount > 0)
				{
					// �ݶ��̴� Ű��
					_call2->GetComponent<BoxCollider>()->SetActive(true);
					// ��ƼŬ Ű��
					_call2->GetComponent<Particle>()->SetActive(true);
					// ����Ʈ�� Ű��
					_call2->GetComponent<Light>()->SetActive(true);
				}

				// ������ �ٶ󺸴� ���� ������
				auto direction = GetDirection();

				// ���� ������ ������
				auto nowPos = _boss->GetComponent<Transform>()->GetPosition();

				auto nowPosVec = DirectX::XMLoadFloat3(&nowPos);

				int step = (animator->GetCurrentFrame() - 20.0f) / 10.0f + 1;

				if (step != _call2PrevStep)
				{
					pattern->_colliderOnCount = pattern->_maxColliderOnCount;
				}

				_call2PrevStep = step;


				// ���� ������ �����ǿ��� �ٶ󺸴� ���� �������� ������
				DirectX::XMVECTOR newPosition = DirectX::XMVectorAdd(nowPosVec, DirectX::XMVectorScale(direction, 20.0f * step));

				_call2->GetComponent<Transform>()->SetPosition(newPosition.m128_f32[0], 5.0f, newPosition.m128_f32[2]);
			}

			if (isAnimationPlaying == false)
			{
				return false;
			}

			return true;
		};

	pattern->SetLogic(callLogic, false);

	_call2Attack = pattern;
}


void KunrealEngine::Kamen::CreateSwordAttack()
{
	_sword = _boss->GetObjectScene()->CreateObject("KamenSword");
	_sword->AddComponent<MeshRenderer>();
	_sword->GetComponent<MeshRenderer>()->SetMeshObject("KamenSword/KamenSword");
	_sword->GetComponent<MeshRenderer>()->SetParentBone(_boss, "Wrist_L");
	_sword->GetComponent<Transform>()->SetPosition(0.0f, 0.0f, 0.0f);
	_sword->GetComponent<Transform>()->SetRotation(0.0f, 0.0f, 0.0f);
	_sword->AddComponent<BoxCollider>()->SetBoxSize(5.0f, 20.0f, 5.0f);
	_sword->GetComponent<BoxCollider>()->SetTransform(_boss, "Wrist_L");

	auto texSize = _sword->GetComponent<MeshRenderer>()->GetTextures().size();

	for (int i = 0; i < texSize; i++)
	{
		_sword->GetComponent<MeshRenderer>()->SetDiffuseTexture(i, "KamenSword/KamenSword_BaseColor.png");
		_sword->GetComponent<MeshRenderer>()->SetNormalTexture(i, "KamenSword/KamenSword_Normal.png");
		_sword->GetComponent<MeshRenderer>()->SetEmissiveTexture(i, "KamenSword/KamenSword_Emissive.png");
	}

	BossPattern* pattern = new BossPattern();

	pattern->SetPatternName("Sword_Attack");

	pattern->SetAnimName("Right_Attack").SetDamage(100.0f).SetSpeed(15.0f).SetRange(_info._attackRange + 25.0f).SetAfterDelay(1.0f);
	pattern->SetIsWarning(false).SetWarningName("");
	pattern->SetAttackState(BossPattern::eAttackState::ePush).SetMaxColliderCount(1);
	pattern->SetSubObject(_sword);

	auto attackLogic = [pattern, this]()
		{
			auto animator = _boss->GetComponent<Animator>();
			auto isAnimationPlaying = animator->Play(pattern->_animName, pattern->_speed, false);

			// ���� �������� �Ѿ�� ������ üũ�� �ݶ��̴��� Ű��
			if (pattern->_colliderOnCount > 0)
			{
				if (animator->GetCurrentFrame() >= 10.0f)
				{
					pattern->_speed = 15.0f;
					_sword->GetComponent<BoxCollider>()->SetActive(true);
				}
				else
				{
					pattern->_speed = 15 * 0.3f;
				}
			}

			if (_swordDissolveTimer < 0.5f)
			{
				_swordDissolveTimer += TimeManager::GetInstance().GetDeltaTime() * 0.2f;
				_sword->GetComponent<MeshRenderer>()->SetDissolve(_swordDissolveTimer);
			}

			if (isAnimationPlaying == false)
			{
				_sword->SetActive(true);
				pattern->DeleteSubObject(_sword);
				return false;
			}

			return true;
		};

	pattern->SetLogic(attackLogic);

	auto swordInitLogic = [pattern, this]()
		{
			pattern->SetSubObject(_sword);

			// ������ ����Ʈ Ű��
			_sword->SetActive(true);
			_sword->GetComponent<MeshRenderer>()->SetActive(true);
			_sword->GetComponent<MeshRenderer>()->SetIsDissolve(true);

			_swordDissolveTimer = 0.0f;
			_sword->GetComponent<MeshRenderer>()->SetDissolve(_swordDissolveTimer);

		};

	// �̴ϼȶ����� ���� �Լ� �־��ֱ�
	pattern->SetInitializeLogic(swordInitLogic);

	_basicSwordAttack = pattern;
}


void KunrealEngine::Kamen::CreateSwordHide()
{
	BossPattern* pattern = new BossPattern();

	pattern->SetPatternName("Sword_Hide");

	pattern->SetAnimName("Idle").SetSpeed(20.0f).SetAfterDelay(0.5f).SetMaxColliderCount(0);
	pattern->SetSubObject(_sword);

	auto swordInitLogic = [pattern, this]()
		{
			// ������ ����Ʈ Ű��
			_sword->GetComponent<MeshRenderer>()->SetActive(true);
			_sword->GetComponent<MeshRenderer>()->SetIsDissolve(true);

			_swordDissolveTimer = 0.5f;
			_sword->GetComponent<MeshRenderer>()->SetDissolve(_swordDissolveTimer);

			_sword->GetComponent<BoxCollider>()->SetActive(false);
		};

	pattern->SetInitializeLogic(swordInitLogic);

	auto hideLogic = [pattern, this]()
		{
			auto animator = _boss->GetComponent<Animator>();
			auto isAnimationPlaying = animator->Play(pattern->_animName, pattern->_speed, true);

			if (_swordDissolveTimer > 0)
			{
				_swordDissolveTimer -= TimeManager::GetInstance().GetDeltaTime() * 0.3f;
				_sword->GetComponent<MeshRenderer>()->SetDissolve(_swordDissolveTimer);
			}
			else
			{
				return false;
			}

			return true;
		};

	pattern->SetLogic(hideLogic);

	_swordHide = pattern;
}

void KunrealEngine::Kamen::CreateEmergenceAttack()
{
	BossPattern* pattern = new BossPattern();

	pattern->SetPatternName("Emergence9Lich");

	pattern->SetAnimName("Emergence").SetSpeed(15.0f);
	pattern->SetMaxColliderCount(0);

	for (int i = 0; i < 9; i++)
	{
		std::string index = "fake" + std::to_string(i + 1);
		auto boss = _boss->GetObjectScene()->CreateObject(index);
		_fakeBoss.emplace_back(boss);

		boss->AddComponent<MeshRenderer>();
		boss->GetComponent<MeshRenderer>()->SetMeshObject("Lich/Lich");

		auto texSize = _boss->GetComponent<MeshRenderer>()->GetTextures().size();
		for (int i = 0; i < texSize; i++)
		{
			boss->GetComponent<MeshRenderer>()->SetDiffuseTexture(i, "Lich/T_Lich_1_D.tga");
			boss->GetComponent<MeshRenderer>()->SetNormalTexture(i, "Lich/T_Lich_N.tga");
			boss->GetComponent<MeshRenderer>()->SetEmissiveTexture(i, "Lich/T_Lich_01_E.tga");
		}

		boss->AddComponent<BoxCollider>();
		boss->GetComponent<BoxCollider>()->SetTransform(boss, "Spine1_M");
		boss->GetComponent<BoxCollider>()->SetBoxSize(6.0f, 18.0f, 10.0f);
		boss->GetComponent<BoxCollider>()->SetOffset(0.0f, -1.5f, 0.0f);

		boss->SetTotalComponentState(false);
		boss->SetActive(false);

		pattern->SetSubObject(boss);
	}

	int index = 0;
	for (int j = 0; j < 3; j++)
	{
		for (int i = 0; i < 3; i++)
		{
			_fakeBoss[index]->GetComponent<Transform>()->SetPosition(40.0f - (40.0 * i), 0.0f, 40.0f - (40.0 * j));
			index++;
		}
	}

	auto core1Logic = [pattern, this]()
		{
			// 9���� �н� �� ������ ����� �� �ε��� ���� ����
			if (!_isRandomStart)
			{
				_targetIndex = ToolBox::GetRandomNum(_fakeBoss.size() - 1);

				_isRandomStart = true;
			}

			// ���� ���� ���� �߾����� �ڷ���Ʈ
			if (!_isRotateFinish)
			{
				auto newPos = _fakeBoss[_targetIndex]->GetComponent<Transform>()->GetPosition();
				auto isTeleportFinish = Teleport(newPos, false, 3.0f);

				// ���� ������ ���� ����
				if (!isTeleportFinish)
				{
					return true;
				}
				else
				{
					// ���� ������ �ٽ� ���ƿ��� �ʵ���
					RotateByAngle(0.0f);

					_isRotateFinish = true;
				}
			}

			/// emergence ���ִϸ��̼� �ѹ� ��������� ���� �̱���

			// ���� ���� �� �ʱ�ȭ���ٰ͵�
			if (!_isCoreStart)
			{
				// ����ũ ������ ����
				for (int i = 0; i < _fakeBoss.size(); i++)
				{
					if (i != _targetIndex)
					{
						_fakeBoss[i]->SetActive(true);
						_fakeBoss[i]->SetTotalComponentState(true);
					}

					else
					{
						_fakeBoss[i]->SetActive(false);
					}
				}

				_isCoreStart = true;
			}

			auto animator = _boss->GetComponent<Animator>();
			auto isAnimationPlaying = animator->Play(pattern->_animName, pattern->_speed, false);

			if (isAnimationPlaying == false)
			{
				_isCoreStart = false;
				_isRotateFinish = false;

				_targetIndex = 0;
				return false;
			}

			return true;
		};

	pattern->SetLogic(core1Logic, false);

	_emergence9Lich = pattern;
}

void KunrealEngine::Kamen::BackStepCallPattern()
{
	BossPattern* backStepCallPattern = new BossPattern();

	backStepCallPattern->SetSkipMove(true);

	backStepCallPattern->SetNextPatternForcePlay(true);

	backStepCallPattern->SetMaxColliderCount(0);

	backStepCallPattern->SetPattern(_backStep);

	backStepCallPattern->SetPattern(_call2Attack);

	backStepCallPattern->SetRange(backStepCallPattern->_patternList[2]->_range - backStepCallPattern->_patternList[1]->_range);

	_basicPattern.emplace_back(backStepCallPattern);
}

void KunrealEngine::Kamen::TeleportSpellPattern()
{
	BossPattern* teleportSpellPattern = new BossPattern();

	teleportSpellPattern->SetNextPatternForcePlay(true);
	teleportSpellPattern->SetSkipChase(true);
	teleportSpellPattern->SetRange(100.0f);

	teleportSpellPattern->SetPattern(_teleportWithLook);

	teleportSpellPattern->SetPattern(_spellAttack);


	_basicPattern.emplace_back(teleportSpellPattern);
}

void KunrealEngine::Kamen::TeleportTurnClockPattern()
{
	BossPattern* teleportTurnClockPattern = new BossPattern();

	teleportTurnClockPattern->SetNextPatternForcePlay(true);
	teleportTurnClockPattern->SetSkipChase(true);
	teleportTurnClockPattern->SetRange(100.0f);

	teleportTurnClockPattern->SetMaxColliderCount(0);

	teleportTurnClockPattern->SetPattern(_teleport);

	teleportTurnClockPattern->SetPattern(_turnClockWise);

	teleportTurnClockPattern->SetRange(teleportTurnClockPattern->_patternList[1]->_range);

	_basicPattern.emplace_back(teleportTurnClockPattern);
}

void KunrealEngine::Kamen::TeleportTurnAntiClockPattern()
{
	BossPattern* teleportTurnAntiClockPattern = new BossPattern();

	teleportTurnAntiClockPattern->SetNextPatternForcePlay(true);
	teleportTurnAntiClockPattern->SetSkipChase(true);
	teleportTurnAntiClockPattern->SetRange(100.0f);

	teleportTurnAntiClockPattern->SetMaxColliderCount(0);

	teleportTurnAntiClockPattern->SetPattern(_teleport);

	teleportTurnAntiClockPattern->SetPattern(_turnAntiClockWise);

	teleportTurnAntiClockPattern->SetPattern(_outsideSafe);

	teleportTurnAntiClockPattern->SetRange(teleportTurnAntiClockPattern->_patternList[1]->_range);

	_basicPattern.emplace_back(teleportTurnAntiClockPattern);
}


void KunrealEngine::Kamen::BasicSwordAttackPattern()
{
	BossPattern* basicSwordAttack = new BossPattern();

	basicSwordAttack->SetNextPatternForcePlay(true);
	basicSwordAttack->SetMaxColliderCount(0);
	basicSwordAttack->SetPattern(_basicSwordAttack);
	basicSwordAttack->SetPattern(_swordHide);

	basicSwordAttack->SetRange(basicSwordAttack->_patternList[1]->_range);

	_basicPattern.emplace_back(basicSwordAttack);
}

void KunrealEngine::Kamen::CoreEmmergencePattern()
{
	BossPattern* coreEmmergence = new BossPattern();

	coreEmmergence->SetNextPatternForcePlay(true);

	coreEmmergence->SetTriggerHp(100.0f);

	coreEmmergence->SetMaxColliderCount(0);

	coreEmmergence->SetPattern(_teleport);

	coreEmmergence->SetPattern(_emergence9Lich);

	coreEmmergence->SetPattern(_leftAttack);

	_basicPattern.emplace_back(coreEmmergence);
}
