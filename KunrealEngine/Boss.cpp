#include <random>
#include <algorithm>
#include <DirectXMath.h>
#include "GameObject.h"
#include "Animator.h"
#include "TimeManager.h"
#include "Transform.h"
#include "SceneManager.h"
#include "MeshRenderer.h"
#include "Scene.h"
#include "Player.h"
#include "ToolBox.h"
#include "BoxCollider.h"
#include "InputSystem.h"
#include "Navigation.h"
#include "Boss.h"

KunrealEngine::Boss::Boss()
	: _info(), _status(BossStatus::ENTER), _boss(nullptr), _player(nullptr), _patternIndex(-1), _exPatternIndex(-1),
	_distance(0.0f), _isCorePattern(false),
	_basicPattern(), _corePattern(), _nowPattern(nullptr),
	_maxColliderOnCount(1),
	_isStart(false), _isHit(false), _isRotateFinish(false),
	_bossTransform(nullptr), _playerTransform(nullptr),
	_stopover(), _nodeCount(0), _direction(), _prevPos(),
	_isMoving(false), _isRotate(false)
{

}

KunrealEngine::Boss::~Boss()
{
	for (auto index : _basicPattern)
	{
		delete index;
	}

	//auto collider = _boss->GetComponent<BoxCollider>();
	//if (collider != nullptr)
	//{
	//	_boss->DeleteComponent(collider);
	//}
	//
	//auto meshRenderer = _boss->GetComponent<MeshRenderer>();
	//if (meshRenderer != nullptr)
	//{
	//	_boss->DeleteComponent(meshRenderer);
	//}
}


void KunrealEngine::Boss::DebugTest()
{
	// �ִϸ��̼� Ȯ�ο�
	GRAPHICS->DrawDebugText(500, 0, 50, "%s", _boss->GetComponent<Animator>()->GetNowAnimationName().c_str());

	// ���� Ȯ�ο�
	switch (_status)
	{
		case BossStatus::ENTER:
		{
			GRAPHICS->DrawDebugText(200, 350, 20, "Boss Status : ENTER");
			break;
		}
		case BossStatus::IDLE:
		{
			GRAPHICS->DrawDebugText(200, 350, 20, "Boss Status : IDLE");
			break;
		}
		case BossStatus::CHASE:
		{
			GRAPHICS->DrawDebugText(200, 350, 20, "Boss Status : CHASE");
			break;
		}
		case BossStatus::STAGGERED:
		{
			GRAPHICS->DrawDebugText(200, 350, 20, "Boss Status : ON_STAGGERED");
			break;
		}
		case BossStatus::OFF_STAGGRED:
		{
			GRAPHICS->DrawDebugText(200, 350, 20, "Boss Status : OFF_STAGGERED");
			break;
		}
		case BossStatus::DEAD:
		{
			GRAPHICS->DrawDebugText(200, 350, 20, "Boss Status : DEAD");
			break;
		}
		case BossStatus::PATTERN_READY:
		{
			GRAPHICS->DrawDebugText(200, 350, 20, "Boss Status : PATTERN_READY");
			break;
		}
		case BossStatus::BASIC_ATTACK:
		{
			GRAPHICS->DrawDebugText(200, 350, 20, "Boss Status : BASIC_ATTACK");
			break;
		}
		case BossStatus::CORE_ATTACK:
		{
			GRAPHICS->DrawDebugText(200, 350, 20, "Boss Status : CORE_ATTACK");
			break;
		}
		case BossStatus::PATTERN_END:
		{
			GRAPHICS->DrawDebugText(200, 350, 20, "Boss Status : PATTERN_END");
			break;
		}
		default:
			break;
	}
}

void KunrealEngine::Boss::Initialize(GameObject* boss)
{
	_player = SceneManager::GetInstance().GetCurrentScene()->GetGameObject("Player");

	_boss = boss;

	_boss->SetTag("BOSS");

	_bossTransform = _boss->GetComponent<Transform>();
	_playerTransform = _player->GetComponent<Transform>();

	SetMesh();

	SetTexture();

	SetBossCollider();

	SetBossTransform();

	CreatePattern();

	SetSubObject(false);

	SortCorePattern();

	SetSubObjectScene();
}

