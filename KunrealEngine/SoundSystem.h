#pragma once
#include <windows.h>
#include <mmsystem.h>
#include <dsound.h>
#include <stdio.h>
#include <assert.h>

#include <vector>
#include <string>

#include "CommonHeader.h"

#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "dxguid")
#pragma comment(lib, "winmm.lib")

namespace KunrealEngine
{
	enum class SOUNDTYPE
	{
		BGM = 0,
		SFX = 1
	};

	struct _DECLSPEC WaveHeaderType
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

	struct _DECLSPEC Sound
	{
		virtual ~Sound() {}
		std::string fileName;
		int volume = 0; // 0~100
		IDirectSoundBuffer8* soundBuffer = nullptr;
		IDirectSound3DBuffer8* sound3DBuffer = nullptr;
		SOUNDTYPE type = SOUNDTYPE::SFX;
	};

	class _DECLSPEC SoundSystem
	{
	public:
		// �̱��� ������ ��ȯ
		static SoundSystem& GetInstance();

		//���� �ʱ�ȭ
		bool Initialize(HWND hWnd);
		void Release();

		//���� ����
		// �̰ɷε� 3D ���� �߰� ����. �ٸ� �Ҹ��� 0,0,0�� ����
		int AddSound(std::string filename, int volume, SOUNDTYPE type = SOUNDTYPE::SFX, int index = -1);
		int Add3DSound(std::string filename, int volume, SOUNDTYPE type = SOUNDTYPE::SFX
			, int index = -1, int xpos = 0, int ypos = 0, int zpos = 0);	// ���� ����. �Ҹ��� pos �� ���� ����
		void RemoveSound(int index);									// index�� �Ҹ��� �����.
		void ClearAllSound();											// �Ҹ��� ���δ� �����
		void Setvolume(int index, int volume);
		void SetvolumeGroup(SOUNDTYPE type, int volume);
		int Change3DorMono(int index);									// �ε����� �Ҹ��� 3D, Ȥ�� ���� �ٲ۴�.
		int GetSoundListSize();

		std::vector<std::string> GetSoundPathList();

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