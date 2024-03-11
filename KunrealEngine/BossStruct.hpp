#pragma once
#include <string>
#include <functional>

// ������ ����
enum class BossStatus
{
	IDLE,				// �����, ���� ���� ������ ����

	CHASE,				// �÷��̾� ����

	STAGGERED,			// ���� ����ȭ
	DEAD,				// ���

	BASIC_ATTACK,		// �⺻ ����
	CORE_ATTACK,		// ��ų ���		// ��ų ������ ���� ���� ����
	PATTERN_END			// ������ ���� ���
};


// ������ �⺻���� ���� ����
struct BossBasicInfo
{
	BossBasicInfo()
		: _hp(0.0f), _phase(0), _armor(0.0f), _damage(0.0f), _moveSpeed(0.0f), _staggeredGauge(0.0f)
	{};

	BossBasicInfo& SetHp(float hp) { _hp = hp;  return *this; };
	BossBasicInfo& SetPhase(unsigned int phase) { _phase = phase;  return *this; };
	BossBasicInfo& SetArmor(float armor) { _armor = armor;  return *this; };
	BossBasicInfo& SetDamage(float damage) { _damage = damage;  return *this; };
	BossBasicInfo& SetMoveSpeed(float moveSpeed) { _moveSpeed = moveSpeed;  return *this; };
	BossBasicInfo& SetsStaggeredGauge(float staggeredGauge) { _staggeredGauge = staggeredGauge;  return *this; };

	float _hp;				// ü��
	unsigned int _phase;	// ü�� ��� ������

	float _armor;			// ����
	float _damage;			// ������

	float _moveSpeed;		// �̵��ӵ�

	float _staggeredGauge;	// ����ȭ ������
};

struct BossPattern
{
	BossPattern()
		: _animName(""), _damage(0.0f), _speed(0.0f), _range(0.0f), _afterDelay(0.0f), _effectName(""), _isWarning(false), _warningName("warningName"), _triggerHp(0.0f), 
		_coolDown(0.0f), _isActive(true),
		_logic(nullptr)
	{
	}

	void Play() { if (_logic!= nullptr) _logic(); }		// ���� ���� �Լ�

	BossPattern& SetAnimName(const std::string& animName) { _animName = animName; return *this; };
	BossPattern& SetDamage(float damage) { _damage = damage; return *this; };
	BossPattern& SetSpeed(float speed) { _speed = speed; return *this; };
	BossPattern& SetRange(float range) { _range = range; return *this; };
	BossPattern& SetAfterDelay(float afterDelay) { _afterDelay = afterDelay; return *this; };
	BossPattern& SetIsWarning(bool isWarning) { _isWarning = isWarning; return *this; };
	BossPattern& SetWarningName(const std::string& warningName) { _warningName = warningName; return *this; };
	BossPattern& SetTriggerHp(float hp) { _triggerHp = hp; return *this; };
	BossPattern& SetCoolDown(float coolDown) { _triggerHp = coolDown; return *this; };
	BossPattern& SetActive(bool isActive) { _triggerHp = isActive; return *this; };
	BossPattern& SetLogic(std::function<void()> logic) { _logic = logic; return *this; };

	std::string _animName;			// ���� �ִϸ��̼� �̸�
									
	float _damage;					// ������
									
	float _speed;					// ����, �ִϸ��̼� ���ǵ�
									
	float _range;					// ���� ����
									
	float _afterDelay;				// ���� �� ������ �ִ� �ð�
									
	std::string _effectName;		// ���� ����Ʈ �ؽ��� �̸�
									
	bool _isWarning;				// ��ų ������ ���� ������ ���ΰ�
	std::string _warningName;		// ���� �ؽ��� �̸�

	float _triggerHp;				// ������ �ߵ��� ������ hp

	float _coolDown;				// ���� ��Ÿ��

	bool _isActive;					// ���� Ȱ��ȭ ����

	std::function<void()> _logic;	// ���� ����
};