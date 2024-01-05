#pragma once
#include <dinput.h>
#include "CommonHeader.h"

#pragma comment(lib, "dinput8")
#pragma comment(lib, "dxguid")

// https://www.notion.so/KEY-CODE-LIST-3fe3e429fa224b77b8699b6568e052f4 ����

enum class KEY;

namespace KunrealEngine
{
	class _DECLSPEC InputSystem
	{
	private:
		InputSystem();
		~InputSystem();

	private:
		IDirectInput8* _input;
		IDirectInputDevice8* _keyboard;
		IDirectInputDevice8* _mouse;
		LPVOID _cKey;

		char _keybuffer[256] = {};		// �� �����ӿ� ���� Ű����
		char _prevkeybuffer[256] = {};	// ���� �����ӿ� ���� Ű����

		HWND _hWnd;

		DIMOUSESTATE _mouseState;
		BYTE _prevrgbButtons[4] = {};
		int _mouseWheelData;			// DIMOUSESTATE�� lz���� �� �Է°��̴�. 
		// �� ������ lz ���� ���ϴ� ������ ���� �󸶳� ���������� �� �� �ִ�.
		int _prevmouseWheelData;
		//int _screenHeight;
		//int _screenWidth;
		int _mouseX;
		int _mouseY;

	public:
		// �ν��Ͻ��� ������ �� �ִ� �޼���
		static InputSystem* GetInstance();

		// �Է½ý��� �ʱ�ȭ �Լ�
		bool Initialize(HINSTANCE instance, HWND hwnd, int screenHeight, int screenWidth);
		void Finalize();

		// �� �����Ӹ��� �۵�. ������ ����̽����� ������ ���������� �ƴ����� üũ. 
		void Update(float fTime);

		// �Էµ� ��ư�� boolŸ������ ��ȯ (���ȴ°� �ȴ��ȴ°�)
		bool WINAPI KeyInput(KEY keycode);
		bool WINAPI KeyUp(KEY keycode);
		bool WINAPI KeyDown(KEY keycode);

		// �Էµ� ���콺�� boolŸ������ ��ȯ (���ȴ°� �ȴ��ȴ°�)
		bool WINAPI MouseButtonInput(int button);
		bool WINAPI MouseButtonUp(int button);
		bool WINAPI MouseButtonDown(int button);

		// ���콺 �ٰ��� bool Ÿ������ ��ȯ
		bool WINAPI WheelUp();
		bool WINAPI WheelDown();

		// �Ű����� xpos, ypos�� ���콺�� ��ǥ�� �����ϴ� �Լ�.
		void GetMousePosition(int& xpos, int& ypos);

		// �Ű����� curState, prevState�� ���� ���簪, ���������Ӱ��� �����ϴ� �Լ�.
		void GetWheelState(int& curState, int& prevState);
		int GetWheelState();
	};
}