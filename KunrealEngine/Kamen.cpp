#include <functional>
#include "ToolBox.h"
#include "TimeManager.h"
#include "SceneManager.h"
#include "Kamen.h"

KunrealEngine::Kamen::Kamen()
	: Boss(), _leftHand(nullptr), _rightHand(nullptr), _call(nullptr), _lazer(nullptr),
	_callMoveDistance(0.0f), _isRotateFinish(false), _isCoreStart(false), _isRandomStart(false)
{
	BossBasicInfo info;

	info.SetHp(100).SetPhase(3).SetArmor(10).SetDamage(100).SetMoveSpeed(20.0f).SetsStaggeredGauge(100.0f);
	info.SetAttackRange(5.0f);

	SetInfo(info);
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
}

void KunrealEngine::Kamen::SetBossTransform()
{
	_boss->GetComponent<Transform>()->SetPosition(5.0f, 0.0f, -20.0f);
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

	//LeftAttack();
	//RightAttack();
	SpellAttack();
	//CallAttack();

	// �齺�� ���� ����
	//BackStepCallAttack();
	//
	//EmergenceAttack();
}


void KunrealEngine::Kamen::CreateSubObject()
{
	// �޼�
	_leftHand = _boss->GetObjectScene()->CreateObject("LeftHand");
	_leftHand->AddComponent<BoxCollider>();
	_leftHand->GetComponent<BoxCollider>()->SetTransform(_boss, "MiddleFinger1_L");
	_leftHand->GetComponent<BoxCollider>()->SetBoxSize(2.0f, 3.0f, 2.0f);
	_leftHand->GetComponent<BoxCollider>()->SetActive(false);

	// ������
	_rightHand = _boss->GetObjectScene()->CreateObject("rightHand");
	_rightHand->AddComponent<BoxCollider>();
	_rightHand->GetComponent<BoxCollider>()->SetTransform(_boss, "MiddleFinger1_R");
	_rightHand->GetComponent<BoxCollider>()->SetBoxSize(2.0f, 3.0f, 2.0f);
	_rightHand->GetComponent<BoxCollider>()->SetActive(false);

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

	_lazer = _boss->GetObjectScene()->CreateObject("lazer");
	_lazer->AddComponent<Particle>();
	_lazer->GetComponent<Transform>()->SetPosition(0.0f, 16.0f, -4.0f);
	_lazer->GetComponent<Particle>()->SetParticleEffect("Laser", "Resources/Textures/Particles/RailGun_64.dds", 1000);
	_lazer->GetComponent<Particle>()->SetParticleDuration(1.7f, 2.0f);
	_lazer->GetComponent<Particle>()->SetParticleVelocity(84.f, false);
	_lazer->GetComponent<Particle>()->SetParticleDirection(-8.5f, 0.0f, 82.6f);
	_lazer->GetComponent<Particle>()->SetActive(false);
}

void KunrealEngine::Kamen::LeftAttack()
{
	BossPattern* pattern = new BossPattern();

	pattern->SetPatternName("Left_Attack_Once");

	pattern->SetAnimName("Left_Attack").SetDamage(100.0f).SetSpeed(20.0f).SetRange(_info._attackRange).SetAfterDelay(0.5);
	pattern->SetIsWarning(false).SetWarningName("");
	pattern->SetAttackState(BossPattern::eAttackState::ePush);

	pattern->_subObject.emplace_back(_leftHand);

	auto leftHandLogic = CreateBasicAttackLogic(pattern, _leftHand, 10);

	pattern->SetLogic(leftHandLogic);


	// ù ���� ������ �¾Ҵٸ� ������ ���ݵ� ����
	pattern->_subObject.emplace_back(_rightHand);

	auto rightHandLogic = CreateBasicAttackLogic(pattern, "Right_Attack", _rightHand, 10);

	pattern->SetLogic(rightHandLogic);

	_basicPattern.emplace_back(pattern);
}

