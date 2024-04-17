#pragma once
#include "IWindow.h"



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

	private:
		float AgentHeight;
		float AgentRadius;
		float AgentMaxClimb;
		float AgentMaxSlope;
	};
}
