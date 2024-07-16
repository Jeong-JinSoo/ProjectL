#include "PlayerMove.h"
#include "MeshRenderer.h"
#include "Animator.h"
#include "InputSystem.h"
#include "GraphicsSystem.h"
#include "TimeManager.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Transform.h"
#include "Camera.h"
#include "Navigation.h"
#include "ToolBox.h"
#include "GameObject.h"

KunrealEngine::PlayerMove::PlayerMove()
	:_transform(nullptr), _playerComp(nullptr), _targetPos(), _isDash(false), _isMoving(false), _isDashReady(true)
	, _stopover(), _errorRange(0.5f), _nodeCount(0), _movedRange(0.0f), _movableRange(0.0f), _posY(2.0f),/* _playerDashParticleStart{0}, _playerDashParticleEnd{0},*/
	_timer(1), _isDashStart(false), _isDashEnd(false)
{

}

KunrealEngine::PlayerMove::~PlayerMove()
{

}

void KunrealEngine::PlayerMove::Initialize()
{
	_transform = GetOwner()->GetComponent<Transform>();
	_playerComp = GetOwner()->GetComponent<Player>();
	_SoundComp = GetOwner()->GetComponent<SoundPlayer>();
	_SoundComp->CreateSoundInfo("Resources/Sound/footstep_short.mp3", true, false, 100);
	_SoundComp->CreateSoundInfo("Resources/Sound/footstep_short.mp3", true, false, 100);
	_SoundComp->CreateSound(0, 1);
	_SoundComp->CreateSound(1, 1);
	DashParticleSetting();
}

void KunrealEngine::PlayerMove::Release()
{

}

void KunrealEngine::PlayerMove::FixedUpdate()
{
	if (this->_playerComp->_playerStatus == Player::Status::PARALYSIS || this->_playerComp->_playerStatus == Player::Status::STAGGERED || this->_playerComp->_playerStatus == Player::Status::SWEEP || this->_playerComp->_playerStatus == Player::Status::DEAD)
	{
		ClearNavigation();
	}
}

void KunrealEngine::PlayerMove::Update()
{
	// ���콺 ��Ŭ����	// Ȧ�嵵 ����
	if (InputSystem::GetInstance()->MouseButtonUp(1))
	{
		if (_playerComp->_playerStatus == Player::Status::IDLE || _playerComp->_playerStatus == Player::Status::WALK)
		{
			GRAPHICS->DeleteAllLine();

			// ��ǥ������ ������Ʈ
			UpdateTargetPosition();
			UpdateMoveNode();

			// �̵����·� �������
			_playerComp->_playerStatus = Player::Status::WALK;
			_isMoving = true;
		}
	}

	UpdateSound();

	if (InputSystem::GetInstance()->KeyDown(KEY::SPACE) && this->_isDashReady)
	{
		if (_playerComp->_playerStatus == Player::Status::IDLE || _playerComp->_playerStatus == Player::Status::WALK
			|| _playerComp->_playerStatus == Player::Status::DASH || _playerComp->_playerStatus == Player::Status::ABILITY
			)
		{
			this->_isDashReady = false;
			this->_isDashStart = true;
			Startcoroutine(dashReady);

			// �̵� ���� ����
			_isMoving = false;
			_movedRange = 0.0f;
			_movableRange = 0.0f;
			_timer = 0.0f;

			UpdateTargetPosition();
			UpdateDashNode();
			_isDash = true;
		}
	}

	if (InputSystem::GetInstance()->KeyDown(KEY::S))
	{
		StopPlayer();
	}

	/// ������
	//if (_stopover.size() > 0)
	//{
	//	for (const auto& path : _stopover)
	//	{
	//		GRAPHICS->CreateDebugLine(path.first, path.second, DirectX::XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f));
	//	}
	//}

	//MoveToTarget(_targetPos, 15.f * TimeManager::GetInstance().GetDeltaTime());
	//PlayerDash(_targetPos, _playerComp->_playerInfo._dashSpeed * TimeManager::GetInstance().GetDeltaTime());

	NavigationMove(15.f * TimeManager::GetInstance().GetDeltaTime());
	NavigationDash(15.f * TimeManager::GetInstance().GetDeltaTime());

	ShowPlayerInfo();
}

void KunrealEngine::PlayerMove::LateUpdate()
{

}

void KunrealEngine::PlayerMove::OnTriggerEnter()
{

}

void KunrealEngine::PlayerMove::OnTriggerStay()
{

}

void KunrealEngine::PlayerMove::OnTriggerExit()
{

}

