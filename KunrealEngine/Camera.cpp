#include "Camera.h"
#include "GameObject.h"
#include "Transform.h"
#include "GraphicsSystem.h"
#include "SceneManager.h"
#include "Scene.h"

///
#include "InputSystem.h"
#include "TimeManager.h"

///

KunrealEngine::Camera::Camera()
	:_camera(nullptr), _transform(nullptr), _fixTarget(false) ,_targetPosition(), _prevPosition(), _prevRotation()
{

}

KunrealEngine::Camera::~Camera()
{

}

void KunrealEngine::Camera::Initialize()
{
	_camera = GRAPHICS->CreateCamera();
	_transform = GetOwner()->GetComponent<KunrealEngine::Transform>();
}

void KunrealEngine::Camera::Release()
{
	_camera->Delete();
}

void KunrealEngine::Camera::FixedUpdate()
{

}

void KunrealEngine::Camera::Update()
{
	if (this->GetOwner()->GetActivated())
	{
		if (!_fixTarget)
		{
			MoveCamera();
		}
		else if (_fixTarget)
		{
			MoveFixedCamera();
		}
	}

	///
	MoveToDebug();
	///
}

void KunrealEngine::Camera::LateUpdate()
{
	
}

void KunrealEngine::Camera::OnTriggerEnter()
{

}

void KunrealEngine::Camera::OnTriggerStay()
{

}

void KunrealEngine::Camera::OnTriggerExit()
{

}

/// ���� ����ī�޶� ������ ����� ����
void KunrealEngine::Camera::SetActive(bool active)
{
	this->_isActivated = active;
}

GInterface::GraphicsCamera* KunrealEngine::Camera::GetCamera()
{
	return this->_camera;
}

void KunrealEngine::Camera::SetCameraPosition(float posX, float posY, float posZ)
{
	_transform->SetPosition(posX, posY, posZ);
	_camera->SetCameraPosition(_transform->GetPosition());

	_prevPosition = _transform->GetPosition();
}

void KunrealEngine::Camera::SetTargetPosition(float posX, float posY, float posZ)
{
	DirectX::XMFLOAT3 targetPos(posX, posY, posZ);

	_targetPosition = targetPos;
	_camera->SetTargetPosition(targetPos);
}

void KunrealEngine::Camera::RotateCamera(float pitch, float yaw)
{
	DirectX::XMFLOAT2 angle(pitch, yaw);

	_camera->RotateCamera(angle);
}

void KunrealEngine::Camera::CameraStrafe(float deltaTime)
{
	_camera->Strafe(deltaTime);
}

void KunrealEngine::Camera::CameraWalk(float deltaTime)
{
	_camera->Walk(deltaTime);
}

void KunrealEngine::Camera::CameraUpDown(float deltaTime)
{
	_camera->UpDown(deltaTime);
}

void KunrealEngine::Camera::MoveCamera()
{
	_camera->SetCameraPosition(_transform->GetPosition());

	//_camera->Strafe(_transform->GetPosition().x - _prevPosition.x);
	//_prevPosition.x = _transform->GetPosition().x;
	//
	//_camera->UpDown(_transform->GetPosition().y - _prevPosition.y);
	//_prevPosition.y = _transform->GetPosition().y;
	//
	//_camera->Walk(_transform->GetPosition().z - _prevPosition.z);
	//_prevPosition.z = _transform->GetPosition().z;

	RotateCamera(_transform->GetRotation().x - _prevRotation.x, _transform->GetRotation().y -_prevRotation.y);
	_prevRotation = _transform->GetRotation();
}

void KunrealEngine::Camera::MoveFixedCamera()
{
	_camera->SetCameraPosition(_transform->GetPosition());
}

void KunrealEngine::Camera::UnfixCamera()
{
	if (this->_fixTarget)
	{
		this->_fixTarget = false;
	}
}

void KunrealEngine::Camera::FixCamera()
{
	if (!this->_fixTarget)
	{
		this->_fixTarget = true;
	}

	SetTargetPosition(_targetPosition.x, _targetPosition.y, _targetPosition.z);
}

void KunrealEngine::Camera::SetMainCamera()
{
	GRAPHICS->SetMainCamera(_camera);
	SceneManager::GetInstance().GetCurrentScene()->SetMainCamera(this->GetOwner());
}

void KunrealEngine::Camera::MoveToDebug()
{
	float x = _transform->GetPosition().x;
	float y = _transform->GetPosition().y;
	float z = _transform->GetPosition().z;

	if (InputSystem::GetInstance()->KeyInput(KEY::A))
	{
		SetCameraPosition(-20.0f * TimeManager::GetInstance().GetDeltaTime() + x, y, z);
	}
	else if (InputSystem::GetInstance()->KeyInput(KEY::D))
	{
		SetCameraPosition(20.0f * TimeManager::GetInstance().GetDeltaTime() + x, y, z);
	}
	else if (InputSystem::GetInstance()->KeyInput(KEY::W))
	{
		SetCameraPosition(x, y, z + 20.0f * TimeManager::GetInstance().GetDeltaTime());
	}
	else if (InputSystem::GetInstance()->KeyInput(KEY::S))
	{
		SetCameraPosition(x, y, z - 20.0f * TimeManager::GetInstance().GetDeltaTime());
	}
	else if (InputSystem::GetInstance()->KeyInput(KEY::Q))
	{
		SetCameraPosition(x, y - 20.0f * TimeManager::GetInstance().GetDeltaTime(), z);
	}
	else if (InputSystem::GetInstance()->KeyInput(KEY::E))
	{
		SetCameraPosition(x, y + 20.0f * TimeManager::GetInstance().GetDeltaTime(), z);
	}
	else if (InputSystem::GetInstance()->KeyInput(KEY::R))
	{
		GRAPHICS->GetMainCamera()->RotateCamera({ 0, -20.0f * TimeManager::GetInstance().GetDeltaTime() });
	}
	else if (InputSystem::GetInstance()->KeyInput(KEY::T))
	{
		GRAPHICS->GetMainCamera()->RotateCamera({ 0, 20.0f * TimeManager::GetInstance().GetDeltaTime() });
	}
}
