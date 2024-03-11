/// <summary>
/// Renderable ��ü���� 
/// ���� �����ʿ��� ����� �Լ��� ��Ƴ��� �������̽� Ŭ����
/// 
/// ����ȭ
/// </summary>

#pragma once
#include "GraphicsStruct.h"

namespace DirectX
{
	struct XMFLOAT3;
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
		// ��ŷ ���� ���� ����
		virtual void SetPickable(bool tf) abstract;
		// ��ŷ ���� ���� ��������
		virtual bool GetPickable() abstract;

	public:
		// �������� ��ü�� Ʈ������ ����								// WorldTM
		virtual void SetTransform(DirectX::XMFLOAT4X4 matrix) abstract;
		// �������� ��ü�� ��ġ�� ����
		virtual void SetPosition(float x =0.0f, float y=0.0f, float z=0.0f) abstract;
		// �������� ��ü�� ȸ���� ����
		virtual void SetRotation(float x = 0.0f, float y = 0.0f, float z = 0.0f) abstract;
		// �������� ��ü�� ũ�Ⱚ ����
		virtual void SetScale(float x = 1.0f, float y = 1.0f, float z = 1.0f) abstract;

		// �������� ��ü�� �׸� ������Ʈ ����
		virtual void SetModel(const char* fileName) abstract;
		// �������� ��ü�� ��ǻ�� �ؽ��� ����
		virtual void SetDiffuseTexture(int index, const char* textureName) abstract;
		// �������� ��ü�� �븻 �ؽ��� ����
		virtual void SetNormalTexture(int index, const char* textureName) abstract;
		// �������� ��ü�� emissive �ؽ��� ����
		virtual void SetEmissiveTexture(int index, const char* textureName) abstract;
		// �������� ��ü�� ���׸��� ��������
		virtual const GInterface::Material GetMaterial() abstract;
		// �������� ��ü�� ���׸��� ����
		virtual void SetMaterial(GInterface::Material material) abstract;
		// �������� ��ü�� �ݻ簪 ����
		virtual void SetReflect(DirectX::XMFLOAT4 reflect) abstract;

	public:
		virtual const std::vector<std::string> GetDiffuseTextureList() abstract;
		virtual const std::vector<std::string> GetNormalTextureList() abstract;

	public:
		// �������� ��ü�� �ִϸ��̼� ������ �ִٸ� ����
		virtual void PlayAnimation(float deltaTime, bool continiousPlay = false) abstract;
		// �������� �ִϸ��̼� ���߱�
		virtual void StopAnimation() abstract;
		// FBX�� �ִϸ��̼� ���� �Լ� (�ε�����), ���������� ���θ� ��ȯ
		virtual bool PlayAnimation(float speed, float deltaTime, int animIndex, bool continuousPlay = false) abstract;
		// FBX�� �ִϸ��̼� ���� �Լ� (�̸�����), ���������� ���θ� ��ȯ
		virtual bool PlayAnimation(float speed, float deltaTime, std::string animName, bool continuousPlay = false) abstract;
		// Mesh�� ������ �ִ� �ִϸ��̼� Ŭ�� �̸��� ��ȯ
		virtual const std::vector<std::string>& GetClipNames() abstract;
		// �ִϸ��̼��� ���� �����ӿ��� �����
		virtual void PauseAnimation() abstract;
		// ���� �ִϸ��̼��� �ٽ� ����Ѵ�
		virtual void ReplayAnimation() abstract;
		// ���� �������� �����´�
		virtual float GetCurrentFrame() abstract;
		// ���� �ִϸ��̼��� �ִ� �������� �����´�
		virtual float GetMaxFrame() abstract;
	};
}