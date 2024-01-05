#pragma once
#include <windows.h>
#include "CommonHeader.h"
#include "../GraphicsInterface/GraphicsHeaders.h"

/// <summary>
/// �׷��Ƚ� �������̽� Ŭ������ ����� �پ��� �κп��� ����ϱ� ���� ���̾�
/// �̱���ȭ�ؼ� ��𼭵� ȣ�� �����ϰ�
/// 
/// �̰�ȣ
/// </summary>

#define GRAPHICS KunrealEngine::GraphicsSystem::GetInstance().GetGraphics()

namespace GInterface
{
	class GraphicsInterface;
	class GraphicsCamera;
}

namespace KunrealEngine
{
	class _DECLSPEC GraphicsSystem
	{
	private:
		GraphicsSystem();
		~GraphicsSystem();

	public:
		// �̱��� ��ü ��ȯ
		static GraphicsSystem& GetInstance();

		void Initialize(HWND hwnd, int screenWidth, int screenHeight);
		void Finalize();

		GInterface::GraphicsInterface* GetGraphics();

	public:
		// ť��� ����
		void CreateCubeMap(const char* cubeMapName, const char* textureName = "", bool isCube = false);

		// ť��� ����	// ���� �� ����ߴ� �̸����� Ž���ؼ� ������
		void DeleteCubeMap(const char* cubeMapName);

		// ȭ�鿡 ��� �� ���� ť��� ����
		// ���� ������Ʈ������ �ϳ��� ť��ʸ� ��� ����
		void SetMainCubeMap(std::string cubeMapName);

		// ť��� ����Ʈ ��ȯ
		const std::vector<std::string> GetCubeMapList();

	public:
		const std::vector<std::string> GetRenderableList();

		const std::vector<std::string> GetTextureList();
		 
	private:
		GInterface::GraphicsInterface* _graphics;
	};
}
