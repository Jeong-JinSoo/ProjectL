/// <summary>
/// �׷��Ƚ� �������� ����ϴ�
/// 
/// 
/// ����ȭ
/// </summary>

#pragma once
#include "../KunrealMath/MathHeaders.h"

namespace GInterface
{
	struct Material
	{
		KunrealEngine::KunrealMath::Float4 ambient;
		KunrealEngine::KunrealMath::Float4 diffuse;
		KunrealEngine::KunrealMath::Float4 specular;
		KunrealEngine::KunrealMath::Float4 reflect;
	};
}
