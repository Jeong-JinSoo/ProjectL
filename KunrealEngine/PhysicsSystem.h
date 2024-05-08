#pragma once

#include <unordered_map>
#include "CommonHeader.h"
#include "../KunrealMath/MathHeaders.h"
#pragma comment(lib,"../Bin/x64/Debug/KunrealMath.lib")

#pragma warning(disable : 26451 26495 6297 33010)
#include <physx/PxPhysics.h>
#include <physx/PxPhysicsAPI.h>
#include <physx/extensions/PxDefaultAllocator.h>
#include <physx/extensions/PxDefaultErrorCallback.h>
#pragma warning(default : 26451 26495 6297 33010)

namespace KunrealEngine
{
	class GameObject;
	class BoxCollider;

	class _DECLSPEC PhysicsSystem : public physx::PxSimulationEventCallback
	{
		// actor�� userData�� �߰��� ����ü
		struct PhysicsUserData
		{
			GameObject* gameObject;
			physx::PxShape* shape;
		};

	private:
		PhysicsSystem();

		// �̱���
	public:
		static PhysicsSystem& GetInstance()
		{
			static PhysicsSystem _instance;
			return _instance;
		}

	public:
		void Initialize();
		void Release();
		void FixedUpdate();

	public:
		// eventcallback
		virtual void onContact(const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs, physx::PxU32 nbPairs) override;
		virtual void onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count) override;
		virtual void onConstraintBreak(physx::PxConstraintInfo*, physx::PxU32) override;
		virtual void onWake(physx::PxActor**, physx::PxU32) override;
		virtual void onSleep(physx::PxActor**, physx::PxU32) override;
		virtual void onAdvance(const physx::PxRigidBody* const* bodyBuffer, const physx::PxTransform * poseBuffer, const physx::PxU32 count) override;

	public:
		void CreatePhysXScene();
		physx::PxScene* GetPxScene();

	public:
		// �����̴� ��ü�� �ο��� box collider
		void CreateDynamicBoxCollider(BoxCollider* collider);

		// �������� �ʴ� ��ü�� �ο��� box collider
		void CreateStaticBoxCollider(BoxCollider* collider);

		// sphere collider
		void CreateDynamicSphereCollider();

		// physx ��ü�� ũ�⸦ BoxCollider�� ũ�⿡ �°�
		void SetBoxSize(BoxCollider* collider);

		// Dynamic actor�� ������Ʈ
		void UpdateDynamics();
		
		// Static actor�� ������Ʈ
		void UpdateStatics();

		void TestFunc();			/// ����� �� ����

	private:
		std::vector<physx::PxRigidDynamic*>	_rigidDynamics;
		std::vector<physx::PxRigidStatic*>	_rigidStatics;
		std::vector<physx::PxFixedJoint*>	_fixedJoints;

		// ������Ʈ���� ������ �ִ� Transform ������ ���������� �Ѱ��ֱ� ����
		// actor�� userData ����� ���� �� �� ���⵵�ѵ�.. �ϴ� �׽�Ʈ��
		std::unordered_map<BoxCollider*, physx::PxRigidDynamic*> _dynamicMap;
		std::unordered_map<BoxCollider*, physx::PxRigidStatic*> _staticMap;

	private:
		physx::PxDefaultAllocator			_allocator;
		physx::PxDefaultErrorCallback		_errorCallback;
		physx::PxTolerancesScale			_torlerance;
		physx::PxFoundation*				_foundation;
		physx::PxPhysics*					_physics;
		physx::PxScene*						_pxScene;
		physx::PxDefaultCpuDispatcher*		_dispatcher;
		physx::PxMaterial*					_material;
		physx::PxPvd*						_pvd;


	// eventcallback �����Լ�
	private:
		bool IsTrigger(const physx::PxFilterData& data);

		static physx::PxFilterFlags TriggerUsingFilterShader(physx::PxFilterObjectAttributes /*attributes0*/, physx::PxFilterData filterData0,
			physx::PxFilterObjectAttributes /*attributes1*/, physx::PxFilterData filterData1,
			physx::PxPairFlags& pairFlags, const void* /*constantBlock*/, physx::PxU32 /*constantBlockSize*/);
	
	private:
		// eventcallback���κ��� ���� actor�� � collider�� �����ϴ���
		BoxCollider* GetColliderFromDynamic(const physx::PxRigidDynamic* dynamic);
	};
}