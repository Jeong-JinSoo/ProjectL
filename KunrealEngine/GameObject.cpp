#include "GameObject.h"
#include "Component.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Transform.h"

#include "../KunrealMath/MathHeaders.h"

#pragma comment(lib,"../Bin/x64/Debug/KunrealMath.lib")

KunrealEngine::GameObject::GameObject()
	:_isActivated(true), _objectName(""), _layer(0), _parent(nullptr), _transform(nullptr)
{
	SceneManager& smInstance = SceneManager::GetInstance();
	int objectCount = 0;
	_objectName = "GameObject";		// �ƹ� �̸��� ���������� �ʾ��� ��� GameObject��� �̸����� ����

	for (auto object : smInstance.GetCurrentScene()->GetObjectList())
	{
		if (_objectName == object->GetObjectName())		// �ߺ� Ȯ��
		{
			objectCount++;			// �̸��� �ߺ��� ��� �ڿ� ��ȣ�� ���ڸ� �ٿ� �ٸ� �̸����� ����
			_objectName = _objectName + " (" + std::to_string(objectCount) + ")";
		}
	}
}

KunrealEngine::GameObject::GameObject(std::string name)
	:_isActivated(true), _objectName(name), _layer(0), _parent(nullptr), _transform(nullptr)
{
	SceneManager& smInstance = SceneManager::GetInstance();
	int objectCount = 0;

	// �ߺ� üũ �˰����� ���� ����
	for (auto object : smInstance.GetCurrentScene()->GetObjectList())
	{
		if (_objectName == object->GetObjectName())
		{
			objectCount++;
			_objectName = _objectName + " (" + std::to_string(objectCount) + ")";
		}
	}
}

KunrealEngine::GameObject::~GameObject()
{

}

void KunrealEngine::GameObject::Initialize()
{
	// ������Ʈ�� �������ڸ��� Ʈ�������� ���� �־���
	AddComponent<Transform>();
	_transform = GetComponent<Transform>();
}

void KunrealEngine::GameObject::Awake()
{

}

void KunrealEngine::GameObject::OnEnable()
{

}

void KunrealEngine::GameObject::Start()
{

}

void KunrealEngine::GameObject::FixedUpdate()
{
	/// if�ȿ� �ݺ����ȿ� if��.. ��ȿ�����̴�
	/// ������Ʈ���� ���¸� üũ �� ������ �����̳ʷ� ������ �ݺ����� ������ ��� ������
	if (_isActivated)
	{
		for (auto& components : _componentContainer)
		{
			if (components->GetActivated())
			{
				components->FixedUpdate();
			}
		}
	}
}

void KunrealEngine::GameObject::Update()
{
	if (_isActivated)
	{
		for (auto& components : _componentContainer)
		{
			if (components->GetActivated()) components->Update();
		}
	}
}

void KunrealEngine::GameObject::LateUpdate()
{
	if (_isActivated)
	{
		for (auto& components : _componentContainer)
		{
			if (components->GetActivated()) components->LateUpdate();
		}
	}
}

void KunrealEngine::GameObject::OnDisable()
{

}

void KunrealEngine::GameObject::Finalize()
{
	ClearComponent();
}

std::string KunrealEngine::GameObject::GetObjectName()
{
	return this->_objectName;
}

void KunrealEngine::GameObject::SetObjectName(const std::string& name)
{
	this->_objectName = name;
}

void KunrealEngine::GameObject::SetParent(GameObject* obj)
{
	this->_parent = obj;
}

KunrealEngine::GameObject* KunrealEngine::GameObject::GetParent()
{
	if (_parent != nullptr)
	{
		return _parent;
	}
	else
	{
		return nullptr;
	}
}

void KunrealEngine::GameObject::SetActive(bool active)
{
	this->_isActivated = active;

	for (auto& components : _componentContainer)
	{
		components->SetActive(active);
	}
}

bool KunrealEngine::GameObject::GetActivated()
{
	return this->_isActivated;
}

void KunrealEngine::GameObject::SetLayer(int layer)
{
	this->_layer = layer;
}

int KunrealEngine::GameObject::GetLayer() const
{
	return this->_layer;
}

void KunrealEngine::GameObject::SetTag(const std::string& tag)
{
	this->_tag = tag;
}

std::string KunrealEngine::GameObject::GetTag()
{
	return this->_tag;
}

std::string KunrealEngine::GameObject::GetOriginalTypeName(std::string name)
{
	std::string typeName = name;

	const std::string classPrefix = "class ";
	const std::string structPrefix = "struct ";

	if (typeName.substr(0, classPrefix.size()) == classPrefix)			// Ŭ�����ΰ�
	{
		typeName = typeName.substr(classPrefix.size());
	}
	else if (typeName.substr(0, structPrefix.size()) == structPrefix)	// ����ü�ΰ�
	{
		typeName = typeName.substr(structPrefix.size());
	}

	size_t colon = typeName.rfind("::");								// ���ӽ����̽� ã�Ƽ� ����
	if (colon != std::string::npos)
	{
		typeName = typeName.substr(colon + 2);
	}

	return typeName;
}

void KunrealEngine::GameObject::DeleteComponent(Component* component)
{
	auto iter = find(_componentContainer.begin(), _componentContainer.end(), component);

	if (iter != _componentContainer.end())			// ã�°� ������
	{
		(*iter)->Finalize();						// ������Ʈ���� �����Ұ� ������ ���ְ�
		delete* iter;								// �ش� ������Ʈ�� ����
		_componentContainer.erase(iter);			// �����̳ʿ��� �����
	}
}

void KunrealEngine::GameObject::ClearComponent()
{
	for (Component* component : _componentContainer)
	{
		component->Finalize();						// ������ ������ ������ ���� �������ְ�
		delete component;							// clear�ϱ����� delete �� �������
	}

	_componentContainer.clear();
}

std::vector<KunrealEngine::Component*> KunrealEngine::GameObject::GetComponentList()
{
	return this->_componentContainer;
}
