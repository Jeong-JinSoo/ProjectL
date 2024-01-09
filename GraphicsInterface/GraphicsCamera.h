/// <summary>
/// Camera ��ü���� 
/// ���� �����ʿ��� ����� �Լ��� ��Ƴ��� �������̽� Ŭ����
/// 
/// ����ȭ
/// </summary>

#pragma once

namespace KunrealEngine
{
	namespace KunrealMath
	{
		struct Float2;
		struct Float3;
	}
}

namespace GInterface
{
	class GraphicsCamera
	{
	public:
		virtual ~GraphicsCamera() {};

	public:
		// ī�޶� ��ü ����
		virtual void Delete() abstract;

	public:
		// ī�޶� ��ġ ����
		virtual void SetCameraPosition(KunrealEngine::KunrealMath::Float3 position) abstract;
		// ī�޶� �ٶ󺸴� Ÿ�� ��ġ ����
		virtual void SetTargetPosition(KunrealEngine::KunrealMath::Float3 position) abstract;
		// ī�޶� ȸ�� angle.x (���Ʒ�), angle.y (�¿�)
		virtual void RotateCamera(KunrealEngine::KunrealMath::Float2 angle) abstract;

	public:
		// ī�޶� �¿� �̵�
		virtual void Strafe(float deltaTime) abstract;
		// ī�޶� �յ� �̵�
		virtual void Walk(float deltaTime) abstract;
		// ī�޶� ���Ʒ� �̵�
		virtual void UpDown(float deltaTime) abstract;
	};
}