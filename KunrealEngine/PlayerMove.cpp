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

KunrealEngine::PlayerMove::PlayerMove()
	:_transform(nullptr), _playerComp(nullptr), _targetPos(), _isDash(false), _isMoving(false)
	, _stopover(), _errorRange(0.5f), _nodeCount(0), _movedRange(0.0f), _posY(0.0f)
{
	_tempX = SceneManager::GetInstance().GetCurrentScene()->GetMainCamera()->GetComponent<Transform>()->GetPosition().x - 0;
	_tempY = SceneManager::GetInstance().GetCurrentScene()->GetMainCamera()->GetComponent<Transform>()->GetPosition().y - 0;
	_tempZ = SceneManager::GetInstance().GetCurrentScene()->GetMainCamera()->GetComponent<Transform>()->GetPosition().z - 0;


}

KunrealEngine::PlayerMove::~PlayerMove()
{

}

void KunrealEngine::PlayerMove::Initialize()
{
	_transform = GetOwner()->GetComponent<Transform>();
	_playerComp = GetOwner()->GetComponent<Player>();
}

void KunrealEngine::PlayerMove::Release()
{
	
}

void KunrealEngine::PlayerMove::FixedUpdate()
{
	
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

	/// ���⿡ ��Ÿ�� ���� �� �÷��̾� ���� ���� �߰��ؾ���
	if (InputSystem::GetInstance()->KeyDown(KEY::SPACE))
	{
		// �̵� ���� ����
		_isMoving = false;
		_movedRange = 0.0f;

		UpdateTargetPosition();
		UpdateDashNode();
		_isDash = true;
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
	_targetPos = GRAPHICS->ScreenToWorldPoint(InputSystem::GetInstance()->GetEditorMousePos().x, InputSystem::GetInstance()->GetEditorMousePos().y);
}

void KunrealEngine::PlayerMove::UpdateMoveNode()
{ 
	// �׺���̼����κ��� �̵���ǥ ������ �޾ƿ�
	Navigation::GetInstance().SetSEpos(0, _transform->GetPosition().x, _transform->GetPosition().y, _transform->GetPosition().z,
		_targetPos.x, _targetPos.y, _targetPos.z);

	_stopover = Navigation::GetInstance().FindStraightPath(0);
	_nodeCount = 0;
}

void KunrealEngine::PlayerMove::UpdateDashNode()
{
	// �׺���̼����κ��� �̵���ǥ ������ �޾ƿ�
	Navigation::GetInstance().SetSEpos(0, _transform->GetPosition().x, _transform->GetPosition().y, _transform->GetPosition().z,
		_targetPos.x, _targetPos.y, _targetPos.z);

	// ��ô� raycast�� ���� ��ġ�� ��ǥ ��ġ �������� ��
	// ���콺 ��ġ�� ������ targetPos�� ��� �Ÿ��� ���
	DirectX::XMFLOAT3 targetPoint = Navigation::GetInstance().FindRaycastPath(0);
	DirectX::XMFLOAT3 currentPoint = _transform->GetPosition();
	DirectX::XMVECTOR currentVector = DirectX::XMLoadFloat3(&currentPoint);

	float distance = ToolBox::GetDistance(currentPoint, targetPoint);

	// ���� ���� ���ϱ�
	DirectX::XMVECTOR direction = ToolBox::GetDirectionVec(currentPoint, targetPoint);

	// �÷��̾��� ���⺤�� ����
	// ����ϸ鼭 �ٽ� �����ҹٿ� �̹� ����� �迡 ���⼭ �߰�����
 	_playerComp->_directionVector = direction;


	// ��ǥ ��ǥ�� ��� ���� �Ÿ����� �ָ� ���� ���
	if (distance > _playerComp->GetPlayerData()._dashRange)
	{
		// ��� �Ÿ���ŭ ��ǥ ��ǥ �̵�
		direction = DirectX::XMVectorScale(direction, _playerComp->GetPlayerData()._dashRange);
		DirectX::XMVECTOR targetVector = DirectX::XMVectorAdd(currentVector, direction);
		
		// _targetPos�� ���갪�� �־���
		DirectX::XMStoreFloat3(&_targetPos, targetVector);

	}
	else
	{
		_targetPos = targetPoint;
	}

	DirectX::XMVECTOR targetPosVec = DirectX::XMLoadFloat3(&_targetPos);

	// �� ���� ���� ������ ���
	DirectX::XMVECTOR currentForward = DirectX::XMVectorSet(0.0f, _transform->GetRotation().y, -1.0f, 0.0f);

	DirectX::XMVECTOR dotResult = DirectX::XMVector3Dot(currentForward, _playerComp->_directionVector);
	float dotX = DirectX::XMVectorGetX(dotResult);

	// ������ ���ȿ��� �����·� ��ȯ
	float angle = acos(dotX);
	angle = DirectX::XMConvertToDegrees(angle);

	// ������ �����Ǵ� ��� ó��
	if (targetPosVec.m128_f32[0] > currentVector.m128_f32[0])
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

				// ī�޶� �̵�
				DirectX::XMFLOAT3 cameraPos(_tempX + newPosition.m128_f32[0], _tempY, _tempZ + newPosition.m128_f32[2]);
				SceneManager::GetInstance().GetCurrentScene()->GetMainCamera()->GetComponent<Transform>()->SetPosition(cameraPos.x, cameraPos.y, cameraPos.z);
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

				// ī�޶� �̵�
				DirectX::XMFLOAT3 cameraPos(_tempX + newPosition.m128_f32[0], _tempY, _tempZ + newPosition.m128_f32[2]);
				SceneManager::GetInstance().GetCurrentScene()->GetMainCamera()->GetComponent<Transform>()->SetPosition(cameraPos.x, cameraPos.y, cameraPos.z);

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

		// ������Ʈ�� transform
		DirectX::XMFLOAT3 trans(_transform->GetPosition().x, _transform->GetPosition().y, _transform->GetPosition().z);

		// ��� ���� �Ÿ��� �Ѿ��µ��� �����̴� ��Ȳ�� ���� ����ó��
		if (_movedRange >= _playerComp->GetPlayerData()._dashRange)
		{
			_isDash = false;
			_playerComp->_playerStatus = Player::Status::IDLE;
		}
		// ��ǥ�� �ϴ� ��ǥ�� �÷��̾��� ��ǥ�� �ٸ� ��
		else if (std::abs(_transform->GetPosition().x - _targetPos.x) > _errorRange ||
			std::abs(_transform->GetPosition().y - _targetPos.y) > _errorRange + 100.0f ||
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

			// ī�޶� �̵�
			DirectX::XMFLOAT3 cameraPos(_tempX + newPosition.m128_f32[0], _tempY, _tempZ + newPosition.m128_f32[2]);
			SceneManager::GetInstance().GetCurrentScene()->GetMainCamera()->GetComponent<Transform>()->SetPosition(cameraPos.x, cameraPos.y, cameraPos.z);
		
		}
		else
		{
			// ��ġ�� �����ߴٸ� �ٽ� ��� ���·�
			_isDash = false;
			_playerComp->_playerStatus = Player::Status::IDLE;
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

			// ī�޶� �̵�
			DirectX::XMFLOAT3 cameraPos(_tempX + newPosition.m128_f32[0], _tempY, _tempZ + newPosition.m128_f32[2]);
			SceneManager::GetInstance().GetCurrentScene()->GetMainCamera()->GetComponent<Transform>()->SetPosition(cameraPos.x, cameraPos.y, cameraPos.z);

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

void KunrealEngine::PlayerMove::ShowPlayerInfo()
{
	//GRAPHICS->DrawDebugText(300, 300, 20, "%.3f", _targetPos.x);
	//GRAPHICS->DrawDebugText(360, 300, 20, "%.3f", _targetPos.y);
	//GRAPHICS->DrawDebugText(420, 300, 20, "%.3f", _targetPos.z);
	GRAPHICS->DrawDebugText(300, 200, 20, "%.3f", GetOwner()->GetComponent<Animator>()->GetMaxFrame());
	GRAPHICS->DrawDebugText(300, 100, 20, "%.3f", GetOwner()->GetComponent<Animator>()->GetCurrentFrame());
	
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
		default:
			GRAPHICS->DrawDebugText(360, 400, 20, "Player : IDK");
			break;
	}
}