/// <summary>
/// DirectionalLight �������̽�
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
	class GraphicsDirLight : public  GInterface::GraphicsLight
	{
	public:
		virtual ~GraphicsDirLight() {};

	public:
		// ambient �� ����
		virtual void SetAmbient(KunrealEngine::KunrealMath::Float4 ambient) abstract;
		// diffuse �� ����
		virtual void SetDiffuse(KunrealEngine::KunrealMath::Float4 diffuse) abstract;
		// specular �� ����
		virtual void SetSpecular(KunrealEngine::KunrealMath::Float4 specular) abstract;
		// direction �� ����
		virtual void SetDirection(KunrealEngine::KunrealMath::Float3 direction) abstract;
	};
}