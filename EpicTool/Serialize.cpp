#include "Serialize.h"
#include "KunrealAPI.h"
#include <nfd.h>
#include <fstream>

EpicTool::Serialize::Serialize()
{

}

EpicTool::Serialize::~Serialize()
{

}

void EpicTool::Serialize::Initialize()
{

}

void EpicTool::Serialize::SaveFile(const std::string& filePath)
{
	nlohmann::json jsonData;
	nlohmann::json newJsonData;

	std::vector<KunrealEngine::GameObject*> gameObjectlist = KunrealEngine::GetCurrentScene()->
		GetObjectList();

	if (std::ifstream(filePath).good())
	{
		std::ifstream inputFile(filePath);
		inputFile >> jsonData;
		inputFile.close();
	}

	// ������Ʈ�� ���� ��ŭ �ݺ� �� ��
	for (const auto gameObject : gameObjectlist)
	{
		pod.animationFrame = 0;
		nlohmann::json saveJson;
		std::string objectName = gameObject->GetObjectName();

		pod.name = objectName;

		pod.transform["position_x"] = gameObject->GetComponent<KunrealEngine::Transform>()->GetPosition().x;
		pod.transform["position_y"] = gameObject->GetComponent<KunrealEngine::Transform>()->GetPosition().y;
		pod.transform["position_z"] = gameObject->GetComponent<KunrealEngine::Transform>()->GetPosition().z;
						   
		pod.transform["rotation_x"] = gameObject->GetComponent<KunrealEngine::Transform>()->GetRotation().x;
		pod.transform["rotation_y"] = gameObject->GetComponent<KunrealEngine::Transform>()->GetRotation().y;
		pod.transform["rotation_z"] = gameObject->GetComponent<KunrealEngine::Transform>()->GetRotation().z;
						  
		pod.transform["scale_x"] = gameObject->GetComponent<KunrealEngine::Transform>()->GetScale().x;
		pod.transform["scale_y"] = gameObject->GetComponent<KunrealEngine::Transform>()->GetScale().y;
		pod.transform["scale_z"] = gameObject->GetComponent<KunrealEngine::Transform>()->GetScale().z;

		if ((gameObject->GetComponent<KunrealEngine::MeshRenderer>()) != NULL)
		{
			pod.meshRenderer["RenderingState"] = gameObject->GetComponent<KunrealEngine::MeshRenderer>()->GetRenderingState();
			pod.meshRenderer["Mesh"] = gameObject->GetComponent<KunrealEngine::MeshRenderer>()->GetMeshName();
			pod.meshRenderer["Diffuse"] = gameObject->GetComponent<KunrealEngine::MeshRenderer>()->GetTextureName();
			pod.meshRenderer["Normal"] = gameObject->GetComponent<KunrealEngine::MeshRenderer>()->GetNormalName();
		
			pod.animationFrame = gameObject->GetComponent<KunrealEngine::Animator>()->GetCurrentFrame();
		}

		if ((gameObject->GetComponent<KunrealEngine::Light>()) != NULL)
		{
			pod.lightType["LightType"] = gameObject->GetComponent<KunrealEngine::Light>()->GetLightType();

			if (gameObject->GetComponent<KunrealEngine::Light>()->GetLightType() == KunrealEngine::LightType::DirectionalLight)
			{
				pod.light["AmbientX"] = gameObject->GetComponent<KunrealEngine::Light>()->GetAmbient().x;
				pod.light["AmbientY"] = gameObject->GetComponent<KunrealEngine::Light>()->GetAmbient().y;
				pod.light["AmbientZ"] = gameObject->GetComponent<KunrealEngine::Light>()->GetAmbient().z;
				pod.light["AmbientW"] = gameObject->GetComponent<KunrealEngine::Light>()->GetAmbient().w;

				pod.light["DiffuseX"] = gameObject->GetComponent<KunrealEngine::Light>()->GetDiffuse().x;
				pod.light["DiffuseY"] = gameObject->GetComponent<KunrealEngine::Light>()->GetDiffuse().y;
				pod.light["DiffuseZ"] = gameObject->GetComponent<KunrealEngine::Light>()->GetDiffuse().z;
				pod.light["DiffuseZ"] = gameObject->GetComponent<KunrealEngine::Light>()->GetDiffuse().w;

				pod.light["DirectionX"] = gameObject->GetComponent<KunrealEngine::Light>()->GetDirection().x;
				pod.light["DirectionY"] = gameObject->GetComponent<KunrealEngine::Light>()->GetDirection().y;
				pod.light["DirectionZ"] = gameObject->GetComponent<KunrealEngine::Light>()->GetDirection().z;

				pod.light["SpecularX"] = gameObject->GetComponent<KunrealEngine::Light>()->GetSpecular().x;
				pod.light["SpecularY"] = gameObject->GetComponent<KunrealEngine::Light>()->GetSpecular().y;
				pod.light["SpecularZ"] = gameObject->GetComponent<KunrealEngine::Light>()->GetSpecular().z;
				pod.light["SpecularZ"] = gameObject->GetComponent<KunrealEngine::Light>()->GetSpecular().w;
			}
			else if (gameObject->GetComponent<KunrealEngine::Light>()->GetLightType() == KunrealEngine::LightType::PointLight)
			{
				pod.light["AmbientX"] = gameObject->GetComponent<KunrealEngine::Light>()->GetAmbient().x;
				pod.light["AmbientY"] = gameObject->GetComponent<KunrealEngine::Light>()->GetAmbient().y;
				pod.light["AmbientZ"] = gameObject->GetComponent<KunrealEngine::Light>()->GetAmbient().z;
				pod.light["AmbientW"] = gameObject->GetComponent<KunrealEngine::Light>()->GetAmbient().w;

				pod.light["DiffuseX"] = gameObject->GetComponent<KunrealEngine::Light>()->GetDiffuse().x;
				pod.light["DiffuseY"] = gameObject->GetComponent<KunrealEngine::Light>()->GetDiffuse().y;
				pod.light["DiffuseZ"] = gameObject->GetComponent<KunrealEngine::Light>()->GetDiffuse().z;
				pod.light["DiffuseZ"] = gameObject->GetComponent<KunrealEngine::Light>()->GetDiffuse().w;

				pod.light["SpecularX"] = gameObject->GetComponent<KunrealEngine::Light>()->GetSpecular().x;
				pod.light["SpecularY"] = gameObject->GetComponent<KunrealEngine::Light>()->GetSpecular().y;
				pod.light["SpecularZ"] = gameObject->GetComponent<KunrealEngine::Light>()->GetSpecular().z;
				pod.light["SpecularZ"] = gameObject->GetComponent<KunrealEngine::Light>()->GetSpecular().w;

				pod.light["Range"] = gameObject->GetComponent<KunrealEngine::Light>()->GetPointRange();
			}
		}


		if ((gameObject->GetComponent<KunrealEngine::ImageRenderer>()) != NULL)
		{
			std::string teststr = "abc";
			pod.image["ImageName"] = teststr;  // ���� ������ �ִ� �̹����� get �� �� ����
			pod.image["ImageStatus"] = gameObject->GetComponent<KunrealEngine::ImageRenderer>()->GetImageStatus();
		}

		if ((gameObject->GetComponent<KunrealEngine::Player>()) != NULL)  // �׳� addCompoent�� ���ָ� �Ǵ°�?
		{
			pod.customCompoenent["Player"] = true;
		}

		nlohmann::json serializedData;

		PreEncoding(serializedData);  // pod�� ������ json���� ����
	
		newJsonData.push_back(serializedData); //������ ������ �߰�

		// �ʵ带 �ʱ�ȭ
		pod.transform.clear();
		pod.lightType.clear();
		pod.light.clear();
		pod.meshRenderer.clear();
		pod.image.clear();
		pod.customCompoenent.clear();
		
	}
	
	// json ��ü�� ���Ϸ� ���
	std::ofstream outputFile("output.json");
	if (outputFile.is_open())
	{
		outputFile << std::setw(4) << newJsonData << std::endl; // setw(4)�� pretty print�� ���� ��
		outputFile.close();
	}

	
}
  
bool EpicTool::Serialize::PreEncoding(json& data) const
{
	EpicTool::FieldPreEncoding<boost::pfr::tuple_size_v<Field>>(pod, data["POD"]);

	return true;
}

bool EpicTool::Serialize::PostEncoding(json& data) const
{
	// ������ ���� �κ��� �̱��� => ���غ���
	//EpicTool::FieldPostEncoding<boost::pfr::tuple_size_v<Field>>(pod, data["POD"]);

	return true;
}
