
#include "imgui.h"
//#include "FileSave.h"
#include <nlohmann/json.hpp> 
#include <iostream>
#include <fstream>
#include "DataControlWindow.h"
#include "KunrealAPI.h"
#include "Serialize.h"
#include "Deserialize.h"
#include "ExportObj.h"

#include <string.h>
 // NFD�� �����ε� ���� lib ���尡 �ȵǾ��� Ȯ���ؾ���
// ���� �Ϸ� ���� ���������� �۵�

/// 2023.10.19 �輺��
/// �������� ///�� ������ �ȵǴ� ���� // �ӽ��ذ� ���� ���������� ���� ��
/// lib�� svn�� �ȿö�ͼ� ���尡 �ȵǴ� ������ �ִ�.
/// 

/// <summary>
/// 2024.03.14 �輺��
/// ������ ����� �ε带 ���� ����⸦ �����ϰ��� ������ 
/// �浹�� ���� ����
/// </summary>

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

void EpicTool::DataControlWindow::ShowWindow(bool& close) //���̺� ��ư �ٸ� ������ �̵� �ؾ��ҵ�
{
	std::string samplefilePath = _saveFilePath;

	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;



	

	if (_show_save_editor)
	{
		ImGui::Begin(" ", &_show_save_editor, ImGuiWindowFlags_MenuBar);

		if (ImGui::Button("Save"))
		{
			SaveToFile(samplefilePath);

		}
		ImGui::SameLine();
		if (ImGui::Button("ExportObj"))
		{
			_exportObj->ExportToObj("testObj.obj");
		}

		ImGui::SameLine(); // ���� ���ο� ��ġ

		if (ImGui::Button("Load"))
		{
			LoadToFile(samplefilePath);
		}

		ImGui::SameLine();

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
	_exportObj = new ExportObj();
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
			_serialize = new Serialize();  // �������� ����� ������?
			_serialize->SaveFile(_saveFilePath);
        }
    }
}

void EpicTool::DataControlWindow::LoadToFile(const std::string& filePath)
{
	nfdchar_t* outPath = NULL;
	nfdresult_t result = NFD_OpenDialog("json", NULL, &outPath);

	if (result == NFD_OKAY)
	{
		if (outPath)
		{
			std::string chosenPath = outPath;

			_loadFilePath = chosenPath;

			free(outPath);


			KunrealEngine::GetCurrentScene()->GetObjectList().clear();

			Deserialize* _deserialize = new Deserialize();
			_deserialize->Initialize(_loadFilePath);			
		}
	}
}

