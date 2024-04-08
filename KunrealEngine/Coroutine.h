// ���� ����ű� ������ �ʹ� ������!!!!!!!
// �ڷ�ƾ
#pragma once
#include <coroutine>
#include <functional>
#include <vector>
#include <map>

// ��� �� ���� �̰͸� ����ص� ��.
// https://www.notion.so/60508ce780814fd2a9e449ea16831a17 ���� ������ �ξ����� ����.

#define Waitforsecond(time) co_await KunrealEngine::Coroutine::Coroutine_type::WaitForSeconds(time)
#define Coroutine_Func(coro_func) std::function<KunrealEngine::Coroutine::Coroutine_type()> coro_func = [this]() ->KunrealEngine::Coroutine::Coroutine_type
#define Startcoroutine(coro_func) Coroutine::StartCoroutine(coro_func)
#define Updatecoroutine Coroutine::UpdateCoroutines

namespace KunrealEngine
{
	// ���� �ϱ����� ��ü. �����⿡ �ش� �ϴ� �κ�.
	// �ڷ�ƾ�� �����ϰ� �����ϱ� ���� �κ����� �����̴�.
	class Coroutine
	{
	public:
		Coroutine();
		~Coroutine();

		// �̰� �̱��� ��ü
		// �ʿ��� �μ��߿� duration�� �ִµ� �ð��� ���⿡ ��´�.
		// ������ �Ʒ� ������ �޴� �κ��̶� �װ� Ȯ���ϴ� �κ��� �޶�
		//  �Ѵ� �� �� �ְ� �Ϸ��� ���� �ʿ�������.
		class DurationManager
		{
		public:
			static DurationManager& getInstance();
			float getDuration() const;
			void setDuration(float newDuration);

		private:
			float duration = 0;
		};

		// �ڷ�ƾ�� ��ü
		struct Coroutine_type
		{
			struct promise_type	// promise_type�� �⺻������ coroutine ��ü�� �����ϱ� ���� �ʿ��� ��ü.
								// �갡 ������ �ڷ�ƾ���� �ν��� ���ϴ���
			{
				Coroutine_type get_return_object();
				std::suspend_never initial_suspend() noexcept;	// �� �־�� �ϴ� �Լ�
				std::suspend_always final_suspend() noexcept;	// �� �־�� �ϴ� �Լ�
				void return_void() noexcept;					// �� �־�� �ϴ� �Լ�
				void unhandled_exception() noexcept;			// �� �־�� �ϴ� �Լ�
				bool await_ready() noexcept;					// �� �־�� �ϴ� �Լ�

				float timer = 0;
				float duration = 0;
			};


			class WaitForSeconds
			{
			public:
				WaitForSeconds(float seconds);						// �ڷ�ƾ�� co_await�� ���� ���ؼ� �� ��ü�� �־�� �ϴ���.
				bool await_ready();
				void await_suspend(std::coroutine_handle<> handle);
				void await_resume() const noexcept;
			};

			Coroutine_type();
			Coroutine_type(std::coroutine_handle<promise_type> handle); /*: coro_handle(handle)*/
			~Coroutine_type();

			std::coroutine_handle<promise_type> coro_handle;
			int mapKey = 0;
		};

		static void StartCoroutine(std::function<Coroutine_type()> coro);	// �ڷ�ƾ �Լ��� �޾Ƽ� ���۽�Ŵ
		static void UpdateCoroutines();	// �ڷ�ƾ �Լ����� ������Ʈ ������

	private:
		static std::vector<Coroutine_type*> _coroutines;	// �ڷ�ƾ���� ��Ƶδ� ��
		static std::map<int, std::function<Coroutine_type()>> _AddedCoroutines; // �ڷ�ƾ�� �ּҸ� �����ϴ� ����
		static int idexKey;
	};
}