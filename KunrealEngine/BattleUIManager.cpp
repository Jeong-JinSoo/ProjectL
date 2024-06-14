#include "BattleUIManager.h"
#include "EventManager.h"
#include "TimeManager.h"
#include "PlayerAbility.h"

KunrealEngine::BattleUIManager::BattleUIManager()
	:_bosshpsize(5.0f), _playerhpsize(5.0f), _skillcoolsize(1.21f)
{
	_eventmanager = &KunrealEngine::EventManager::GetInstance();
}

KunrealEngine::BattleUIManager::~BattleUIManager()
{
}

void KunrealEngine::BattleUIManager::Initialize()
{
	KunrealEngine::SceneManager& scene = KunrealEngine::SceneManager::GetInstance();

	KunrealEngine::GameObject* background;			// ���� UI ���
	KunrealEngine::GameObject* _bosshp_barback;		// ���� ü�¹� ���
	KunrealEngine::GameObject* _playerhp_barback;	// �÷��̾� ü�¹� ���
	KunrealEngine::GameObject* _ui_skill1;		// 1�� ��ų
	KunrealEngine::GameObject* _ui_skill2;		// 2�� ��ų
	KunrealEngine::GameObject* _ui_skill3;		// 3�� ��ų
	KunrealEngine::GameObject* _ui_skill4;		// 4�� ��ų

	_battleuibox = this->GetOwner();

	background = scene.GetCurrentScene()->CreateObject("background");
	background->SetParent(_battleuibox);
	background->AddComponent<ImageRenderer>();
	background->GetComponent<ImageRenderer>()->SetImage("backposition.png");
	background->GetComponent<ImageRenderer>()->SetPosition(460.0f, 815.0f);
	background->GetComponent<Transform>()->SetScale(5.4f, 2.5f, 1.0f);

	_ui_skill1 = scene.GetCurrentScene()->CreateObject("ui_skill1");
	_ui_skill1->SetParent(_battleuibox);
	_ui_skill1->AddComponent<ImageRenderer>();
	_ui_skill1->GetComponent<ImageRenderer>()->SetImage("backposition.png");
	_ui_skill1->GetComponent<ImageRenderer>()->SetPosition(570.f, 910.f);
	_ui_skill1->GetComponent<Transform>()->SetScale(0.7f, 1.21f, 1.0f);

	_ui_skill1_cool = scene.GetCurrentScene()->CreateObject("_ui_skill1_cool");
	_ui_skill1_cool->SetParent(_battleuibox);
	_ui_skill1_cool->AddComponent<ImageRenderer>();
	_ui_skill1_cool->GetComponent<ImageRenderer>()->SetImage("black_sheet.png");
	_ui_skill1_cool->GetComponent<ImageRenderer>()->SetPosition(570.f, 910.f);
	_ui_skill1_cool->GetComponent<Transform>()->SetScale(0.7f, 1.21f, 1.0f);

	_ui_skill2 = scene.GetCurrentScene()->CreateObject("ui_skill2");
	_ui_skill2->SetParent(_battleuibox);
	_ui_skill2->AddComponent<ImageRenderer>();
	_ui_skill2->GetComponent<ImageRenderer>()->SetImage("backposition.png");
	_ui_skill2->GetComponent<ImageRenderer>()->SetPosition(724.f, 910.f);
	_ui_skill2->GetComponent<Transform>()->SetScale(0.7f, 1.21f, 1.0f);

	_ui_skill2_cool = scene.GetCurrentScene()->CreateObject("_ui_skill2_cool");
	_ui_skill2_cool->SetParent(_battleuibox);
	_ui_skill2_cool->AddComponent<ImageRenderer>();
	_ui_skill2_cool->GetComponent<ImageRenderer>()->SetImage("black_sheet.png");
	_ui_skill2_cool->GetComponent<ImageRenderer>()->SetPosition(724.f, 910.f);
	_ui_skill2_cool->GetComponent<Transform>()->SetScale(0.7f, 1.21f, 1.0f);

	_ui_skill3 = scene.GetCurrentScene()->CreateObject("ui_skill3");
	_ui_skill3->SetParent(_battleuibox);
	_ui_skill3->AddComponent<ImageRenderer>();
	_ui_skill3->GetComponent<ImageRenderer>()->SetImage("backposition.png");
	_ui_skill3->GetComponent<ImageRenderer>()->SetPosition(880.f, 910.f);
	_ui_skill3->GetComponent<Transform>()->SetScale(0.7f, 1.21f, 1.0f);

	_ui_skill3_cool = scene.GetCurrentScene()->CreateObject("_ui_skill3_cool");
	_ui_skill3_cool->SetParent(_battleuibox);
	_ui_skill3_cool->AddComponent<ImageRenderer>();
	_ui_skill3_cool->GetComponent<ImageRenderer>()->SetImage("black_sheet.png");
	_ui_skill3_cool->GetComponent<ImageRenderer>()->SetPosition(880.f, 910.f);
	_ui_skill3_cool->GetComponent<Transform>()->SetScale(0.7f, 1.21f, 1.0f);

	_ui_skill4 = scene.GetCurrentScene()->CreateObject("ui_skill4");
	_ui_skill4->SetParent(_battleuibox);
	_ui_skill4->AddComponent<ImageRenderer>();
	_ui_skill4->GetComponent<ImageRenderer>()->SetImage("backposition.png");
	_ui_skill4->GetComponent<ImageRenderer>()->SetPosition(1035.f, 910.f);
	_ui_skill4->GetComponent<Transform>()->SetScale(0.7f, 1.21f, 1.0f);

	_ui_skill4_cool = scene.GetCurrentScene()->CreateObject("_ui_skill4_cool");
	_ui_skill4_cool->SetParent(_battleuibox);
	_ui_skill4_cool->AddComponent<ImageRenderer>();
	_ui_skill4_cool->GetComponent<ImageRenderer>()->SetImage("black_sheet.png");
	_ui_skill4_cool->GetComponent<ImageRenderer>()->SetPosition(1035.f, 910.f);
	_ui_skill4_cool->GetComponent<Transform>()->SetScale(0.7f, 1.21f, 1.0f);

	_potion = scene.GetCurrentScene()->CreateObject("ui_skill5");
	_potion->SetParent(_battleuibox);
	_potion->AddComponent<ImageRenderer>();
	_potion->GetComponent<ImageRenderer>()->SetImage("backposition.png");
	_potion->GetComponent<ImageRenderer>()->SetPosition(1310.f, 940.f);
	_potion->GetComponent<Transform>()->SetScale(0.5f, 0.9f, 1.0f);

	_dash = scene.GetCurrentScene()->CreateObject("ui_skill6");
	_dash->SetParent(_battleuibox);
	_dash->AddComponent<ImageRenderer>();
	_dash->GetComponent<ImageRenderer>()->SetImage("backposition.png");
	_dash->GetComponent<ImageRenderer>()->SetPosition(1200.f, 940.f);
	_dash->GetComponent<Transform>()->SetScale(0.5f, 0.9f, 1.0f);


	_playerhp_bar_downGauge = scene.GetCurrentScene()->CreateObject("playerhp_bar");
	_playerhp_bar_downGauge->SetParent(_battleuibox);
	_playerhp_bar_downGauge->AddComponent<ImageRenderer>();
	_playerhp_bar_downGauge->GetComponent<ImageRenderer>()->SetImage("volume-in.png");
	_playerhp_bar_downGauge->GetComponent<ImageRenderer>()->SetPosition(500.f, 850.f);
	_playerhp_bar_downGauge->GetComponent<Transform>()->SetScale(_playerhpsize, 0.3f, 1.0f);

	_playerhp_bar = scene.GetCurrentScene()->CreateObject("playerhp_bar");
	_playerhp_bar->SetParent(_battleuibox);
	_playerhp_bar->AddComponent<ImageRenderer>();
	_playerhp_bar->GetComponent<ImageRenderer>()->SetImage("backposition.png");
	_playerhp_bar->GetComponent<ImageRenderer>()->SetPosition(500.f, 850.f);
	_playerhp_bar->GetComponent<Transform>()->SetScale(_playerhpsize, 0.3f, 1.0f);

	_bosshp_bar_downGauge = scene.GetCurrentScene()->CreateObject("_bosshp_bar_downGauge");
	_bosshp_bar_downGauge->SetParent(_battleuibox);
	_bosshp_bar_downGauge->AddComponent<ImageRenderer>();
	_bosshp_bar_downGauge->GetComponent<ImageRenderer>()->SetImage("volume-in.png");
	_bosshp_bar_downGauge->GetComponent<ImageRenderer>()->SetPosition(500.f, 15.f);
	_bosshp_bar_downGauge->GetComponent<Transform>()->SetScale(_bosshpsize, 0.3f, 1.0f);

	_bosshp_bar = scene.GetCurrentScene()->CreateObject("bosshp_bar");
	_bosshp_bar->SetParent(_battleuibox);
	_bosshp_bar->AddComponent<ImageRenderer>();
	_bosshp_bar->GetComponent<ImageRenderer>()->SetImage("backposition.png");
	_bosshp_bar->GetComponent<ImageRenderer>()->SetPosition(500.f, 15.f);
	_bosshp_bar->GetComponent<Transform>()->SetScale(_bosshpsize, 0.3f, 1.0f);

	_battleuibox->SetActive(true);
	_ui_skill1_cool->SetActive(false);
	_ui_skill2_cool->SetActive(false);
	_ui_skill3_cool->SetActive(false);
	_ui_skill4_cool->SetActive(false);
}