void KunrealEngine::PlayerMove::SetActive(bool active)
{
	this->_isActivated = active;
}

void KunrealEngine::PlayerMove::UpdateTargetPosition()
{
	/// ������ ���콺 ����
	if (this->GetOwner()->GetObjectScene()->GetSceneName() == "Title")
	{
		_targetPos = GRAPHICS->ScreenToWorldPoint(InputSystem::GetInstance()->GetEditorMousePos().x, InputSystem::GetInstance()->GetEditorMousePos().y, -66.0f);

	}
	else
	{
		_targetPos = GRAPHICS->ScreenToWorldPoint(InputSystem::GetInstance()->GetEditorMousePos().x, InputSystem::GetInstance()->GetEditorMousePos().y);
	}

	//_targetPos = GRAPHICS->ScreenToWorldPoint(InputSystem::GetInstance()->GetEditorMousePos().x, InputSystem::GetInstance()->GetEditorMousePos().y);
}

void KunrealEngine::PlayerMove::UpdateMoveNode()
{
	// �׺���̼����κ��� �̵���ǥ ������ �޾ƿ�
	Navigation::GetInstance().SetSEpos(0, _transform->GetPosition().x, _transform->GetPosition().y, _transform->GetPosition().z,
		_targetPos.x, _transform->GetPosition().y, _targetPos.z);

	_stopover = Navigation::GetInstance().FindStraightPath(0);

	// ��ȯ �� ��尡 ������ == ������ �� ���� ������ Ŭ��������
	while (_stopover.size() == 0)
	{
		if (this->_transform->GetPosition().y > this->_posY)
		{
			this->_transform->SetPosition(this->_transform->GetPosition().x, this->_posY, this->_transform->GetPosition().z);
		}

		// �÷��̾� ��ġ���� ���콺 ��ġ���� ���⺤��
		DirectX::XMFLOAT3 currentPoint = _transform->GetPosition();
		DirectX::XMVECTOR currentVec = DirectX::XMLoadFloat3(&currentPoint);
		DirectX::XMFLOAT3 targetPoint = _targetPos;
		//DirectX::XMFLOAT3 targetWithY = { targetPoint.x, targetPoint.y + _posY, targetPoint.z };
		DirectX::XMFLOAT3 targetWithY = { targetPoint.x, _posY, targetPoint.z };
		DirectX::XMVECTOR targetPosVec = DirectX::XMLoadFloat3(&targetWithY);

		// ��ǥ�������κ��� �÷��̾� ��ġ���� ���⺤��
		DirectX::XMVECTOR direction = ToolBox::GetDirectionVec(targetWithY, currentPoint);

		// �÷��̾� �������� ���ݾ� �̵�
		direction = DirectX::XMVectorScale(direction, 1.0f);
		DirectX::XMVECTOR targetVector = DirectX::XMVectorAdd(targetPosVec, direction);

		// ���� ���� ��ǥ������
		DirectX::XMStoreFloat3(&_targetPos, targetVector);

		Navigation::GetInstance().SetSEpos(0, _transform->GetPosition().x, _transform->GetPosition().y, _transform->GetPosition().z,
			_targetPos.x, _transform->GetPosition().y, _targetPos.z);

		_stopover = Navigation::GetInstance().FindStraightPath(0);
	}

	_nodeCount = 0;
}

