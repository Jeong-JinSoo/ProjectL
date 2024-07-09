#include "BattleUIManager.h"
#include "EventManager.h"
#include "TimeManager.h"
#include "PlayerAbility.h"

KunrealEngine::BattleUIManager::BattleUIManager()
	:_bosshpsize(83.0f), _playerhpsize(47.5f), _skillcoolsize(1.0f), _isdied(false)
{
	_eventmanager = &KunrealEngine::EventManager::GetInstance();
}

KunrealEngine::BattleUIManager::~BattleUIManager()
{
}

void KunrealEngine::BattleUIManager::Initialize()
{
	KunrealEngine::SceneManager& scene = KunrealEngine::SceneManager::GetInstance();

	KunrealEngine::GameObject* player_background;			// ���� UI ���
	KunrealEngine::GameObject* player_background1;			// ���� UI ���
	KunrealEngine::GameObject* player_background2;			// ���� UI ���

	KunrealEngine::GameObject* boss_background;			// ���� UI ���
	KunrealEngine::GameObject* boss_background1;			// ���� UI ���
	KunrealEngine::GameObject* boss_background2;			// ���� UI ���

	KunrealEngine::GameObject* bosshp_barback;		// ���� ü�¹� ���
	KunrealEngine::GameObject* bosshp_background;	// �÷��̾� ü�¹� ���
	KunrealEngine::GameObject* bosshp_background1;	// �÷��̾� ü�¹� ���
	KunrealEngine::GameObject* bosshp_background2;	// �÷��̾� ü�¹� ���

	KunrealEngine::GameObject* playerhp_barback;	// �÷��̾� ü�¹� ���
	KunrealEngine::GameObject* playerhp_background;	// �÷��̾� ü�¹� ���
	KunrealEngine::GameObject* playerhp_background1;	// �÷��̾� ü�¹� ���
	KunrealEngine::GameObject* playerhp_background2;	// �÷��̾� ü�¹� ���

	KunrealEngine::GameObject* ui_skill1;		// 1�� ��ų
	KunrealEngine::GameObject* ui_skill1_icon;		// 1�� ��ų
	KunrealEngine::GameObject* ui_skill2;		// 2�� ��ų
	KunrealEngine::GameObject* ui_skill2_icon;		// 1�� ��ų
	KunrealEngine::GameObject* ui_skill3;		// 3�� ��ų
	KunrealEngine::GameObject* ui_skill3_icon;		// 1�� ��ų
	KunrealEngine::GameObject* ui_skill4;		// 4�� ��ų
	KunrealEngine::GameObject* ui_skill4_icon;		// 1�� ��ų

	KunrealEngine::GameObject* potion;			// ���� �Ա�
	KunrealEngine::GameObject* potion_icon;			// ���� �Ա�
	KunrealEngine::GameObject* dash;			// �뽬(ȸ��?)
	KunrealEngine::GameObject* dash_icon;			// �뽬(ȸ��?)

	_battleuibox = this->GetOwner();

	player_background = scene.GetCurrentScene()->CreateObject("background");
	player_background->SetParent(_battleuibox);
	player_background->AddComponent<ImageRenderer>();
	player_background->GetComponent<ImageRenderer>()->SetImage("ui/freebox-middle.png");
	player_background->GetComponent<ImageRenderer>()->SetPosition(673.0f, 933.0f);
	player_background->GetComponent<Transform>()->SetScale(2.81f, 0.5f, 1.0f);

	player_background1 = scene.GetCurrentScene()->CreateObject("background");
	player_background1->SetParent(_battleuibox);
	player_background1->AddComponent<ImageRenderer>();
	player_background1->GetComponent<ImageRenderer>()->SetImage("ui/freebox-sideL.png");
	player_background1->GetComponent<ImageRenderer>()->SetPosition(651.0f, 933.0f);
	player_background1->GetComponent<Transform>()->SetScale(0.5f, 0.5f, 1.0f);

	player_background2 = scene.GetCurrentScene()->CreateObject("background");
	player_background2->SetParent(_battleuibox);
	player_background2->AddComponent<ImageRenderer>();
	player_background2->GetComponent<ImageRenderer>()->SetImage("ui/freebox-sideR.png");
	player_background2->GetComponent<ImageRenderer>()->SetPosition(1268.0f, 933.0f);
	player_background2->GetComponent<Transform>()->SetScale(0.5f, 0.5f, 1.0f);

	ui_skill1_icon = scene.GetCurrentScene()->CreateObject("ui_skill1_icon");
	ui_skill1_icon->SetParent(_battleuibox);
	ui_skill1_icon->AddComponent<ImageRenderer>();
	ui_skill1_icon->GetComponent<ImageRenderer>()->SetImage("ui/skill_space.png");
	ui_skill1_icon->GetComponent<ImageRenderer>()->SetPosition(708.0f, 980.f);
	ui_skill1_icon->GetComponent<Transform>()->SetScale(1.0f, 1.0f, 1.0f);

	_ui_skill1_cool = scene.GetCurrentScene()->CreateObject("_ui_skill1_cool");
	_ui_skill1_cool->SetParent(_battleuibox);
	_ui_skill1_cool->AddComponent<ImageRenderer>();
	_ui_skill1_cool->GetComponent<ImageRenderer>()->SetImage("ui/skill_backspace.png");
	_ui_skill1_cool->GetComponent<ImageRenderer>()->SetPosition(708.f, 980.f);
	_ui_skill1_cool->GetComponent<Transform>()->SetScale(1.0f, 1.0f, 1.0f);

	ui_skill1 = scene.GetCurrentScene()->CreateObject("ui_skill1");
	ui_skill1->SetParent(_battleuibox);
	ui_skill1->AddComponent<ImageRenderer>();
	ui_skill1->GetComponent<ImageRenderer>()->SetImage("ui/skill space.png");
	ui_skill1->GetComponent<ImageRenderer>()->SetPosition(708.0f, 980.f);
	ui_skill1->GetComponent<Transform>()->SetScale(1.0f, 1.0f, 1.0f);

	ui_skill2_icon = scene.GetCurrentScene()->CreateObject("ui_skill2_icon");
	ui_skill2_icon->SetParent(_battleuibox);
	ui_skill2_icon->AddComponent<ImageRenderer>();
	ui_skill2_icon->GetComponent<ImageRenderer>()->SetImage("ui/skill_space.png");
	ui_skill2_icon->GetComponent<ImageRenderer>()->SetPosition(820.f, 980.f);
	ui_skill2_icon->GetComponent<Transform>()->SetScale(1.0f, 1.0f, 1.0f);

	_ui_skill2_cool = scene.GetCurrentScene()->CreateObject("_ui_skill2_cool");
	_ui_skill2_cool->SetParent(_battleuibox);
	_ui_skill2_cool->AddComponent<ImageRenderer>();
	_ui_skill2_cool->GetComponent<ImageRenderer>()->SetImage("ui/skill_backspace.png");
	_ui_skill2_cool->GetComponent<ImageRenderer>()->SetPosition(820.f, 980.f);
	_ui_skill2_cool->GetComponent<Transform>()->SetScale(1.0f, 1.0f, 1.0f);

	ui_skill2 = scene.GetCurrentScene()->CreateObject("ui_skill2");
	ui_skill2->SetParent(_battleuibox);
	ui_skill2->AddComponent<ImageRenderer>();
	ui_skill2->GetComponent<ImageRenderer>()->SetImage("ui/skill space.png");
	ui_skill2->GetComponent<ImageRenderer>()->SetPosition(820.f, 980.f);
	ui_skill2->GetComponent<Transform>()->SetScale(1.0f, 1.0f, 1.0f);

	ui_skill3_icon = scene.GetCurrentScene()->CreateObject("ui_skill3_icon");
	ui_skill3_icon->SetParent(_battleuibox);
	ui_skill3_icon->AddComponent<ImageRenderer>();
	ui_skill3_icon->GetComponent<ImageRenderer>()->SetImage("ui/skill_space.png");
	ui_skill3_icon->GetComponent<ImageRenderer>()->SetPosition(935.f, 980.f);
	ui_skill3_icon->GetComponent<Transform>()->SetScale(1.0f, 1.0f, 1.0f);

	_ui_skill3_cool = scene.GetCurrentScene()->CreateObject("_ui_skill3_cool");
	_ui_skill3_cool->SetParent(_battleuibox);
	_ui_skill3_cool->AddComponent<ImageRenderer>();
	_ui_skill3_cool->GetComponent<ImageRenderer>()->SetImage("ui/skill_backspace.png");
	_ui_skill3_cool->GetComponent<ImageRenderer>()->SetPosition(935.f, 980.f);
	_ui_skill3_cool->GetComponent<Transform>()->SetScale(1.0f, 1.0f, 1.0f);

	ui_skill3 = scene.GetCurrentScene()->CreateObject("ui_skill3");
	ui_skill3->SetParent(_battleuibox);
	ui_skill3->AddComponent<ImageRenderer>();
	ui_skill3->GetComponent<ImageRenderer>()->SetImage("ui/skill space.png");
	ui_skill3->GetComponent<ImageRenderer>()->SetPosition(935.f, 980.f);
	ui_skill3->GetComponent<Transform>()->SetScale(1.0f, 1.0f, 1.0f);

	ui_skill4_icon = scene.GetCurrentScene()->CreateObject("ui_skill4");
	ui_skill4_icon->SetParent(_battleuibox);
	ui_skill4_icon->AddComponent<ImageRenderer>();
	ui_skill4_icon->GetComponent<ImageRenderer>()->SetImage("ui/skill_space.png");
	ui_skill4_icon->GetComponent<ImageRenderer>()->SetPosition(1050.f, 980.f);
	ui_skill4_icon->GetComponent<Transform>()->SetScale(1.0f, 1.0f, 1.0f);

	_ui_skill4_cool = scene.GetCurrentScene()->CreateObject("_ui_skill4_cool");
	_ui_skill4_cool->SetParent(_battleuibox);
	_ui_skill4_cool->AddComponent<ImageRenderer>();
	_ui_skill4_cool->GetComponent<ImageRenderer>()->SetImage("ui/skill_backspace.png");
	_ui_skill4_cool->GetComponent<ImageRenderer>()->SetPosition(1050.f, 980.f);
	_ui_skill4_cool->GetComponent<Transform>()->SetScale(1.0f, 1.0f, 1.0f);

	ui_skill4 = scene.GetCurrentScene()->CreateObject("ui_skill4_icon");
	ui_skill4->SetParent(_battleuibox);
	ui_skill4->AddComponent<ImageRenderer>();
	ui_skill4->GetComponent<ImageRenderer>()->SetImage("ui/skill space.png");
	ui_skill4->GetComponent<ImageRenderer>()->SetPosition(1050.f, 980.f);
	ui_skill4->GetComponent<Transform>()->SetScale(1.0f, 1.0f, 1.0f);

	potion_icon = scene.GetCurrentScene()->CreateObject("ui_skill5");
	potion_icon->SetParent(_battleuibox);
	potion_icon->AddComponent<ImageRenderer>();
	potion_icon->GetComponent<ImageRenderer>()->SetImage("ui/potion.png");
	potion_icon->GetComponent<ImageRenderer>()->SetPosition(1170.f, 976.f);
	potion_icon->GetComponent<Transform>()->SetScale(0.8f, 0.8f, 1.0f);

	_potion_cool = scene.GetCurrentScene()->CreateObject("ui_skill5");
	_potion_cool->SetParent(_battleuibox);
	_potion_cool->AddComponent<ImageRenderer>();
	_potion_cool->GetComponent<ImageRenderer>()->SetImage("ui/skill_backspace.png");
	_potion_cool->GetComponent<ImageRenderer>()->SetPosition(1170.f, 976.f);
	_potion_cool->GetComponent<Transform>()->SetScale(0.4f, 0.4f, 1.0f);

	potion = scene.GetCurrentScene()->CreateObject("ui_skill5");
	potion->SetParent(_battleuibox);
	potion->AddComponent<ImageRenderer>();
	potion->GetComponent<ImageRenderer>()->SetImage("ui/sub-skill.png");
	potion->GetComponent<ImageRenderer>()->SetPosition(1170.f, 976.f);
	potion->GetComponent<Transform>()->SetScale(0.8f, 0.8f, 1.0f);

	dash_icon = scene.GetCurrentScene()->CreateObject("ui_skill6");
	dash_icon->SetParent(_battleuibox);
	dash_icon->AddComponent<ImageRenderer>();
	dash_icon->GetComponent<ImageRenderer>()->SetImage("ui/potion.png");
	dash_icon->GetComponent<ImageRenderer>()->SetPosition(1170.f, 1023.f);
	dash_icon->GetComponent<Transform>()->SetScale(0.8f, 0.8f, 1.0f);

	_dash_cool = scene.GetCurrentScene()->CreateObject("ui_skill6");
	_dash_cool->SetParent(_battleuibox);
	_dash_cool->AddComponent<ImageRenderer>();
	_dash_cool->GetComponent<ImageRenderer>()->SetImage("ui/skill_backspace.png");
	_dash_cool->GetComponent<ImageRenderer>()->SetPosition(1170.f, 1023.f);
	_dash_cool->GetComponent<Transform>()->SetScale(0.4f, 0.4f, 1.0f);

	dash = scene.GetCurrentScene()->CreateObject("ui_skill6");
	dash->SetParent(_battleuibox);
	dash->AddComponent<ImageRenderer>();
	dash->GetComponent<ImageRenderer>()->SetImage("ui/sub-skill.png");
	dash->GetComponent<ImageRenderer>()->SetPosition(1170.f, 1023.f);
	dash->GetComponent<Transform>()->SetScale(0.8f, 0.8f, 1.0f);

	/// player
	playerhp_background = scene.GetCurrentScene()->CreateObject("_playerhp_background");
	playerhp_background->SetParent(_battleuibox);
	playerhp_background->AddComponent<ImageRenderer>();
	playerhp_background->GetComponent<ImageRenderer>()->SetImage("ui/Gauge-middle.png");
	playerhp_background->GetComponent<ImageRenderer>()->SetPosition(689.0f, 950.f);
	playerhp_background->GetComponent<Transform>()->SetScale(32.14, 1.0f, 0.1f);

	_playerhp_bar_downGauge = scene.GetCurrentScene()->CreateObject("playerhp_bar");
	_playerhp_bar_downGauge->SetParent(_battleuibox);
	_playerhp_bar_downGauge->AddComponent<ImageRenderer>();
	_playerhp_bar_downGauge->GetComponent<ImageRenderer>()->SetImage("ui/GaugeFillY-middle.png");
	_playerhp_bar_downGauge->GetComponent<ImageRenderer>()->SetPosition(678.0f, 954.f);
	_playerhp_bar_downGauge->GetComponent<Transform>()->SetScale(_playerhpsize, 1.0f, 0.1f);

	_playerhp_bar = scene.GetCurrentScene()->CreateObject("playerhp_bar");
	_playerhp_bar->SetParent(_battleuibox);
	_playerhp_bar->AddComponent<ImageRenderer>();
	_playerhp_bar->GetComponent<ImageRenderer>()->SetImage("ui/GaugeFillR-middle.png");
	_playerhp_bar->GetComponent<ImageRenderer>()->SetPosition(678.0f, 954.f);
	_playerhp_bar->GetComponent<Transform>()->SetScale(_playerhpsize, 1.0f, 0.1f);

	playerhp_background1 = scene.GetCurrentScene()->CreateObject("backgroundL");
	playerhp_background1->SetParent(_battleuibox);
	playerhp_background1->AddComponent<ImageRenderer>();
	playerhp_background1->GetComponent<ImageRenderer>()->SetImage("ui/gauge-sideL_cover.png");
	playerhp_background1->GetComponent<ImageRenderer>()->SetPosition(674.0f, 950.0f);
	playerhp_background1->GetComponent<Transform>()->SetScale(1.0f, 1.0f, 1.0f);

	playerhp_background2 = scene.GetCurrentScene()->CreateObject("backgroundR");
	playerhp_background2->SetParent(_battleuibox);
	playerhp_background2->AddComponent<ImageRenderer>();
	playerhp_background2->GetComponent<ImageRenderer>()->SetImage("ui/gauge-sideR_cover.png");
	playerhp_background2->GetComponent<ImageRenderer>()->SetPosition(1235.0f, 950.0f);
	playerhp_background2->GetComponent<Transform>()->SetScale(1.0f, 1.0f, 1.0f);

	/// boss
	boss_background = scene.GetCurrentScene()->CreateObject("boss_background");
	boss_background->SetParent(_battleuibox);
	boss_background->AddComponent<ImageRenderer>();
	boss_background->GetComponent<ImageRenderer>()->SetImage("ui/freebox-middle.png");
	boss_background->GetComponent<ImageRenderer>()->SetPosition(454.0f, 6.0f);
	boss_background->GetComponent<Transform>()->SetScale(4.71f, 0.11f, 1.0f);

	boss_background1 = scene.GetCurrentScene()->CreateObject("boss_background1");
	boss_background1->SetParent(_battleuibox);
	boss_background1->AddComponent<ImageRenderer>();
	boss_background1->GetComponent<ImageRenderer>()->SetImage("ui/freebox-sideL.png");
	boss_background1->GetComponent<ImageRenderer>()->SetPosition(445.0f, 6.0f);
	boss_background1->GetComponent<Transform>()->SetScale(0.2f, 0.11f, 1.0f);

	boss_background2 = scene.GetCurrentScene()->CreateObject("boss_background2");
	boss_background2->SetParent(_battleuibox);
	boss_background2->AddComponent<ImageRenderer>();
	boss_background2->GetComponent<ImageRenderer>()->SetImage("ui/freebox-sideR.png");
	boss_background2->GetComponent<ImageRenderer>()->SetPosition(1452.0f, 6.0f);
	boss_background2->GetComponent<Transform>()->SetScale(0.2f, 0.11f, 1.0f);

	bosshp_background = scene.GetCurrentScene()->CreateObject("bosshp_background");
	bosshp_background->SetParent(_battleuibox);
	bosshp_background->AddComponent<ImageRenderer>();
	bosshp_background->GetComponent<ImageRenderer>()->SetImage("ui/Gauge-middle.png");
	bosshp_background->GetComponent<ImageRenderer>()->SetPosition(469.f, 11.f);
	bosshp_background->GetComponent<Transform>()->SetScale(57.0, 1.0f, 0.1f);

	_bosshp_bar_downGauge = scene.GetCurrentScene()->CreateObject("_bosshp_bar_downGauge");
	_bosshp_bar_downGauge->SetParent(_battleuibox);
	_bosshp_bar_downGauge->AddComponent<ImageRenderer>();
	_bosshp_bar_downGauge->GetComponent<ImageRenderer>()->SetImage("ui/GaugeFillY-middle.png");
	_bosshp_bar_downGauge->GetComponent<ImageRenderer>()->SetPosition(455.f, 14.f);
	_bosshp_bar_downGauge->GetComponent<Transform>()->SetScale(_bosshpsize, 1.0f, 1.f);

	_bosshp_bar = scene.GetCurrentScene()->CreateObject("_bosshp_bar");
	_bosshp_bar->SetParent(_battleuibox);
	_bosshp_bar->AddComponent<ImageRenderer>();
	_bosshp_bar->GetComponent<ImageRenderer>()->SetImage("ui/GaugeFillR-middle.png");
	_bosshp_bar->GetComponent<ImageRenderer>()->SetPosition(455.0f, 14.f);
	_bosshp_bar->GetComponent<Transform>()->SetScale(_bosshpsize, 1.0f, 1.f);

	bosshp_background1 = scene.GetCurrentScene()->CreateObject("bosshp_background1");
	bosshp_background1->SetParent(_battleuibox);
	bosshp_background1->AddComponent<ImageRenderer>();
	bosshp_background1->GetComponent<ImageRenderer>()->SetImage("ui/gauge-sideL_cover.png");
	bosshp_background1->GetComponent<ImageRenderer>()->SetPosition(454.0f, 11.0f);
	bosshp_background1->GetComponent<Transform>()->SetScale(1.0f, 1.0f, 1.0f);

	bosshp_background2 = scene.GetCurrentScene()->CreateObject("bosshp_background2");
	bosshp_background2->SetParent(_battleuibox);
	bosshp_background2->AddComponent<ImageRenderer>();
	bosshp_background2->GetComponent<ImageRenderer>()->SetImage("ui/gauge-sideR_cover.png");
	bosshp_background2->GetComponent<ImageRenderer>()->SetPosition(1436.0f, 11.0f);
	bosshp_background2->GetComponent<Transform>()->SetScale(1.0f, 1.0f, 1.0f);

	// �÷��̾ �׾����� �� UI
	_died1 = scene.GetCurrentScene()->CreateObject("you_die1");
	_died1->SetParent(_battleuibox);
	_died1->AddComponent<ImageRenderer>();
	_died1->GetComponent<ImageRenderer>()->SetImage("ui/you_die.png");
	_died1->GetComponent<ImageRenderer>()->SetPosition(0.0f, 0.0f);
	_died1->GetComponent<Transform>()->SetScale(1.0f, 1.0f, 1.0f);

	_died3 = scene.GetCurrentScene()->CreateObject("you_die3");
	_died3->SetParent(_battleuibox);
	_died3->AddComponent<ImageRenderer>();
	_died3->GetComponent<ImageRenderer>()->SetImage("ui/youdied_light.png");
	_died3->GetComponent<ImageRenderer>()->SetPosition(0.0f, 0.0f);
	_died3->GetComponent<Transform>()->SetScale(1.0f, 1.0f, 1.0f);

	_died2 = scene.GetCurrentScene()->CreateObject("you_die2");
	_died2->SetParent(_battleuibox);
	_died2->AddComponent<ImageRenderer>();
	_died2->GetComponent<ImageRenderer>()->SetImage("ui/youdie_text.png");
	_died2->GetComponent<ImageRenderer>()->SetPosition(0.0f, 0.0f);
	_died2->GetComponent<Transform>()->SetScale(1.0f, 1.0f, 1.0f);

	_battleuibox->SetActive(true);
	_ui_skill1_cool->SetActive(false);
	_ui_skill2_cool->SetActive(false);
	_ui_skill3_cool->SetActive(false);
	_ui_skill4_cool->SetActive(false);
	_dash_cool->SetActive(false);
	_potion_cool->SetActive(false);

	_died1->SetActive(false);
	_died2->SetActive(false);
	_died3->SetActive(false);
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
	auto& bossinfo = _eventmanager->_bossComp->GetBossInfo();
	auto& playerinfo = _eventmanager->_playerComp->GetPlayerData();


	// �̰� �׽�Ʈ ������ ������ �÷��̾��� ü���� ������ ���� �� �ְ� �ص� �κ�
	if (InputSystem::GetInstance()->KeyDown(KEY::UP))
	{
		bossinfo._hp += 10;
	}

	else if (InputSystem::GetInstance()->KeyDown(KEY::DOWN))
	{
		bossinfo._hp -= 10;
	}

	if (InputSystem::GetInstance()->KeyDown(KEY::PGUP))
	{
		playerinfo._hp += 10;
	}

	else if (InputSystem::GetInstance()->KeyDown(KEY::PGDOWN))
	{
		playerinfo._hp -= 100;
	}
	 
	// ü���� �޾ƿͼ� �������� �ݿ��ϴ� �κ�
	int bosshp = bossinfo._hp;
	int playerhp = playerinfo._hp;

	if (pre_bosshp != bosshp)
	{
		SetBossHpbar();
	}
	if (pre_playerhp != playerhp)
	{
		SetPlayerHpBar();
	}

	pre_bosshp = bosshp;
	pre_playerhp = playerhp;

	// �÷��̾� ü���� 0�� �Ǿ��� ��?
	if (playerhp <= 0 && !_isdied)
	{
		_isdied = true;
		ActiveDiedUI();
	}

	// ��ų ��Ÿ�� ����
	if (abill->_isShotDetected)
	{
		SetSkillcool1();
	}

	if (abill->_isIceDetected)
	{
		SetSkillcool2();
	}

	if (abill->_isLaserDetected)
	{
		SetSkillcool3();
	}

	if (abill->_isMeteorDetected)
	{
		SetSkillcool4();
	}

	if (InputSystem::GetInstance()->KeyDown(KEY::SPACE))
	{
		Setdashcool();
	}

//	if (InputSystem::GetInstance()->KeyDown(KEY::SPACE))
//	{
//		Setpotioncool();
//	}
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
		_bosshp_bar->GetComponent<Transform>()->SetScale(booshp_targetscale, 1.0f, 1.0f);
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
			float speed = 6.0f;


			while (targetscale - TimeManager::GetInstance().GetDeltaTime() * speed < scale)
			{
				downGauge->GetComponent<Transform>()->SetScale(scale, 1.0f, 1.0f);
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
	float maxhp = 1000;
	float minhp = 0;
	float minhpbar = 0;

	playerhp_targetscale = ((currenthp - minhp) / (maxhp - minhp)) * (_bosshpsize - minhpbar) + minhpbar;
	_playerhp_bar->GetComponent<Transform>()->SetScale(playerhp_targetscale, 1.0f, 1.0f);
	_CoroutineIs(playerdowngauge)
	{
		//auto  hpbarcontrol = this;
		auto  downGauge = _playerhp_bar_downGauge;	//GameObject
		float scale = downGauge->GetComponent<Transform>()->GetScale().x;
		float& targetscale = playerhp_targetscale;
		float speed = 3.0f;

		while (targetscale - TimeManager::GetInstance().GetDeltaTime() * speed < scale)
		{
			downGauge->GetComponent<Transform>()->SetScale(scale, 1.0f, 1.0f);
			scale -= TimeManager::GetInstance().GetDeltaTime() * speed;
			Return_null;
		}
		targetscale = 0;
	};

	Startcoroutine(playerdowngauge);
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
		auto control = this;
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
			skillcool->GetComponent<Transform>()->SetScale(1.0f, nowscale, 1.0f);

			Return_null;
		}

		control->_eventmanager->_playerAbill->_isShotDetected = false;
		skillcool->SetActive(false);
		skillcool->GetComponent<Transform>()->SetScale(1.0f, _skillcoolsize, 1.0f);
	};
	Startcoroutine(skillgauge1);
}

