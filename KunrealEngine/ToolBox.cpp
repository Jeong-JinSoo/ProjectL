#include "ToolBox.h"
#include <random>

int KunrealEngine::ToolBox::GetRandomNum(int maxNum)
{
	std::random_device rd;								// ���� �õ尪 ����
	std::mt19937 generator(rd());						// �õ尪�� ���� ���� ����
	std::uniform_int_distribution<> dist(0, maxNum);	// ������ ���� ����

	return dist(generator);								// ������ ���� ��ȯ
}

int KunrealEngine::ToolBox::GetRandomNum(int minNum, int maxNum)
{
	std::random_device rd;
	std::mt19937 generator(rd());
	std::uniform_int_distribution<> dist(minNum, maxNum);

	return dist(generator);
}

float KunrealEngine::ToolBox::GetDistance(DirectX::XMFLOAT3& src, DirectX::XMFLOAT3& dst)
{
	DirectX::XMVECTOR source = DirectX::XMLoadFloat3(&src);
	DirectX::XMVECTOR destination = DirectX::XMLoadFloat3(&dst);

	DirectX::XMVECTOR distance = DirectX::XMVectorSubtract(destination, source);

	float result = DirectX::XMVectorGetX(DirectX::XMVector3Length(distance));

	return result;
}

DirectX::XMVECTOR KunrealEngine::ToolBox::GetDirectionVec(DirectX::XMFLOAT3& src, DirectX::XMFLOAT3& dst)
{
	DirectX::XMVECTOR source = DirectX::XMLoadFloat3(&src);
	DirectX::XMVECTOR destination = DirectX::XMLoadFloat3(&dst);

	DirectX::XMVECTOR result = DirectX::XMVector3Normalize(DirectX::XMVectorSubtract(destination, source));

	return result;
}

float KunrealEngine::ToolBox::GetAngle(DirectX::XMFLOAT3 src, DirectX::XMFLOAT3 dst)
{
	DirectX::XMVECTOR currentPosVec = DirectX::XMLoadFloat3(&src);
	DirectX::XMVECTOR targetPosVec = DirectX::XMLoadFloat3(&dst);

	DirectX::XMVECTOR currentForward = DirectX::XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f);
	DirectX::XMVECTOR targetDirection = DirectX::XMVectorSubtract(targetPosVec, currentPosVec);
	targetDirection.m128_f32[1] = 0.0f;
	targetDirection = DirectX::XMVector3Normalize(targetDirection);

	// �� ���� ���� ������ ���
	auto dotResult = DirectX::XMVector3Dot(currentForward, targetDirection);
	float dotProduct = DirectX::XMVectorGetX(dotResult);

	// ������ ���ȿ��� ���� ��ȯ
	float angle = acos(dotProduct);
	angle = DirectX::XMConvertToDegrees(angle);

	if (targetPosVec.m128_f32[0] > currentPosVec.m128_f32[0])
	{
		angle *= -1;
	}

	return angle;
}

DirectX::XMFLOAT3 KunrealEngine::ToolBox::QuaternionToEulerAngles(const DirectX::XMVECTOR& quaternion)
{
	DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixRotationQuaternion(quaternion);

	float pitch, yaw, roll;
	if (rotationMatrix.r[2].m128_f32[1] > 0.998f)
	{
		pitch = DirectX::XM_PI / 2.0f;
		yaw = atan2f(rotationMatrix.r[0].m128_f32[2], rotationMatrix.r[0].m128_f32[0]);
		roll = 0.0f;
	}
	else if (rotationMatrix.r[2].m128_f32[1] < -0.998f)
	{
		pitch = -DirectX::XM_PI / 2.0f;
		yaw = atan2f(rotationMatrix.r[0].m128_f32[2], rotationMatrix.r[0].m128_f32[0]);
		roll = 0.0f;
	}
	else
	{
		pitch = asinf(-rotationMatrix.r[2].m128_f32[1]);
		yaw = atan2f(rotationMatrix.r[0].m128_f32[1], rotationMatrix.r[1].m128_f32[1]);
		roll = atan2f(rotationMatrix.r[2].m128_f32[0], rotationMatrix.r[2].m128_f32[2]);
	}

	return DirectX::XMFLOAT3(roll, pitch, yaw);
}