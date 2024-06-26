#include "GameObject.h"
#include "Player.h"
#include "PlayerAbility.h"
#include "PlayerMove.h"
#include "Kamen.h"
#include "ToolBox.h"
#include "EventManager.h"
#include "Camera.h"
#include "ToolBox.h"

KunrealEngine::EventManager::EventManager()
	:_player(nullptr), _boss(nullptr), _playerComp(nullptr), _bossComp(nullptr), _playerAbill(nullptr),
	_eventStart(false), _mainCamera(nullptr), _insideSafeCount(0), _iscamfollow(true)
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
	 
	if (_iscamfollow && _mainCamera != nullptr && _player != nullptr)
	{
		DirectX::XMFLOAT3 plpos = _player->GetComponent<Transform>()->GetPosition();
		static DirectX::XMFLOAT3 capos = _mainCamera->GetComponent<Transform>()->GetPosition();

		plpos.x += capos.x + _camshakex;
		plpos.y += capos.y;
		plpos.z += capos.z + _camshakez;

		_mainCamera->GetComponent<Transform>()->SetPosition(plpos);
	}
}

void KunrealEngine::EventManager::SetCamera(std::string name)
{
	_mainCamera = SceneManager::GetInstance().GetCurrentScene()->GetGameObject(name);
}

std::vector<DirectX::XMFLOAT2> KunrealEngine::EventManager::CamShake(float radius, int numPoints)
{
	// ���� ī�޶� ȸ��
	// ���� �׵θ����� ������ numpoint ��ŭ �߷��� ��, �� ������ ��ȯ.
	// ������ ��ġ�� ���������� �����Ѵٸ� ī�޶�� ���� �׸��� ��鸰��.(��ǥ)
#define M_PI       3.14159265358979323846
	std::vector<DirectX::XMFLOAT2> result;

	for (int i = 0; i < numPoints; ++i) {
		float random = ToolBox::GetRandomFloat(0.0f, radius * M_PI);
		float theta = random;
		float x = radius * std::cos(theta);
		float y = radius * std::sin(theta);
		result.push_back(DirectX::XMFLOAT2(x, y));
	}

	return result;
}

std::vector<float> KunrealEngine::EventManager::CamShakeLinear(float sigma, int numPoints)
{
	// �յ�� ī�޶� ����
	// sigma�� ���� ��ŭ ī�޶� ����. �� sigma ���� ���� �۾�����.
	// �̶��� �¿�� ���� �ϴ� Ƚ���� numpoint ���̴�.
	std::vector<float> result;
	float cur_sigma = sigma;

	for (float a = 0; a < numPoints ; a++)
	{
		// ���� ���� ������ ����Ʈ�� �߰� (���)
		result.push_back(static_cast<int>(cur_sigma));
		// ���� ���� ������ ����Ʈ�� �߰� (����)
		result.push_back(static_cast<int>(-cur_sigma));
		// ���� ���� ����
		cur_sigma -= sigma /numPoints;
	}
	result.push_back(static_cast<int>(0));

	return result;
}