void KunrealEngine::Kamen::RightAttack()
{
	BossPattern* pattern = new BossPattern();

	pattern->SetPatternName("Right_Attack_Once");

	pattern->SetAnimName("Right_Attack").SetDamage(100.0f).SetSpeed(20.0f).SetRange(_info._attackRange).SetAfterDelay(0.5f);
	pattern->SetIsWarning(false).SetWarningName("");
	pattern->SetAttackState(BossPattern::eAttackState::ePush);

	pattern->_subObject.emplace_back(_rightHand);

	// ù ������ ������ �¾Ҵٸ� ���� ���ݵ� ����
	auto rightAttackLogic = [pattern, this]()
		{
			auto animator = _boss->GetComponent<Animator>();
			auto isAnimationPlaying = animator->Play("Right_Attack", pattern->_speed, false);

			// ���� �������� �Ѿ�� ������ üũ�� �ݶ��̴��� Ű��
			if (_maxColliderOnCount > 0)
			{
				if (animator->GetCurrentFrame() >= 10.0f)
				{
					if (_rightHand != nullptr)
					{
						_rightHand->GetComponent<BoxCollider>()->SetActive(true);
					}
				}
			}
			
			// 2Ÿ ����
			if (isAnimationPlaying == false)
			{
				pattern->SetNextLogic(true);
				_maxColliderOnCount = pattern->_maxColliderOnCount;
				return false;
			}

			return true;
		};

	pattern->SetLogic(rightAttackLogic);

	pattern->_subObject.emplace_back(_leftHand);

	// ù ������ ������ �¾Ҵٸ� ���� ���ݵ� ����
	auto attackLogic = [pattern, this]()
		{
			auto animator = _boss->GetComponent<Animator>();

			// ȸ�� ���� �ȵƴٸ�
			if (_isRotateFinish == false)
			{
				animator->Play("Idle", pattern->_speed, false);

				// ȸ�� ��Ŵ
				auto rotateFinish = Rotate(180, 200.0f);

				// ȸ�� ������ �ʾҴٸ�
				if (rotateFinish == false)
				{
					// ��� ȸ����Ű�� ����
					return true;
				}
				else
				{
					_isRotateFinish = true;
				}
			}

			if (animator->GetNowAnimationName() == "Idle")
			{
				animator->Stop();
			}

			auto isAnimationPlaying = animator->Play("Left_Attack", pattern->_speed, false);

			// ���� �������� �Ѿ�� ������ üũ�� �ݶ��̴��� Ű��
			if (_maxColliderOnCount > 0)
			{
				if (animator->GetCurrentFrame() >= 10.0f)
				{
					if (_leftHand != nullptr)
					{
						_leftHand->GetComponent<BoxCollider>()->SetActive(true);
					}
				}
			}

			if (isAnimationPlaying == false)
			{
				_isRotateFinish = false;
				return false;
			}

			return true;
		};

	pattern->SetLogic(attackLogic);

	_basicPattern.emplace_back(pattern);
}

void KunrealEngine::Kamen::SpellAttack()
{
	BossPattern* pattern = new BossPattern();

	pattern->SetPatternName("Spell");

	pattern->SetAnimName("Spell").SetDamage(100.0f).SetSpeed(20.0f).SetRange(_info._attackRange + 20.0f).SetAfterDelay(2.0f);
	pattern->SetIsWarning(true).SetWarningName("Spell");

	pattern->_subObject.emplace_back(_lazer);

	auto spellLogic = [pattern, this]()
	{
		auto animator = _boss->GetComponent<Animator>();
		auto isAnimationPlaying = animator->Play(pattern->_animName, pattern->_speed, false);

		// ���� �������� �Ѿ�� ������ üũ�� �ݶ��̴��� Ű��
		if (_maxColliderOnCount > 0)
		{
			if (animator->GetCurrentFrame() >= 30.0f)
			{
				if (animator->GetCurrentFrame() >= 55.0f)
				{
					_lazer->GetComponent<Particle>()->SetActive(false);
				}
				else
				{
					pattern->SetSpeed(10.0f);
					_lazer->GetComponent<Particle>()->SetActive(true);
					_lazer->GetComponent<Particle>()->SetParticlePos(0.0f, 0.0f, 0.0f);
					_lazer->GetComponent<Particle>()->SetParticleSize(30.f * ToolBox::GetRandomFloat(0.3f, 1.0f), 20.0f * ToolBox::GetRandomFloat(0.1f, 1.0f));
				}
			}
		}
		if (isAnimationPlaying == false)
		{
			pattern->SetSpeed(20.0f);
			_lazer->SetActive(false);
			_maxColliderOnCount = pattern->_maxColliderOnCount;
			return false;
		}

		return true;
	};


	pattern->SetLogic(spellLogic);

	pattern->SetPattern(pattern);

	_basicPattern.emplace_back(pattern);
}


