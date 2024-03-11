#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

#include <boost/pfr.hpp>
#include <boost/pfr/core.hpp>
#include <boost/pfr/core_name.hpp>
#include <nlohmann/json.hpp>
#include "Storable.h"

using json = nlohmann::json;

namespace KunrealEngine
{
	class Transform;
	class MeshRenderer;
	class Component;
	class Light;
	class GameObject;
}

struct Field
{
	std::string name;

	std::map<std::string, float> transform;

	//mesh
	std::map<std::string, std::string> meshRenderer;

	float animationFrame;

	//light
	std::map<std::string, std::string> lightType;

	std::map<std::string, float> light;

	//image
	std::map<std::string, std::string> image;

	// ���ҿ� ���� ���� ������Ʈ
	std::map<std::string, bool> customCompoenent;

	TO_JSON(Field)
};

//BOOST_PFR_FUNCTIONS_FOR(Field);  // �ȵǴ� ������ �ؽ� Ư��ȭ�� float�� ������ ���Ͱ� ���?
//
////std::hash Ư��ȭ
//namespace std  // �ؽ� Ư��ȭ
//{
//	template <>
//	struct hash<Field>
//	{
//		size_t operator()(const Field& obj) const
//		{
//			size_t hashValue = 0;
//			// �̰��� �ʿ��� �ʵ带 ��� �߰��Ͽ� �ؽ��մϴ�.
//			hashValue ^= std::hash<std::string>()(obj.name);
//
//			return hashValue;
//		}
//	};
//
//	template <>
//	struct hash<std::vector<float>>
//	{
//		size_t operator()(const std::vector<float>& vec) const
//		{
//			size_t hashValue = 0;
//			for (const float& val : vec)
//			{
//				hashValue ^= std::hash<float>()(val);
//			}
//			return hashValue;
//		}
//	};
//
//	template<>
//	struct hash<std::map<std::string, int>>
//	{
//		size_t operator()(const std::map<std::string, int>& vec) const
//		{
//			size_t hashValue = 0;
//			for (const auto& pair : vec)
//			{
//				hashValue ^= std::hash<std::string>()(pair.first);		
//				hashValue ^= std::hash<int>()(pair.second);		
//			}
//			return hashValue;
//		}
//	};
//
//	template<>
//	struct hash<std::map<std::string, float>>
//	{
//		size_t operator()(const std::map<std::string, float>& vec) const
//		{
//			size_t hashValue = 0;
//			for (const auto& pair : vec)
//			{
//				hashValue ^= std::hash<std::string>()(pair.first);
//				hashValue ^= std::hash<float>()(pair.second);
//			}
//			return hashValue;
//		}
//	};
//
//	template<>
//	struct hash<std::map<std::string, std::string>>
//	{
//		size_t operator()(const std::map<std::string, std::string>& vec) const
//		{
//			size_t hashValue = 0;
//			for (const auto& pair : vec)
//			{
//				hashValue ^= std::hash<std::string>()(pair.first);
//				hashValue ^= std::hash<std::string>()(pair.second);
//			}
//			return hashValue;
//		}
//	};
//
//	template<>
//	struct hash<Transform>  // ����ü�� ��츦 ������ �׽�Ʈ Ư��ȭ�� ����
//	{
//		size_t operator()(const Transform& obj) const
//		{
//			size_t hashValue = 0;
//			hashValue ^= std::hash<float>()(obj.a);
//			hashValue ^= std::hash<float>()(obj.b);
//			hashValue ^= std::hash<float>()(obj.c);
//		}
//	};
//}
//
//
//template <typename T, std::size_t... is>
//void SaveJsonInternal(const T& obj, nlohmann::json& jsonSave, std::index_sequence<is...>)
//{
//	(..., (jsonSave[boost::pfr::get_name<is, T>()] = boost::pfr::get<is>(obj)));
//
//}
//
//template <typename T>
//void SaveJson(const T& obj, nlohmann::json& jsonSave)
//{
//	constexpr std::size_t fieldCount = boost::pfr::tuple_size_v<T>;
//	SaveJsonInternal(obj, jsonSave, std::make_index_sequence<fieldCount>());
//}


namespace EpicTool
{

	class Serialize : public Storable
	{
	public:
		Serialize();
		~Serialize();

	public:


		void Initialize();

		void SaveFile(const std::string& filePath);

		Field pod;

	protected:
		virtual bool PreEncoding(json& data) const override;
		virtual bool PostEncoding(json& data) const override;

	private:
		std::vector<KunrealEngine::GameObject*> _gameObjectlist;
	
		
	};
}