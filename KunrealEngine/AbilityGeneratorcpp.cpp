#include "AbilityGenerator.h"
#include "Transform.h"
#include "SceneManager.h"
#include "Scene.h"
#include "GameObject.h"
#include "PlayerAbility.h"
#include "Projectile.h"

KunrealEngine::AbilityGenerator::AbilityGenerator()
	:_paComponent(nullptr)
{

}

KunrealEngine::AbilityGenerator::~AbilityGenerator()
{

}

void KunrealEngine::AbilityGenerator::SetPlayerAbility(PlayerAbility* pa)
{
	this->_paComponent = pa;
}

KunrealEngine::Ability* KunrealEngine::AbilityGenerator::Shot()
{
	/// ����
	Ability* shot = new Ability();
	shot->SetTotalData(
		"Shot",				// ��ų �̸�
		100.0f,				// ���ط�
		10.0f,				// �Ҹ� ���
		20.0f,				// ����ȭ ���ط�
		5.0f,				// ���� ���ð�
		10.f				// ��Ÿ�
	);

	

	_paComponent->AddToContanier(shot);
	return shot;
}

void KunrealEngine::AbilityGenerator::ShotLogic()
{
	GameObject* sp = SceneManager::GetInstance().GetCurrentScene()->CreateObject("ShotProjectile");
	GameObject* pla = SceneManager::GetInstance().GetCurrentScene()->GetGameObject("Player");
	Transform* playerTrans = pla->GetComponent<Transform>();

	sp->GetComponent<Transform>()->SetPosition(playerTrans->GetPosition().x, playerTrans->GetPosition().y, playerTrans->GetPosition().z);
	sp->AddComponent<Projectile>();

	sp->GetComponent<Projectile>()->CreateInfo(pla);
	sp->GetComponent<Projectile>()->SetMeshObject("cube/cube", "bricks.dds");


	
}