void KunrealEngine::BattleUIManager::Release()
{
}

void KunrealEngine::BattleUIManager::FixedUpdate()
{
}

void KunrealEngine::BattleUIManager::Update()
{
	auto  abill = _eventmanager->_playerAbill;

	if (InputSystem::GetInstance()->KeyDown(KEY::UP))
	{
		int currenthp = _eventmanager->_bossComp->GetBossInfo()._hp += 10;
		SetBossHpbar();
	}

	if (InputSystem::GetInstance()->KeyDown(KEY::DOWN))
	{
		int currenthp = _eventmanager->_playerComp->GetPlayerData()._hp -= 10;
		if (_eventmanager->_bossComp->GetBossInfo()._hp <= 0)
		{
			currenthp = _eventmanager->_bossComp->GetBossInfo()._hp = 1;
		}
		SetBossHpbar();
	}

	if (InputSystem::GetInstance()->KeyDown(KEY::PGUP))
	{
		_eventmanager->_playerComp->GetPlayerData()._hp += 10;
		SetPlayerHpBar();
	}

	if (InputSystem::GetInstance()->KeyDown(KEY::PGDOWN))
	{
		_eventmanager->_playerComp->GetPlayerData()._hp -= 10;
		SetPlayerHpBar();
	}

	if (abill->_isShotDetected)
	{
		SetSkillcool1();
	}

	if (abill->_isIceDetected)
	{
		SetSkillcool2();
	}

	if (abill->_isAreaDetected)
	{
		SetSkillcool3();
	}

	if (abill->_isMeteorDetected)
	{
		SetSkillcool4();
	}
}