void KunrealEngine::PlayerMove::UpdateDashNode()
{
	// �÷��̾� ��ġ���� ���콺 ��ġ���� ���⺤��
	DirectX::XMFLOAT3 currentPoint = _transform->GetPosition();
	DirectX::XMVECTOR currentVec = DirectX::XMLoadFloat3(&currentPoint);
	DirectX::XMFLOAT3 targetPoint = _targetPos;
	DirectX::XMFLOAT3 targetWithY = { targetPoint.x, targetPoint.y + _posY, targetPoint.z };

	DirectX::XMVECTOR direction = ToolBox::GetDirectionVec(currentPoint, targetWithY);
	_playerComp->_directionVector = direction;

	// �÷��̾� ��ġ���� ���⺤�� �������� ��� �Ÿ���ŭ�� ��ǥ
	direction = DirectX::XMVectorScale(direction, _playerComp->GetPlayerData()._dashRange);
	DirectX::XMVECTOR targetVector = DirectX::XMVectorAdd(currentVec, direction);

	// ��ǥ���� ��������� ������� �־��ְ�
	DirectX::XMStoreFloat3(&_targetPos, targetVector);

	// ���� ��ǥ�� �׺���̼��� SetEPos �Լ��� �Ű������� �ִ´�
	Navigation::GetInstance().SetSEpos(0, _transform->GetPosition().x, _transform->GetPosition().y, _transform->GetPosition().z,
		_targetPos.x, _targetPos.y + _posY, _targetPos.z);

	// ��ǥ ������ �׺���̼��� RayCast �Լ��� ��ȯ������ ������Ʈ
	_targetPos = Navigation::GetInstance().FindRaycastPath(0);
	DirectX::XMVECTOR targetPosVec = DirectX::XMLoadFloat3(&_targetPos);

	// ��ȯ���� ��带 �������� ��� ���� �Ÿ� ���
	_movableRange = ToolBox::GetDistance(currentPoint, _targetPos);

	// �� ���� ���� ������ ���
	DirectX::XMVECTOR currentForward = DirectX::XMVectorSet(0.0f, _transform->GetRotation().y, -1.0f, 0.0f);

	DirectX::XMVECTOR dotResult = DirectX::XMVector3Dot(currentForward, _playerComp->_directionVector);
	float dotX = DirectX::XMVectorGetX(dotResult);

	// ������ ���ȿ��� �����·� ��ȯ
	float angle = acos(dotX);
	angle = DirectX::XMConvertToDegrees(angle);

	// ������ �����Ǵ� ��� ó��
	if (targetPosVec.m128_f32[0] > currentVec.m128_f32[0])
	{
		angle *= -1;
	}

	// �÷��̾��� ������ ����
	this->_transform->SetRotation(0.0f, angle, 0.0f);

	_nodeCount = 0;
}

void KunrealEngine::PlayerMove::MoveToTarget(DirectX::XMFLOAT3 targetPos, float speed)
{
	// ���ó� �̵������� ��
	// ��ų ������̰ų� �÷��̾ ����ȭ �Ǿ��� ���� �۵� ����
	if (_playerComp->_playerStatus == Player::Status::IDLE || _playerComp->_playerStatus == Player::Status::WALK)
	{
		if (_isMoving)
		{
			// �̵����·� �������
			_playerComp->_playerStatus = Player::Status::WALK;

			DirectX::XMFLOAT3 trans(_transform->GetPosition().x, _transform->GetPosition().y, _transform->GetPosition().z);

			// ��ǥ�� �ϴ� ��ǥ�� �÷��̾��� ��ǥ�� �ٸ� ��
			if (std::abs(_transform->GetPosition().x - targetPos.x) > _errorRange ||
				std::abs(_transform->GetPosition().y - targetPos.y) > _errorRange ||
				std::abs(_transform->GetPosition().z - targetPos.z) > _errorRange)

			{
				DirectX::XMVECTOR currentPosVec = DirectX::XMLoadFloat3(&trans);
				DirectX::XMVECTOR targetPosVec = DirectX::XMLoadFloat3(&targetPos);

				DirectX::XMVECTOR currentForward = DirectX::XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f);
				DirectX::XMVECTOR targetDirection = DirectX::XMVectorSubtract(targetPosVec, currentPosVec);
				targetDirection.m128_f32[1] = 0.0f;
				targetDirection = DirectX::XMVector3Normalize(targetDirection);

				// �� ���� ���� ������ ���
				auto dotResult = DirectX::XMVector3Dot(currentForward, targetDirection);
				float dotProduct = DirectX::XMVectorGetX(dotResult);

				// ������ ���ȿ��� ���� ��ȯ
				float angle = acos(dotProduct);
				angle = DirectX::XMConvertToDegrees(angle);

				if (targetPosVec.m128_f32[0] > currentPosVec.m128_f32[0])
				{
					angle *= -1;
				}

				_transform->SetRotation(0.0f, angle, 0.0f);

				DirectX::XMVECTOR direction = DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&targetPos), currentPosVec);
				direction = DirectX::XMVector3Normalize(direction);

				// �÷��̾��� ���⺤�� ������Ʈ
				_playerComp->_directionVector = direction;

				// �÷��̾� �̵�
				DirectX::XMVECTOR newPosition = DirectX::XMVectorAdd(currentPosVec, DirectX::XMVectorScale(direction, speed * _playerComp->_playerInfo._speedScale));
				_transform->SetPosition(newPosition.m128_f32[0], _posY, newPosition.m128_f32[2]);
			}
			else
			{
				// �̵��� ������ ���� ���·�
				_playerComp->_playerStatus = Player::Status::IDLE;
				_isMoving = false;
			}
		}
	}
}

