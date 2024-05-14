#include <functional>
#include "ToolBox.h"
#include "TimeManager.h"
#include "SceneManager.h"
#include "Ent.h"

KunrealEngine::Ent::Ent()
	: Boss(), _leftHand(nullptr), _rightHand(nullptr), _leftRoot(nullptr), _rightRoot(nullptr), _bigRootShotStart(false),
	_callMoveDistance(0.0f), _isRotateFinish(false), _isCoreStart(false), _isRandomStart(false),
	_leftAttack(nullptr), _rightAttack(nullptr), _bigRootShot(nullptr), _isIdleHealing(false), _rootY(-50.0f), randomX(0.0f), randomZ(0.0f)
{
	BossBasicInfo info;

	info.SetHp(100).SetPhase(3).SetArmor(10).SetDamage(100).SetMoveSpeed(15.0f).SetsStaggeredGauge(100.0f);
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

	CorePatternObjectFirst();
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
	//CreateRandomRootAttack();
	CreateEyeLight();
	//CreateJumpAttack();
	//CreateCorePatternFirst();
}
    
void KunrealEngine::Ent::CreateSubObject()
{
	// �޼�
	_leftHand = _boss->GetObjectScene()->CreateObject("LeftHand");
	_leftHand->AddComponent<BoxCollider>();
	_leftHand->GetComponent<BoxCollider>()->SetTransform(_boss, "ik_hand_l");
	_leftHand->GetComponent<BoxCollider>()->SetBoxSize(4.0f, 3.0f, 4.0f);
	_leftHand->GetComponent<BoxCollider>()->SetActive(false);
	
	// ������
	_rightHand = _boss->GetObjectScene()->CreateObject("rightHand");
	_rightHand->AddComponent<BoxCollider>();
	_rightHand->GetComponent<BoxCollider>()->SetTransform(_boss, "ik_hand_r");
	_rightHand->GetComponent<BoxCollider>()->SetBoxSize(4.0f, 3.0f, 4.0f);
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
		smallRoot->GetComponent<BoxCollider>()->SetBoxSize(3.0f, 3.0f, 12.0f);
		smallRoot->GetComponent<BoxCollider>()->SetOffset(-3.0f, 8.0f, 0.0f);
		smallRoot->SetActive(false);
		_smallRootVector.push_back(smallRoot);
	}

	_colJumpAttack = _boss->GetObjectScene()->CreateObject("Jump");
	_colJumpAttack->AddComponent<BoxCollider>();
	_colJumpAttack->GetComponent<BoxCollider>()->SetOffset(0.0f, 3.0f, 0.0f);
	_colJumpAttack->GetComponent<BoxCollider>()->SetBoxSize(10.0f, 5.0f, 10.0f);
	_colJumpAttack->SetActive(false);


	// �ȱ�
	// ���̵�Ÿ�� 3
	// ������Ÿ�� 12
	// ������ 2

	_eyeLight = _boss->GetObjectScene()->CreateObject("EyeLight");
	_eyeLight->AddComponent<Particle>();
	_eyeLight->GetComponent<Particle>()->SetParticleEffect("Laser", "Resources/Textures/Particles/RailGun_64.dds", 1000);
	_eyeLight->GetComponent<Particle>()->SetParticleDuration(3.0f, 12.0f);
	_eyeLight->GetComponent<Particle>()->SetParticleVelocity(70.0f, false);
	_eyeLight->GetComponent<Particle>()->SetTransform(_boss, "jaw");
	_eyeLight->GetComponent<Particle>()->SetParticleRotation(210.0f, _bossTransform->GetRotation().y, 0.0f);
	_eyeLight->GetComponent<Particle>()->SetParticleSize(4.0f, 4.0f);
	_eyeLight->GetComponent<Particle>()->SetActive(false);

}

void KunrealEngine::Ent::CreateLeftAttack()
{
	BossPattern* pattern = new BossPattern();

	pattern->SetPatternName("Left_Attack_Once");
	pattern->SetAnimName("Anim_Hand_Attack_L");
	// �ִϸ��̼� �߰��ؾ���
	pattern->SetDamage(100.0f);
	pattern->SetSpeed(60.0f);         
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
	pattern->SetSpeed(60.0f);
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
	pattern->SetSpeed(50.0f);
	pattern->SetRange(60.0f);
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
	pattern->SetSpeed(50.0f);
	pattern->SetRange(60.0f);
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
	pattern->SetAnimName("Anim_Root_Shot");
	pattern->SetDamage(100.0f);
	pattern->SetSpeed(40.0f);	
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



				
			for (auto smallRoot : _smallRootVector)
			{
				if (animator->GetCurrentFrame() >= 15 && animator->GetCurrentFrame() < 16)
				{
					randomX = GetRandomRange(nowPlayerPos.x, smallRootSummonRange);
					randomZ = GetRandomRange(nowPlayerPos.z, smallRootSummonRange);
					smallRoot->GetComponent<Transform>()->SetPosition(randomX, _rootY, randomZ);
					
				}

				if (animator->GetCurrentFrame() >= 40)
				{
					smallRoot->GetComponent<MeshRenderer>()->SetActive(true);

					if (pattern->_colliderOnCount > 0)
					{
						// �ݶ��̴� Ű��
						smallRoot->GetComponent<BoxCollider>()->SetActive(true);
					}

					smallRoot->GetComponent<MeshRenderer>()->SetActive(true);

					smallRoot->SetActive(true);
					
					// �輺�� ����ȣ�� ������ �츮��
					if (_rootY < 0.0f)
					{
						_rootY += 0.5f;
						int a = 10;
					}
					smallRoot->GetComponent<Transform>()->SetPosition(smallRoot->GetComponent<Transform>()->GetPosition().x, _rootY, smallRoot->GetComponent<Transform>()->GetPosition().z);
				}
			}

			if (isAnimationPlaying == false)
			{
				//pattern->SetSpeed(20.0f);
				for (auto smallRoot : _smallRootVector)
				{
					smallRoot->GetComponent<Transform>()->SetPosition(nowPos);
				}
				_rootY = -20.0f;
				return false;
			}


	};

	pattern->SetLogic(SmallRootShotLogic);

	_basicPattern.emplace_back(pattern);
}

