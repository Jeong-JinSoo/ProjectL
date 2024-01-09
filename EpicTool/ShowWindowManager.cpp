#include "ShowWindowManager.h"
#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "FileSave.h"
#include "KunrealAPI.h"
#include "SceneManager.h"
#include "InspectorWindow.h"
#include "HierarchyWindow.h"

EpicTool::ShowWindowManager::ShowWindowManager()
    :_selectedObjectIndex(-1), _inspector(nullptr), _Hierarchy(nullptr)
{

}

EpicTool::ShowWindowManager::~ShowWindowManager()
{

}



void EpicTool::ShowWindowManager::Initialize()
{
    _inspector = new InspectorWindow();
    _inspector->Initialize();
    _Hierarchy = new HierarchyWindow();
}

void EpicTool::ShowWindowManager::ShowWindow()
{ 
    std::vector<KunrealEngine::GameObject*> gameObjectlist = KunrealEngine::GetCurrentScene()->
        GetObjectList();

    IM_ASSERT(ImGui::GetCurrentContext() != NULL && "Missing Dear ImGui context. Refer to examples app!");

    ImGui::Begin("Hierarchy");  

    _Hierarchy->ShowWindow(_selectedObjectIndex);

    ImGui::End();
   
   
   // �ſ� �ϵ彺��� ���ø��� ���� �����غ���, ������Ʈ �κ� ������ ������Ʈ�� �����ϴ°� �ƴ�
                          // ������ �ʿ��Ұ��̴�.
    ImGui::Begin("inspector");

    _inspector->ShowWindow(_selectedObjectIndex);

    ImGui::End();
   
}



void EpicTool::ShowWindowManager::ShowWindow(bool* _open, std::vector<Object>& object)
{

}
