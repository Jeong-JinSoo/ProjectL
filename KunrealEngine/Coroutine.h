#pragma once
#include <coroutine>
#include <vector>
#include <map>

#define Waitforsecond(time) co_await KunrealEngine::Coroutine::Coroutine_type::WaitForSeconds(time)
#define Coroutine_Func KunrealEngine::Coroutine::Coroutine_type
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
			struct promise_type
			{
				Coroutine_type get_return_object();
				std::suspend_never initial_suspend() noexcept;	// �� �־�� �ϴ� �Լ�
				std::suspend_always final_suspend() noexcept;	// �� �־�� �ϴ� �Լ�
				void return_void() noexcept;
				void unhandled_exception() noexcept;
				bool await_ready() noexcept;

				float timer = 0;
				float duration = 0;
			};


			class WaitForSeconds
			{
			public:
				WaitForSeconds(float seconds);
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

		static void StartCoroutine(Coroutine_type(*coro)());
		static void UpdateCoroutines();

	private:
		static std::vector<Coroutine_type*> _coroutines;
		static std::map<int, void*> _AddedCoroutines; // �ڷ�ƾ�� �ּҸ� �����ϴ� ����
		static int idexKey;
	};
}