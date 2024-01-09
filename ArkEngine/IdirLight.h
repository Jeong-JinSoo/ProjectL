/// <summary>
/// 
/// �׷��Ƚ� �������̽��� DirectionalLight����
/// ������ ���� Ŭ����
/// 
/// ����ȭ
/// </summary>

#pragma once
#include "GraphicsDirLight.h"

namespace ArkEngine
{
	class IdirLight : public GInterface::GraphicsDirLight
	{
	public:
		IdirLight(unsigned int lightIndex);
		~IdirLight();

	public:
		virtual void Delete() override;

	public:
		// ambinet �� ����
		virtual void SetAmbient(KunrealEngine::KunrealMath::Float4 ambient) override;
		// diffuse �� ����
		virtual void SetDiffuse(KunrealEngine::KunrealMath::Float4 diffuse) override;
		// specular �� ����
		virtual void SetSpecular(KunrealEngine::KunrealMath::Float4 specular) override;
		// direction �� ����
		virtual void SetDirection(KunrealEngine::KunrealMath::Float3 direction) override;

	public:
		virtual bool GetActive() override;

		virtual void SetActive(bool ox) override;

	public:
		int GetActiveIndex();
		void MinusIndex();
		void MinusActiveIndex();

	private:
		unsigned int _index;
		int _activeIndex;
		bool _isActive;
	};
}

