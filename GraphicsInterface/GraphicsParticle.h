#pragma once

namespace GInterface
{
	class GraphicsParticle
	{
		virtual ~GraphicsParticle() {};

	public:
		// ��ƼŬ ����
		virtual void play() abstract;
		// ��ƼŬ ����
		virtual void Pause() abstract;
		// ��ƼŬ ����
		virtual void Stop() abstract;

	public:
		// ��ƼŬ ������ ��������
		virtual DirectX::XMFLOAT2 GetSize() abstract;
		// ��ƼŬ �ӵ� ��������
		virtual float GetVelocity() abstract;
		// ��ƼŬ ���� ���� ��������
		virtual bool GetRandom() abstract;

	public:
		// ��ƼŬ ������ ����
		virtual void SetSize(float x, float y) abstract;
		// ��ƼŬ �ӵ� ����
		virtual void SetVelocity(float speed) abstract;
		// ��ƼŬ ������ �ο� ���� ����
		virtual void SetRandom(bool tf) abstract;
	};
}