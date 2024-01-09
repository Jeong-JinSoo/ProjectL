/// <summary>
/// 
/// �׷��Ƚ� �������̽��� PointLight����
/// ������ ���� Ŭ����
/// 
/// ����ȭ
/// </summary>

#pragma once
#include "GraphicsPointLight.h"

namespace ArkEngine
{
	class IPointLight : public GInterface::GraphicsPointLight
	{
	public:
		IPointLight(unsigned int lightIndex);
		~IPointLight();

	public:
		virtual void Delete() override;

	public:
		// ambient �� ����
		virtual void SetAmbient(KunrealEngine::KunrealMath::Float4 ambient) override;
		// diffuse �� ����
		virtual void SetDiffuse(KunrealEngine::KunrealMath::Float4 diffuse) override;
		// specular �� ����
		virtual void SetSpecular(KunrealEngine::KunrealMath::Float4 specular) override;
		// position �� ����
		virtual void SetPosition(KunrealEngine::KunrealMath::Float3 position) override;
		// ragne �� ����
		virtual void SetRange(float range) override;

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

