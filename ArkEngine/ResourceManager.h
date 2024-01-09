/// <summary>
/// ���ҽ� ������ ���� �Ŵ���
/// �������� �ʿ��� ���ҽ����� ���ϰ� ���� ����
/// 
/// ����ȭ
/// </summary>

#pragma once
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <vector>
#include "DXTKFont.h"
#include "IResource.h"
#include "IRenderable.h"
#include "IDebugObject.h"
#include "ICamera.h"
#include "ICubeMap.h"

class ASEParser;

struct TextPosition {
	int x;
	int y;

	// �� ������ ����
	bool operator==(const TextPosition& other) const {
		return x == other.x && y == other.y;
	}
};

// Point ����ü�� ����ϴ� �ؽ� �Լ� ����
namespace std
{
	template <>
	struct hash<TextPosition>
	{
		std::size_t operator()(const TextPosition& p) const
		{
			std::size_t h1 = std::hash<int>()(p.x);
			std::size_t h2 = std::hash<int>()(p.y);
			// �ؽ� �Լ��� �����Ͽ� ���� �ؽ� ���� ��ȯ
			return h1 ^ (h2 << 1);
		}
	};
}


namespace ArkEngine
{
	class ResourceManager
	{
	private:
		ResourceManager();
		~ResourceManager();

		static ResourceManager* instance;

	public:
		static ResourceManager* GetInstance();
		static void Delete();

	public:
		// IResource ������ ����Ǿ��ִ� ���ҽ��� �������� ��ȯ����
		template <typename Resource>
		Resource* GetResource(const std::string& fileName);

		// IResource ������ ���ҽ��� ���Ϳ� ������
		void SetResource(const std::string& fileName, IResource* resource);

		// �������� Ư�� Ȯ������ ������ ��� ã�� �� �̸����� ���ͷ� �Ѱ���
		std::vector<std::string> FindSpecificResources(const std::string& folderName, const std::string& extension);

		// ���Ϳ� �ִ� Ư�� ���ҽ��� ����
		void DeleteResource(const std::string& fileName);

	public:
		// ������ �� ������Ʈ���� ��Ƴ��� ���� ��ȯ
		const std::vector<ArkEngine::IRenderable*> GetRenderableList();

		// ������ �� ������Ʈ���� ��Ƴ��� ����
		void AddRenderable(ArkEngine::IRenderable* renderable);

		// Ư�� �������ϴ� ������Ʈ�� ����
		void DeleteRenderable(ArkEngine::IRenderable* renderable);

	public:
		// ��� ����� ������Ʈ���� ��Ƴ��� ���� ��ȯ
		const std::vector<ArkEngine::IDebugObject*> GetDebugObjectList();

		void AddDebugObject(ArkEngine::IDebugObject* object);

		void DeleteDebugObject(ArkEngine::IDebugObject* object);

	public:
		// ��� ť����� ��Ƴ��� ���� ��ȯ
		const std::unordered_set<ArkEngine::ICubeMap*> GetCubeMapList();

		std::vector<std::string> GetCubeMapNameList();

		// ���� �������ǰ� �ִ� ť����� ��ȯ
		ArkEngine::ICubeMap* GetNowCubeMap();

		void SetNowCubeMap(ArkEngine::ICubeMap* cubeMap);

		void SetCubeMapNameList(std::vector <std::string> nameVector);

		// ť��� ���Ϳ� ť����� �߰�
		void AddCubeMap(ArkEngine::ICubeMap* cubeMap);

		// ť��� ���Ϳ��� ť����� ����
		void DeleteCubeMap(ArkEngine::ICubeMap* cubeMap);

	public:
		// ������ �� ������Ʈ���� ��Ƴ��� ���� ��ȯ
		const std::vector<ArkEngine::ICamera*> GetCameraList();

		// ������ �� ������Ʈ���� ��Ƴ��� ����
		void AddCamera(ArkEngine::ICamera* camera);

		// Ư�� �������ϴ� ������Ʈ�� ����
		void DeleteCamera(ArkEngine::ICamera* camera);

	public:
		const std::unordered_map<TextPosition, std::string>& GetTextList();
		// �ؽ�Ʈ ����Ʈ�� �ؽ�Ʈ �߰�
		void SetTextList(int posX, int posY, std::string text);

	public:
		ASEParser* GetASEParser(const std::string& fileName);

		void AddASEParser(const std::string& fileName, ASEParser* parser);

		void DeleteASEParser(const std::string& fileName);

	public:
		// �����Ϳ� �� �̸��� �Ѱ��ֱ� ����
		std::vector<std::string> GetRenderableNameList();

		void SetTextureNameList(std::string name);

		// �����Ϳ� �ؽ��� �̸��� �Ѱ��ֱ� ����
		const std::vector<std::string> GetTextureNameList();

	public:
		void ReleaseAll();

	private:
		std::unordered_map<std::string, IResource*> _resourceList;

	private:
		// �����ϴ� ��� �׸� �� �ִ� ������Ʈ��
		std::vector<IRenderable*> _renderableList;

		std::vector<IDebugObject*> _debugList;

		// �����ϴ� ��� ť���
		ICubeMap* _nowCubeMap;

		std::unordered_set<ICubeMap*> _cubeMapList;

		std::vector<std::string> _cubeMapNameList;

		// �����ϴ� ��� ī�޶� ������Ʈ��
		std::vector<ICamera*> _cameraList;

		// ����� �ؽ�Ʈ ����Ʈ
		std::unordered_map<TextPosition, std::string> _textList;

		// ASE �ļ� ����Ʈ
		std::unordered_map<std::string, ASEParser*> _ASEParserList;

	private:
		// �����ϴ� ��� �ؽ��� �̸� ����Ʈ // �����ִ°� ��ȿ�����ΰ� ��� ��
		std::vector<std::string> _textureNameList;
	};

	template <typename Resource>
	Resource* ArkEngine::ResourceManager::GetResource(const std::string& fileName)
	{
		auto iter = _resourceList.find(fileName);

		if (iter != _resourceList.end())
		{
			return dynamic_cast<Resource*>(_resourceList.at(fileName));
		}
		else
		{
			return nullptr;
		}
	}
}