void KunrealEngine::PlayerMove::NavigationMove(float speed)
{
	// ���ó� �̵������� ��
	// ��ų ������̰ų� �÷��̾ ����ȭ, ���� ���°� �Ǿ��� ���� �۵� ����
	if (_playerComp->_playerStatus == Player::Status::IDLE || _playerComp->_playerStatus == Player::Status::WALK)
	{
		// �������� ������ �����̰� ��� ����ŭ �̵����� �ʾ��� ��
		if (_isMoving && _nodeCount < _stopover.size())
		{
			// �̵����·� �������
			_playerComp->_playerStatus = Player::Status::WALK;

			DirectX::XMFLOAT3 trans(_transform->GetPosition().x, _transform->GetPosition().y, _transform->GetPosition().z);

			// ��ǥ�� �ϴ� ��� ��ǥ�� �÷��̾��� ��ǥ�� �ٸ� ��
			if (std::abs(trans.x - _stopover[_nodeCount].second.x) > _errorRange ||
				std::abs(trans.y - _stopover[_nodeCount].second.y) > _errorRange + 100.0f ||
				std::abs(trans.z - _stopover[_nodeCount].second.z) > _errorRange)
			{
				DirectX::XMVECTOR currentPosVec = DirectX::XMLoadFloat3(&trans);
				DirectX::XMVECTOR targetPosVec = DirectX::XMLoadFloat3(&_stopover[_nodeCount].second);

				DirectX::XMVECTOR currentForward = DirectX::XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f);
				DirectX::XMVECTOR targetDirection = DirectX::XMVectorSubtract(targetPosVec, currentPosVec);
				targetDirection.m128_f32[1] = 0.0f;
				targetDirection = DirectX::XMVector3Normalize(targetDirection);

				// �� ���� ���� ������ ���
				DirectX::XMVECTOR dotResult = DirectX::XMVector3Dot(currentForward, targetDirection);
				float dotProduct = DirectX::XMVectorGetX(dotResult);

				// ������ ���ȿ��� ���� ��ȯ
				float angle = acos(dotProduct);
				angle = DirectX::XMConvertToDegrees(angle);

				if (targetPosVec.m128_f32[0] > currentPosVec.m128_f32[0])
				{
					angle *= -1;
				}

				// ����� ������ �÷��̾��� rotation ����
				_transform->SetRotation(0.0f, angle, 0.0f);

				// ���⺤�� ���
				DirectX::XMVECTOR direction = DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&_stopover[_nodeCount].second), currentPosVec);
				direction = DirectX::XMVector3Normalize(direction);

				// �÷��̾��� ���⺤�� ������Ʈ
				_playerComp->_directionVector = direction;

				// �÷��̾� �̵�
				DirectX::XMVECTOR newPosition = DirectX::XMVectorAdd(currentPosVec, DirectX::XMVectorScale(direction, speed * _playerComp->_playerInfo._speedScale));
				_transform->SetPosition(newPosition.m128_f32[0], _posY, newPosition.m128_f32[2]);
			}
			else
			{
				_nodeCount++;
			}
		}
		else if (_isMoving && _nodeCount == _stopover.size())
		{
			// �̵��� ������ ���� ���·�
			_playerComp->_playerStatus = Player::Status::IDLE;
			_isMoving = false;
		}
		else
		{
			// �̵� ���°� �ƴ϶�� �ƹ� ó���� ����
		}
	}
}