void KunrealEngine::Boss::Update()
{
	// ����� Ȯ�ο�
	DebugTest();

	// ĳ���Ͱ� �̵���
	if (InputSystem::GetInstance()->MouseButtonUp(1) || InputSystem::GetInstance()->KeyDown(KEY::SPACE))
	{
		// �̵�, ȸ�� ���õ� �͵� �ʱ�ȭ����
		_isRotateFinish = false;
		_isMoving = false;
		_nodeCount = 0;
	}

	Hit();
	Attack();

	switch (_status)
	{
		case BossStatus::ENTER:
		{
			Enter();
			break;
		}
		case BossStatus::IDLE:
		{
			Idle();
			GRAPHICS->DeleteAllLine();
			break;
		}
		case BossStatus::CHASE:
		{
			if (_stopover.size() > 0)
			{
				for (const auto& path : _stopover)
				{
					GRAPHICS->CreateDebugLine(path.first, path.second, DirectX::XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f));
				}
			}

			Chase();
			break;
		}
		case BossStatus::STAGGERED:
		{
			Staggred();
			break;
		}
		case BossStatus::OFF_STAGGRED:
		{
			OffStaggred();
			break;
		}
		case BossStatus::DEAD:
		{
			Dead();
			break;
		}
		case BossStatus::PATTERN_READY:
		{
			PatternReady();
			break;
		}
		case BossStatus::BASIC_ATTACK:
		{
			BasicAttack();
			break;
		}
		case BossStatus::CORE_ATTACK:
		{
			CoreAttack();
			break;
		}
		case BossStatus::PATTERN_END:
		{
			PatternEnd();
			break;
		}
		default:
		{
			break;
		}
	}
}

void KunrealEngine::Boss::Enter()
{
	auto bossPosition = _boss->GetComponent<Transform>()->GetPosition();

	auto playerPosition = _player->GetComponent<Transform>()->GetPosition();

	auto distance = ToolBox::GetDistance(bossPosition, playerPosition);

	// ���� �Ÿ� �̳��� �Ǹ� ����
	if (distance <= 1000.0f)
	{
		_isStart = true;
	}

	// �����ϸ� 
	if (_isStart)
	{
		// ���� ���� �ִϸ��̼� ����
		_boss->GetComponent<Animator>()->Play("Emergence", _info._baseAnimSpeed, false);

		// ���� ���� �ִϸ��̼� ������ �����ٸ�
		if (_boss->GetComponent<Animator>()->GetIsAnimationPlaying() == false)
		{
			// ���� �ִϸ��̼� ������ ���� �ִϸ��̼� �������� 0���� �ǵ���
			_boss->GetComponent<Animator>()->Stop();
			// IDLE�� ���� ����
			_status = BossStatus::IDLE;
		}
	}
}

void KunrealEngine::Boss::Idle()
{
	_boss->GetComponent<Animator>()->Play("Idle", _info._baseAnimSpeed, true);

	// �ٽ� ������ �����Ǿ��ִٸ�
	if (!(_corePattern).empty())
	{
		// �ٽ� ������ back�������� hp�� ���� ������ �Ǿ��ֱ⿡
		// ������ ü���� �ٽ� ������ �ߵ� ü�� ���ϰ� �ȴٸ�
		if (_info._hp <= (_corePattern).back()->_triggerHp)
		{
			// �ھ��������� ���� Ȯ��
			_isCorePattern = true;

			_nowPattern = _corePattern.back();

			// ���� �ʱ�ȭ������� �͵� �ʱ�ȭ
			_nowPattern->Initialize();

			// ���� �غ� ���·� ����
			_status = BossStatus::PATTERN_READY;

			// �Լ� ����
			return;
		}
	}

	// �ٽ� ������ �ƴ� �⺻ ������ ��쿡 ���� �⺻ ������ ����
	if (_patternIndex == -1 && !_isCorePattern)
	{
		// ���� ������ ���� ���� �ε����� ������
		_patternIndex = ToolBox::GetRandomNum(0, _basicPattern.size() - 1);

		if (_basicPattern.size() > 1)
		{
			while (_patternIndex == _exPatternIndex)
			{
				// ���� ������ ���� ���� �ε����� ������
				_patternIndex = ToolBox::GetRandomNum(0, _basicPattern.size() - 1);
			}

			// ������ ���� ������ Ȱ��ȭ�Ǿ����� �ʴٸ�
			while ((_basicPattern)[_patternIndex]->_isActive == false)
			{
				// ������ ���� ������ Ȱ��ȭ �Ǿ� ���������� �ٽ� ���� �ε����� ����
				_patternIndex = ToolBox::GetRandomNum(0, _basicPattern.size() - 1);
			}
		}
	}

	_nowPattern = _basicPattern[_patternIndex];

	// Attack ó���� ���� ���� �� �ִ� �浹�� ��� �Ͼ������ ���ϰ� ����  
	_maxColliderOnCount = _nowPattern->_maxColliderOnCount;

	// ���� �ʱ�ȭ������� �͵� �ʱ�ȭ
	_nowPattern->Initialize();

	// Chase�� ���� ��ȯ
	_status = BossStatus::CHASE;
}

