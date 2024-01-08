#pragma once
#include <windows.h>
#include <mmsystem.h>
#include <dsound.h>
#include <stdio.h>
#include <assert.h>

#include <vector>
#include <string>

#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "dxguid")
#pragma comment(lib, "winmm.lib")

namespace KunrealEngine
{
	struct WaveHeaderType
	{
		char chunkId[4];
		unsigned long chunkSize;
		char format[4];
		char subChunkId[4];
		unsigned long subChunkSize;
		unsigned short audioFormat;
		unsigned short numChannels;
		unsigned long sampleRate;
		unsigned long bytesPerSample;
		unsigned short blockAlign;
		unsigned short bitsPerSample;
		char dataChunkId[4];
		unsigned long dataSize;
	};


	struct Sound
	{
		std::string soundName;
		std::string fileName;
		IDirectSoundBuffer8* soundBuffer = nullptr;
		int volume = 0; // 0~100
	};

	class SoundSystem
	{
	public:
		// �̱��� ������ ��ȯ
		static SoundSystem& GetInstance();

		//���� �ʱ�ȭ
		bool Initialize(HWND hWnd);
		void Terminate();

		//���� ����
		void AddSound(std::string filename, std::string soundname, int volume);
		void RemoveSound(int index);
		void ClearAllSound();
		void Setvolume(int index, int volume);

		//���� ���Ϳ��� ã�°� ���° �ε������� ã��
		int FindIndexOfSound(std::string soundname);
		//�׳� ���� ��ü�� ��������
		Sound& FindSound(std::string soundname);

		// ���� ��� ����
		void Play(int index);
		void Loop(int index);
		void Pause(int index);
		void Stop(int index);

	private:
		SoundSystem();
		SoundSystem(const SoundSystem& soundClass);
		~SoundSystem();

		bool LoadWaveFile(std::string filename, IDirectSoundBuffer8** secondaryBuffer);
		void TerminateWaveFile(Sound* index);

	private:
		IDirectSound8* _directSound = nullptr;
		IDirectSoundBuffer* _primaryBuffer = nullptr;
		std::vector<Sound*> _soundBuffer;
	};
}
