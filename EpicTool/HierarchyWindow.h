#pragma once
#include <vector>
#include "IWindow.h"

/// <summary>
/// �ڵ� �и� �۾���
/// ���̾��Ű �κ��� �����
/// </summary>

namespace KunrealEngine
{
    class GameObject;
}

namespace EpicTool
{

    class HierarchyWindow : public IWindow // �������̽��� �ʿ����� ��εȴ�
    {
    public:
        HierarchyWindow();
        ~HierarchyWindow();

    public:

        void Initialize();

        virtual void ShowWindow() override; // â�� ���
        virtual void ShowWindow(bool* p_open, std::vector<Object>& object) override;

        void ShowWindow(int& selectedObjectIndex);  // ���̾��Ű�� �ν����Ϳ����� ���

        void CreateObject(std::string objectName, int& count);

        void DeleteObject(std::vector<KunrealEngine::GameObject*>& gameObjectlist, int& selectedObjectIndex);

        void UpdateGameObject();  // ������Ʈ�� ������ �ٲ�

        //void RenderDragBox(int& selectedObjectIndex); // �巡�� �� �۵��� ���� 



    private:
        // �� ������Ʈ�� ����
        int _createEmptyCount;
        int _cubeCount;
        int _sphereCount;

        // ������Ʈ ����� ������ ����
		int _draggedIndex;
		int _dropTargetIndex;

        // imgui â�� ����ϱ� ���� ����
		float _hierarchyWindowX1;
		float _hierarchyWindowY1;
		float _hierarchyWindowX2;
		float _hierarchyWindowY2;

        //������Ʈ ����Ʈ
        std::vector<KunrealEngine::GameObject*> _gameObjectlist;

        // �巡�׽� �����ϴ� ������ �ڽ� ��ġ

    };
}

