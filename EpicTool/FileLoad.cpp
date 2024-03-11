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
/// 
/// ������Ʈ ����̶� ��ø������� �ʿ��ϴٰ� �Ǵ���
/// 2024.03.04
/// </summary>
void EpicTool::FileLoad::Initialize()  // �Ʒ� �κ��� ���Ŀ� �ݺ������� ȣ�� �� �� �ִ� �κ��̱⿡ �и��� ���� �ִ� , 
{
    //json���� �߿� �ռ� �����ص� ������ �о��
   std::ifstream inputFile("output.json");
   if (inputFile.is_open())
   {
	   nlohmann::json jsonData;
	   inputFile >> jsonData;
	   inputFile.close();
	   // json�� ���� ������Ʈ�� ������
	   for (const auto& jsonItem : jsonData)
	   {
		   std::string objectName = jsonItem["POD"]["name"];
		   KunrealEngine::GetCurrentScene()->CreateObject(objectName);

		   KunrealEngine::GameObject* object = KunrealEngine::GetCurrentScene()->GetGameObject(objectName);
		   std::vector<KunrealEngine::GameObject*> objectIndex = KunrealEngine::GetCurrentScene()->
			   GetObjectList();

		   if (object)
		   {
			   KunrealEngine::Transform* trans = object->GetComponent<KunrealEngine::Transform>();

			   trans->SetPosition(jsonItem["POD"]["position_x"], jsonItem["POD"]["position_y"], jsonItem["POD"]["position_z"]);
			   trans->SetRotation(jsonItem["POD"]["rotation_x"], jsonItem["POD"]["rotation_y"], jsonItem["POD"]["rotation_z"]);
			   trans->SetScale(jsonItem["POD"]["scale_x"], jsonItem["POD"]["scale_y"], jsonItem["POD"]["scale_z"]);


			   // �޽� ����
			   auto meshRenderer = jsonItem["POD"].find("meshRenderer");
			   auto diffuse = jsonItem["POD"]["meshRenderer"].find("Diffuse");
			   auto mesh = jsonItem["POD"]["meshRenderer"].find("Mesh");
			   auto normal = jsonItem["POD"]["meshRenderer"].find("Normal");
			   auto renderingState = jsonItem["POD"]["meshRenderer"].find("RenderingState");

			   if (meshRenderer != jsonItem["POD"].end() && !jsonItem["POD"]["meshRenderer"].empty())
			   {
				   object->AddComponent<KunrealEngine::MeshRenderer>();
				   KunrealEngine::MeshRenderer* meshRenderer = object->GetComponent<KunrealEngine::MeshRenderer>();

				   if (diffuse != jsonItem["POD"]["meshRenderer"].end() && (!jsonItem["POD"]["meshRenderer"]["Diffuse"].empty()))
				   {
					   object->GetComponent<KunrealEngine::MeshRenderer>()->SetDiffuseTexture(0, jsonItem["POD"]["meshRenderer"]["Diffuse"].get<std::string>().c_str());
				   }
				   if (mesh != jsonItem["POD"]["meshRenderer"].end() && (!jsonItem["POD"]["meshRenderer"]["Mesh"].empty()))
				   {
					   object->GetComponent<KunrealEngine::MeshRenderer>()->SetMeshObject(jsonItem["POD"]["meshRenderer"]["Mesh"].get<std::string>().c_str());
				   }
				   if (normal != jsonItem["POD"]["meshRenderer"].end() && (!jsonItem["POD"]["meshRenderer"]["Normal"].empty()))
				   {
					   // ����� ��ǻ��� ���� ������ ����
				   }
				   //if (renderingState != jsonItem["POD"]["meshRenderer"].end() && jsonItem["POD"]["meshRenderer"]["RenderingState"] == "\u0001")
				   //{
					  // object->GetComponent<KunrealEngine::MeshRenderer>()->
					  // �⺻������ true��� ����
				   //}

			   }


			   // ����Ʈ

			   // �̹���

			   // Ŀ����������Ʈ

			   // �ִϸ��̼� ������ (�ִϸ��̼� ���� ui �̱����� ����)

		   }
	   }

   }

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