void KunrealEngine::BattleUIManager::LateUpdate()
{
}

void KunrealEngine::BattleUIManager::OnTriggerEnter()
{
}

void KunrealEngine::BattleUIManager::OnTriggerStay()
{
}

void KunrealEngine::BattleUIManager::OnTriggerExit()
{
}

void KunrealEngine::BattleUIManager::SetActive(bool active)
{

}

void KunrealEngine::BattleUIManager::SetBossHpbar()
{
	if (_eventmanager->_bossComp != nullptr)
	{
		float currenthp = _eventmanager->_bossComp->GetBossInfo()._hp;
		float maxhp = _eventmanager->_bossComp->GetBossInfo().GetMaxHP();
		float minhp = 0;
		float minhpbar = 0;

		booshp_targetscale = ((currenthp - minhp) / (maxhp - minhp)) * (_bosshpsize - minhpbar) + minhpbar;
		_bosshp_bar->GetComponent<Transform>()->SetScale(booshp_targetscale, 0.3f, 1.0f);
		_CoroutineIs(bossdowngauge)
		{
			//���ҽ� ü�� ���� ������ ��� �ؾ� �ұ�
			// �ϴ� ü���� ���δ�, �پ�� ü���� �տ� �ٿ�������� �д�, �ٿ� �������� �������� ���� ü�� ��ġ ��ŭ ü���
			// �׸��� ���� �������� ���δ�. 0�� �ɶ�����. 
			// �پ�� ü�¹��� �������� ��� �������̸�, ���� ü�� �������� ������(������ ����)�� ���� �Ұ� �ΰ�?
			// �ٸ� ����� ������
			// �ϴ� �Ѵ� ���. ���� ü���� �ٷ� ���, ��� ü���� õõ�� ���.
			// �̷��� ��ġ ���ϰ� ������ ���ϰ� �� �ʿ䰡 ���� �� ������ 
			// �̰ɷ� �� �غ��� 

			auto  hpbarcontrol = this;
			auto  downGauge = _bosshp_bar_downGauge;	//GameObject
			float scale = downGauge->GetComponent<Transform>()->GetScale().x;
			float& targetscale = booshp_targetscale;
			float speed = 1.0f;


			while (targetscale - TimeManager::GetInstance().GetDeltaTime() * speed < scale)
			{
				downGauge->GetComponent<Transform>()->SetScale(scale, 0.3f, 1.0f);
				scale -= TimeManager::GetInstance().GetDeltaTime() * speed;
				Return_null;
			}
			targetscale = 0;
		};

		Startcoroutine(bossdowngauge);
	}
}