void KunrealEngine::PlayerMove::NavigationDash(float speed)
{
	// ��ô� ���������� ������ => ��尡 �ʿ����
	if (_isDash)
	{
		// �÷��̾��� ���¸� ��÷�
		this->_playerComp->_playerStatus = Player::Status::DASH;
		this->GetOwner()->GetComponent<MeshRenderer>()->SetActive(false);

		// ������Ʈ�� transform
		DirectX::XMFLOAT3 trans(_transform->GetPosition().x, _transform->GetPosition().y, _transform->GetPosition().z);

		// ��� ���� �Ÿ��� �Ѿ��µ��� �����̴� ��Ȳ�� ���� ����ó��
		//if (_movedRange >= _playerComp->GetPlayerData()._dashRange)
		if (_movedRange >= _movableRange)
		{
			_isDash = false;
			_playerComp->_playerStatus = Player::Status::IDLE;
		}
		// ��ǥ�� �ϴ� ��ǥ�� �÷��̾��� ��ǥ�� �ٸ� ��
		else if (std::abs(_transform->GetPosition().x - _targetPos.x) > _errorRange ||
			//std::abs(_transform->GetPosition().y - _targetPos.y) > _errorRange + _posY ||
			std::abs(_transform->GetPosition().z - _targetPos.z) > _errorRange)
		{

			// �÷��̾��� ��ġ�� ��ǥ ��ġ���� ��ǥ �񱳰���
			DirectX::XMVECTOR currentPosVec = DirectX::XMLoadFloat3(&trans);
			DirectX::XMVECTOR targetPosVec = DirectX::XMLoadFloat3(&_targetPos);

			// �÷��̾� �̵�
			DirectX::XMVECTOR newPosition = DirectX::XMVectorAdd(currentPosVec, DirectX::XMVectorScale(_playerComp->_directionVector, _playerComp->_playerInfo._dashSpeed * _playerComp->_playerInfo._speedScale * TimeManager::GetInstance().GetDeltaTime()));
			_transform->SetPosition(newPosition.m128_f32[0], _posY, newPosition.m128_f32[2]);

			// �̵��� �Ÿ� ���
			_movedRange += DirectX::XMVectorGetX(DirectX::XMVector3Length(DirectX::XMVectorSubtract(newPosition, currentPosVec)));
		}
		else
		{
			// ��ġ�� �����ߴٸ� �ٽ� ��� ���·�
			_isDash = false;
			_playerComp->_playerStatus = Player::Status::IDLE;
		}

		if (_isDashStart == true)
		{
			_isDashStart = false;
			_isDashEnd = true;
			for (auto& playerDashParticleStart : _playerDashParticleStart)
			{
				playerDashParticleStart->GetComponent<Transform>()->SetPosition(_transform->GetPosition().x, _transform->GetPosition().y, _transform->GetPosition().z);
			}
		}

		for (auto& playerDashParticleStart : _playerDashParticleStart)
		{
			if (playerDashParticleStart->GetComponent<Particle>()->GetParticleSize().x > 0)
			{
				playerDashParticleStart->GetComponent<Particle>()->SetActive(true);
			}
			else
			{
				playerDashParticleStart->GetComponent<Particle>()->SetActive(false);
			}
		}

		_timer += TimeManager::GetInstance().GetDeltaTime();
		_playerDashParticleStart[0]->GetComponent<Particle>()->SetParticleSize(30 - (120 * _timer), 30 - (120 * _timer));
		_playerDashParticleStart[1]->GetComponent<Particle>()->SetParticleSize(30 - (120 * _timer), 30 - (120 * _timer));
		_playerDashParticleStart[2]->GetComponent<Particle>()->SetParticleSize(10 - (40 * _timer), 10 - (40 * _timer));
		_playerDashParticleStart[3]->GetComponent<Particle>()->SetParticleSize(25 - (100 * _timer), 25 - (100 * _timer));

	}
	else
	{
		this->GetOwner()->GetComponent<MeshRenderer>()->SetActive(true);
		if (this->GetOwner()->GetComponent<MeshRenderer>()->GetActivated() == true)
		{
			if (_isDashEnd == true)
			{
				_timer = 0;
				_isDashEnd = false;
				for (auto& playerDashParticleEnd : _playerDashParticleEnd)
				{
					playerDashParticleEnd->GetComponent<Transform>()->SetPosition(_transform->GetPosition().x, _transform->GetPosition().y, _transform->GetPosition().z);
				}
			}

			_timer += TimeManager::GetInstance().GetDeltaTime();
			for (auto& playerDashParticleEnd : _playerDashParticleEnd)
			{
				if (_timer < 0.22)
				{
					playerDashParticleEnd->GetComponent<Particle>()->SetActive(true);
				}
				else
				{
					playerDashParticleEnd->GetComponent<Particle>()->SetActive(false);
				}
			}

			if (_timer < 0.22)
			{
				_playerDashParticleEnd[0]->GetComponent<Particle>()->SetParticleSize(120 * _timer, 120 * _timer);
				_playerDashParticleEnd[1]->GetComponent<Particle>()->SetParticleSize(120 * _timer, 120 * _timer);
				_playerDashParticleEnd[2]->GetComponent<Particle>()->SetParticleSize(40 * _timer, 40 * _timer);
				_playerDashParticleEnd[3]->GetComponent<Particle>()->SetParticleSize(100 * _timer, 100 * _timer);
			}
			else if (_timer > 0.22)
			{
				_playerDashParticleEnd[0]->GetComponent<Particle>()->SetParticleSize(30, 30);
				_playerDashParticleEnd[1]->GetComponent<Particle>()->SetParticleSize(30, 30);
				_playerDashParticleEnd[2]->GetComponent<Particle>()->SetParticleSize(10, 10);
				_playerDashParticleEnd[3]->GetComponent<Particle>()->SetParticleSize(25, 25);
			}

		}
	}
}