/// 2�� ��ų
void KunrealEngine::BattleUIManager::SetSkillcool2()
{
	_CoroutineIs(skillgauge2)
	{
		auto control = this;
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
			skillcool->GetComponent<Transform>()->SetScale(1.0f, nowscale, 1.0f);

			Return_null;
		}

		control->_eventmanager->_playerAbill->_isIceDetected = false;
		skillcool->SetActive(false);
		skillcool->GetComponent<Transform>()->SetScale(1.0f, _skillcoolsize, 1.0f);
	};

	Startcoroutine(skillgauge2);
}

/// 3�� ��ų
void KunrealEngine::BattleUIManager::SetSkillcool3()
{
	_CoroutineIs(skillgauge3)
	{
		auto control = this;
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
			skillcool->GetComponent<Transform>()->SetScale(1.0f, nowscale, 1.0f);

			Return_null;
		}

		control->_eventmanager->_playerAbill->_isLaserDetected = false;
		skillcool->SetActive(false);
		skillcool->GetComponent<Transform>()->SetScale(1.0f, _skillcoolsize, 1.0f);
	};

	Startcoroutine(skillgauge3);
}

/// 4�� ��ų
void KunrealEngine::BattleUIManager::SetSkillcool4()
{
	_CoroutineIs(skillgauge4)
	{
		auto control = this;
		auto skillcool = _ui_skill4_cool;
		auto ability = _eventmanager->_playerAbill->_abilityContainer[3];

		// ��ų ��Ÿ�� ���� skillcool ������Ʈ Ȱ��ȭ
		skillcool->SetActive(true);

		float currenttime = ability->_cooldown;
		float maxcool = ability->_cooldown;
		float mincool = 0;

		float _coolsize = 0.8;
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
			skillcool->GetComponent<Transform>()->SetScale(1.0f, nowscale, 1.0f);

			Return_null;
		}

		control->_eventmanager->_playerAbill->_isMeteorDetected = false;
		skillcool->SetActive(false);
		skillcool->GetComponent<Transform>()->SetScale(0.8f, 0.8f, 1.0f);
	};

	Startcoroutine(skillgauge4);
}

