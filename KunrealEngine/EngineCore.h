#pragma once
#include <windows.h>
#include "CommonHeader.h"

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
		void ParticleTest();

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

