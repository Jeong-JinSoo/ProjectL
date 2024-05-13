#include <functional>
#include "ToolBox.h"
#include "TimeManager.h"
#include "SceneManager.h"
#include "Ent.h"

KunrealEngine::Ent::Ent()
	: Boss(), _leftHand(nullptr), _rightHand(nullptr), _leftRoot(nullptr), _rightRoot(nullptr), _bigRootShotStart(false),
	_callMoveDistance(0.0f), _isRotateFinish(false), _isCoreStart(false), _isRandomStart(false), _isMove(false),
	_leftAttack(nullptr), _rightAttack(nullptr), _bigRootShot(nullptr), _backStep(nullptr)
{
	BossBasicInfo info;

	info.SetHp(100).SetPhase(3).SetArmor(10).SetDamage(100).SetMoveSpeed(5.0f).SetsStaggeredGauge(100.0f);
	info.SetAttackRange(5.0f);

	SetInfo(info);
}

KunrealEngine::Ent::~Ent()
{

}

void KunrealEngine::Ent::Initialize()
{
	// �ݵ�� �ؾ��Ѵٰ� ��
	Boss::Initialize(this->GetOwner());	
}

void KunrealEngine::Ent::Release()
{

}

void KunrealEngine::Ent::FixedUpdate()
{

}

void KunrealEngine::Ent::Update()
{
	// �ݵ�� �ؾ��Ѵٰ� ��
	Boss::Update();
}

void KunrealEngine::Ent::LateUpdate()
{

}

void KunrealEngine::Ent::OnTriggerEnter()
{

}

void KunrealEngine::Ent::OnTriggerStay()
{

}

void KunrealEngine::Ent::OnTriggerExit()
{

}

void KunrealEngine::Ent::SetActive(bool active)
{
	//this->_isActivated = active;
}

void KunrealEngine::Ent::SetMesh()
{
	_boss->AddComponent<MeshRenderer>();
	_boss->GetComponent<MeshRenderer>()->SetMeshObject("Ent_Generic/Ent_Generic");
}

void KunrealEngine::Ent::SetTexture()
{
	auto texSize = _boss->GetComponent<MeshRenderer>()->GetTextures().size();
	for (int i = 0; i < texSize; i++)
	{
		_boss->GetComponent<MeshRenderer>()->SetDiffuseTexture(i, "Ent_Generic/T_Ent_1_D.tga");
		_boss->GetComponent<MeshRenderer>()->SetNormalTexture(i, "Ent_Generic/T_Ent_N.tga");
		_boss->GetComponent<MeshRenderer>()->SetEmissiveTexture(i, "Ent_Generic/T_Ent_1_E.tga");
	}
}

void KunrealEngine::Ent::SetBossTransform()
{
	_boss->GetComponent<Transform>()->SetPosition(5.0f, 0.0f, -20.0f);
	_boss->GetComponent<Transform>()->SetScale(10.0f, 10.0f, 10.0f);
}

void KunrealEngine::Ent::SetBossCollider()
{
	_boss->AddComponent<BoxCollider>();
	//_boss->GetComponent<BoxCollider>()->SetTransform(_boss, "Spine1_M");
	_boss->GetComponent<BoxCollider>()->SetBoxSize(6.0f, 18.0f, 10.0f);
	_boss->GetComponent<BoxCollider>()->SetOffset(0.0f, -1.5f, 0.0f);
}

void KunrealEngine::Ent::CreatePattern()
{
	CreateSubObject();
	//CreateLeftAttack();
	//CreateRightAttack();
	//CreateLeftRootShot();
	//CreateRightRootShot();
	CreateRandomRootAttack();
}
    
