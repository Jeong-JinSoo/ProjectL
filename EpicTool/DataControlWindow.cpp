
#include "imgui.h"
#include "FileSave.h"
#include <nlohmann/json.hpp> 
#include <iostream>
#include <fstream>
#include "DataControlWindow.h"
#include "KunrealAPI.h"

#include <string.h>
 // NFD�� �����ε� ���� lib ���尡 �ȵǾ��� Ȯ���ؾ���
// ���� �Ϸ� ���� ���������� �۵�

/// 2023.10.19 �輺��
/// �������� ///�� ������ �ȵǴ� ���� // �ӽ��ذ� ���� ���������� ���� ��
/// lib�� svn�� �ȿö�ͼ� ���尡 �ȵǴ� ������ �ִ�.
/// 


EpicTool::DataControlWindow::DataControlWindow()
    :_show_save_editor(true), _save_Scene(true), _new_Scene(true), _opt_padding(false), _fileSave(nullptr)
{

}

EpicTool::DataControlWindow::~DataControlWindow()
{

}

void EpicTool::DataControlWindow::ShowWindow()
{

}

void EpicTool::DataControlWindow::ShowWindow(bool& close)
{
	std::string samplefilePath = _saveFilePath;

	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

	_fileSave = nullptr;
	if (_fileSave == nullptr)
	{
		_fileSave = new FileSave();
	}

	if (_show_save_editor)
	{
		ImGui::Begin(" ", &_show_save_editor, ImGuiWindowFlags_MenuBar);

		if (ImGui::Button("Save"))
		{
			SaveToFile(samplefilePath);

		}
		ImGui::SameLine(); // ���� ���ο� ��ġ

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("New Scene", NULL, &_new_Scene))
				{

				}
				if (ImGui::MenuItem("Save Scene", NULL, &_save_Scene))
				{

				}

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Window"))
			{
				if (ImGui::MenuItem("Inspector"), NULL, &_save_Scene)  // ���� �ӽ÷� �־�� window���� �����Ҽ� �ִ� â��
					// �����츦 �����ϴ� ������ ������ ���� �����ͼ� ������ ���� ������?
				{

				}
				if (ImGui::MenuItem("Hierarchy"), NULL, &_save_Scene)
				{

				}
				if (ImGui::MenuItem("GameWindow"), NULL, &_save_Scene)
				{

				}
				if (ImGui::MenuItem("GraphicsWindow"), NULL, &_save_Scene)
				{

				}
				if (ImGui::MenuItem("ResouceWindow"), NULL, &_save_Scene)
				{

				}

				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		ImGui::End();
	}
	else
	{
		close = true;
	}
}

void EpicTool::DataControlWindow::ShowWindow(bool* _open, std::vector<Object>& object)
{

}

void EpicTool::DataControlWindow::Initialize()
{
   
}
 
void EpicTool::DataControlWindow::SaveToFile(const std::string& filePath)
{
    nfdchar_t* outPath = NULL;
    nfdresult_t result = NFD_SaveDialog("json", "json", &outPath);

    if (result == NFD_OKAY) {
        if (outPath) {
            std::string chosenPath = outPath;
            if (chosenPath.rfind(".json") == std::string::npos)
            {
                chosenPath += ".json";
            }

            _saveFilePath = chosenPath;

            free(outPath);
            _fileSave->OnFileSave(_saveFilePath);
        }
    }
}


