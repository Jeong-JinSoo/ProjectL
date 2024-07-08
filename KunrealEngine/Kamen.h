#pragma once
#include "KunrealAPI.h"
#include "Boss.h"
#include "Coroutine.h"

namespace KunrealEngine
{
	class _DECLSPEC Kamen : public Component, public Boss
	{
	public:
		Kamen();
		virtual ~Kamen();

		void Initialize() override;
		void Release() override;

		void FixedUpdate() override;
		void Update() override;
		void LateUpdate() override;

		void OnTriggerEnter() override;
		void OnTriggerStay() override;
		void OnTriggerExit() override;

		void SetActive(bool active) override;

		virtual void SetMesh() override;
		virtual void SetTexture() override;
		virtual void SetBossTransform() override;
		virtual void SetBossCollider() override;

	public:
		virtual void CreatePattern() override;

		virtual void Idle() override;

		virtual void SpecialAttack2() override;

	public:
		void GamePattern();

	private:
		void CreateParticleObject();

	private:
		void CreateSubObject();
		
	private:
		void BasicPattern();

	private:
		// �⺻ ����
		void LeftRightPattern();
		void RightLeftPattern();
		void BackStepCallPattern();
		void TeleportSpellPattern();
		void EmergenceAttackPattern();

		// �� �ܵ� ����
		void SwordTurnClockPattern();
		void SwordTurnAntiClockPattern();
		void SwordLinearAttackPattern();
		void SwordChopPattern();

		// �� WITH ī��
		void BasicSwordAttackPattern();

		// �ھ� ����
	private:
		void CoreEmmergencePattern();

		// �⺻ ���� ������
	private:
		void CreateLeftAttack();
		void CreateLeftAttackThrowingFire();
		void CreateRightAttack();
		void CreateRightAttackThrowingFire();
		void CreateSpellAttack();
		void CreateCallAttack();
		void CreateCall2Attack();
		void CreateBossRandomInsideWarning();
		void CreateFiveWayAttack();

		// �� with ī�� ����
	private:
		void CreateSwordAttack();

		// �� �ܵ� ����
	private:
		void CreateSwordEmergence();
		void CreateSwordHide();

		void CraeteSwordTurnClockWise();
		void CreateSwordTurnAntiClock();
		void CreateOutsideSafe();
		void CreateInsideSafe();
		void CreateDonutSafe();
		void CreateDonutAttack();

		void CreateSwordLookPlayer();

		void CreateSwordLinearReady();
		void CreateSwordLinearAttack();

		void CreateSwordChopAttack();

		//�� �ٽ� ����
	private:
		void CreateSwordMultipleAttack();

		// 3������ with ��
	private:
		void CreateSwordSwingVertical();
		void CreateSwordSwingTwice();
		void CreateSwordSwingTwiceHard();
		void CreateSwordSwingHorizontal();
		void CreateSwordRotationAttack();

		// ī�� �̵� ����
	private:
		void CreateTurn180();
		void CreateBackStep();
		void CreateTeleportToCenter();
		void CreateTeleportToCenterWithLook();
		void CreateTeleportRandomWithLook();
		void CreateTeleportWithSwordSwing();
		void CreateTurnClockWise();
		void CreateTurnAntiClockWise();
		void CreateReverseEmergence();
		void CreateEmergence();

		// �ٽ� ��� ����
	private:
		void CreateEmergenceAttack();

	public:
		const DirectX::XMFLOAT3& GetSwordDirection();

		const DirectX::XMFLOAT3& GetSwordPos();

		const DirectX::XMFLOAT3& GetEgoPos();

		const float GetDonutSize();

		// �нſ� ���Ǽ� �Լ�
	private:
		DirectX::XMVECTOR GetEgoDirection();

		// ī�࿡ ���̴� Į �ݶ��̴� ��ġ ������
	private:
		void SetKamenSwordCollider(int startFrame, int EndFrame, int nowFrame);
		void SetKamenSwordCollider();

	private:
		BossPattern* _leftAttack;
		BossPattern* _rightAttack;
		BossPattern* _spellAttack;
		BossPattern* _callAttack;
		BossPattern* _call2Attack;
		BossPattern* _bossRandomInsideWarning;
		BossPattern* _fiveWayAttack;

		BossPattern* _backStep;
		BossPattern* _turn180;
		BossPattern* _teleport;
		BossPattern* _teleportWithLook;
		BossPattern* _teleportRandomWithLook;
		BossPattern* _reverseEmergence;
		BossPattern* _emergence;

		BossPattern* _leftFireAttack;
		BossPattern* _rightFireAttack;

		BossPattern* _turnClockWise;
		BossPattern* _turnAntiClockWise;
		BossPattern* _outsideSafe;
		BossPattern* _insideSafe;
		BossPattern* _donutSafe;
		BossPattern* _donutAttack;

		BossPattern* _basicSwordAttack;
		BossPattern* _swordEmmergence;
		BossPattern* _swordHide;
		BossPattern* _swordTurnClockWise;
		BossPattern* _swordTurnAntiClockWise;
		BossPattern* _swordLinearAtack;
		BossPattern* _swordLinearReady;
		BossPattern* _swordLookPlayer;
		BossPattern* _swordChopAttack;

