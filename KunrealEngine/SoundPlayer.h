#pragma once
#include "CommonHeader.h"
#include "Component.h"

/// <summary>
/// ���� ������ ������ְ� ��� ���θ� �������ִ� ������Ʈ
/// 
/// ���� ���带 ����� �� �ִµ�, ������ ����ڴ� � ������ ������Ʈ�� ���° �������� �˰� �־����
/// �����͸� ���� Ȯ���ϸ� ū ���� ������
/// </summary>

namespace KunrealEngine
{
	class _DECLSPEC SoundPlayer : public Component
	{
		friend class GameObject;
	private:
		SoundPlayer();
	public:
		~SoundPlayer();

		virtual void Initialize() override;
		virtual void Release() override;

		virtual void FixedUpdate() override;
		virtual void Update() override;
		virtual void LateUpdate() override;

		virtual void OnTriggerEnter() override;
		virtual void OnTriggerStay() override;
		virtual void OnTriggerExit() override;

		virtual void SetActive(bool active) override;

	public:
		// �Ҹ� ��� ������ ���� ����ü
		struct SoundInfo
		{
			std::string _soundPath;			// ���� ���� ���
			bool _is3DSound;				// 3D ��������					// �⺻�� false
			bool _isLoop;					// �ݺ��Ǵ� ��������			// �⺻�� false
			int _volume;
			int _soundIndex;				// SoundSystem ���ο��� �ٷ�� index	// �� ������Ʈ�� index�ʹ� �ٸ��� ����
			unsigned int _innerIndex;
		};

	public:
		int _createdCnt;					// ���带 � ���������
		std::vector<SoundInfo> _soundList;	// ���� ������ ���� �����̳�

	public:
		void CreateSoundInfo(std::string soundFile, bool is3D = false, bool isLoop = true, int volume = 100);

		// ���带 ������ ��� ���� ���� index�� ��������Ѵ�
		// �ϳ��� ���� ��� ������ �Ű�������
		void SetVolume(int vol, int index = -1);

		// �Է��� �ε����� �ش��ϴ� ���带 ���		// ù��° ����� 0�� �ε���
		void Play(int index);

		// �Է��� �ε����� �ش��ϴ� ���带 �Ͻ�����
		void Pause(int index);

		// �Է��� �ε����� �ش��ϴ� ���带 ����
		void Stop(int index);

		// ��� ���� ��� ����
		void StopAll();

	};
}