void KunrealEngine::Ent::CreateJumpAttack()
{
	BossPattern* pattern = new BossPattern();

	pattern->SetPatternName("Jump_Attack");
	pattern->SetAnimName("Anim_Jump_Start");
	pattern->SetDamage(100.0f);
	pattern->SetSpeed(60.0f);
	pattern->SetRange(100.0f);
	//pattern->SetAfterDelay(0.5f);
	pattern->SetIsWarning(true);  // ���ǥ�� ���� ������
	pattern->SetIsWarning("SmallRootShot");
	pattern->SetAttackState(BossPattern::eAttackState::ePush);
	pattern->SetMaxColliderCount(1);

	std::function logic = [this]()
		{
			Startcoroutine(JumpAttackCo);
			return true;
		};

	pattern->SetLogic(logic);
	_basicPattern.emplace_back(pattern);
}

void KunrealEngine::Ent::CreateEyeLight()
{
	BossPattern* pattern = new BossPattern();

	pattern->SetPatternName("Eye_Light");
	pattern->SetAnimName("Mutant_Roaring");
	pattern->SetDamage(100.0f);
	pattern->SetSpeed(40.0f);
	pattern->SetRange(100.0f);
	//pattern->SetAfterDelay(0.5f);
	pattern->SetIsWarning(true);  // ���ǥ�� ���� ������
	pattern->SetIsWarning("EyeLight");
	pattern->SetAttackState(BossPattern::eAttackState::ePush);
	pattern->SetMaxColliderCount(1);

	pattern->_subObject.emplace_back(_eyeLight);

	auto eyeLightLogic = [pattern, this]()
		{
			auto animator = _boss->GetComponent<Animator>();
			auto isAnimationPlaying = animator->Play(pattern->_animName, pattern->_speed, false);

			if (pattern->_colliderOnCount > 0)
			{
				if (animator->GetCurrentFrame() >= 35)
				{
					if (_eyeLight->GetComponent<Particle>() != nullptr)
					{
						_eyeLight->GetComponent<Particle>()->SetActive(true);
					}
				}
			}

			//auto nowRot = _boss->GetComponent<Transform>()->GetRotation();
			//_eyeLight->GetComponent<Particle>()->SetParticleRotation(nowRot.x, nowRot.y, nowRot.z);

			if (pattern->_colliderOnCount == 0)
			{
				pattern->SetNextPatternForcePlay(true);
			}

			if (isAnimationPlaying == false)
			{
				return false;
			}			
		};

	pattern->SetLogic(eyeLightLogic);
	_basicPattern.emplace_back(pattern);
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

void KunrealEngine::Ent::CreateCorePatternFirst()
{
	BossPattern* pattern = new BossPattern();

	pattern->SetPatternName("Core_Pattern_First"); // ���������� ���Ŀ�
	pattern->SetAnimName("Anim_Healing");
	pattern->SetDamage(100.0f);
	pattern->SetSpeed(30.0f);
	pattern->SetRange(100.0f);
	//pattern->SetAfterDelay(0.5f);
	pattern->SetIsWarning(true);  // ���ǥ�� ���� ������
	pattern->SetIsWarning("CorePatternFirst");

	pattern->SetAttackState(BossPattern::eAttackState::ePush);
	pattern->SetMaxColliderCount(1);

	auto CorePatternFirstLogic = [pattern, this]()
	{
			auto isPatternEnd = false;
			auto animator = _boss->GetComponent<Animator>();
			// ���� ������ ������
			auto nowPos = _boss->GetComponent<Transform>()->GetPosition();
			auto nowPlayerPos = _player->GetComponent<Transform>()->GetPosition();

			if (_isIdleHealing == false)
			{
				while (true)
				{
					if (!(animator->Play("Anim_Idle_To_Healing", pattern->_speed, false)))
					{
						_isIdleHealing = true;
						animator->Stop();
						break;
					}
				}
			}

			auto isAnimationPlaying = animator->Play(pattern->_animName, pattern->_speed, true); // ��� ����

			if (animator->GetCurrentFrame() >= 60)
			{
				isPatternEnd = true;		
			}

			for (auto treeObject : _treeObject)
			{
				treeObject->SetActive(true);
				treeObject->GetComponent<MeshRenderer>()->SetActive(true);
				// �������� ��ȭ���ʿ� ����Ѱ� �־���
				int index = 0;
				for (int j = 0; j < 2; j++)
				{
					//for (int i = 0; i < 2; i++)
					//{
						_treeObject[index]->GetComponent<Transform>()->SetPosition(80.0f - (80.0 * j * 2.0f), 0.0f, 0.0f);
						index++;
						_treeObject[index]->GetComponent<Transform>()->SetPosition(0.0f, 0.0f, 80.0f - (80.0 * j * 2.0f));
						index++;
					//}
				}

				if (treeObject->GetComponent<BoxCollider>()->IsCollided())
				{
					if (treeObject->GetComponent<BoxCollider>()->GetTargetObject() == _player)  // ���� ������ �ǰ� ���ϴ� ������ ����, ���߿� ����
					{
						// �ݶ��̴� ���� üũ�� ���� �ϱ�

						if (treeObject->GetObjectName() == "treeObjectReal") // ������Ʈ�� �̸��� ���� �ٸ� ó��
						{
							// �÷��̾ �� ������ ���ݷ� �ʿ�
							

							// ������ ü���� �������־����, �ϴ� ����
							


						}
						else
						{

						}
					}
				}

				

			}






			if (isAnimationPlaying == false && isPatternEnd == true)
			{
				return false;
			}
	};

	pattern->SetLogic(CorePatternFirstLogic);
	_basicPattern.emplace_back(pattern);
}

void KunrealEngine::Ent::CorePatternObjectFirst()
{
	int treeCount = 3;

	for (int i = 0; i < treeCount; ++i)
	{
		GameObject* treeObject = nullptr;
		treeObject = _boss->GetObjectScene()->CreateObject("treeObject");
		treeObject->AddComponent<MeshRenderer>();
		treeObject->GetComponent<MeshRenderer>()->SetMeshObject("SM_tree_large_ancient_01/SM_tree_large_ancient_01");
		treeObject->GetComponent<MeshRenderer>()->SetActive(true);
		treeObject->GetComponent<Transform>()->SetRotation(0.0f, 0.0f, 0.0f);
		treeObject->GetComponent<Transform>()->SetScale(0.1f, 0.1f, 0.1f);
		treeObject->AddComponent<BoxCollider>();
		treeObject->GetComponent<BoxCollider>()->SetBoxSize(5.0f, 10.0f, 5.0f);
		treeObject->GetComponent<BoxCollider>()->SetOffset(0.0f, 8.0f, 0.0f);
		treeObject->SetActive(false);
		_treeObject.push_back(treeObject);
	}

	_treeObjectReal = _boss->GetObjectScene()->CreateObject("treeObjectReal");
	_treeObjectReal->AddComponent<MeshRenderer>();
	_treeObjectReal->GetComponent<MeshRenderer>()->SetMeshObject("SM_tree_large_ancient_01/SM_tree_large_ancient_01");
	_treeObjectReal->GetComponent<MeshRenderer>()->SetActive(true);
	_treeObjectReal->GetComponent<Transform>()->SetRotation(0.0f, 0.0f, 0.0f);
	_treeObjectReal->GetComponent<Transform>()->SetScale(0.1f, 0.1f, 0.1f);
	_treeObjectReal->AddComponent<BoxCollider>();
	_treeObjectReal->GetComponent<BoxCollider>()->SetBoxSize(5.0f, 10.0f, 5.0f);
	_treeObjectReal->GetComponent<BoxCollider>()->SetOffset(0.0f, 8.0f, 0.0f);
	_treeObjectReal->SetActive(false);
	_treeObject.push_back(_treeObjectReal);

}

float KunrealEngine::Ent::GetRandomRange(float center, float range)
{
	float randomNumber = ((float)rand() / RAND_MAX) * 2.0f - 1.0f; 
	return center + randomNumber * range;
}

bool KunrealEngine::Ent::Move(DirectX::XMFLOAT3& startPos, DirectX::XMFLOAT3& targetPos, float speed)
{
	float moveSpeed = speed * TimeManager::GetInstance().GetDeltaTime();

	// ���������� �Ÿ� ���
	auto dist = ToolBox::GetDistance(startPos, targetPos);
	if (dist > 0.5f)
	{
		DirectX::XMVECTOR currentPosVec = DirectX::XMLoadFloat3(&startPos);

		DirectX::XMVECTOR direction = DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&targetPos), currentPosVec);

		direction = DirectX::XMVector3Normalize(direction);

		DirectX::XMVECTOR newPosition = DirectX::XMVectorAdd(currentPosVec, DirectX::XMVectorScale(direction, moveSpeed));
		_bossTransform->SetPosition(newPosition.m128_f32[0], 0.0f, newPosition.m128_f32[2]);

		return true;
	}

	return false;
}