void KunrealEngine::BattleUIManager::SetPlayerHpBar()
{
	float currenthp = _eventmanager->_playerComp->GetPlayerData()._hp;
	//float maxhp = _eventmanager->_playerComp->GetPlayerInfo()._Maxhp;
	float maxhp = 500;
	float minhp = 0;
	float minhpbar = 0;

	playerhp_targetscale = ((currenthp - minhp) / (maxhp - minhp)) * (_bosshpsize - minhpbar) + minhpbar;
	_playerhp_bar->GetComponent<Transform>()->SetScale(playerhp_targetscale, 0.3f, 1.0f);
	_CoroutineIs(bossdowngauge)
	{
		auto  hpbarcontrol = this;
		auto  downGauge = _playerhp_bar_downGauge;	//GameObject
		float scale = downGauge->GetComponent<Transform>()->GetScale().x;
		float& targetscale = playerhp_targetscale;
		float speed = 0.5f;

		while (targetscale - TimeManager::GetInstance().GetDeltaTime() * speed < scale)
		{
			downGauge->GetComponent<Transform>()->SetScale(scale, 0.3f, 1.0f);
			scale -= TimeManager::GetInstance().GetDeltaTime() * speed;
			Return_null;
		}
		targetscale = 0;
	};

	Startcoroutine(bossdowngauge);
}

/// 1�� ��ų
void KunrealEngine::BattleUIManager::SetSkillcool1()
{
	// ��ų ��ٿ��� ����.
	// �̳��� ��ų�� ���� � ȭ���� �򸰴�.
	// ��Ÿ���� �帧�� ���� �ڽ��� �������� �پ���. ���� �Ʒ���.
	// �׷��鼭 � ȭ���� ���� ������� �������� 0�� �Ǹ� ��ų���� �� ���ư���. active�� ����.
	_CoroutineIs(skillgauge1)
	{
		auto  hpbarcontrol = this;
		auto skillcool = _ui_skill1_cool;
		auto ability = _eventmanager->_playerAbill->_abilityContainer[0];

		// ��ų ��Ÿ�� ���� skillcool ������Ʈ Ȱ��ȭ
		skillcool->SetActive(true);

		// ���� ������ ���� ������
		// ���⼭ ���� �ؾ� �� �κ��� ��Ÿ�Ӱ� �������̴�.
		// ��Ÿ���� �޾ƿµ� ���⼭ ��ŸŸ���� ��� ���ش� 0�� �ɶ�����
		// �������� �� ���� ��Ÿ�Ӱ� �����ؼ� ���� �������� ���Ѵ�.

		float currenttime = ability->_cooldown;
		float maxcool = ability->_cooldown;
		float mincool = 0;

		float _coolsize = _skillcoolsize;
		float coolgauge = 0;

		float speed = 1.0f;
		
		while (0.1 <= currenttime)
		{
			currenttime -= TimeManager::GetInstance().GetDeltaTime();
			if (currenttime < 0.1)
			{
				currenttime == 0.1;
			}

			float nowscale = ((currenttime - mincool) / (maxcool - mincool)) * (_coolsize - coolgauge) + coolgauge;
			skillcool->GetComponent<Transform>()->SetScale(0.7f, nowscale, 1.0f);

			Return_null;
		}

		hpbarcontrol->_eventmanager->_playerAbill->_isShotDetected = false;
		skillcool->SetActive(false);
		skillcool->GetComponent<Transform>()->SetScale(0.7f, _skillcoolsize, 1.0f);
	};
	Startcoroutine(skillgauge1);
}