void KunrealEngine::BattleUIManager::Setdashcool()
{
	_CoroutineIs(dashgauge)
	{
		auto control = this;
		auto skillcool = _dash_cool;
		float boxsize = 0.4f;
		//auto ability = _eventmanager->_playerAbill->_abilityContainer[3];

		// ��ų ��Ÿ�� ���� skillcool ������Ʈ Ȱ��ȭ
		skillcool->SetActive(true);

		float currenttime = 8.0f;
		float maxcool = 8.0f;
		float mincool = 0;

		float _coolsize = boxsize;
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
			skillcool->GetComponent<Transform>()->SetScale(boxsize, nowscale, 1.0f);

			Return_null;
		}

		control->_eventmanager->_playerAbill->_isMeteorDetected = false;
		skillcool->SetActive(false);
		skillcool->GetComponent<Transform>()->SetScale(boxsize, boxsize, 1.0f);
	};

	Startcoroutine(dashgauge);
}

void KunrealEngine::BattleUIManager::Setpotioncool()
{
	_CoroutineIs(potiongauge)
	{
		auto control = this;
		auto skillcool = _potion_cool;
		auto ability = _eventmanager->_playerAbill->_abilityContainer[3];
		float boxsize = 0.4f;

		// ��ų ��Ÿ�� ���� skillcool ������Ʈ Ȱ��ȭ
		skillcool->SetActive(true);

		float currenttime = ability->_cooldown;
		float maxcool = ability->_cooldown;
		float mincool = 0;

		float _coolsize = boxsize;
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
			skillcool->GetComponent<Transform>()->SetScale(boxsize, nowscale, 1.0f);

			Return_null;
		}

		control->_eventmanager->_playerAbill->_isMeteorDetected = false;
		skillcool->SetActive(false);
		skillcool->GetComponent<Transform>()->SetScale(boxsize, boxsize, 0.4f);
	};

	Startcoroutine(potiongauge);
}