void KunrealEngine::EventManager::CamShake()
{
	// ������ ī�޶� ����. 
	// �׳� ������ ���� �� �������� �����ڴ´�.
	_camshakex = ToolBox::GetRandomFloat(0.0f, 2.0f);
	_camshakez = ToolBox::GetRandomFloat(0.0f, 2.0f);
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
		else if (nowPattern->_colliderType == BossPattern::eColliderType::eBox)
		{
			auto subObjectList = nowPattern->_subObject;

			for (int i = 0; i < subObjectList.size(); i++)
			{
				auto collider = subObjectList[i]->GetComponent<BoxCollider>();
					
				if (collider != nullptr && !nowPattern->_isColliderActive[i])
				{
					collider->SetActive(false);
				}

				// �ݶ��̴��� �������� ���� ���� ����
				if (nowPattern->_isColliderActive[i] && !nowPattern->_isColliderHit[i])
				{
					collider->SetActive(true);

					if (collider->IsCollided() && collider->GetTargetObject() == _player)
					{
						// ������ ���� ������Ʈ ������ �𷺼����� �Ѿ�߸�
						auto colliderDirVec = SetBossAttackDirection(subObjectList[i]);

						_playerComp->CalculateSweep(colliderDirVec);

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

		else if (nowPattern->_colliderType == BossPattern::eColliderType::eCylinder)
		{
			auto subObjectList = nowPattern->_subObject;

			for (int i = 0; i < subObjectList.size(); i++)
			{
				auto collider = subObjectList[i]->GetComponent<CylinderCollider>();

				if (collider != nullptr && !nowPattern->_isColliderActive[i])
				{
					collider->SetActive(false);
				}

				// �ݶ��̴��� �������� ���� ���� ����
				if (nowPattern->_isColliderActive[i] && !nowPattern->_isColliderHit[i])
				{
					collider->SetActive(true);

					if (collider->IsCollided() && collider->GetTargetObject() == _player)
					{
						auto colliderDirVec = SetWarningAttackDirection(subObjectList[i]);
						_playerComp->CalculateSweep(colliderDirVec);

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

		// �ڽ� �ݶ��̴��� ���
		else if (nowPattern->_colliderType == BossPattern::eColliderType::eBox)
		{
			auto subObjectList = nowPattern->_subObject;

			for (int i = 0; i < subObjectList.size(); i++)
			{
				auto collider = subObjectList[i]->GetComponent<BoxCollider>();

				if (collider != nullptr && !nowPattern->_isColliderActive[i])
				{
					collider->SetActive(false);
				}

				// �ݶ��̴��� �������� ���� ���� ����
				if (nowPattern->_isColliderActive[i] && !nowPattern->_isColliderHit[i])
				{
					collider->SetActive(true);

					if (collider->IsCollided() && collider->GetTargetObject() == _player)
					{
						// �ҵ��� ���� ���ͷ� �Ѿ�߸�
						_playerComp->CalculateSweep(_bossComp->GetSwordDirection());

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

		// ���� ���� ����
		else if (nowPattern->_colliderType == BossPattern::eColliderType::eCylinder)
		{
			auto subObjectList = nowPattern->_subObject;

			for (int i = 0; i < subObjectList.size(); i++)
			{
				auto collider = subObjectList[i]->GetComponent<CylinderCollider>();

				if (collider != nullptr && !nowPattern->_isColliderActive[i])
				{
					collider->SetActive(false);
				}

				// �ݶ��̴��� �������� ���� ���� ����
				if (nowPattern->_isColliderActive[i] && !nowPattern->_isColliderHit[i])
				{
					collider->SetActive(true);

					if (collider->IsCollided() && collider->GetTargetObject() == _player)
					{
						auto colliderDirVec = SetWarningAttackDirection(subObjectList[i]);
						_playerComp->CalculateSweep(colliderDirVec);

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
				auto collider = subObjectList[i]->GetComponent<CylinderCollider>();

				if (collider != nullptr && !nowPattern->_isColliderActive[i])
				{
					collider->SetActive(false);

					_insideSafeCount = 0;
				}

				// �ݶ��̴��� �������� ���� ���� ����
				if (nowPattern->_isColliderActive[i] && !nowPattern->_isColliderHit[i])
				{
					collider->SetActive(true);

					collider->FixedUpdate();

					if (collider->IsCollided() && collider->GetTargetObject() == _player)
					{
						
					}
					else
					{
						_insideSafeCount++;

						if (_insideSafeCount > 1)
						{
							// �ҵ�� �������Ʈ�� �ݶ��̴�
							auto colliderDirVec = SetWarningAttackDirection(subObjectList[i]);

							_playerComp->CalculateSweep(colliderDirVec);

							auto damage = nowPattern->_damage;

							// �÷��̾��� hp���� ������ ��������ŭ ������Ŵ
							_playerComp->GetPlayerData()._hp -= damage;
							_playerComp->SetHitState(static_cast<int> (nowPattern->_attackState));

							nowPattern->_isColliderHit[i] = true;
						}
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

		else if (nowPattern->_colliderType == BossPattern::eColliderType::eDonut)
		{
			auto subObjectList = nowPattern->_subObject;
		
			for (int i = 0; i < subObjectList.size(); i++)
			{
				auto collider = subObjectList[i]->GetComponent<CylinderCollider>();

				if (collider!=nullptr && !nowPattern->_isColliderActive[i])
				{
					collider->SetActive(false);
				}

				// �ݶ��̴��� �������� ���� ���� ����
				if (nowPattern->_isColliderActive[i] && !nowPattern->_isColliderHit[i])
				{
					collider->SetActive(true);

					if (collider->IsCollided() && collider->GetTargetObject() == _player)
					{

						auto a = _bossComp->GetSwordPos();


						auto colliderDirVec = SetWarningAttackDirection(subObjectList[i]);
						_playerComp->CalculateSweep(colliderDirVec);

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
						//nowPattern->_isColliderHit[i] = true;
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


const DirectX::XMVECTOR& KunrealEngine::EventManager::SetWarningAttackDirection(GameObject* subObject)
{
	auto colliderPos = subObject->GetComponent<Transform>()->GetPosition();
	auto playerPos = _player->GetComponent<Transform>()->GetPosition();
	colliderPos.y = playerPos.y;

	auto colliderDirVec = ToolBox::GetDirectionVec(colliderPos, playerPos);

	return colliderDirVec;
}

const DirectX::XMVECTOR& KunrealEngine::EventManager::SetBossAttackDirection(GameObject* subObject)
{
	auto colliderPos = subObject->GetComponent<Transform>()->GetPosition();
	auto bossPos = _boss->GetComponent<Transform>()->GetPosition();
	auto playerPos = _player->GetComponent<Transform>()->GetPosition();

	colliderPos.y = playerPos.y;
	bossPos.y = playerPos.y;

	auto colliderDirVec = ToolBox::GetDirectionVec(bossPos, colliderPos);

	return colliderDirVec;
}
