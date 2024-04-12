#pragma once
#include "CommonHeader.h"
#include <DirectXMath.h>

/// <summary>
/// ���ӷ����� �����ϴ� Ŭ����
/// �Լ��� ��� static
/// </summary>

namespace KunrealEngine
{
	class ToolBox
	{
		/// ���� ����
	public:
		// 0���� �Ű������� ������ ����
		static int GetRandomNum(int maxNum);

		// ���� ����
		static int GetRandomNum(int minNum, int maxNum);

		// float �� random ��ȯ
		static float GetRandomFloat(float minNum, float maxNum);

		/// ���� ����
	public:
		// �� XMFLOAT3 ��ǥ ������ �Ÿ�
		static float GetDistance(DirectX::XMFLOAT3 src, DirectX::XMFLOAT3 dst);

		// �� XMFLOAT3 ��ǥ ���հ�
		static float GetAngle(DirectX::XMFLOAT3 src, DirectX::XMFLOAT3 dst);

		// ���ʹϾ��� ���Ϸ���
		static DirectX::XMFLOAT3 QuaternionToEulerAngles(const DirectX::XMVECTOR& quaternion);
	};

}
