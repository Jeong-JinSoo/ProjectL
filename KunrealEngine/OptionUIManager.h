// �ɼ�UI�� ��Ʈ�� �ϱ� ���� ������Ʈ...���� �갡 �� ���� �Ҷ� ����⵵ �Ұž�.
// ������ UI�� ��ư��� ����� ���� �ý����� ���� ���ִ°�.
// �ϴ�... bgm��ư �迭�� sfx��ư �迭�� �ʿ��Ұ� ����.
#pragma once
#include "CommonHeader.h"
#include "KunrealAPI.h"

namespace KunrealEngine
{
	class OptionUIManager : public Component
	{
	public:
		OptionUIManager();
		~OptionUIManager();

		// Component�� �����Լ��� �������̵�
		void Initialize() override;
		void Release() override;

		void FixedUpdate() override;
		void Update() override;
		void LateUpdate() override;

		void OnTriggerEnter() override;
		void OnTriggerStay() override;
		void OnTriggerExit() override;

		void SetActive(bool active) override;

		void Setvolumebutton(int vol, int* type);

	private:
		// ���õ� ����. 0~10���� �̼��ڱ����� ��ư�� ���� ĥ�ϰ� �̼��� ������ ��ư�� ȸ������ �д�.
		// ����ϰ� ���嵵 �̰Ŷ� ���� ���� ���� -> SetVolume(bgm_step*10)
		// 0���� ����ϹǷ� �⺻���� -1�� �Ѱ�.
		int _bgm_step = -1;
		int _sfx_step = -1;

		// ���� ������ ��ư���� ���⿡ �����ϰ� ��Ƶα�
		std::vector<GameObject*> _bgms;
		std::vector<GameObject*> _sfxs;
	};
}