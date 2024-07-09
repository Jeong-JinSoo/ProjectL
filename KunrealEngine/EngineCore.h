#pragma once
#include <windows.h>
#include "CommonHeader.h"

//
#include <vector>
//

/// <summary>
/// ���ӿ����� �ھ� �κ�
/// ���ӿ��� ������ �ʿ��� �������� �κ��� �ʱ�ȭ �� ����
/// 
/// �̰�ȣ
/// </summary>

/// <summary>
/// �׷��Ƚ� ���� ���漱�� �κ�
/// </summary>

namespace GInterface
{
	class GraphicsInterface;
	class GraphicsImage;
}

namespace DirectX
{
	struct XMFLOAT3;
	struct XMFLOAT2;
}

namespace KunrealEngine
{
	class SceneManager;
	class GameObject;

	struct Point3D
	{
		float x, y, z;
	};

	class _DECLSPEC EngineCore
	{
	/// private���� ���߿� �ٲ�
	public:
		EngineCore();
		~EngineCore();

	public:
		void Initialize(HWND hwnd, HINSTANCE hInstance, int screenWidth, int screenHeight);
		void Release();

	private:
		void Update();
		void FixedUpdate();
		void LateUpdate();

	public:
		void UpdateAll();

		// �׷��Ƚ� ���� ������ ���� ������ ����
		void Render();

		void SetEditorMousePos(POINT position);


	public:
		// ���� �׽�Ʈ�Ҷ� ����� �� ����� �Լ� ���� �İ� ������ �� ���� ��
		void PlayGround();
		void CheckMousePosition();
		void MakeObstacle();
		void RemoveObstacle();

		void CreateTitleScene();
	
	private: // ��ƼŬ �׽�Ʈ
		void ParticleTest();
		DirectX::XMFLOAT3 Bezier(DirectX::XMFLOAT3 startPoint, DirectX::XMFLOAT3 p1, DirectX::XMFLOAT3 p2, DirectX::XMFLOAT3 endPoint, float t);
		DirectX::XMFLOAT2 BezierPoint2D(DirectX::XMFLOAT2 startPoint, DirectX::XMFLOAT2 p1, DirectX::XMFLOAT2 p2, DirectX::XMFLOAT2 endPoint, float t);
		std::vector<DirectX::XMFLOAT3> BezierSetting(GameObject* bezierObject);

	private:
		std::vector<GameObject*> _bezierObjectList;
		std::vector<std::vector<DirectX::XMFLOAT3>> _bezierPointsList;
		std::vector<std::vector<DirectX::XMFLOAT3>> _bezierSwordSoulPointsList;
		std::vector<DirectX::XMFLOAT3> _particlePointList;
		std::vector<DirectX::XMFLOAT3> _particleSwordSoulPointList;
		std::vector<DirectX::XMFLOAT2> _particleTelepotyPointList;
		std::vector<float> _timeCountList;
		std::vector<KunrealEngine::GameObject*> _particleTeleportList;

		float _timeCount;
		float _timeCountPlayerR;
		bool _isSettingTimer;
		bool _isBezierStartSetting;
		bool _isBezierBoomSetting;
		bool _isBezierTeleportSetting;
			// �������
	private:
		// �׷��Ƚ� ���� ��ü�� �� �κ�	
		GInterface::GraphicsInterface* _gInterface;

		/// �ӽÿ� �̹��� ������Ʈ
		GInterface::GraphicsImage* cursorimage;

		void UpdateParticleTest();

		void MapParticleSetting();

	private:
		// �ΰ��� ���콺 ������
		int _ingameMouseX = 0;
		int _ingameMouseY = 0;

		//�ӽÿ� ������ ���콺 ������
		POINT _editorMousepos = {-1, -1};

		POINT _finalMousePosition = { 0,0 };

		bool _isEditor;			// ������ ��������
	};
}

