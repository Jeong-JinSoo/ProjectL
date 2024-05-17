#include "TransparentMesh.h"
#include "GameObject.h"
#include "Transform.h"
#include "GraphicsSystem.h"
#include "TimeManager.h"

KunrealEngine::TransparentMesh::TransparentMesh()
	:_transform(nullptr), _tMesh(nullptr)
	, _objectName(), _textureName(), _transparency(1.0f), _isCircle(false), _timer(0.0f)
{

}

KunrealEngine::TransparentMesh::~TransparentMesh()
{

}

void KunrealEngine::TransparentMesh::Initialize()
{
	this->_transform = this->GetOwner()->GetComponent<Transform>();
}

void KunrealEngine::TransparentMesh::Release()
{
	if (this->_tMesh != nullptr)
	{
		this->_tMesh->Delete();
	}
}

void KunrealEngine::TransparentMesh::FixedUpdate()
{

}

void KunrealEngine::TransparentMesh::Update()
{
	if (GetMeshStatus())
	{
		this->_tMesh->SetTransform(_transform->GetWorldTM());
		auto condition = this->_tMesh->RenderWithTimer(TimeManager::GetInstance().GetDeltaTime(), _timer);
	}
}

void KunrealEngine::TransparentMesh::LateUpdate()
{

}

void KunrealEngine::TransparentMesh::OnTriggerEnter()
{

}

void KunrealEngine::TransparentMesh::OnTriggerStay()
{

}

void KunrealEngine::TransparentMesh::OnTriggerExit()
{

}

void KunrealEngine::TransparentMesh::SetActive(bool active)
{
	this->_isActivated = active;
}

void KunrealEngine::TransparentMesh::CreateTMesh(const std::string& objectName, const std::string& textureName, float transparency, bool isCircle)
{
	this->_tMesh = GRAPHICS->CreateTransParentMesh(objectName, textureName, transparency, isCircle);
	this->_tMesh->SetRenderType(0);		// �⺻��

	this->_objectName = objectName;
	this->_textureName = textureName;
	this->_transparency = transparency;
	this->_isCircle = isCircle;
}

bool KunrealEngine::TransparentMesh::GetMeshStatus()
{
	if (_tMesh != nullptr)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void KunrealEngine::TransparentMesh::SetTexture(const std::string& textureName)
{
	if (this->_tMesh != nullptr)
	{
		this->_tMesh->SetTexture(textureName);
		this->_textureName = textureName;
	}
}

void KunrealEngine::TransparentMesh::SetRenderType(unsigned int index)
{
	// 0�� : �⺻
	// 1�� : ��->��� �׸� 
	// 2�� : �Ʒ� -> ���� �׸� 
	// 3�� �߾� -> �ܰ����� �׸�
	// 4�� �߾� -> �ܰ����� ����ä�� ä���� (circle�϶��� ����� ����)

	if (this->_tMesh != nullptr)
	{
		this->_tMesh->SetRenderType(index);
	}
}

void KunrealEngine::TransparentMesh::SetTimer(float time)
{
	this->_timer = time;
}

void KunrealEngine::TransparentMesh::Reset()
{
	// tMesh �� ����� �̰� �ҷ��ٴ°� ��ü�� �����ϱ� ����ó���� ����
	this->_tMesh->Reset();
}