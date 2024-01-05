#pragma once
#include "CommonHeader.h"
#include <chrono>

/// <summary>
/// �ð��� �����ϴ� Ŭ����
/// 
/// QueryPerformanceCounter�� �̿��� ������� chrono ���̺귯���� �̿��� ����� �����
/// -> QPC�� ���� ���е��� ���������� �÷����� ���� ��� ����� �޶� �̽ļ��� ������
/// => chrono������ QPC�� ����� ���� ������ ���� �����̶� �� �� �ִ�
/// 
/// �� ������ ����ȣȯ�� �ƴϰ�, ���� ���α׷��� �ٷ�� ��쿡�� QPC�� ������ ���� �ִ�
/// 
/// �̰�ȣ
/// </summary>

namespace KunrealEngine
{
	class TimeManager
	{
	private:
		TimeManager();
		~TimeManager();

		// �̱���
	public:
		static TimeManager& GetInstance()
		{
			static TimeManager _instance;
			return _instance;
		}

	public:
		void Initialize();
		void Finalize();
		void Update();

		//Ŭ���̾�Ʈ�� �Ѱ��� �Լ���
	public:
		// ��ŸŸ���� �������ִ� �Լ�
		float GetDeltaTime();

		// Ÿ�ӽ������� �����ϴ� �Լ�	0���ϴ� �Ұ���
		void SetTimeScale(float scale);

		// �� �Լ��� ���� ������Ʈ�� ���� �ٸ� Ÿ�ӽ������� ������ �� �� ����
		float GetTimeScale();
	private:
		std::chrono::high_resolution_clock::time_point m_startTime;		// Ÿ�̸��� ���۽ð�
		std::chrono::high_resolution_clock::time_point m_prevTime;		//
		std::chrono::high_resolution_clock::time_point m_currentTime;	// ��ŸŸ���� ����ϱ� ����, �ð� �� ����� ���� ������

		float m_deltaTime;			// ��ŸŸ��
		float m_timeScale;			// Ÿ�ӽ����� Ȥ�� ������Ʈ�� ������ �帣�� �ð��� �ٸ��� �ְ� ���� ���� ������	
	};
}

