#pragma once
#include "IWindow.h"


/// <summary>
/// �׷��Ƚ� �������� ���� ȭ���� ��� â
/// 2023.10.19 �輺��
/// </summary>
namespace EpicTool
{
    class GraphicWindow : public IWindow
    {
    public:
        GraphicWindow();
        ~GraphicWindow();

    public:

        virtual void ShowWindow() override; // ������ â�� ����ϴ� �Լ�
        // virtual void ShowWindow(bool* p_open, std::vector<Object> object) override;
        virtual void ShowWindow(bool* _open, std::vector<Object>& object) override; // ��ɾ���

        void ShowWindow(void* _texture);
    };
}