void KunrealEngine::PlayerMove::PlayerDash(DirectX::XMFLOAT3 targetPos, float speed)
{

	if (_isDash)
	{
		// ���ð� �̵������� ��
		// �÷��̾ ����ȭ �Ǿ��� ���� �۵� ����
		//if (_playerComp->_playerStatus == Player::Status::IDLE || _playerComp->_playerStatus == Player::Status::WALK)
		{
			// �÷��̾��� ���¸� ��÷�
			this->_playerComp->_playerStatus = Player::Status::DASH;

			// ������Ʈ�� transform
			DirectX::XMFLOAT3 trans(_transform->GetPosition().x, _transform->GetPosition().y, _transform->GetPosition().z);

			// �÷��̾��� ��ġ�� ��ǥ ��ġ���� ��ǥ �񱳰���
			DirectX::XMVECTOR currentPosVec = DirectX::XMLoadFloat3(&trans);
			DirectX::XMVECTOR targetPosVec = DirectX::XMLoadFloat3(&targetPos);

			DirectX::XMVECTOR currentForward = DirectX::XMVectorSet(0.0f, _transform->GetRotation().y, -1.0f, 0.0f);
			DirectX::XMVECTOR targetDirection = DirectX::XMVectorSubtract(targetPosVec, currentPosVec);
			//targetDirection.m128_f32[1] = 0.0f;
			targetDirection = DirectX::XMVector3Normalize(targetDirection);

			// �� ���� ���� ������ ���
			auto dotResult = DirectX::XMVector3Dot(currentForward, targetDirection);
			float dotProduct = DirectX::XMVectorGetX(dotResult);

			// ������ ���ȿ��� �����·� ��ȯ
			float angle = acos(dotProduct);
			angle = DirectX::XMConvertToDegrees(angle);

			// ������ �����Ǵ� ��� ó��
			if (targetPosVec.m128_f32[0] > currentPosVec.m128_f32[0])
			{
				angle *= -1;
			}

			// ������Ʈ�� ������ ����
			this->_transform->SetRotation(0.0f, angle, 0.0f);

			// ���� ����
			DirectX::XMVECTOR direction = DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&targetPos), currentPosVec);
			direction = DirectX::XMVector3Normalize(direction);

			// �÷��̾��� ���⺤�� ������Ʈ
			_playerComp->_directionVector = direction;

			// ��� �Ÿ���ŭ �̵� ��ǥ ���
			DirectX::XMVECTOR dashTo = DirectX::XMVectorScale(direction, _playerComp->_playerInfo._dashRange);

			// �÷��̾� �̵�
			DirectX::XMVECTOR newPosition = DirectX::XMVectorAdd(currentPosVec, DirectX::XMVectorScale(dashTo, speed * _playerComp->_playerInfo._speedScale));
			this->_transform->SetPosition(newPosition.m128_f32[0], _posY, newPosition.m128_f32[2]);

			// ��ǥ ��ǥ�� �������� �� ��� ���θ� �ٽ� false��
			if (_targetPos.x - _transform->GetPosition().x <= _errorRange
				&& _targetPos.y - _transform->GetPosition().y <= _errorRange
				&& _targetPos.z - _transform->GetPosition().z <= _errorRange
				)
			{
				_isDash = false;

				// �̵� �� �÷��̾��� ���¸� ���� ���·�
				this->_playerComp->_playerStatus = Player::Status::IDLE;

			}

			// ��ǥ ��ǥ�� ���콺 ��ġ�� �Ǿ������Ƿ� �÷��̾� ��ġ���� ���ߵ���
			this->_targetPos = this->_transform->GetPosition();
		}
	}
}


void KunrealEngine::PlayerMove::SetPlayerY(float y)
{
	this->_posY = y;
}


float KunrealEngine::PlayerMove::GetPlayerY()
{
	return this->_posY;
}