void KunrealEngine::Ent::CreateSubObject()
{
	// �޼�
	_leftHand = _boss->GetObjectScene()->CreateObject("LeftHand");
	_leftHand->AddComponent<BoxCollider>();
	_leftHand->GetComponent<BoxCollider>()->SetTransform(_boss, "ik_hand_l");
	_leftHand->GetComponent<BoxCollider>()->SetBoxSize(2.0f, 3.0f, 2.0f);
	_leftHand->GetComponent<BoxCollider>()->SetActive(false);
	
	// ������
	_rightHand = _boss->GetObjectScene()->CreateObject("rightHand");
	_rightHand->AddComponent<BoxCollider>();
	_rightHand->GetComponent<BoxCollider>()->SetTransform(_boss, "ik_hand_r");
	_rightHand->GetComponent<BoxCollider>()->SetBoxSize(2.0f, 3.0f, 2.0f);
	_rightHand->GetComponent<BoxCollider>()->SetActive(false);

	_leftRoot = _boss->GetObjectScene()->CreateObject("leftRoot");
	_leftRoot->AddComponent<MeshRenderer>();
	_leftRoot->GetComponent<MeshRenderer>()->SetMeshObject("SM_root_large_02/SM_root_large_02"); // w�ٱ� �̸�
	_leftRoot->GetComponent<MeshRenderer>()->SetActive(true);
	_leftRoot->GetComponent<Transform>()->SetRotation(86.0f, -200.0f, 250.0f);
	_leftRoot->GetComponent<Transform>()->SetScale(0.1f, 0.1f, 0.1f);
	_leftRoot->AddComponent<BoxCollider>();
	_leftRoot->GetComponent<BoxCollider>()->SetBoxSize(5.0f, 5.0f, 5.0f);
	_leftRoot->GetComponent<BoxCollider>()->SetOffset(-5.0f, 10.0f, 0.0f);
	_leftRoot->SetActive(false);

	_rightRoot = _boss->GetObjectScene()->CreateObject("rightRoot");
	_rightRoot->AddComponent<MeshRenderer>();
	_rightRoot->GetComponent<MeshRenderer>()->SetMeshObject("SM_root_large_02/SM_root_large_02"); // w�ٱ� �̸�
	_rightRoot->GetComponent<MeshRenderer>()->SetActive(true);
	_rightRoot->GetComponent<Transform>()->SetRotation(86.0f, -200.0f, 250.0f);
	_rightRoot->GetComponent<Transform>()->SetScale(0.1f, 0.1f, 0.1f);
	_rightRoot->AddComponent<BoxCollider>();
	_rightRoot->GetComponent<BoxCollider>()->SetBoxSize(5.0f, 5.0f, 5.0f);
	_rightRoot->GetComponent<BoxCollider>()->SetOffset(-5.0f, 10.0f, 0.0f);
	_rightRoot->SetActive(false);

	
	int smallRootCount = 10;

	for (int i = 0; i < smallRootCount;  ++i)
	{
		GameObject* smallRoot = nullptr;

		smallRoot = _boss->GetObjectScene()->CreateObject("smallRoot");
		smallRoot->AddComponent<MeshRenderer>();
		smallRoot->GetComponent<MeshRenderer>()->SetMeshObject("SM_root_large_02/SM_root_large_02");
		smallRoot->GetComponent<MeshRenderer>()->SetActive(true);
		smallRoot->GetComponent<Transform>()->SetRotation(90.0f, 0.0f,0.0f);
		smallRoot->GetComponent<Transform>()->SetScale(0.05f, 0.05f, 0.05f);
		smallRoot->AddComponent<BoxCollider>();
		smallRoot->GetComponent<BoxCollider>()->SetBoxSize(4.0f, 4.0f, 4.0f);
		smallRoot->GetComponent<BoxCollider>()->SetOffset(-3.0f, 8.0f, 0.0f);
		smallRoot->SetActive(false);
		_smallRootVector.push_back(smallRoot);
	}

}

void KunrealEngine::Ent::CreateLeftAttack()
{
	BossPattern* pattern = new BossPattern();

	pattern->SetPatternName("Left_Attack_Once");
	pattern->SetAnimName("Anim_Hand_Attack_L");
	// �ִϸ��̼� �߰��ؾ���
	pattern->SetDamage(100.0f);
	pattern->SetSpeed(20.0f);         
	pattern->SetRange(_info._attackRange);
	pattern->SetAfterDelay(0.5f);

	pattern->SetIsWarning(false).SetWarningName("");
	pattern->SetAttackState(BossPattern::eAttackState::ePush).SetMaxColliderCount(1);

	pattern->_subObject.emplace_back(_leftHand);

	auto leftHandLogic = CreateBasicAttackLogic(pattern, _leftHand, 10);

	pattern->SetLogic(leftHandLogic);

	//_leftAttack = pattern;
	_basicPattern.emplace_back(pattern);
}

void KunrealEngine::Ent::CreateRightAttack()
{
	BossPattern* pattern = new BossPattern();

	pattern->SetPatternName("Right_Attack_Once");
	pattern->SetAnimName("Anim_Hand_Attack_R");
	pattern->SetDamage(100.0f);
	pattern->SetSpeed(20.0f);
	pattern->SetRange(_info._attackRange);
	pattern->SetAfterDelay(0.5f);

	pattern->SetIsWarning(false).SetWarningName("");
	pattern->SetAttackState(BossPattern::eAttackState::ePush).SetMaxColliderCount(1);

	pattern->_subObject.emplace_back(_rightHand);

	auto rightHandLogic = CreateBasicAttackLogic(pattern, _rightHand, 10);

	pattern->SetLogic(rightHandLogic);

	//_rightAttack = pattern;
	_basicPattern.emplace_back(pattern);

}