void KunrealEngine::Kamen::CallAttack()
{
	BossPattern* pattern = new BossPattern();

	pattern->SetPatternName("Call");

	pattern->SetAnimName("Call").SetDamage(100.0f).SetSpeed(20.0f).SetRange(_info._attackRange + 50.0f).SetAfterDelay(1.0f);
	pattern->SetIsWarning(true).SetWarningName("Call").SetMaxColliderCount(1).SetAttackState(BossPattern::eAttackState::ePush);
	pattern->SetRangeOffset(-20.0f);
	pattern->SetAttackState(BossPattern::eAttackState::eParalysis);

	pattern->_subObject.emplace_back(_call);

	auto callLogic = [pattern, this]()
	{
		auto animator = _boss->GetComponent<Animator>();
		auto isAnimationPlaying = animator->Play(pattern->_animName, pattern->_speed, false);

		// ���� �������� �Ѿ�� ������ üũ�� �ݶ��̴��� Ű��
		if (animator->GetCurrentFrame() >= 20)
		{
			if (_maxColliderOnCount > 0)
			{
				// �ݶ��̴� Ű��
				_call->GetComponent<BoxCollider>()->SetActive(true);
				// ��ƼŬ Ű��
				_call->GetComponent<Particle>()->SetActive(true);
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
				_callMoveDistance += (pattern->_speed*1.5) * TimeManager::GetInstance().GetDeltaTime();
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
			_call->GetComponent<BoxCollider>()->SetActive(false);
			_call->GetComponent<Particle>()->SetActive(false);
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

	_basicPattern.emplace_back(pattern);
}


void KunrealEngine::Kamen::EmergenceAttack()
{
	BossPattern* pattern = new BossPattern();

	pattern->SetPatternName("core1");

	pattern->SetAnimName("Emergence").SetDamage(100.0f).SetSpeed(15.0f).SetRange(_info._attackRange).SetAfterDelay(3.0f);
	pattern->SetIsWarning(true).SetWarningName("Call").SetRangeOffset(-20.0f);
	pattern->SetMaxColliderCount(1);
	pattern->SetAttackState(BossPattern::eAttackState::eParalysis);
	pattern->SetTriggerHp(100.0f);

	for (int i = 0; i < 9; i++)
	{
		std::string index = "fake" + std::to_string(i+1);
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

		boss->SetActive(false);

		pattern->_subObject.emplace_back(boss);
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
				int targetIndex = ToolBox::GetRandomNum(_fakeBoss.size() - 1);
				_fakeBoss[targetIndex]->SetActive(false);

				_isRandomStart = true;
			}

			// ���� ���� ���� �߾����� �ڷ���Ʈ
			if (!_isRotateFinish)
			{
				auto isTeleportFinish = Teleport(DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), true, 3.0f);

				// ���� ������ ���� ����
				if (!isTeleportFinish)
				{
					return true;
				}
				else
				{

					// ���� ������ �ٽ� ���ƿ��� �ʵ���
					_isRotateFinish = true;
				}
			}

			/// emergence ���ִϸ��̼� �ѹ� ��������� ���� �̱���

			// ���� ���� �� �ʱ�ȭ���ٰ͵�
			if (! _isCoreStart)
			{
				for (const auto& index : _fakeBoss)
				{
					if (index->GetActivated() == false)
					{
						ForceMove(index->GetComponent<Transform>()->GetPosition());
						_bossTransform->SetRotation(_bossTransform->GetRotation().x, index->GetComponent<Transform>()->GetRotation().y, _bossTransform->GetRotation().z);
					}
					else
					{
						index->GetComponent<MeshRenderer>()->SetActive(true);
						index->GetComponent<BoxCollider>()->SetActive(true);
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

				// ���� ������ �����Ŵ
				pattern->_playNextPattern = true;
				return false;
			}

			return true;
		};

	pattern->SetLogic(core1Logic, false);


	auto attackLogic = [pattern, this]()
		{
			auto animator = _boss->GetComponent<Animator>();

			auto isAnimationPlaying = animator->Play("Left_Attack", pattern->_speed, false);

			if (animator->GetCurrentFrame() >= 0)
			{
				if (_maxColliderOnCount > 0)
				{
					// �ݶ��̴� Ű��
					_leftHand->SetActive(true);
					_leftHand->GetComponent<BoxCollider>()->SetActive(true);
				}
			}

			if (_maxColliderOnCount == 0)
			{
				_isRotateFinish = false;
				return false;
			}

			if (isAnimationPlaying == false)
			{
				if (_maxColliderOnCount > 0)
				{
					int a = 5;
				}
			}

			return true;
		};

	pattern->SetLogic(attackLogic, false);

	pattern->_subObject.emplace_back(_leftHand);

	_corePattern.emplace_back(pattern);
}

void KunrealEngine::Kamen::BackStepCallAttack()
{
	BossPattern* pattern = new BossPattern();

	pattern->SetPatternName("BackStepCall");

	pattern->SetAnimName("Call").SetDamage(100.0f).SetSpeed(15.0f).SetRange(_info._attackRange + 70.0f).SetAfterDelay(2.0f);
	pattern->SetIsWarning(true).SetWarningName("Call").SetRangeOffset(-20.0f);
	pattern->SetMaxColliderCount(1);
	pattern->SetAttackState(BossPattern::eAttackState::eParalysis);
	pattern->_subObject.emplace_back(_call);

	// ���� �Լ� ���� �����ϵ��� �־��ֱ�
	pattern->SetLogic(CreateBackStepLogic(pattern, 50.0f, 30.0f));

	auto callLogic = [pattern, this]()
	{
		auto animator = _boss->GetComponent<Animator>();
		auto isAnimationPlaying = animator->Play(pattern->_animName, pattern->_speed, false);

		// ���� �������� �Ѿ�� ������ üũ�� �ݶ��̴��� Ű��
		if (animator->GetCurrentFrame() >= 20)
		{
			if (_maxColliderOnCount > 0)
			{
				// ��ƼŬ Ű��
				_call->GetComponent<BoxCollider>()->SetActive(true);
				// �ݶ��̴� Ű��
				_call->GetComponent<Particle>()->SetActive(true);
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
			_call->GetComponent<BoxCollider>()->SetActive(false);
			_call->GetComponent<Particle>()->SetActive(false);
			return false;
		}

		return true;
	};

	pattern->SetLogic(callLogic, false);
	
	// �̴ϼȶ����� ���� �Լ� �־��ֱ�
	pattern->SetInitializeLogic(_callInitLogic);

	_basicPattern.emplace_back(pattern);
}