/// 2�� ��ų
void KunrealEngine::BattleUIManager::SetSkillcool2()
{
	_CoroutineIs(skillgauge2)
	{
		auto  hpbarcontrol = this;
		auto skillcool = _ui_skill2_cool;
		auto ability = _eventmanager->_playerAbill->_abilityContainer[1];

		// ��ų ��Ÿ�� ���� skillcool ������Ʈ Ȱ��ȭ
		skillcool->SetActive(true);

		float currenttime = ability->_cooldown;
		float maxcool = ability->_cooldown;
		float mincool = 0;

		float _coolsize = _skillcoolsize;
		float coolgauge = 0;

		float speed = 1.0f;

		while (0.1 <= currenttime)
		{
			currenttime -= TimeManager::GetInstance().GetDeltaTime();
			if (currenttime < 0.1)
			{
				currenttime == 0.1;
			}

			float nowscale = ((currenttime - mincool) / (maxcool - mincool)) * (_coolsize - coolgauge) + coolgauge;
			skillcool->GetComponent<Transform>()->SetScale(0.7f, nowscale, 1.0f);

			Return_null;
		}

		hpbarcontrol->_eventmanager->_playerAbill->_isIceDetected = false;
		skillcool->SetActive(false);
		skillcool->GetComponent<Transform>()->SetScale(0.7f, _skillcoolsize, 1.0f);
	};

	Startcoroutine(skillgauge2);
}

/// 3�� ��ų
void KunrealEngine::BattleUIManager::SetSkillcool3()
{
	_CoroutineIs(skillgauge3)
	{
		auto  hpbarcontrol = this;
		auto skillcool = _ui_skill3_cool;
		auto ability = _eventmanager->_playerAbill->_abilityContainer[2];

		// ��ų ��Ÿ�� ���� skillcool ������Ʈ Ȱ��ȭ
		skillcool->SetActive(true);

		float currenttime = ability->_cooldown;
		float maxcool = ability->_cooldown;
		float mincool = 0;

		float _coolsize = _skillcoolsize;
		float coolgauge = 0;

		float speed = 1.0f;

		while (0.1 <= currenttime)
		{
			currenttime -= TimeManager::GetInstance().GetDeltaTime();
			if (currenttime < 0.1)
			{
				currenttime == 0.1;
			}

			float nowscale = ((currenttime - mincool) / (maxcool - mincool)) * (_coolsize - coolgauge) + coolgauge;
			skillcool->GetComponent<Transform>()->SetScale(0.7f, nowscale, 1.0f);

			Return_null;
		}

		hpbarcontrol->_eventmanager->_playerAbill->_isAreaDetected = false;
		skillcool->SetActive(false);
		skillcool->GetComponent<Transform>()->SetScale(0.7f, _skillcoolsize, 1.0f);
	};

	Startcoroutine(skillgauge3);
}

/// 4�� ��ų
void KunrealEngine::BattleUIManager::SetSkillcool4()
{
	_CoroutineIs(skillgauge4)
	{
		auto  hpbarcontrol = this;
		auto skillcool = _ui_skill4_cool;
		auto ability = _eventmanager->_playerAbill->_abilityContainer[3];

		// ��ų ��Ÿ�� ���� skillcool ������Ʈ Ȱ��ȭ
		skillcool->SetActive(true);

		float currenttime = ability->_cooldown;
		float maxcool = ability->_cooldown;
		float mincool = 0;

		float _coolsize = _skillcoolsize;
		float coolgauge = 0;

		float speed = 1.0f;

		while (0.1 <= currenttime)
		{
			currenttime -= TimeManager::GetInstance().GetDeltaTime();
			if (currenttime < 0.1)
			{
				currenttime == 0.1;
			}

			float nowscale = ((currenttime - mincool) / (maxcool - mincool)) * (_coolsize - coolgauge) + coolgauge;
			skillcool->GetComponent<Transform>()->SetScale(0.7f, nowscale, 1.0f);

			Return_null;
		}

		hpbarcontrol->_eventmanager->_playerAbill->_isMeteorDetected = false;
		skillcool->SetActive(false);
		skillcool->GetComponent<Transform>()->SetScale(0.7f, _skillcoolsize, 1.0f);
	};

	Startcoroutine(skillgauge4);
}