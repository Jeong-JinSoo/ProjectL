/// <summary>
/// ����ȭ�� ������Ʈ�� ���� Ŭ����
/// 
/// ����ȭ
/// </summary>

#pragma once
#include <string>
#include <DirectXMath.h>

namespace GInterface
{
	class GraphicsTransparentMesh
	{
	public:
		virtual ~GraphicsTransparentMesh() {};

	public:
		virtual void Delete() abstract;

		// �޽��� �ؽ��� ����
		virtual void SetTexture(const std::string& textureName) abstract;

		// timer�� �ð���ŭ �׸��� (������ �־����)
		virtual bool RenderWithTimer(float deltaTime, float timer) abstract;
 
		// timer �ʱ�ȭ
		virtual void Reset() abstract;

		// 0�� : �⺻
		// 1�� : ��->��� �׸� 
		// 2�� : �Ʒ� -> ���� �׸� 
		// 3�� �߾� -> �ܰ����� �׸�
		// 4�� �߾� -> �ܰ����� ����ä�� ä���� (circle�϶��� ����� ����)
		// 5�� ���� -> �߰��� Ư�� ������ �������� ����ä ä��
		virtual void SetRenderType(unsigned int index) abstract;

		// ������ ����
		virtual void SetTransform(const DirectX::XMFLOAT4X4& matrix) abstract;

		// ������ ���� ������ ����
		virtual void SetExceptRange(const DirectX::XMFLOAT3& center, float range) abstract;
	};
}