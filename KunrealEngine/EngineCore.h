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
		void InitializeInputSystem();
		void Finalize();

	private:
		void Update();
		void FixedUpdate();
		void LateUpdate();

	public:
		void UpdateAll();

		// �׷��Ƚ� ���� ������ ���� ������ ����
		void Render();

		void* GetRenderingImage();

		void* GetDevice();

		void* GetDeviceContext();

		void* GetRenderTargetView();

	private:
		// �׷��Ƚ� ���� ��ü�� �� �κ�	
		GInterface::GraphicsInterface* _gInterface;

	/// �ӽ÷� ��ŸŸ��
	public:
		float GetDeltaTime();
	};
}

