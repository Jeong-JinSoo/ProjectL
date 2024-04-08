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

KunrealEngine::PlayerMove::PlayerMove()
	:_transform(nullptr), _playerComp(nullptr), _targetPos(), _isDash(false), _isMoving(false)
	, _stopover(), _errorRange(0.5f), _nodeCount(0)
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
	if (InputSystem::GetInstance()->MouseButtonInput(1))
	{
		// ��ǥ������ ������Ʈ
		UpdateTargetPosition();

		// �̵����·� �������
		_playerComp->_playerStatus = Player::Status::WALK;
		_isMoving = true;
	}

	/// ���⿡ ��Ÿ�� ���ǵ� �߰��ؾ���
	if (InputSystem::GetInstance()->KeyDown(KEY::SPACE))
	{
		UpdateTargetPosition();
		_isDash = true;
	}

	MoveToTarget(_targetPos, 15.f * TimeManager::GetInstance().GetDeltaTime());
	PlayerDash(_targetPos, _playerComp->_playerInfo._dashSpeed * TimeManager::GetInstance().GetDeltaTime());

	//NavigationMove(_targetPos, 15.f * TimeManager::GetInstance().GetDeltaTime());
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

	// ��� ���� ������Ʈ
	_nodeCount = _stopover.size();
}

void KunrealEngine::PlayerMove::MoveToTarget(DirectX::XMFLOAT3 targetPos, float speed)
{
	// ���ð� �̵������� ��
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
				_transform->SetPosition(newPosition.m128_f32[0], 0, newPosition.m128_f32[2]);

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

void KunrealEngine::PlayerMove::NavigationMove(DirectX::XMFLOAT3 targetPos, float speed)
{
	if (_playerComp->_playerStatus == Player::Status::IDLE || _playerComp->_playerStatus == Player::Status::WALK)
	{
		if (_isMoving)
		{
			// �׺���̼����κ��� �̵���ǥ ������ �޾ƿ�
			Navigation::GetInstance().SetSEpos(0, _transform->GetPosition().x, _transform->GetPosition().y, _transform->GetPosition().z,
						_targetPos.x, _targetPos.y, _targetPos.z);
			_stopover = Navigation::GetInstance().FindStraightPath(0);
			
			for (int i = 0; i < _stopover.size(); i++)
			{
				DirectX::XMFLOAT3 trans(_transform->GetPosition().x, _transform->GetPosition().y, _transform->GetPosition().z);

				// ��ǥ�� �ϴ� ��ǥ�� �÷��̾��� ��ǥ�� �ٸ� ��
				if (std::abs(_transform->GetPosition().x - _stopover[i].second.x) > _errorRange ||
					std::abs(_transform->GetPosition().y - _stopover[i].second.y) > _errorRange ||
					std::abs(_transform->GetPosition().z - _stopover[i].second.z) > _errorRange)

				{
					DirectX::XMVECTOR currentPosVec = DirectX::XMLoadFloat3(&trans);
					DirectX::XMVECTOR targetPosVec = DirectX::XMLoadFloat3(&_stopover[i].second);

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

					DirectX::XMVECTOR direction = DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&_stopover[i].second), currentPosVec);
					direction = DirectX::XMVector3Normalize(direction);

					// �÷��̾��� ���⺤�� ������Ʈ
					_playerComp->_directionVector = direction;

					// �÷��̾� �̵�
					DirectX::XMVECTOR newPosition = DirectX::XMVectorAdd(currentPosVec, DirectX::XMVectorScale(direction, speed * _playerComp->_playerInfo._speedScale));
					_transform->SetPosition(newPosition.m128_f32[0], 0, newPosition.m128_f32[2]);

					// ī�޶� �̵�
					DirectX::XMFLOAT3 cameraPos(_tempX + newPosition.m128_f32[0], _tempY, _tempZ + newPosition.m128_f32[2]);
					SceneManager::GetInstance().GetCurrentScene()->GetMainCamera()->GetComponent<Transform>()->SetPosition(cameraPos.x, cameraPos.y, cameraPos.z);
				}
				else
				{
					int a = 10;

				}
			}

			
		}
	}
}

//void KunrealEngine::PlayerMove::NavigationMove(DirectX::XMFLOAT3 targetPos, float speed)
//{
//	navi->SetSEpos(_transform->GetPosition().x, _transform->GetPosition().y, _transform->GetPosition().z,
//		_targetPos.x, _targetPos.y, _targetPos.z
//	);
//
//	_stopover = navi->FindPath();
//
//	if (_targetPos.x != 0 && _targetPos.z != 0)
//	{
//		int a = 10;
//	}
//}

void KunrealEngine::PlayerMove::PlayerDash(DirectX::XMFLOAT3 targetPos, float speed)
{

	if (_isDash)
	{
		// ���ð� �̵������� ��
		// ��ų ������̰ų� �÷��̾ ����ȭ �Ǿ��� ���� �۵� ����
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
			this->_transform->SetPosition(newPosition.m128_f32[0], 0, newPosition.m128_f32[2]);

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
