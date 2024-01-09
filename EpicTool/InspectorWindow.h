#pragma once
#include <vector>
#include <string>
#include "IWindow.h"


namespace KunrealEngine
{
    class GameObject;
    class Transform;
    class MeshRenderer;
    class Compoent;

}


namespace EpicTool
{

	enum LightType
	{
		DirectionalLight,
		PointLight,
		SpotLight,
		None
	};


    class InspectorWindow : public IWindow
    {
    public:
        InspectorWindow();
        ~InspectorWindow();

    public:
        void Initialize();

        virtual void ShowWindow() override;
        virtual void ShowWindow(bool* _open, std::vector<Object>& object) override;

        void ShowWindow(int& selectedObjectIndex);

        template<typename PieceType> // Ư��ȭ�� ���� ������Ʈ�� ������ ����
        void DrawComponentPiece(PieceType& data, const std::string name); // ������ ����� �ΰ� ���� ����

        // ��ȭ���� �� �����δ� �ϴ� ���ø��� ���� �⺻ ���븦 ��� �����صΰ� �װ��� ������ ����� ���� ���ٰ� �ߴ�
        template<typename ComponentType>
        void DrawComponentInfo(ComponentType* instance);

        void DeleteComponent(KunrealEngine::Component* instance);

        void SetSelectObject(); // ���õ� ������Ʈ�� ������ �� ���

        void IsCheckItem(bool& Item);

        void SetMeshObjectEditor(std::vector<std::string> & meshList, int selectedItem);
      
        void ComboMeshControl(std::vector<std::string> list, std::vector<std::string> listEditor, int selected, const char* name);

        void ComboLightControl(std::vector<std::string> list, std::vector<std::string> listEditor, int selected);

    private:
        void ListToRemove(std::vector<std::string>& list, std::vector<std::string>& listEditor, const std::string stringToRemove);

        void ListStrToRemove(std::string& listStr , const std::string removeString);
    private:
        std::vector<KunrealEngine::GameObject*> _gameObjectlist;

        std::vector<KunrealEngine::Component*> _compoenetList;

        std::vector<std::string> _lightList;

        std::string _lightSelect;

        int _selectedObjectIndex;

        int _selectedObjectIndex2;


    private:
        // ������Ʈ�� ���� ���� ���� ����
        // �ش� ������ ���� ������Ʈ�� �θ���
        KunrealEngine::Transform* _tranform;

        KunrealEngine::MeshRenderer* _meshRenderer;

        KunrealEngine::Camera* _camera;

        KunrealEngine::Light* _light;

        bool _meshState;

        bool _cameraFix; // ī�޶�

        

        //int _selectedMesh = -1; // �޽� ������ ������ ����

        int _selectedNormal;


    private:
        // �޺����� ������ �κ��� �Ǵ��ϴ� ����
        int _comboMeshSelect;

        int _comboNormalSelect; 

        int _comboDiffuseSelect;

        int _comboLightSelect;

       std::string _selectObjectName;
        int _selectObjectNumber;

        // ������Ʈ�� Ȱ��ȭ�� ������ ����
        bool _isObjectActive;

        bool _isMeshRenderActive;

        bool _isLightActive;

        bool _isCameraActive;

    private:
        // �׷��Ƚ����� �޾ƿ� ����� ����
        std::vector<std::string> _meshList;
          
        std::vector<std::string> _meshListEditor;

        std::vector<std::string> _textureList;

        std::vector<std::string> _TextureListEditor;

        const std::string _meshStringToRemove = "Resources/ASEFile/";

        const std::string _textureStringToRemove = "Resources/Textures/";
    
        const std::string _directionalLight = "DirectionalLight";

        const std::string _pointLight = "PointLight";

        const std::string _spotLight = "SpotLight";

    private:

       float _ambient[4];
       float _diffuse[4];
       float _specular[4];
       float _direction[3];

       float _pointAmbient[4];
       float _pointDiffuse[4];
       float _pointSpecular[4];
       float _pointRange;

       bool _lightGet;
    };
}
