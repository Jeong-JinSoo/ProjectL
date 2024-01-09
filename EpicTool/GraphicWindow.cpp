#include "imgui.h"
#include "GraphicWindow.h"
#include "KunrealAPI.h"

EpicTool::GraphicWindow::GraphicWindow()
	
{

}

EpicTool::GraphicWindow::~GraphicWindow()
{

}

void EpicTool::GraphicWindow::ShowWindow(void* _texture)
{
	// ���� ����
	IM_ASSERT(ImGui::GetCurrentContext() != NULL && "Missing Dear ImGui context. Refer to examples app!");


	ImGui::Begin("GraphicsWindow");//, ImGuiWindowFlags_NoMove);

	ImVec2 imageSize(1280, 720); // �̹��� ũ��
	
	ImGui::Image(_texture, imageSize); // �ؽ�ó�� �̹����� ���

	ImGuiIO& io = ImGui::GetIO();
	auto _fps = io.Framerate;
	ImGui::Text("FPS: %.1f", _fps);

	ImGui::End();
	
}

void EpicTool::GraphicWindow::ShowWindow()
{
    
}

void EpicTool::GraphicWindow::ShowWindow(bool* _open, std::vector<Object>& object)
{

}
