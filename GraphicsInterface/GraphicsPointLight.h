/// <summary>
/// PointLight �������̽�
/// 
/// �̰�ȣ ����ȭ
/// </summary>

#pragma once
#include "GraphicsLight.h"

namespace KunrealEngine
{
	namespace KunrealMath
	{
		struct Float3;
		struct Float4;
	}
}

namespace GInterface
{
	class GraphicsPointLight : public  GInterface::GraphicsLight
	{
	public:
		virtual ~GraphicsPointLight() {};

	public:
		// ambient �� ����
		virtual void SetAmbient(KunrealEngine::KunrealMath::Float4 ambient) abstract;
		// diffuse �� ����
		virtual void SetDiffuse(KunrealEngine::KunrealMath::Float4 diffuse) abstract;
		// specular �� ����
		virtual void SetSpecular(KunrealEngine::KunrealMath::Float4 specular) abstract;
		// position �� ����
		virtual void SetPosition(KunrealEngine::KunrealMath::Float3 position) abstract;
		// ragne �� ����
		virtual void SetRange(float range) abstract;
	};
}