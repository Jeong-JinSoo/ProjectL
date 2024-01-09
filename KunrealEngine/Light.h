#pragma once
#include "Component.h"
#include "GraphicsSystem.h"
#include "../KunrealMath/MathHeaders.h"

/// <summary>
/// Directional light, Point light, spotlight ������ �پ��ϴ�
/// 
/// ���� ������Ʈ�� ���� ���ΰ� �ƴϸ� �ϳ��� ������Ʈ�� ��ȯ�� �����ϰ� �� ���ΰ�..
/// </summary>

#pragma warning(disable: 4251)

namespace KunrealEngine
{
	class Transform;

	enum LightType
	{
		DirectionalLight,
		PointLight,
		SpotLight,
		None
	};

	class _DECLSPEC Light : public Component
	{

	public:
		Light();
		~Light();

		virtual void Initialize() override;
		virtual void Finalize() override;

		virtual void FixedUpdate() override;
		virtual void Update() override;
		virtual void LateUpdate() override;

		virtual void OnTriggerEnter() override;
		virtual void OnTriggerStay() override;
		virtual void OnTriggerExit() override;

		// light�� �⺻ state�� false���� �������
		virtual void SetActive(bool active) override;

	private:
		// � light���� ������
		LightType _type;

		// ������Ʈ�� Transform
		Transform* _transform;

	public:
		LightType GetLightType();

	private:
		// light �������� ���� �θ�
		GInterface::GraphicsLight* _light;

	public:
		// light�� �����Ǿ����� Ȯ��
		bool GetLightStatus();

	private:
		// Directional Light�� �ʿ��� ������
		KunrealMath::Float4 _ambient;
		KunrealMath::Float4 _diffuse;
		KunrealMath::Float4 _specular;
		KunrealMath::Float3 _direction;

	private:
		// PointLight�� �ʿ��� ������
		//KunrealMath::Float3 _positionP;	// �������� ���� ������ �ʿ䰡 ����
		float _rangeP;

	public:
	/// Light ���� �Լ�
	/// Ambient, Diffuse, Specular�� ��� ������ ���� ����
	/// Light �������� ĳ�����ؼ� ���
	
		void SetAmbient(float x, float y, float z, float w);
		KunrealMath::Float4 GetAmbient();

		void SetDiffuse(float x, float y, float z, float w);
		KunrealMath::Float4 GetDiffuse();

		void SetSpecular(float x, float y, float z, float w);
		KunrealMath::Float4 GetSpecular();

	public:
		/// Directional Light ����
		// Directional light ����
		// ����Ʈ �Ű������� ��� ���� 0
		void CreateDirectionalLight(KunrealMath::Float4 ambient = { 0.f, 0.f, 0.f, 0.f }, KunrealMath::Float4 diffuse = { 0.f, 0.f, 0.f, 0.f }, KunrealMath::Float4 specular = { 0.f, 0.f, 0.f, 0.f }, KunrealMath::Float3 direction = { 0.f, 0.f, 0.f });

		void SetDirection(float x, float y, float z);
		KunrealMath::Float3 GetDirection();

	public:
		/// PointLight ����
		// PointLight ����
		// ����Ʈ �Ű������� ��� ���� 0
		// �������� ���� ��ġ�̹Ƿ� transform���� �������� �Ǵϱ� ����
		void CreatePointLight(KunrealEngine::KunrealMath::Float4 ambient = {0.f, 0.f, 0.f, 0.f}, KunrealEngine::KunrealMath::Float4 diffuse = { 0.f, 0.f, 0.f, 0.f }, KunrealEngine::KunrealMath::Float4 specular = { 0.f, 0.f, 0.f, 0.f }, float range = 0.f);
	
		void SetPointPosition();

		void SetPointRange(float range);
		float GetPointRange();
	};
}

#pragma warning(default: 4251)