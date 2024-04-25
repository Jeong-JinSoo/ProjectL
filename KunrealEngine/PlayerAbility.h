#pragma once

#include <DirectXMath.h>
#include "CommonHeader.h"
#include "Component.h"
#include "Player.h"

namespace KunrealEngine
{
	class Ability;

	class _DECLSPEC PlayerAbility : public Component
	{
	public:
		PlayerAbility();
		~PlayerAbility();

		void Initialize() override;
		void Release() override;

		void FixedUpdate() override;
		void Update() override;
		void LateUpdate() override;

		void OnTriggerEnter() override;
		void OnTriggerStay() override;
		void OnTriggerExit() override;

		void SetActive(bool active) override;

	private:
		std::vector<Ability*> _abilityContainer;
		Player* _playerComp;

		GameObject* _shot;			// Q ��ų ����ü ��ü
		GameObject* _meteor;		// R ��ų � ��ü

	private:
		void CreateAbility1();

		void ResetMeteorPos();
		void CreateAbility4();

	public:
		void AddToContanier(Ability* abil);
	};
}