void KunrealEngine::PlayerMove::RecalculateNavigation()
{
	// ��ȯ �� ��尡 ������ == ������ �� ���� ������ Ŭ��������
	if (_stopover.size() == 0)
	{
		// �÷��̾� ��ġ���� ���콺 ��ġ���� ���⺤��
		DirectX::XMFLOAT3 currentPoint = _transform->GetPosition();
		DirectX::XMVECTOR currentVec = DirectX::XMLoadFloat3(&currentPoint);
		DirectX::XMFLOAT3 targetPoint = _targetPos;
		DirectX::XMFLOAT3 targetWithY = { targetPoint.x, targetPoint.y + _posY, targetPoint.z };
		DirectX::XMVECTOR targetPosVec = DirectX::XMLoadFloat3(&targetWithY);

		// ��ǥ�������κ��� �÷��̾� ��ġ���� ���⺤��
		DirectX::XMVECTOR direction = ToolBox::GetDirectionVec(targetWithY, currentPoint);

		// �÷��̾� �������� ���ݾ� �̵�
		direction = DirectX::XMVectorScale(direction, 1.0f);
		DirectX::XMVECTOR targetVector = DirectX::XMVectorAdd(targetPosVec, direction);

		// ���� ���� ��ǥ������
		DirectX::XMStoreFloat3(&_targetPos, targetVector);
	}
	else
	{
		return;
	}
}


void KunrealEngine::PlayerMove::ClearNavigation()
{
	this->_stopover.clear();
}

void KunrealEngine::PlayerMove::ShowPlayerInfo()
{
	GRAPHICS->DrawDebugText(300, 300, 20, "%.3f", _targetPos.x);
	GRAPHICS->DrawDebugText(360, 300, 20, "%.3f", _targetPos.y);
	GRAPHICS->DrawDebugText(420, 300, 20, "%.3f", _targetPos.z);
	//GRAPHICS->DrawDebugText(300, 200, 20, "%.3f", GetOwner()->GetComponent<Animator>()->GetMaxFrame());
	//GRAPHICS->DrawDebugText(300, 100, 20, "%.3f", GetOwner()->GetComponent<Animator>()->GetCurrentFrame());

	switch (_playerComp->_playerStatus)
	{
		case Player::Status::IDLE:
			GRAPHICS->DrawDebugText(360, 400, 20, "Player : IDLE");
			break;
		case Player::Status::WALK:
			GRAPHICS->DrawDebugText(360, 400, 20, "Player : WALK");
			break;
		case Player::Status::DASH:
			GRAPHICS->DrawDebugText(360, 400, 20, "Player : DASH");
			break;
		case Player::Status::PARALYSIS:
			GRAPHICS->DrawDebugText(360, 400, 20, "Player : PARALYSIS");
			break;
		case Player::Status::ABILITY:
			GRAPHICS->DrawDebugText(360, 400, 20, "Player : ABILITY");
			break;
		case Player::Status::DEAD:
			GRAPHICS->DrawDebugText(360, 400, 20, "Player : DEAD");
			break;
		default:
			GRAPHICS->DrawDebugText(360, 400, 20, "Player : IDK");
			break;
	}
}