void KunrealEngine::Ent::CreateLeftRootShot()
{
	BossPattern* pattern = new BossPattern();

	pattern->SetPatternName("Left_Root_Shot");
	pattern->SetAnimName("Anim_Leg_Attack_L");
	pattern->SetDamage(100.0f);
	pattern->SetSpeed(60.0f);
	pattern->SetRange(100.0f);
	pattern->SetAfterDelay(0.5f);
	pattern->SetIsWarning(true);  // ���ǥ�� ���� ������
	pattern->SetIsWarning("LRootShot");

	pattern->SetAttackState(BossPattern::eAttackState::ePush);
	pattern->SetMaxColliderCount(1);

	pattern->_subObject.emplace_back(_leftRoot);

	auto bigRootShotLogic = [pattern, this]()
		{
			
			auto animator = _boss->GetComponent<Animator>();
			auto isAnimationPlaying = animator->Play(pattern->_animName, pattern->_speed, false);

			// ���� ������ ������
			auto nowPos = _boss->GetComponent<Transform>()->GetPosition();

			if (animator->GetCurrentFrame() >= 55)
			{				
				if (pattern->_colliderOnCount > 0)
				{
					// �ݶ��̴� Ű��
					_leftRoot->GetComponent<BoxCollider>()->SetActive(true);
					// �޽� Ű��
					
				}

				// ������ �ٶ󺸴� ���� ������
				auto direction = GetDirection();
			
				auto nowPosVec = DirectX::XMLoadFloat3(&nowPos);

				auto callNowPos = _leftRoot->GetComponent<Transform>()->GetPosition();
				auto distance = ToolBox::GetDistance(nowPos, callNowPos);

				if (distance < pattern->_range)
				{
					_callMoveDistance += (pattern->_speed * 3) * TimeManager::GetInstance().GetDeltaTime();
					_leftRoot->GetComponent<MeshRenderer>()->SetActive(true);
				}
				else
				{
					animator->Stop();
					_leftRoot->GetComponent<Transform>()->SetPosition(nowPos);      
					isAnimationPlaying = false;
				}
				// ���� ������ �����ǿ��� �ٶ󺸴� ���� �������� ������
				DirectX::XMVECTOR newPosition = DirectX::XMVectorAdd(nowPosVec, DirectX::XMVectorScale(direction, _callMoveDistance));

				_leftRoot->GetComponent<Transform>()->SetPosition(newPosition.m128_f32[0], newPosition.m128_f32[1], newPosition.m128_f32[2]);
			}

			if (isAnimationPlaying == false)
			{
			//	pattern->SetSpeed(20.0f);
				_leftRoot->GetComponent<Transform>()->SetPosition(nowPos);
				_callMoveDistance = 0;
				return false;
			}			
			return true;
		};

	pattern->SetLogic(bigRootShotLogic);

	_basicPattern.emplace_back(pattern);

}

void KunrealEngine::Ent::CreateRightRootShot()
{
	BossPattern* pattern = new BossPattern();

	pattern->SetPatternName("Right_Root_Shot");
	pattern->SetAnimName("Anim_Leg_Attack_R");
	pattern->SetDamage(100.0f);
	pattern->SetSpeed(60.0f);
	pattern->SetRange(100.0f);
	pattern->SetAfterDelay(0.5f);
	pattern->SetIsWarning(true);  // ���ǥ�� ���� ������
	pattern->SetIsWarning("RRootShot");

	pattern->SetAttackState(BossPattern::eAttackState::ePush);
	pattern->SetMaxColliderCount(1);

	pattern->_subObject.emplace_back(_rightRoot);

	auto bigRootShotLogic = [pattern, this]()
		{

			auto animator = _boss->GetComponent<Animator>();
			auto isAnimationPlaying = animator->Play(pattern->_animName, pattern->_speed, false);

			// ���� ������ ������
			auto nowPos = _boss->GetComponent<Transform>()->GetPosition();

			if (animator->GetCurrentFrame() >= 40)
			{
				if (pattern->_colliderOnCount > 0)
				{
					// �ݶ��̴� Ű��
					_rightRoot->GetComponent<BoxCollider>()->SetActive(true);
					// �޽� Ű��

				}

				// ������ �ٶ󺸴� ���� ������
				auto direction = GetDirection();

				auto nowPosVec = DirectX::XMLoadFloat3(&nowPos);

				auto callNowPos = _rightRoot->GetComponent<Transform>()->GetPosition();
				auto distance = ToolBox::GetDistance(nowPos, callNowPos);

				if (distance < pattern->_range)
				{
					_callMoveDistance += (pattern->_speed * 3) * TimeManager::GetInstance().GetDeltaTime();
					_rightRoot->GetComponent<MeshRenderer>()->SetActive(true);
				}
				else
				{
					animator->Stop();
					_rightRoot->GetComponent<Transform>()->SetPosition(nowPos);
					isAnimationPlaying = false;
				}
				// ���� ������ �����ǿ��� �ٶ󺸴� ���� �������� ������
				DirectX::XMVECTOR newPosition = DirectX::XMVectorAdd(nowPosVec, DirectX::XMVectorScale(direction, _callMoveDistance));

				_rightRoot->GetComponent<Transform>()->SetPosition(newPosition.m128_f32[0] + 5, newPosition.m128_f32[1], newPosition.m128_f32[2] + 5);
			}

			if (isAnimationPlaying == false)
			{
				//pattern->SetSpeed(20.0f);
				_rightRoot->GetComponent<Transform>()->SetPosition(nowPos);
				_callMoveDistance = 0;
				return false;
			}
			return true;
		};

	pattern->SetLogic(bigRootShotLogic);

	_basicPattern.emplace_back(pattern);
}

