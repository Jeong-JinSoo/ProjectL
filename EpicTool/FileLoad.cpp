#include "FileLoad.h"

#include "KunrealAPI.h"
#include "SceneManager.h"

EpicTool::FileLoad::FileLoad()
{

}

EpicTool::FileLoad::~FileLoad()
{

}
/// <summary>
/// ���÷������� ��κ� ��� ����
/// 2024.01.08
/// </summary>
void EpicTool::FileLoad::Initialize()  // �Ʒ� �κ��� ���Ŀ� �ݺ������� ȣ�� �� �� �ִ� �κ��̱⿡ �и��� ���� �ִ� , 
{
    ////json���� �߿� �ռ� �����ص� ������ �о��
    //std::ifstream inputFile("editor_stte.json");
    //if (inputFile.is_open()) // �ش� �κ� �������� ������ ����
    //{
    //    nlohmann::json jsonData;
    //    inputFile >> jsonData;
    //    inputFile.close();
    //    // json�� ���� ������Ʈ�� ������
    //    for (const auto& jsonItem : jsonData)
    //    {
    //        std::string sceneName = jsonItem["SceneName"];

    //        std::string objectName = jsonItem["ObjectName"];  // �̸� �����ֱ�

    //        std::string mainCamera = "MainCamera";

    //        {
    //        	////���� ���ϸ� ����� ����  // �ش�κ� ���� �߻� (�ּ�ó���ϸ� ���� ������Ʈ�� �ȸ���� �ߺ����� ����)
				//KunrealEngine::CreateScene(sceneName);
				//if (objectName != mainCamera)
				//{
				//	KunrealEngine::GetScene(sceneName)->CreateObject(objectName);
				//}

    //        	KunrealEngine::GameObject* object = KunrealEngine::GetCurrentScene()->GetGameObject(objectName); 
    //        	std::vector<KunrealEngine::GameObject*> objectIndex = KunrealEngine::GetCurrentScene()->
    //        		GetObjectList();

    //            if(object)
    //        	{    
    //        		// ������Ʈ�� �����͸� ������
    //        		// ������Ʈ�� �ִٸ� Ʈ�������� ������
    //        		KunrealEngine::Transform* trans = object->GetComponent<KunrealEngine::Transform>();
    //                
    //        		// json ���� ���� Transform�� ��������
    //        		trans->SetPosition(jsonItem["TransformData"]["position_x"], jsonItem["TransformData"]["position_y"], jsonItem["TransformData"]["position_z"]);
    //        		trans->SetRotation(jsonItem["TransformData"]["rotation_x"], jsonItem["TransformData"]["rotation_y"], jsonItem["TransformData"]["rotation_z"]);
    //        		trans->SetScale(jsonItem["TransformData"]["scale_x"], jsonItem["TransformData"]["scale_y"], jsonItem["TransformData"]["scale_z"]);

    //                // json���� ���� mesh component�� ��������

				//	auto meshRenderer = jsonItem["MeshData"].find("MeshRenderer");
				//	auto textureRenderer = jsonItem["MeshData"].find("TextureRenderer");

				//	if (meshRenderer != jsonItem["MeshData"].end() && !jsonItem["MeshData"]["MeshRenderer"].empty())
    //                {
				//		object->AddComponent<KunrealEngine::MeshRenderer>();
				//		KunrealEngine::MeshRenderer* mesh = object->GetComponent<KunrealEngine::MeshRenderer>();

				//		if (!jsonItem["MeshData"]["MeshRenderer"][0]["Mesh"].empty())
    //                    {
				//			if (!mesh->GetMeshStatus())
    //                        {
				//				std::string meshName = jsonItem["MeshData"]["MeshRenderer"][0]["Mesh"];
				//				mesh->SetMeshObject(meshName.c_str());

				//				if (jsonItem["MeshData"]["MeshRenderer"].size() > 1)
    //                            {
				//					mesh->SetActive(jsonItem["MeshData"]["MeshRenderer"][1]["RenderingState"]);
				//				}
				//			}
				//			else
    //                        {
				//				std::string meshName = jsonItem["MeshData"]["MeshRenderer"][0]["Mesh"];
				//				mesh->ChangeMeshObject(meshName.c_str());

				//				if (jsonItem["MeshData"]["MeshRenderer"].size() > 1)
    //                            {
				//					mesh->SetActive(jsonItem["MeshData"]["MeshRenderer"][1]["RenderingState"]);
				//				}
				//			}
				//		}
				//	}

				//	if (textureRenderer != jsonItem["MeshData"].end() && !jsonItem["MeshData"]["TextureRenderer"].empty())
    //                {
				//		object->AddComponent<KunrealEngine::MeshRenderer>();
				//		KunrealEngine::MeshRenderer* mesh = object->GetComponent<KunrealEngine::MeshRenderer>();

    //                    if (mesh->GetMeshStatus())
    //                    {
				//			if (!jsonItem["MeshData"]["TextureRenderer"][0]["Diffuse"].empty())
				//			{
				//				std::string diffuseTexture = jsonItem["MeshData"]["TextureRenderer"][0]["Diffuse"];
				//				mesh->SetDiffuseTexture(diffuseTexture.c_str());
				//			}

				//			if (jsonItem["MeshData"]["TextureRenderer"].size() > 1 && !jsonItem["MeshData"]["TextureRenderer"][1]["Normal"].empty())
				//			{
				//				std::string normalTexture = jsonItem["MeshData"]["TextureRenderer"][1]["Normal"];
				//				mesh->SetNormalTexture(normalTexture.c_str());
				//			}
				//		}
				//	}


    //        		objectIndex.emplace_back(object);
    //        	}
    //        }
    //    }
    //}
}

void EpicTool::FileLoad::Update()
{

}
