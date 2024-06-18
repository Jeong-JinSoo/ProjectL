#include "GameObject.h"
#include "Player.h"
#include "PlayerAbility.h"
#include "PlayerMove.h"
#include "Kamen.h"
#include "EventManager.h"

KunrealEngine::EventManager::EventManager()
	:_player(nullptr), _boss(nullptr), _playerComp(nullptr), _bossComp(nullptr), _playerAbill(nullptr),
	_eventStart(false)
{

}

KunrealEngine::EventManager::~EventManager()
{

}

void KunrealEngine::EventManager::Initialize()
{
	SetBossObject();

	_eventStart = true;
}

void KunrealEngine::EventManager::Release()
{
	_eventStart = false;

	_playerAbill = nullptr;
	_bossComp = nullptr;
	_playerComp = nullptr;

	_boss = nullptr;
	_player = nullptr;
}

void KunrealEngine::EventManager::Update()
{
	if (_eventStart)
	{
		CalculateDamageToBoss();

		//CalculateDamageToPlayer();

		CalculateDamageToPlayer();

		CalculateDamageToPlayer2();

		if (_bossComp->isDead())
		{
			Release();
		}
	}
}

void KunrealEngine::EventManager::CalculateDamageToBoss()
{
	auto& bossInfo = _bossComp->GetBossInfo();

	auto damage = _playerAbill->GetDamage();

	if (damage > 0)
	{
		auto finalDamage = damage * (100.0f / 100 + bossInfo._armor);

		_bossComp->GetBossInfo()._hp -= finalDamage;
	}
}

void KunrealEngine::EventManager::exCalculateDamageToPlayer()
{
	//auto nowPattern = _bossComp->GetNowPattern();
	//
	//// ������ ���� �ݶ��̴��� ���鼭
	//if (_bossComp->GetNowPattern() != nullptr)
	//{
	//	for (const auto& pattern : nowPattern->_patternList)
	//	{
	//		for (const auto& object : pattern->_subObject)
	//		{
	//			auto collider = object->GetComponent<BoxCollider>();
	//
	//			if (collider != nullptr)
	//			{
	//				// �ݶ��̴��� �浹�Ͽ��� �� ����� �÷��̾���
	//				if (collider->GetActivated())
	//				{
	//					if (nowPattern->_colliderOnCount > 0)
	//					{
	//						if (collider->IsCollided() && collider->GetTargetObject() == _player)
	//						{
	//							// ������ ���������� �Ǵ°Ÿ� ���� ���� �ݶ��̴��� ����
	//							collider->SetActive(false);
	//
	//							// ������ �ִ� Ÿ�� Ƚ������ �ϳ��� ���ҽ�Ŵ
	//							nowPattern->_colliderOnCount--;
	//
	//							// ������ �������� ������
	//							auto damage = nowPattern->_damage;
	//
	//							// �÷��̾��� hp���� ������ ��������ŭ ������Ŵ
	//							_playerComp->GetPlayerData()._hp -= damage;
	//							_playerComp->SetHitState(static_cast<int> (nowPattern->_attackState));
	//
	//							// �������� �� �� �޽��� �����Ѵٸ�
	//							if (object->GetComponent<MeshRenderer>() != nullptr && !nowPattern->_isRemainMesh)
	//							{
	//								// �޽��� ������
	//								object->GetComponent<MeshRenderer>()->SetActive(false);
	//							}
	//						}
	//					}
	//				}
	//			}
	//		}
	//	}
	//}
}


void KunrealEngine::EventManager::CalculateDamageToPlayer()
{
	auto nowPattern = _bossComp->GetNowPlayingPattern();

	// ������ �������϶���?
	if (nowPattern != nullptr)
	{
		// �ݶ��̴��� ���� ���
		if (nowPattern->_colliderType == BossPattern::eColliderType::eNone)
		{
			return;
		}

		// �ڽ� �ݶ��̴��� �Ǹ��� �ݶ��̴��� ���
		else if (nowPattern->_colliderType == BossPattern::eColliderType::eBox || 
			nowPattern->_colliderType == BossPattern::eColliderType::eCylinder)
		{
			auto subObjectList = nowPattern->_subObject;

			for (int i = 0; i < subObjectList.size(); i++)
			{
				auto collider = subObjectList[i]->GetComponent<BoxCollider>();
					
				// �ݶ��̴��� �������� ���� ���� ����
				if (nowPattern->_isColliderActive[i] && !nowPattern->_isColliderHit[i])
				{
					collider->SetActive(true);

					if (collider->IsCollided() && collider->GetTargetObject() == _player)
					{
						auto dirVec = DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f);
						_playerComp->CalculateSweep(DirectX::XMLoadFloat3(&dirVec));

						auto damage = nowPattern->_damage;

						// �÷��̾��� hp���� ������ ��������ŭ ������Ŵ
						_playerComp->GetPlayerData()._hp -= damage;
						_playerComp->SetHitState(static_cast<int> (nowPattern->_attackState));

						// �������� �� �� �޽��� �����Ѵٸ�
						if (!nowPattern->_isRemainMesh && subObjectList[i]->GetComponent<MeshRenderer>() != nullptr)
						{
							// �޽��� ������
							subObjectList[i]->GetComponent<MeshRenderer>()->SetActive(false);
						}

						// �¾Ҵٰ� üũ
						nowPattern->_isColliderHit[i] = true;
					}
				}

				// �ݶ��̴��� ������ ���� ����
				if (nowPattern->_isColliderActive[i] && nowPattern->_isColliderHit[i])
				{
					// �¾����� �ݶ��̴� ����
					nowPattern->_isColliderActive[i] = false;

					collider->SetActive(false);
				}
			}
		}
		else
		{
		}
	}
}


