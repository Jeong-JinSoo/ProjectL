#pragma once
#include <vector>
#include "IWindow.h"

/// <summary>
/// ���̾��Ű, �ν����͸� ����
/// �ٸ� â�� ��� �������� �����
/// 2023.10.19 �輺��
/// </summary>

/// <summary>
/// ���̾��Ű�� �ν����͸� ���� �и��Ͽ���
/// </summary>

namespace KunrealEngine
{
    class GameObject;
}


namespace EpicTool
{
    class InspectorWindow;
    class HierarchyWindow;

    class ShowWindowManager : public IWindow
    {
    public:
        ShowWindowManager();
        ~ShowWindowManager();

    public:

        void Initialize();

        virtual void ShowWindow() override; // â�� ���

        virtual void ShowWindow(bool* _open, std::vector<Object>& object) override;

    private:

        int _selectedObjectIndex;  // ���� ������������� �ϴ� �ӽ÷�

        // �� �������� ��ü ����
        InspectorWindow* _inspector;
        HierarchyWindow* _Hierarchy;
    };
}
