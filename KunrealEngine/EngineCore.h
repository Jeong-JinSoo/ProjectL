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
	
	private: // ��ƼŬ �׽�Ʈ
		void ParticleTest();
		Point3D Bezier(Point3D startPoint, Point3D p1, Point3D p2, Point3D endPoint, float t);
		void BezierSetting(GameObject* bezierObject);

	private:
		std::vector<GameObject*> _bezierObjectList;
		std::vector<std::vector<Point3D>> _bezierPointsList;
		std::vector<Point3D> _particlePointList;
		float _timeCount;
		bool _checkReverse;
		bool _isBezierStartSetting;
			// �������
	private:
		// �׷��Ƚ� ���� ��ü�� �� �κ�	
		GInterface::GraphicsInterface* _gInterface;



		/// �ӽÿ� �̹��� ������Ʈ
		GInterface::GraphicsImage* cursorimage;

	/// �ӽ÷� ��ŸŸ��
	public:
		float GetDeltaTime();

	private:
		// �ΰ��� ���콺 ������
		int _ingameMouseX = 0;
		int _ingameMouseY = 0;

		//�ӽÿ� ������ ���콺 ������
		POINT _editorMousepos = {-1, -1};

		POINT _finalMousePosition = { 0,0 };

		float _timeMan = 1.0f;

		bool _isEditor;			// ������ ��������
	};
}

