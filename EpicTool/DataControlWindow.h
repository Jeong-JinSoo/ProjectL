#pragma once
#include "IWindow.h"
#include <vector>

#include <nfd.h>

/// <summary>
/// ���� ���� ���� ����â�� ���
/// ����� �Բ� �ٸ� â�� ������ �����ϰ� �� ��
/// 2023.10.19 �輺��
/// </summary>

class FileSave;


namespace EpicTool
{
    class Serialize;

    class DataControlWindow : public IWindow
    {
    public:
        DataControlWindow();
        ~DataControlWindow();

    public:

        virtual void ShowWindow() override;                                // ������ â�� ����ϴ� �Լ�

        virtual void ShowWindow(bool* _open, std::vector<Object>& object) override;   // ������ü�� �ٸ� �Լ����� ����

        void ShowWindow(bool& close);
        void Initialize(); // �ʱ�ȭ, ���� �̱���

        // ������ ���
        void SaveToFile(const std::string& filePath);

    private:
        //void SaveButton();


    private:
        FileSave* _fileSave; // ���� ����� ��ü
        Serialize* _serialize; // ���� ����� ��ü
        std::string _saveFilePath; // ������ ��� ���� , �ʱ�ȭ �������ƺ�

        // �� ����â�� �����ϴ� ������
        bool _show_save_editor;
        bool _save_Scene;
        bool _new_Scene;
        bool _opt_padding;

    };
}
