/// <summary>
/// Renderable ��ü���� 
/// ���� �����ʿ��� ����� �Լ��� ��Ƴ��� �������̽� Ŭ����
/// 
/// ����ȭ
/// </summary>

#pragma once
#include "GraphicsStruct.h"

namespace KunrealEngine
{
	namespace KunrealMath
	{
		struct Float4;
	}
}

namespace GInterface
{
	class GraphicsRenderable
	{
	public:
		virtual ~GraphicsRenderable() {};

	public:
		// �������� ��ü ����
		virtual void Delete() abstract;
		// ���� ������ ���� ��������
		virtual bool GetRenderingState() abstract;
		// ������ ���� ����
		virtual void SetRenderingState(bool tf) abstract;

	public:
		// �������� ��ü�� Ʈ������ ����								// WorldTM
		virtual void SetTransform(KunrealEngine::KunrealMath::Matrix4x4 matrix) abstract;
		// �������� ��ü�� ��ġ�� ����
		virtual void SetPosition(float x =0.0f, float y=0.0f, float z=0.0f) abstract;
		// �������� ��ü�� ȸ���� ����
		virtual void SetRotation(float x = 0.0f, float y = 0.0f, float z = 0.0f) abstract;
		// �������� ��ü�� ũ�Ⱚ ����
		virtual void SetScale(float x = 1.0f, float y = 1.0f, float z = 1.0f) abstract;

		// �������� ��ü�� �׸� ������Ʈ ����
		virtual void SetModel(const char* fileName) abstract;
		// �������� ��ü�� ��ǻ�� �ؽ��� ����
		virtual void SetDiffuseTexture(const char* textureName) abstract;
		// �������� ��ü�� �븻 �ؽ��� ����
		virtual void SetNormalTexture(const char* textureName) abstract;	
		// �������� ��ü�� ���׸��� ��������
		virtual const GInterface::Material GetMaterial() abstract;
		// �������� ��ü�� ���׸��� ����
		virtual void SetMaterial(GInterface::Material material) abstract;
		// �������� ��ü�� �ݻ簪 ����
		virtual void SetReflect(KunrealEngine::KunrealMath::Float4 reflect) abstract;


	public:
		// �������� ��ü�� �ִϸ��̼� ������ �ִٸ� ����
		virtual void PlayAnimation(float deltaTime, bool continiousPlay = false) abstract;
		// �������� �ִϸ��̼� ���߱�
		virtual void StopAnimation() abstract;
		// FBX�� �ִϸ��̼� ���� �Լ�
		virtual void PlayFBXAnimation(float speed, int animIndex, bool continuousPlay = false) abstract;
	};
}