void KunrealEngine::Boss::Chase()
{
	// ���� ���� ���� �ִϸ��̼��� Idle �̶��
	if (_boss->GetComponent<Animator>()->GetNowAnimationName() == "Run")
	{
		_boss->GetComponent<Animator>()->Play("Run", _info._baseAnimSpeed, true);
	}
	else
	{
		_boss->GetComponent<Animator>()->Play("Idle", _info._baseAnimSpeed, true);
	}

	auto patternRange = _nowPattern->_range;

	auto bossPosition = _bossTransform->GetPosition();

	auto playerPosition = _playerTransform->GetPosition();

	// ������ �÷��̾� ������ �Ÿ��� ����
	_distance = ToolBox::GetDistance(bossPosition, playerPosition);

	// ������ ��Ÿ��� �޾ƿ�
	if (!_isRotateFinish)
	{
		// ������ �÷��̾� ������ ������ ����
		auto angle = CalculateAngle(bossPosition, playerPosition);

		// ������ �÷��̾ �ٶ󺸰� ��
		_isRotateFinish = LookAtPlayer(angle, _info._rotationSpeed);
	}

	// ������ �÷��̾ �ٶ󺸰� �Ǿ��� ��
	if (_isRotateFinish)
	{
		// ���� ��Ÿ����� �÷��̾���� �Ÿ��� ����� ��� ���� ����
		if (_distance <= patternRange + _nowPattern->_rangeOffset)
		{
			// ���� �غ� ���·� ����
			_status = BossStatus::PATTERN_READY;

			return;
		}

		// ���� ��Ÿ����� �÷��̾���� �Ÿ��� �� ���
		else
		{
			// ���� ��Ÿ����� �÷��̾��� �Ÿ��� ������ ���� �Ÿ� �̳��� ��� �ɾ �̵�
			if (_distance - patternRange <= 50.0f)
			{
				// ������ �÷��̾��� �� �� �ִ� ���� ���� ���
				if (_isMoving == false)
				{
					UpdateMoveNode();
				}

				// ����� ��� ���Ͱ� �����Ѵٸ�
				if (_stopover.size() > 0)
				{
					// �÷��̾ ���Ͽ� �̵�
					auto isChasing = MoveToPlayer(_stopover[_nodeCount].first, _stopover[_nodeCount].second, _info._moveSpeed, patternRange);

					// ù ��常ŭ �Ѿư��ٸ�
					if (!isChasing)
					{
						// ���� ��带 ������� �̵�
						_nodeCount++;
					}
				}

				// ��� ��带 �о� ������ �÷��̾ �� �Ѿư��ٸ�
				if (_nodeCount >= _stopover.size())
				{
					// ���� �غ� ���·� ����
					_status = BossStatus::PATTERN_READY;

					return;
				}
			}

			// ���� ��Ÿ����� �÷����� �Ÿ��� �ְ� ���� �Ÿ��� ��� ��� �ڷ���Ʈ�� �̵�
			else
			{
				TeleportToPlayer();

				// ���� �غ� ���·� ����
				_status = BossStatus::PATTERN_READY;

				return;
			}
		}
	}
}

