#include "Coroutine.h"
#include "TimeManager.h"

namespace KunrealEngine
{
	std::vector<KunrealEngine::Coroutine::Coroutine_type*> KunrealEngine::Coroutine::_coroutines;
	std::map<int, std::function<KunrealEngine::Coroutine::Coroutine_type()>> KunrealEngine::Coroutine::_AddedCoroutines; // �ڷ�ƾ�� �ּҸ� �����ϴ� ����
	int KunrealEngine::Coroutine::idexKey;

	Coroutine::Coroutine()
	{

	};

	Coroutine::~Coroutine()
	{

	};

	/////////////////////////////////////////
	/////////////DurationManager/////////////
	/////////////////////////////////////////

	Coroutine::DurationManager& Coroutine::DurationManager::getInstance()
	{
		static DurationManager instance;
		return instance;
	}

	float Coroutine::DurationManager::getDuration() const
	{
		return duration;
	}

	void Coroutine::DurationManager::setDuration(float newDuration)
	{
		duration = newDuration;
	}

	void Coroutine::DurationManager::durationtonull()
	{
		duration = 0;
	}

	/////////////////////////////////
	/////////////Coroutine///////////
	/////////////////////////////////

	Coroutine::Coroutine_type::Coroutine_type()
	{

	}

	Coroutine::Coroutine_type::Coroutine_type(std::coroutine_handle<promise_type> handle) : coro_handle(handle)
	{

	}

	Coroutine::Coroutine_type::~Coroutine_type()
	{
		coro_handle.destroy();
	}

	////////////////////////////////////
	/////////////promise_type///////////
	////////////////////////////////////

	Coroutine::Coroutine_type Coroutine::Coroutine_type::promise_type::get_return_object()
	{
		return Coroutine_type(std::coroutine_handle<promise_type>::from_promise(*this));
	}

	std::suspend_never Coroutine::Coroutine_type::promise_type::initial_suspend() noexcept
	{
		return {};
	}

	std::suspend_always Coroutine::Coroutine_type::promise_type::final_suspend() noexcept
	{
		return {};
	}

	void Coroutine::Coroutine_type::promise_type::return_void() noexcept
	{

	}

	void Coroutine::Coroutine_type::promise_type::unhandled_exception() noexcept
	{

	}

	bool Coroutine::Coroutine_type::promise_type::await_ready() noexcept
	{
			timer += TimeManager::GetInstance().GetDeltaTime();

			if (timer > duration)
			{
				timer = 0;
				return true;
				DurationManager::getInstance().durationtonull();
			}

			else
			{
				return false;
			}

		return true;
	}

	//////////////////////////////////////
	/////////////WaitForSeconds///////////
	//////////////////////////////////////

	// WaitForSeconds ����
	Coroutine::Coroutine_type::WaitForSeconds::WaitForSeconds(float seconds)
	{
		DurationManager::getInstance().setDuration(seconds);
	}

	bool Coroutine::Coroutine_type::WaitForSeconds::await_ready()
	{
		return false;
	}

	void Coroutine::Coroutine_type::WaitForSeconds::await_suspend(std::coroutine_handle<> handle)
	{

	}

	void Coroutine::Coroutine_type::WaitForSeconds::await_resume() const noexcept {}

	////////////////////////////////////////
	/////////////////CheckBool//////////////
	////////////////////////////////////////

	Coroutine::Coroutine_type::CheckBool::CheckBool(bool& param)
		:flag(param)
	{

	}
	bool Coroutine::Coroutine_type::CheckBool::await_ready()
	{
		return false;
	}
	void Coroutine::Coroutine_type::CheckBool::await_suspend(std::coroutine_handle<> handle)
	{
		if (flag)
		{
			handle.resume();
		}
	}

	void Coroutine::Coroutine_type::CheckBool::await_resume() const noexcept
	{

	}

	////////////////////////////////////////
	/////////////////ReturnNull//////////////	
	////////////////////////////////////////

	Coroutine::Coroutine_type::ReturnNull::ReturnNull()
	{

	}

	bool Coroutine::Coroutine_type::ReturnNull::await_ready()
	{
		return false;
	}

	void Coroutine::Coroutine_type::ReturnNull::await_suspend(std::coroutine_handle<> handle)
	{
		
	}

	void Coroutine::Coroutine_type::ReturnNull::await_resume() const noexcept
	{
	}

	//////////////////////////////////////
	/////////////StartCoroutine///////////
	//////////////////////////////////////

	void Coroutine::StartCoroutine(std::function<Coroutine_type()> coro)
	{
		// �̹� �߰��� �ڷ�ƾ���� Ȯ��
		for (auto& coron : _AddedCoroutines)
		{
			if (coron.second.target_type() == coro.target_type())
			{
				return;
			}
		}

		Coroutine_type* coroutineInstance = new Coroutine_type(coro());
		_coroutines.emplace_back(coroutineInstance);
		idexKey++;
		coroutineInstance->mapKey = idexKey;
		coroutineInstance->duration = DurationManager::getInstance().getDuration();
		DurationManager::getInstance().durationtonull();

		_AddedCoroutines.insert(std::make_pair(idexKey, coro));
	}

	////////////////////////////////////////
	/////////////UpdateCoroutines///////////
	////////////////////////////////////////

	void Coroutine::UpdateCoroutines()
	{
		for (auto& coroutine : _coroutines)
		{
			coroutine->coro_handle.promise().duration = coroutine->duration;
			if (!coroutine->coro_handle.done() && coroutine->coro_handle.promise().await_ready())
			{
				// �ڷ�ƾ�� �Ϸ���� �ʰ�, ������ �ð��� ������� ���� ��� resume���� ����
				coroutine->coro_handle.resume();
			}

			else if (coroutine->coro_handle.done())
			{
				_AddedCoroutines.erase(coroutine->mapKey);

				auto iter = std::find(_coroutines.begin(), _coroutines.end(), coroutine);
				delete coroutine;
				coroutine = nullptr;
				_coroutines.erase(iter); // ���Ϳ��� ����
			}
		}
	}
}