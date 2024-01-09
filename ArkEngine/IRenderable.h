/// <summary>
/// 
/// ������Ʈ�� ������ �� �ʼ� ��Ҹ� ���� �������̽�
/// 
/// ����ȭ
/// </summary>

#pragma once

namespace ArkEngine
{
	class ICamera;
}

namespace ArkEngine
{
	class IRenderable abstract
	{
	public:
		IRenderable() {};
		virtual ~IRenderable() {};

	public:
		virtual void Initialize() abstract;

		virtual void Update(ArkEngine::ICamera* camera) abstract;

		virtual void Render() abstract;

		virtual void Finalize() abstract;

	public:
		virtual bool GetRenderingState() abstract;
		virtual void SetRenderingState(bool tf) abstract;

	public:
		virtual void SetPosition(float x = 0.0f, float y = 0.0f, float z = 0.0f) abstract;
		virtual void SetRotation(float x = 0.0f, float y = 0.0f, float z = 0.0f) abstract;
		virtual void SetScale(float x = 1.0f, float y = 1.0f, float z = 1.0f) abstract;
		virtual void SetDiffuseTexture(const char* textureName) abstract;
		virtual void SetNormalTexture(const char* textureName) abstract;
		virtual void SetAnimator() abstract;

	public:
		virtual void PlayAnimation(float deltaTime, bool continiousPlay) abstract;
		virtual void StopAnimation() abstract;
	};
}