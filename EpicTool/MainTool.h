#pragma once
#include <windows.h>
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

struct ImVec4;

struct ImGuiIO;
/// <summary>
/// ���� ����� �и����̴�
/// 2023_10_23 �輺��
/// </summary>

namespace KunrealEngine
{
    class EngineCore;
}

namespace EpicTool
{
    class ShowWindowManager;
    class ImGuiLayer;
    class ResourceWindow;
    class DataControlWindow;
    class GraphicWindow;
    class GameWindow;

    class IWindow;

    class MainTool
    {
    public:
        MainTool();
       ~MainTool();
    public:
        HRESULT Initialize(); //  ������ �ʱ�ȭ �Ѵ�

        void Loop(); // �⺻ ����

        void Release(); // ����

        

    private:
        // ���� ���� �Լ�

        bool CreateDeviceD3D(HWND hWnd); // �� �κ��� �׷��Ƚ� ���� ����

        void CleanupDeviceD3D();

        bool CreateSwapChain();

        void CreateRenderTarget();

        void CleanupRenderTarget();

        void CleanupSwapChain();

        void ShowWindowFunction(); // ������ ��� �κ��� ��Ƶ�

        void LoopImGuiRender(ImGuiIO& io);


        void UpdateAll();   // ���� ������ �κ��� ����
        void RenderAll(ImGuiIO& io);
    private:
        // ��ü���� ����
        ResourceWindow* _resourceWindow;
        GraphicWindow* _graphicWindow;
        DataControlWindow* _saveloadWindow;
        GameWindow* _gameWindow;
        ShowWindowManager* _windowManager;
        


        // ����
        KunrealEngine::EngineCore* _core;

        // ������ Ȯ�ο� ����
        //float _fps;

        // ������ ���� ���� ����
        bool _toolClose;

    private:

     HWND _hwnd; // ��� �ӽ� �׷��Ƚ� ������
     HWND _eHwnd;

     HWND _console;

     WNDCLASSEXW wc;
     WNDCLASSEXW ewc;

	 ImVec4 _clearColor;

    private:
		int _selectedObjectIndex;  // ������Ʈ����Ʈ���� ������ �����ߴ��� ����
    };
}

