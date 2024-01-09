#include <cassert>
#include "PhysicsSystem.h"
#include "TimeManager.h"
#include "GameObject.h"
#include "Transform.h"

KunrealEngine::PhysicsSystem::PhysicsSystem()
	:_foundation(nullptr), _physics(nullptr), _dispatcher(nullptr), _pxScene(nullptr),
	_material(nullptr), _pvd(nullptr)
{

}

void KunrealEngine::PhysicsSystem::Initialize()
{
	_foundation = PxCreateFoundation(PX_PHYSICS_VERSION, _allocator, _errorCallback);

	// visual debugger ����, ���ÿ� ����
	_pvd = PxCreatePvd(*_foundation);
	physx::PxPvdTransport* transport = physx::PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
	_pvd->connect(*transport, physx::PxPvdInstrumentationFlag::eALL);

	// ����, ����, ���� ���� ������ ���� ����						// _tolerance�� ���� �������� �ʾҴٸ�
	_physics = PxCreatePhysics(PX_PHYSICS_VERSION, *_foundation, physx::PxTolerancesScale(), true, _pvd);

	PxInitExtensions(*_physics, _pvd);

	// ���� ���� ����
	CreatePhysXScene();

	// ��Ƽ���� ����(����)
	_material = _physics->createMaterial(0.5f, 0.5f, 0.5f);

	/// ����� �ڵ� ���� �����
	// ���� �����
	physx::PxRigidStatic* groundPlane = physx::PxCreatePlane(*_physics, physx::PxPlane(0, 1, 0, 1), *_material);
	_pxScene->addActor(*groundPlane);

	

	// collider ���� �����.
	//MakeAllCollider();
}

void KunrealEngine::PhysicsSystem::Finalize()
{

}

void KunrealEngine::PhysicsSystem::FixedUpdate()
{
	_pxScene->simulate(TimeManager::GetInstance().GetDeltaTime());
	_pxScene->fetchResults(true);
}

void KunrealEngine::PhysicsSystem::CreatePhysXScene()
{
	// ���� ���� ����		// SceneDescription
	physx::PxSceneDesc sceneDesc(_physics->getTolerancesScale());
	sceneDesc.gravity = physx::PxVec3(0.f, -9.81f, 0.f);				// �߷� ����
	_dispatcher = physx::PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = _dispatcher;
	sceneDesc.filterShader = physx::PxDefaultSimulationFilterShader;
	_pxScene = _physics->createScene(sceneDesc);

	// Pvd�� ���� ������
	physx::PxPvdSceneClient* pvdClient = _pxScene->getScenePvdClient();
	if (pvdClient)
	{
		pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
		pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
		pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
	}
}

physx::PxScene* KunrealEngine::PhysicsSystem::GetPxScene()
{
	return _pxScene;
}


void KunrealEngine::PhysicsSystem::CreateBoxCollider(GameObject* obj)
{
	/// Collider �� BoxCollider ������Ʈ �����ϴ� �κ� �ʿ�

	//physx::PxShape* shape = _physics->createShape(physx::PxBoxGeometry(box->GetWidth() / 2, box->GetHeight() / 2, box->GetDepth() / 2), *_material);

	KunrealMath::Float3 pos = obj->GetComponent<KunrealEngine::Transform>()->GetPosition();
	physx::PxTransform localTm(physx::PxVec3(pos.x, pos.y, pos.z));
	physx::PxRigidDynamic* body = _physics->createRigidDynamic(localTm);
	//body->attachShape(*shape);
	body->setAngularDamping(0.5f);
	body->setLinearDamping(5.0f);

	//if (box->gameObject->objName == "player")
	{
		body->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_X, true);
		body->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y, true);
		body->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z, true);
	}
	//body->setMass(4.5f);
	//physx::PxRigidBodyExt::updateMassAndInertia(*body, 1.0f);
	//_scene->addActor(*body);

	//box->SetPhysXRigid(body);
	//body->userData = box;
	_rigidDynamics.push_back(body);

	//shape->release();
}

void KunrealEngine::PhysicsSystem::AddActorsToScene()
{
	
}