void KunrealEngine::EventManager::CalculateDamageToPlayer2()
{
	auto nowPattern = _bossComp->GetNowSpecialPattern();

	if (nowPattern != nullptr)
	{
		// �ݶ��̴��� ���� ���
		if (nowPattern->_colliderType == BossPattern::eColliderType::eNone)
		{
			return;
		}

		// �ڽ� �ݶ��̴��� �Ǹ��� �ݶ��̴��� ���
		else if (nowPattern->_colliderType == BossPattern::eColliderType::eBox ||
			nowPattern->_colliderType == BossPattern::eColliderType::eCylinder)
		{
			auto subObjectList = nowPattern->_subObject;

			for (int i = 0; i < subObjectList.size(); i++)
			{
				auto collider = subObjectList[i]->GetComponent<BoxCollider>();

				// �ݶ��̴��� �������� ���� ���� ����
				if (nowPattern->_isColliderActive[i] && !nowPattern->_isColliderHit[i])
				{
					collider->SetActive(true);

					if (collider->IsCollided() && collider->GetTargetObject() == _player)
					{
						auto dirVec = DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f);
						_playerComp->CalculateSweep(DirectX::XMLoadFloat3(&dirVec));

						auto damage = nowPattern->_damage;

						// �÷��̾��� hp���� ������ ��������ŭ ������Ŵ
						_playerComp->GetPlayerData()._hp -= damage;
						_playerComp->SetHitState(static_cast<int> (nowPattern->_attackState));

						// �������� �� �� �޽��� �����Ѵٸ�
						if (!nowPattern->_isRemainMesh && subObjectList[i]->GetComponent<MeshRenderer>() != nullptr)
						{
							// �޽��� ������
							subObjectList[i]->GetComponent<MeshRenderer>()->SetActive(false);
						}

						// �¾Ҵٰ� üũ
						nowPattern->_isColliderHit[i] = true;
					}
				}

				// �ݶ��̴��� ������ ���� ����
				if (nowPattern->_isColliderActive[i] && nowPattern->_isColliderHit[i])
				{
					// �¾����� �ݶ��̴� ����
					nowPattern->_isColliderActive[i] = false;

					collider->SetActive(false);
				}
			}
		}
		else if (nowPattern->_colliderType == BossPattern::eColliderType::eCircleSafe)
		{
			auto subObjectList = nowPattern->_subObject;

			for (int i = 0; i < subObjectList.size(); i++)
			{
				auto collider = subObjectList[i]->GetComponent<BoxCollider>();

				// �ݶ��̴��� �������� ���� ���� ����
				if (nowPattern->_isColliderActive[i] && !nowPattern->_isColliderHit[i])
				{
					collider->SetActive(true);

					if (collider->IsCollided() && collider->GetTargetObject() == _player)
					{
						
					}
					else
					{
						auto dirVec = DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f);
						_playerComp->CalculateSweep(DirectX::XMLoadFloat3(&dirVec));

						auto damage = nowPattern->_damage;

						// �÷��̾��� hp���� ������ ��������ŭ ������Ŵ
						_playerComp->GetPlayerData()._hp -= damage;
						_playerComp->SetHitState(static_cast<int> (nowPattern->_attackState));

						nowPattern->_isColliderHit[i] = true;
					}
				}

				// �ݶ��̴��� ������ ���� ����
				if (nowPattern->_isColliderActive[i] && nowPattern->_isColliderHit[i])
				{
					// �¾����� �ݶ��̴� ����
					nowPattern->_isColliderActive[i] = false;

					collider->SetActive(false);
				}
			}
		}
	}
}

void KunrealEngine::EventManager::SetBossObject()
{
	_player = SceneManager::GetInstance().GetCurrentScene()->GetObjectWithTag("Player");
	_boss = SceneManager::GetInstance().GetCurrentScene()->GetObjectWithTag("Boss");

	_playerComp = _player->GetComponent<Player>();
	_playerAbill = _player->GetComponent<PlayerAbility>();
	_bossComp = _boss->GetComponent<Kamen>();
}