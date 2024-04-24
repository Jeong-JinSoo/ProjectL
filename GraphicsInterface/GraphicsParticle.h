#pragma once
#include <string>

namespace GInterface
{
	class GraphicsParticle
	{
	public:
		GraphicsParticle() {};
		virtual ~GraphicsParticle() {};

	public:
		// ��ƼŬ ����
		virtual void Start() abstract;
		// ��ƼŬ ���� ����
		virtual void Stop() abstract;

	public:
		// ��ƼŬ�� ��ġ ����
		virtual void SetEmitPos(const DirectX::XMFLOAT3& emitPos) abstract;
		// ��ƼŬ�� ũ�� ����
		virtual void SetParticleSize(const DirectX::XMFLOAT2& particleSize) abstract;
		// ��ƼŬ�� ���� �ر�� �������� ����
		virtual void SetEmitVelocity(float emitVelocity, bool isRandom) abstract;
		// ��ƼŬ�� ���� �� �������� �ٷ����� �ð�(Fade)�� �����Ҹ�ð�(Life) ����
		virtual void SetParticleTime(float particleFadeTime, float particleLifeTime) abstract;
		// ��ƼŬ�� ���� ����(RGB)
		virtual void SetParticleColor(const DirectX::XMFLOAT3& particleColor) abstract;
		// ��ƼŬ�� ���� ���� ����
		virtual void SetParticleDirection(const DirectX::XMFLOAT3& particleDirection) abstract;

		// ��ƼŬ�� �ٽ� 0���� �����ϰ� �����
		virtual void Reset() abstract;
	};
}