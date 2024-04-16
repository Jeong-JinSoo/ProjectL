#pragma once
#include <string>
#include <functional>
#include "SceneManager.h"
#include "Scene.h"
#include "GameObject.h"

// ������ ����
enum class BossStatus
{
	ENTER,				// ���� �� �÷��̾� ����ġ�� ��
	IDLE,				// ���� ���� ������ ����

	CHASE,				// �÷��̾� ����

	STAGGERED,			// ���� ����ȭ
	OFF_STAGGRED,
	DEAD,				// ���

	PATTERN_READY,		// ���� �غ�
	BASIC_ATTACK,		// �⺻ ����
	CORE_ATTACK,		// ��ų ���		// ��ų ������ ���� ���� ����
	PATTERN_END			// ������ ���� ���
};


// ������ �⺻���� ���� ����
struct BossBasicInfo
{
	BossBasicInfo()
		: _hp(0.0f), _phase(0), _armor(0.0f), _damage(0.0f), _attackRange(0.0f), _moveSpeed(20.0f), _rotationSpeed(100.0f), _baseAnimSpeed(30.0f),
		_staggeredGauge(0.0f), _maxHp(0.0f), _maxStaggeredGauge(0.0f)
	{};

	BossBasicInfo& SetHp(float hp) { _maxHp = hp; _hp = hp;   return *this; };
	BossBasicInfo& SetPhase(unsigned int phase) { _phase = phase;  return *this; };
	BossBasicInfo& SetArmor(float armor) { _armor = armor;  return *this; };
	BossBasicInfo& SetDamage(float damage) { _damage = damage;  return *this; };
	BossBasicInfo& SetAttackRange(float attackRange) { _attackRange = attackRange;  return *this; };
	BossBasicInfo& SetMoveSpeed(float moveSpeed) { _moveSpeed = moveSpeed;  return *this; };
	BossBasicInfo& SetRotateSpeed(float rotationSpeed) { _rotationSpeed = rotationSpeed;  return *this; };
	BossBasicInfo& SetBaseAnimationSpeed(float animSpeed) { _baseAnimSpeed = animSpeed;  return *this; };
	BossBasicInfo& SetsStaggeredGauge(float staggeredGauge) { _maxStaggeredGauge = staggeredGauge; _staggeredGauge = staggeredGauge;  return *this; };

	float GetMaxHP() { return _maxHp; };
	float GetMaxStaggeredGauage() { return _maxStaggeredGauge; };

	float _hp;				// ü��
	unsigned int _phase;	// ü�� ��� ������

	float _armor;			// ����
	float _damage;			// ������
	float _attackRange;		// �⺻ ���� ����

	float _moveSpeed;		// �̵��ӵ�
	float _rotationSpeed;	// ȸ�� �ӵ�
	float _baseAnimSpeed;	// �ִϸ��̼� �ӵ�

	float _staggeredGauge;	// ����ȭ ������

private:
	float _maxHp;				// �ִ� ü��
	float _maxStaggeredGauge;	// ����ȭ ������ �ִ�ġ

	// ������ ������
};

struct BossPattern
{
	BossPattern()
		: _patternName(""), _animName(""), _damage(0.0f), _speed(0.0f), _range(0.0f), _afterDelay(0.0f), _effectName(""), _isWarning(false), _warningName("warningName"), _triggerHp(0.0f),
		_coolDown(0.0f), _isActive(true), _maxColliderOnCount(1), _subObject(),
		_logic(nullptr), _initializeLogic(nullptr)
	{
	}

	~BossPattern()
	{
		for (auto object : _subObject)
		{
			KunrealEngine::SceneManager::GetInstance().GetCurrentScene()->DeleteGameObject(object);
		}
	}

	bool Play() { if (_logic!= nullptr) return _logic(); }		// ���� ���� �Լ�
	void Initialize() { if (_initializeLogic != nullptr) _initializeLogic(); }	// �ʱ�ȭ ���� �Լ�
	BossPattern& SetPatternName(const std::string& patterName) { _patternName = patterName; return *this; };
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
	BossPattern& SetMaxColliderCount(unsigned int count) { _maxColliderOnCount = count; return *this; };
	BossPattern& SetLogic(std::function<bool()> logic) { _logic = logic; return *this; };
	BossPattern& SetInitializeLogic(std::function<void()> initialize) { _initializeLogic = initialize; return *this; };

	std::string _patternName;		// ���� �̸�

	std::string _animName;			// ���� �ִϸ��̼� �̸�
									
	float _damage;					// ������
									
	float _speed;					// ����, �ִϸ��̼� ���ǵ�
									
	float _range;					// ���� ����
									
	float _afterDelay;				// ���� �� ������ �ִ� �ð� (��)
									
	std::string _effectName;		// ���� ����Ʈ �ؽ��� �̸�
									
	bool _isWarning;				// ��ų ������ ���� ������ ���ΰ�
	std::string _warningName;		// ���� �ؽ��� �̸�

	float _triggerHp;				// ������ �ߵ��� ������ hp

	float _coolDown;				// ���� ��Ÿ��

	bool _isActive;					// ���� Ȱ��ȭ ����

	std::function<bool()> _logic;	// ���� ����

	std::function<void()> _initializeLogic;	// �� ���� �ʱ�ȭ������� �͵��� ��Ƴ���

	unsigned int _maxColliderOnCount; // ���� �� �ݶ��̴��� ������ Ƚ�� ���� (�⺻ 1)

	std::vector<KunrealEngine::GameObject*> _subObject;
};