void KunrealEngine::Boss::Hit()
{
	auto bossCollider = _boss->GetComponent<BoxCollider>();

	// ���� ���� ������ ��
	if (_isHit == false)
	{
		// ������ �ݶ��̴��� �����ִٸ�
		if (bossCollider->IsCollided())
		{
			// �ݶ��̴��� �÷��̾�� �����ߴٸ�
			if (bossCollider->GetTargetObject() == _player)
			{
				// ü���� 0�� �Ǹ�
				if (_info._hp <= 0)
				{
					// ���� ���� ���� �ִϸ��̼��� ����
					_boss->GetComponent<Animator>()->Stop();

					_status = BossStatus::DEAD;
				}

				// ����ȭ �������� 0�� �Ǹ�
				if (_info._staggeredGauge <= 0)
				{
					// ���� ���� ���� �ִϸ��̼��� ����
					_boss->GetComponent<Animator>()->Stop();

					_status = BossStatus::STAGGERED;
				}

				// ������ hit �Ǵ°Ÿ� �����ϱ� ���� Hit�� �Ǿ��ٴ� ���¸� üũ
				_isHit = true;

				// ������ �ݶ��̴��� ���𰡰� �浹�Ѵٸ�
				// �÷��̾��� �ֱ� ������ �������� �޾ƿͼ� ��ƾ���
				// 
				// ������ �÷��̾ �˰� �ִ°� �´°�?
				_info._hp; // -= _player->GetComponent<Player>()->_playerInfo.
			}
		}
	}

	// Hit�� �ִϸ��̼� ����
	//if (_isHit)
	//{
	//	_boss->GetComponent<Animator>()->Play("Hit", ANIMATION_SPEED, false);
	//
	//	// Hit �ִϸ��̼��� ������
	//	if (_boss->GetComponent<Animator>()->GetIsAnimationPlaying() == false)
	//	{
	//		// ���� �ִϸ��̼� ������ ���� �ִϸ��̼� �������� 0���� �ǵ���
	//		_boss->GetComponent<Animator>()->Stop();
	//		// �ٽ� Hit �����ϵ��� ���� ���¸� false�� ����
	//		_isHit = false;
	//	}
	//}
}


void KunrealEngine::Boss::Attack()
{
	// ������ ���� �ݶ��̴��� ���鼭
	if (_patternIndex != -1)
	{
		for (const auto& object : _basicPattern[_patternIndex]->_subObject)
		{
			auto collider = object->GetComponent<BoxCollider>();
			// �ݶ��̴��� �浹�Ͽ��� �� ����� �÷��̾���
			if (collider->IsCollided() && collider->GetTargetObject() == _player)
			{
				// ������ ���������� �Ǵ°Ÿ� ���� ���� �ݶ��̴��� ����
				collider->SetActive(false);

				// ������ �ִ� Ÿ�� Ƚ������ �ϳ��� ���ҽ�Ŵ
				_maxColliderOnCount--;

				// ������ �������� ������
				auto damage = _nowPattern->_damage;

				// �÷��̾��� hp���� ������ ��������ŭ ������Ŵ
				_player->GetComponent<Player>()->GetPlayerData()._hp -= damage;
			}
		}
	}
}

void KunrealEngine::Boss::Staggred()
{
	// ����ȭ �ִϸ��̼� ����
	_boss->GetComponent<Animator>()->Play("Staggred", _info._baseAnimSpeed, false);

	// ����ȭ �ִϸ��̼��� �����ٸ�
	if (_boss->GetComponent<Animator>()->GetIsAnimationPlaying() == false)
	{
		_boss->GetComponent<Animator>()->Stop();
		_status = BossStatus::OFF_STAGGRED;
	}
}

void KunrealEngine::Boss::OffStaggred()
{
	// ����ȭ ���� �ִϸ��̼� ����
	_boss->GetComponent<Animator>()->Play("OffStaggred", _info._baseAnimSpeed, false);

	// ����ȭ ���� �ִϸ��̼��� �����ٸ�
	if (_boss->GetComponent<Animator>()->GetIsAnimationPlaying() == false)
	{
		_boss->GetComponent<Animator>()->Stop();

		_info._staggeredGauge = _info.GetMaxStaggeredGauage();
		_status = BossStatus::IDLE;
	}
}

void KunrealEngine::Boss::Dead()
{
	// ������ ������ �ִϸ��̼� ����
	_boss->GetComponent<Animator>()->Play("Dead", _info._baseAnimSpeed, false);
}


void KunrealEngine::Boss::PatternReady()
{
	// ���� ���� ���� Idle �ִϸ��̼��� ����
	_boss->GetComponent<Animator>()->Stop();


	// �÷��̾ �ٶ󺸴� ���͸� ���
	CalculateDirection();

	// ������ ���� ���� ������Ʈ���� ��� ��
	for (const auto& object : _nowPattern->_subObject)
	{
		object->SetActive(true);
	}

	// �ھ� ������ ���
	if (_isCorePattern)
	{
		// Core Attack���� ���� ����
		_status = BossStatus::CORE_ATTACK;
	}
	else
	{
		// Basic Attack���� ���� ����
		_status = BossStatus::BASIC_ATTACK;
	}
}