		/// 3������
		BossPattern* _swordSwingVertical;
		BossPattern* _swordSwingTwice;
		BossPattern* _swordSwingTwiceHard;
		BossPattern* _swordSwingHorizontal;
		BossPattern* _swordRotationAttack;

		BossPattern* _swordSwingTeleport;

	private:
		BossPattern* _emergence9Lich;
		BossPattern* _swordMultipleAttack;

	private:
		// ���Ͽ� �ʿ��� subObject��
		GameObject* _leftHand;
		GameObject* _rightHand;
		GameObject* _call;
		GameObject* _call2;
		GameObject* _lazer;
		GameObject* _lazerCollider;
		GameObject* _bossInsideWarning;
		GameObject* _bossInsideAttack;
		std::vector<GameObject*> _fiveAttack;

		GameObject* _swordInsideWarning;
		GameObject* _swordInsideAttack;
		GameObject* _swordOutsideWarning;
		GameObject* _swordDonutWarning1;
		GameObject* _swordDonutWarning2;
		GameObject* _swordDonutWarning3;

		std::vector<GameObject*> _swordDonutAttack;

		GameObject* _freeSword;
		GameObject* _freeSwordCollider;
		GameObject* _swordPath;

		GameObject* _kamenSword;
		GameObject* _kamenSwordCollider;
		std::vector<GameObject*> _blade;
		GameObject* _largeBlade;

		std::vector<GameObject*> _handFire;

		GameObject* _leftHandBone;
		GameObject* _rightHandBone;

		GameObject* _alterEgo;

		std::vector<GameObject*> _egoHandFire;

		GameObject* _egoCall2;

		GameObject* _egoLazer;
		GameObject* _egoLazerCollider;

		GameObject* _egoLeftHandBone;
		GameObject* _egoRightHandBone;

		GameObject* _egoInsideWarning;

		GameObject* _egoInsideAttack;

		GameObject* _egoEmAttack;

		std::vector<GameObject*> _fakeBoss;

		GameObject* _kamenSwordLight;

	private:
		// Call �Ÿ� üũ��
		float _callMoveDistance;
		unsigned int _call2PrevStep;
		bool _isRotateFinish;

		unsigned int _egoCall2PrevStep;

	private:
		std::vector<DirectX::XMFLOAT3> _fakeDirVec;
		std::vector<float> _fakeMoveDistance;

	private:
		std::vector<bool> _handFireReady;
		std::vector<DirectX::XMFLOAT3> _handFireDir;

	private:
		std::vector<bool> _egoHandFireReady;
		std::vector<DirectX::XMFLOAT3> _egoHandFireDir;

	private:
		bool _isCoreStart;
		bool _isRandomStart;

	private:
		unsigned int _targetIndex;

	private:
		float _donutSize;

	private:
		std::function<void()> _callInitLogic;

	private:
		DirectX::XMFLOAT3 _centerPos;

	private:
		DirectX::XMFLOAT3 _randomPos;

	private:
		DirectX::XMFLOAT3 _emergencePos;

	private:
		float _swordRotateAngle;

		DirectX::XMFLOAT3 _swordStartPos;
		DirectX::XMFLOAT3 _swordOriginPos;

		float _swordCircleWarningSize;

		float _bossCircleWarningSize;


		float _swordMoveDistance;
		float _swordMoveDistance2;


		DirectX::XMFLOAT3 _swordDirection;
		DirectX::XMFLOAT3 _swordRotation;

		DirectX::XMFLOAT3 _swordDirection2;

		float _swordChopSpeed;

		float _timer;

		float _timer2;

		float _timer3;

		float _swordTimer;

		float _warningMaxTimer;

	private:
		bool _isSwordSecondAttackStart;

	private:
		bool _isSpecial2Ready;
		bool _isSpecial2Playing;
		bool _isEgoAppearInit;
		bool _isEgoAppearFinish;
		bool _isEgoAttackReady;
		bool _isEgoAttack;
		float _egoTimer;

		// ��ƼŬ
	private:
		std::vector<GameObject*> _particleBossfire1;
		std::vector<GameObject*> _particleBossfire2;

		std::vector<GameObject*> _particleEgofire1;
		std::vector<GameObject*> _particleEgofire2;

		std::vector<GameObject*> _particleCall2;
		std::vector<GameObject*> _particleEgoCall2;

		std::vector<GameObject*> _particleSwordOutsideAttack;
		std::vector<GameObject*> _particleSwordDonutAttack;

		std::vector<GameObject*> _particleEmergenceAttack;
		std::vector<GameObject*> _particleEgoEmergenceAttack;

		std::vector<GameObject*> _particleSwordDonut1;
		std::vector<GameObject*> _particleSwordDonut2;
		std::vector<GameObject*> _particleSwordDonut3;

		std::vector <GameObject*> _verticalParticleSide;

		std::vector <GameObject*> _verticalParticle;

		std::vector<GameObject*> _kamenSwordParticle;

		std::vector<GameObject*> _kamenSwordAfterImageParticle;



		// ���� �Ҹ갪�� ���� ����

		float _spellDel;
		float _spellWaveDel;
	};
}