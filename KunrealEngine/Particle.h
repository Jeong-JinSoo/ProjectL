#pragma once

#include <DirectXMath.h>
#include "CommonHeader.h"
#include "Component.h"

namespace GInterface
{
	class GraphicsParticle;
}

namespace KunrealEngine
{
	class _DECLSPEC Particle : public Component
	{
		friend class GameObject;
	public:
		Particle();
		~Particle();

		void Initialize() override;
		void Release() override;

		void FixedUpdate() override;
		void Update() override;
		void LateUpdate() override;

		void OnTriggerEnter() override;
		void OnTriggerStay() override;
		void OnTriggerExit() override;

		void SetActive(bool active) override;

	private:
		// �׷��Ƚ��� ��ƼŬ ��ü
		GInterface::GraphicsParticle* _particle;

	public:
		// � ��ƼŬ�� ����� ���ΰ�
		void SetParticleEffect(std::string name, std::string fileName, unsigned int maxParticle);

		// ��ƼŬ ���� Ȥ�� �л� 
		void Start();

		// ��ƼŬ ����/�л縦 �ߴ�
		void Stop();

		// ��ƼŬ�� ó�� ��ġ���� �ٽ� �л�
		void Reset();

		// ��ƼŬ ��ġ
		void SetParticlePos(float x, float y, float z);

		// ��ƼŬ ũ�� ����
		void SetParticleSize(float x, float y);

		// ��ƼŬ ���� ���� �� ���Ⱒ���� �������� �Ұ����� ����
		void SetParticleVelocity(float velocity, bool isRandom);

		// ��ƼŬ�� fadeout �Ǵ� �ð� �� ������ ������� �ð� ����	// �� ���� ���� �ʿ� ����
		void SetParticleDuration(float fade, float life);

		// ��ƼŬ�� ���� ������ �Լ�	// �⺻������ ���� ������ ����
		// 0.0 ~ 1.0
		void AddParticleColor(float x, float y, float z);

		// ��ƼŬ ���� ���� ����
		void SetParticleDirection(float x, float y, float z);

	};
}