void KunrealEngine::Boss::BasicAttack()
{
	// ������ ����
	auto isPatternPlaying = _nowPattern->Play();

	// ���� ������ �����ٸ�
	if (isPatternPlaying == false)
	{
		_boss->GetComponent<Animator>()->Stop();
		_status = BossStatus::PATTERN_END;
	}
}

void KunrealEngine::Boss::CoreAttack()
{
	// ������ ����
	auto isPatternPlaying = _nowPattern->Play();

	// ���� ������ �����ٸ�
	if (isPatternPlaying == false)
	{
		_boss->GetComponent<Animator>()->Stop();
		_status = BossStatus::PATTERN_END;
	}
}

void KunrealEngine::Boss::PatternEnd()
{
	// ���� ������ ���� �ʱ�ȭ
	_isMoving = false;
	_isRotateFinish = false;

	// ���� ���� �ε��� ����
	_exPatternIndex = _patternIndex;

	// Idle �ִϸ��̼� ����
	_boss->GetComponent<Animator>()->Play("Idle", _info._baseAnimSpeed, true);

	// ��� ���� ������Ʈ�� ��
	for (const auto& object : _basicPattern[_patternIndex]->_subObject)
	{
		object->SetActive(false);
	}

	// 1�ʵ��� ����� �� Idle ���·� ��ȯ
	Startcoroutine(patternEnd);
}

void KunrealEngine::Boss::SetInfo(BossBasicInfo& info)
{
	_info = info;
}

void KunrealEngine::Boss::SetBasicPatternList(std::vector<BossPattern*>* basicPatternList)
{
}

void KunrealEngine::Boss::SetCorePatternList(std::vector<BossPattern*>* corePatternList)
{
	SortCorePattern();
}

const BossStatus& KunrealEngine::Boss::GetStatus()
{
	return _status;
}

bool KunrealEngine::Boss::CompareCorePattern(const BossPattern* pattern1, const BossPattern* pattern2)
{
	return pattern1->_triggerHp < pattern2->_triggerHp;
}

void KunrealEngine::Boss::SortCorePattern()
{
	std::sort((_corePattern).begin(), (_corePattern).end(), CompareCorePattern);
}


void KunrealEngine::Boss::SetSubObjectScene()
{
	//auto bossScene = _boss->GetObjectScene();
	//
	//for (const auto& index : _basicPattern)
	//{
	//	for (const auto& object : index->_subObject)
	//	{
	//		object->changescene()?
	//	}
	//}
}

float KunrealEngine::Boss::CalculateAngle(const DirectX::XMFLOAT3& bossPosition, const DirectX::XMFLOAT3& playerPosition)
{
	// ������ �÷��̾� ������ ������ ���
	auto angle = ToolBox::GetAngle(bossPosition, playerPosition);

	auto bossAngle = _boss->GetComponent<Transform>()->GetRotation().y;

	// ���纸���� ������ ����� �÷��̾���� �������� ���̰� 180 �����ϰ��
	// angle��ŭ �״�� �����̸� �� (�̷��� ���ϸ� 90, 270�� �� 270���� ���ƹ��� ��ȿ����)
	if (std::abs(bossAngle - angle) < 180.0f)
	{
		return angle;
	}
	// �������� ���̰� 180���� �������
	else
	{
		// angle�� ������ ���
		if (angle < 0)
		{
			return 360.0f + angle;
		}
		else
		{
			return angle - 360.0f;
		}
	}
}

bool KunrealEngine::Boss::MoveToPlayer(DirectX::XMFLOAT3& targetPos, float speed, float patternRange)
{
	float moveSpeed = speed * TimeManager::GetInstance().GetDeltaTime();

	auto bossTransform = _boss->GetComponent<Transform>();

	DirectX::XMFLOAT3 bossPosition = bossTransform->GetPosition();

	auto distance = ToolBox::GetDistance(bossPosition, targetPos);

	// ���� �Ÿ� ���� �÷��̾ �������� ���� ���
	if (distance > 1.0f)
	{
		// �÷��̾� �̵�
		_boss->GetComponent<Animator>()->Play("Run", speed, true);

		DirectX::XMVECTOR currentPosVec = DirectX::XMLoadFloat3(&bossPosition);

		DirectX::XMVECTOR direction = DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&targetPos), currentPosVec);

		direction = DirectX::XMVector3Normalize(direction);

		DirectX::XMVECTOR newPosition = DirectX::XMVectorAdd(currentPosVec, DirectX::XMVectorScale(direction, moveSpeed));
		bossTransform->SetPosition(newPosition.m128_f32[0], 0, newPosition.m128_f32[2]);

		return true;
	}

	else
	{
		return false;
	}
}