void KunrealEngine::PlayerMove::DashParticleSetting()
{
	for (int i = 0; i < 2; i++)
	{
		GameObject* dashParticle = SceneManager::GetInstance().GetCurrentScene()->CreateObject("dashParticle");
		dashParticle->AddComponent<Particle>();
		dashParticle->_autoAwake = true;
		dashParticle->GetComponent<Particle>()->SetParticleEffect("fx_BlastWave2", "Resources/Textures/Particles/fx_BlastWave2.dds", 1000);
		dashParticle->GetComponent<Particle>()->SetParticleDuration(1.0f, 0.1f);
		dashParticle->GetComponent<Particle>()->SetParticleVelocity(1.0f, true);
		dashParticle->GetComponent<Particle>()->SetParticleSize(30.0f, 30.0f);
		dashParticle->GetComponent<Particle>()->AddParticleColor(0.6f, 0.3f, 1.0f);
		dashParticle->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 0.0f);
		dashParticle->GetComponent<Particle>()->SetOffSet(0.0f, 10.0f, 0.0f);
		dashParticle->GetComponent<Particle>()->SetActive(false);


		if (i == 0)
		{
			_playerDashParticleStart.emplace_back(dashParticle);
		}
		else if (i == 1)
		{
			_playerDashParticleEnd.emplace_back(dashParticle);
		}
	}

	for (int i = 0; i < 2; i++)
	{
		GameObject* dashParticle = SceneManager::GetInstance().GetCurrentScene()->CreateObject("dashParticle");
		dashParticle->AddComponent<Particle>();
		dashParticle->_autoAwake = true;
		dashParticle->GetComponent<Particle>()->SetParticleEffect("fx_EnergyBolt7", "Resources/Textures/Particles/fx_EnergyBolt7.dds", 1000);
		dashParticle->GetComponent<Particle>()->SetParticleDuration(1.0f, 0.1f);
		dashParticle->GetComponent<Particle>()->SetParticleVelocity(1.0f, true);
		dashParticle->GetComponent<Particle>()->SetParticleSize(30.f, 30.0f);
		dashParticle->GetComponent<Particle>()->AddParticleColor(0.3f, 0.15f, 0.5f);
		dashParticle->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 0.0f);
		dashParticle->GetComponent<Particle>()->SetOffSet(0.0f, 10.0f, 0.0f);
		dashParticle->GetComponent<Particle>()->SetActive(false);

		if (i == 0)
		{
			_playerDashParticleStart.emplace_back(dashParticle);
		}
		else if (i == 1)
		{
			_playerDashParticleEnd.emplace_back(dashParticle);
		}
	}

	for (int i = 0; i < 2; i++)
	{
		GameObject* dashParticle = SceneManager::GetInstance().GetCurrentScene()->CreateObject("dashParticle");
		dashParticle->AddComponent<Particle>();
		dashParticle->_autoAwake = true;
		dashParticle->GetComponent<Particle>()->SetParticleEffect("Lightning2", "Resources/Textures/Particles/fx_Lightning2.dds", 1000);
		dashParticle->GetComponent<Particle>()->SetParticleDuration(1.0f, 1.0f);
		dashParticle->GetComponent<Particle>()->SetParticleVelocity(30.0f, true);
		dashParticle->GetComponent<Particle>()->SetParticleSize(10.f, 10.0f);
		dashParticle->GetComponent<Particle>()->AddParticleColor(0.6f, 0.3f, 1.0f);
		dashParticle->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 0.0f);
		dashParticle->GetComponent<Particle>()->SetOffSet(0.0f, 10.0f, 0.0f);
		dashParticle->GetComponent<Particle>()->SetActive(false);

		if (i == 0)
		{
			_playerDashParticleStart.emplace_back(dashParticle);
		}
		else if (i == 1)
		{
			_playerDashParticleEnd.emplace_back(dashParticle);
		}
	}

	for (int i = 0; i < 2; i++)
	{
		GameObject* dashParticle = SceneManager::GetInstance().GetCurrentScene()->CreateObject("dashParticle");
		dashParticle->AddComponent<Particle>();
		dashParticle->_autoAwake = true;
		dashParticle->GetComponent<Particle>()->SetParticleEffect("LightFlash2", "Resources/Textures/Particles/fx_LightFlash2.dds", 1000);
		dashParticle->GetComponent<Particle>()->SetParticleDuration(1.0f, 0.1f);
		dashParticle->GetComponent<Particle>()->SetParticleVelocity(1.0f, true);
		dashParticle->GetComponent<Particle>()->SetParticleSize(25.f, 25.0f);
		dashParticle->GetComponent<Particle>()->AddParticleColor(0.6f, 0.3f, 1.0f);
		dashParticle->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 0.0f);
		dashParticle->GetComponent<Particle>()->SetOffSet(0.0f, 10.0f, 0.0f);
		dashParticle->GetComponent<Particle>()->SetActive(false);

		if (i == 0)
		{
			_playerDashParticleStart.emplace_back(dashParticle);
		}
		else if (i == 1)
		{
			_playerDashParticleEnd.emplace_back(dashParticle);
		}

	}

}

void KunrealEngine::PlayerMove::UpdateSound()
{
	if (this->_playerComp->_playerStatus == Player::Status::WALK)
	{
		auto comp = GetOwner()->GetComponent<Animator>();
		float fram = comp->GetCurrentFrame();
		//GRAPHICS->DrawDebugText(200, 30, 20, "%f", fram);
		if ((fram > 1.0) && (fram < 1.2))
		{
			_SoundComp->Play(0);
		}

		if ((fram > 13.0) && (fram < 13.2))
		{
			_SoundComp->Play(1);
		}
	}
}

bool KunrealEngine::PlayerMove::GetisDashed()
{
	return _isDash;
}

DirectX::XMFLOAT3 KunrealEngine::PlayerMove::GetTargetPosition()
{
	return _targetPos;
}

float& KunrealEngine::PlayerMove::GetMovedRange()
{
	return _movedRange;
}

void KunrealEngine::PlayerMove::StopPlayer()
{
	// �÷��̾ �̵����̶��	// �����, ��ų �����, �ǰ� ���϶� �Ұ�
	if (this->_playerComp->_playerStatus == Player::Status::WALK)
	{
		// ��� ���·� �����
		this->_playerComp->_playerStatus = Player::Status::IDLE;
		
		// ��带 �����
		this->_stopover.clear();
	}
}
