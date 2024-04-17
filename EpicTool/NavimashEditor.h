#pragma once
#include "IWindow.h"


namespace KunrealEngine
{
	class Navigation;
}

struct ImDrawList;
struct ImVec2;

namespace EpicTool
{

	class NavimashEditor : public IWindow
	{
	public:
		NavimashEditor();
		~NavimashEditor();

		virtual void ShowWindow() override; // ������ â�� ����ϴ� �Լ�
		// virtual void ShowWindow(bool* p_open, std::vector<Object> object) override;
		virtual void ShowWindow(bool* _open, std::vector<Object>& object) override; // ��ɾ���

		void Initialize();
		void DrawCylinder(ImDrawList* drawList, ImVec2 windowPos, ImVec2 windowSize, float centerX, float centerY, float radius, float height);

	private:
		int _naviIndex;

		float _agentHeight;
		float _agentRadius;
		float _agentMaxClimb;
		float _agentMaxSlope;

		KunrealEngine::Navigation* _navimashEditor;
	};
}