bool KunrealEngine::Boss::MoveToPlayer(DirectX::XMFLOAT3& startPos, DirectX::XMFLOAT3& targetPos, float speed, float patternRange)
{
	float moveSpeed = speed * TimeManager::GetInstance().GetDeltaTime();

	// ���� �Ÿ� ���� �÷��̾ �������� ���� ���
	if (_distance > patternRange + _nowPattern->_rangeOffset)
	{
		// �÷��̾� �̵�
		_prevPos = startPos;
		_boss->GetComponent<Animator>()->Play("Run", speed, true);

		DirectX::XMVECTOR currentPosVec = DirectX::XMLoadFloat3(&startPos);

		DirectX::XMVECTOR direction = DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&targetPos), currentPosVec);

		direction = DirectX::XMVector3Normalize(direction);

		DirectX::XMVECTOR newPosition = DirectX::XMVectorAdd(currentPosVec, DirectX::XMVectorScale(direction, moveSpeed));
		_bossTransform->SetPosition(newPosition.m128_f32[0], 0.0f, newPosition.m128_f32[2]);

		return true;
	}

	else
	{
		return false;
	}
}

void KunrealEngine::Boss::TeleportToPlayer()
{
	auto playerDirection = _player->GetComponent<Player>()->GetDirectionVector();
	auto originPlayerPos = _playerTransform->GetPosition();
	auto currentPosVec = DirectX::XMLoadFloat3(&originPlayerPos);

	auto patternRange = _nowPattern->_range;
	DirectX::XMVECTOR newPlayerPosition = DirectX::XMVectorAdd(currentPosVec, DirectX::XMVectorScale(playerDirection, 10.0f + patternRange));

	// ������ �÷��̾� ������ ��� ���
	Navigation::GetInstance().SetSEpos(1, _bossTransform->GetPosition().x, _bossTransform->GetPosition().y, _bossTransform->GetPosition().z,
		newPlayerPosition.m128_f32[0], 0.0f, newPlayerPosition.m128_f32[2]);

	_stopover = Navigation::GetInstance().FindStraightPath(1);

	if (_stopover.empty())
	{
		return;
	}

	auto bossRotation = _bossTransform->GetRotation();
	auto playerRotation = _playerTransform->GetRotation();

	// ���⿡ �÷��̾��� ���� ���� * ���� �Ÿ��� ���� �÷��̾�� ��¦ ���������� �ؾ� ��
	// �÷��̾��� ���� ���͸� �����;���

	auto target = _stopover.back().second;
	_bossTransform->SetPosition(target.x, 0.0f, target.z);

	// �÷��̾ �ٶ󺸵��� 
	_bossTransform->SetRotation(bossRotation.x, 180 + playerRotation.y, bossRotation.z);
}


void KunrealEngine::Boss::Teleport(const DirectX::XMFLOAT3& targetPos, bool lookAtPlayer)
{
	// Ÿ�� ������������ ��� ���
	DirectX::XMVECTOR targetVec = DirectX::XMLoadFloat3(&targetPos);

	Navigation::GetInstance().SetSEpos(1, _bossTransform->GetPosition().x, _bossTransform->GetPosition().y, _bossTransform->GetPosition().z,
		targetVec.m128_f32[0], 0.0f, targetVec.m128_f32[2]);

	_stopover = Navigation::GetInstance().FindStraightPath(1);

	if (_stopover.empty())
	{
		return;
	}

	auto bossRotation = _bossTransform->GetRotation();
	auto playerRotation = _playerTransform->GetRotation();

	auto target = _stopover.back().second;
	_bossTransform->SetPosition(target.x, 0.0f, target.z);

	// �÷��̾ �ٶ󺸵��� 
	if (lookAtPlayer)
	{
		_bossTransform->SetRotation(bossRotation.x, 180 + playerRotation.y, bossRotation.z);
	}
}