void KunrealEngine::Ent::CreateRandomRootAttack()
{
	BossPattern* pattern = new BossPattern();

	pattern->SetPatternName("Small_Root_Shot");
	pattern->SetAnimName("Anim_Healing");
	pattern->SetDamage(100.0f);
	pattern->SetSpeed(60.0f);
	pattern->SetRange(100.0f);
	//pattern->SetAfterDelay(0.5f);
	pattern->SetIsWarning(true);  // ���ǥ�� ���� ������
	pattern->SetIsWarning("SmallRootShot");

	pattern->SetAttackState(BossPattern::eAttackState::ePush);
	pattern->SetMaxColliderCount(1);

	for (auto smallRoot : _smallRootVector)
	{
		pattern->_subObject.emplace_back(smallRoot);	
	}

	auto SmallRootShotLogic = [pattern, this]()
	{
			auto animator = _boss->GetComponent<Animator>();
			auto isAnimationPlaying = animator->Play(pattern->_animName, pattern->_speed, false);

			// ���� ������ ������
			auto nowPos = _boss->GetComponent<Transform>()->GetPosition();
			auto nowPlayerPos = _player->GetComponent<Transform>()->GetPosition();

			float smallRootSummonRange = 20;


			float currentTime = TimeManager::GetInstance().GetDeltaTime();
			static float checkTime = 0.0f;

			if (animator->GetCurrentFrame() >= 80)
			{
				if (_isMove == false)
				{
					for (auto smallRoot : _smallRootVector)
					{
						smallRoot->GetComponent<MeshRenderer>()->SetActive(true);

						if (pattern->_colliderOnCount > 0)
						{
							// �ݶ��̴� Ű��
							smallRoot->GetComponent<BoxCollider>()->SetActive(true);
						}


						smallRoot->GetComponent<MeshRenderer>()->SetActive(true);
						float randomX = GetRandomRange(nowPlayerPos.x, smallRootSummonRange);
						float randomZ = GetRandomRange(nowPlayerPos.z, smallRootSummonRange);

						smallRoot->GetComponent<Transform>()->SetPosition(randomX, smallRoot->GetComponent<Transform>()->GetPosition().y, randomZ);

						smallRoot->SetActive(true);

						if (currentTime > checkTime)
						{
							while (currentTime > checkTime)
							{
								checkTime += 0.002f;
							}
						}
					}
					_isMove = true;
				}
				
			}

			if (isAnimationPlaying == false)
			{
				//pattern->SetSpeed(20.0f);
				for (auto smallRoot : _smallRootVector)
				{
					smallRoot->GetComponent<Transform>()->SetPosition(nowPos);
				}
				_isMove = false;
				return false;
			}


	};

	pattern->SetLogic(SmallRootShotLogic);

	_basicPattern.emplace_back(pattern);
}

void KunrealEngine::Ent::CreateJumpAttack()
{

}

void KunrealEngine::Ent::CreateLeftFireShot()
{

}    

void KunrealEngine::Ent::CreateRightFireShot()
{

}

void KunrealEngine::Ent::CreateRandomFireAttack()
{

}

float KunrealEngine::Ent::GetRandomRange(float center, float range)
{
	float randomNumber = ((float)rand() / RAND_MAX) * 2.0f - 1.0f; 
	return center + randomNumber * range;
}
