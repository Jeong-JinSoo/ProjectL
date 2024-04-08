#pragma once
#include "CommonHeader.h"
#include "Ability.h"

/// <summary>
/// �÷��̾ ����� ��ų ������ �������ִ� Ŭ����
/// ��𼭵� �Ҹ��°� �ƴ϶� PlayerAbility �ʿ����� �Ҹ��� �ϰ� ������..
/// </summary>

namespace KunrealEngine
{
	class PlayerAbility;	

	class AbilityGenerator
	{
	private:
		AbilityGenerator();
		~AbilityGenerator();

		PlayerAbility* _paComponent;

		// �̱���
	public:
		static AbilityGenerator& GetInstance()
		{
			static AbilityGenerator _instance;
			return _instance;
		}

		// playerability �ʿ��� �÷��̾� ��ü�� ���� ������ �Ѱ���
		// pa�� ���� �����̳ʿ� ������� ��ų ������ �־���
		void SetPlayerAbility(PlayerAbility* pa);

		Ability* Shot();
		void ShotLogic();
	};
}