bool KunrealEngine::Boss::LookAtPlayer(float angle, float rotateSpeed)
{
	// ȸ�� �ӵ�
	float speed = TimeManager::GetInstance().GetDeltaTime() * rotateSpeed;

	// ���� ����
	float errorRange = speed * 2.0f;

	// ���� ������ ��ǥ�� �ϴ� �������� ���� ���
	if (_bossTransform->GetRotation().y < angle)
	{
		// ȸ�� �ӵ���ŭ ȸ��
		_bossTransform->SetRotation(_bossTransform->GetRotation().x, _bossTransform->GetRotation().y + speed, _bossTransform->GetRotation().z);
	}
	// ���� ������ ��ǥ�� �ϴ� �������� Ŭ ���
	else
	{
		// ȸ�� �ӵ���ŭ ȸ��
		_bossTransform->SetRotation(_bossTransform->GetRotation().x, _bossTransform->GetRotation().y - speed, _bossTransform->GetRotation().z);
	}

	// ������ ������ ��ǥ�� �ϴ� ������ �������� ���� ���� ���
	if (angle - errorRange <= _bossTransform->GetRotation().y && _bossTransform->GetRotation().y <= angle + errorRange)
	{
		// ���� ���� ���� �ִϸ��̼��� Idle �̶��
		if (_boss->GetComponent<Animator>()->GetNowAnimationName() == "Idle")
		{
			// Idle �ִϸ��̼��� ���߰�
			_boss->GetComponent<Animator>()->Stop();
		}
		// ȸ���� �Ϸ�Ǿ��ٰ� ��ȯ
		return true;
	}
	else
	{
		return false;
	}
}


bool KunrealEngine::Boss::Move(DirectX::XMFLOAT3& targetPos, float speed, bool roateToTarget, bool rayCast)
{
	// Ÿ�� �������� ȸ���� ���Ҷ�
	bool finshRoate = true;
	if (roateToTarget == true)
	{
		finshRoate = RotateToTarget(targetPos);
	}

	// ȸ���� �����ٸ�
	if (finshRoate == true)
	{
		// �� �Լ��� ó�� ���ͼ� �ѹ��� ���������� �ʾҴٸ� ��ã�� (�ѹ��� ��������)
		if (_isMoving == false)
		{
			// �տ� ��ü�� ������ �ű������ �̵���Ű�� �ʹٸ�
			if (rayCast)
			{
				// ���θ��� ��ü�� �������� ã��
				Navigation::GetInstance().SetSEpos(1, _bossTransform->GetPosition().x, _bossTransform->GetPosition().y, _bossTransform->GetPosition().z,
					targetPos.x, targetPos.y, targetPos.z);

				auto targetPos = Navigation::GetInstance().FindRaycastPath(1);

				// �� ��ü�� �������� ������� �׺�޽� ��ã��
				Navigation::GetInstance().SetSEpos(1, _bossTransform->GetPosition().x, _bossTransform->GetPosition().y, _bossTransform->GetPosition().z,
					targetPos.x, targetPos.y, targetPos.z);

				_stopover = Navigation::GetInstance().FindStraightPath(1);
			}
			// �׳� Ÿ�� �����Ǳ��� �׺�޽��� ��ã��
			else
			{
				Navigation::GetInstance().SetSEpos(1, _bossTransform->GetPosition().x, _bossTransform->GetPosition().y, _bossTransform->GetPosition().z,
					targetPos.x, targetPos.y, targetPos.z);

				_stopover = Navigation::GetInstance().FindStraightPath(1);
			}

			// ���� �����̱� ����
			_isMoving = true;
		}

		// ���� �����ִٸ�
		if (_stopover.size() > 0)
		{
			// Ÿ���� ���Ͽ� �̵�
			auto trs = _bossTransform->GetPosition();
			auto isChasing = MoveToTarget(trs, _stopover[_nodeCount].second, _info._moveSpeed);

			// ��常ŭ �Ѿư��ٸ�
			if (!isChasing)
			{
				// ���� ��带 ������� �̵�
				_nodeCount++;
			}

			// ��� ��带 �о� ������ Ÿ�ٿ� �����ߴٸ�
			if (_nodeCount >= _stopover.size())
			{
				// ���� ������ ���� �ʱ�ȭ
				_nodeCount = 0;
				_isRotate = false;
				_isRotateFinish = false;

				return true;
			}
		}
	}

	return false;
}


std::function<bool()> KunrealEngine::Boss::CreateBackStepLogic(BossPattern* pattern, float moveSpeed, float distance)
{
	auto backStepLogic = [pattern, this]()
	{
		auto moveFinish = BackStep(50.0f, 30.0f);

		if (moveFinish == true)
		{
			return false;
		}

		return true;
	};

	return backStepLogic;
}

