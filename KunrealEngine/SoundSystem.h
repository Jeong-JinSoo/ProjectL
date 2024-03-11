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
		virtual ~Sound() {}
		std::string fileName;
		int volume = 0; // 0~100
		IDirectSoundBuffer8* soundBuffer = nullptr;
		IDirectSound3DBuffer8* sound3DBuffer = nullptr;
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
		int AddSound(std::string filename, int volume);
		int Add3DSound(std::string filename, int volume, int xpos = 0, int ypos = 0, int zpos = 0);
		void RemoveSound(int index);
		void ClearAllSound();
		void Setvolume(int index, int volume);

		//3D ���忡�� ��ġ ���� ����
		void updateSoundPosition(int index, float x, float y, float z);	// ������ ��ġ�� ����
		void updateListenerPosition(float x, float y, float z);			// ������(û��)�� ��ġ�� ����

		// ���� ��� ����
		void Play(int index);											// ���
																		// 3D ������ ��� ���������� ������ �ִ� ��ġ���� ���
		void PlayWithPos(int index, int xpos, int ypos, int zpos);		// ���� ��������� �Ҹ��� ��ġ�� �Է¹��� ���������� �̵�
		void Loop(int index);											// �Ҹ��� �ݺ��Ͽ� ���
		void Pause(int index);											// �Ҹ��� �Ͻ�����
		void Stop(int index);											// �Ҹ��� ����

	private:
		SoundSystem();
		SoundSystem(const SoundSystem& soundClass);
		~SoundSystem();

		bool LoadWaveFile(std::string filename, IDirectSoundBuffer8** secondaryBuffer, bool is3D = false);
		bool Load3DWaveFile(std::string filename, IDirectSoundBuffer8** secondaryBuffer, IDirectSound3DBuffer8** secondary3DBuffer);
		void TerminateWaveFile(Sound* index);
		int FindIndex(const std::vector<Sound*>& vec, Sound* value);

	private:
		IDirectSound8* _directSound = nullptr;
		IDirectSoundBuffer* _primaryBuffer = nullptr;
		IDirectSound3DListener8* _listener = nullptr;
		std::vector<Sound*> _soundBuffer;
	};
}