void KunrealEngine::BattleUIManager::ActiveDiedUI()
{
	_CoroutineIs(diedcoro)
	{
		// UI����? �ϴ� �ΰ����� �з�
		// 1�ܰ�� ��UI�� ���. �ſ� �̹��� �ð����� textui �� ���
		// textui�� õõ�� Ŀ���� ���� ��ui ���� �ƽ��ƽ��ϰ� Ŀ���� �ɵ�
		// ������ �ϴ°�? �����
		auto uimanager = this;
		float delta = 0;
		float scale = 1.0f;

		uimanager->_died1->SetActive(true);
		uimanager->_died2->SetActive(true);
		uimanager->_died3->SetActive(true);

		int loop = 0;
		while (loop < 50)
		{
			scale += 0.005;
			uimanager->_died2->GetComponent<Transform>()->SetScale(scale, scale, 1.0f);
			uimanager->_died3->GetComponent<Transform>()->SetScale(scale, scale, 1.0f);
			auto preimgsize = uimanager->_died2->GetComponent<ImageRenderer>()->GetImageSize();
			
			Return_null;

			DirectX::XMUINT2 imgsize = uimanager->_died2->GetComponent<ImageRenderer>()->GetImageSize();
			auto nowpos = uimanager->_died2->GetComponent<Transform>()->GetPosition();

			float a = nowpos.x - ((float)(imgsize.x - preimgsize.x)/2);
			float b = nowpos.y - ((float)(imgsize.y - preimgsize.y)/2);
			uimanager->_died2->GetComponent<Transform>()->SetPosition(a, b, 1.0f);
			uimanager->_died3->GetComponent<Transform>()->SetPosition(a, b, 1.0f);
			

			loop++;
		}
		float alpha = 1.0f;
		
		for (; alpha > 0 ; alpha -= 0.1f)
		{
			uimanager->_died3->GetComponent<ImageRenderer>()->SetAlpha(alpha);
			Waitforsecond(0.1f);
		}

		uimanager->_died3->SetActive(false);
		//
		// ���⿡ Ÿ��Ʋ ������ ���ư��� �ڵ带 ���� ��
		//
	};
	Startcoroutine(diedcoro);
}