bool KunrealEngine::Boss::BackStep(float speed, float distance)
{
	DirectX::XMFLOAT3 newPosition;
	newPosition.x = _bossTransform->GetPosition().x - distance * _direction.x;
	newPosition.y = _bossTransform->GetPosition().y - distance * _direction.y;
	newPosition.z = _bossTransform->GetPosition().z - distance * _direction.z;


	return Move(newPosition, speed, false, true);
}

const DirectX::XMVECTOR KunrealEngine::Boss::GetDirection()
{
	auto direction = DirectX::XMLoadFloat3(&_direction);

	return direction;
}


void KunrealEngine::Boss::SetBossCollider()
{
	_boss->AddComponent<BoxCollider>();
}


void KunrealEngine::Boss::SetSubObject(bool tf)
{
	for (const auto& pattern : _basicPattern)
	{
		for (const auto& object : pattern->_subObject)
		{
			object->SetActive(tf);
		}
	}
}

bool KunrealEngine::Boss::RotateToTarget(const DirectX::XMFLOAT3& targetPos)
{
	auto bossPosition = _bossTransform->GetPosition();
	// ������ ��Ÿ��� �޾ƿ�
	if (!_isRotate)
	{
		// ������ �÷��̾� ������ ������ ����
		auto angle = CalculateAngle(bossPosition, targetPos);

		// ������ �÷��̾ �ٶ󺸰� ��
		_isRotate = LookAtPlayer(angle, _info._rotationSpeed);
	}

	if (_isRotate)
	{
		return true;
	}

	return false;
}

void KunrealEngine::Boss::SetTexture()
{

}

void KunrealEngine::Boss::SetBossTransform()
{

}


const std::pair<DirectX::XMFLOAT3, DirectX::XMFLOAT3> KunrealEngine::Boss::GetBossPosition()
{
	std::pair<DirectX::XMFLOAT3, DirectX::XMFLOAT3> pos;

	pos.first = _prevPos;
	pos.second = _bossTransform->GetPosition();

	return pos;
}

void KunrealEngine::Boss::UpdateMoveNode()
{
	Navigation::GetInstance().SetSEpos(1, _bossTransform->GetPosition().x, _bossTransform->GetPosition().y, _bossTransform->GetPosition().z,
		_playerTransform->GetPosition().x, _playerTransform->GetPosition().y, _playerTransform->GetPosition().z);

	_stopover = Navigation::GetInstance().FindStraightPath(1);
	_nodeCount = 0;
}


void KunrealEngine::Boss::CalculateDirection()
{
	auto bossPosition = _bossTransform->GetPosition();

	DirectX::XMVECTOR currentPosVec = DirectX::XMLoadFloat3(&bossPosition);

	auto targetPos = _playerTransform->GetPosition();

	DirectX::XMVECTOR direction = DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&targetPos), currentPosVec);

	direction = DirectX::XMVector3Normalize(direction);

	DirectX::XMStoreFloat3(&_direction, direction);
}

bool KunrealEngine::Boss::MoveToTarget(DirectX::XMFLOAT3& startPos, DirectX::XMFLOAT3& targetPos, float speed)
{
	float moveSpeed = speed * TimeManager::GetInstance().GetDeltaTime();

	auto bossPosition = _bossTransform->GetPosition();

	_distance = ToolBox::GetDistance(bossPosition, targetPos);

	// ���� �Ÿ� ���� Ÿ���� �������� ���� ���
	if (_distance > 1.0f)
	{
		// �÷��̾� �̵�
		_prevPos = startPos;
		_boss->GetComponent<Animator>()->Play("Run", speed, true);

		DirectX::XMVECTOR currentPosVec = DirectX::XMLoadFloat3(&startPos);

		DirectX::XMVECTOR direction = DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&targetPos), currentPosVec);

		direction = DirectX::XMVector3Normalize(direction);

		DirectX::XMVECTOR newPosition = DirectX::XMVectorAdd(currentPosVec, DirectX::XMVectorScale(direction, moveSpeed));
		_bossTransform->SetPosition(newPosition.m128_f32[0], 0.0f, newPosition.m128_f32[2]);

		return true;
	}

	else
	{
		return false;
	}
}

void KunrealEngine::Boss::UpdateMoveNode(DirectX::XMFLOAT3& targetPos)
{
	Navigation::GetInstance().SetSEpos(1, _bossTransform->GetPosition().x, _bossTransform->GetPosition().y, _bossTransform->GetPosition().z,
		targetPos.x, targetPos.y, targetPos.z);

	_stopover = Navigation::GetInstance().FindStraightPath(1);
	_nodeCount = 0;
}
