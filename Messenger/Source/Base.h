#pragma once


#include <Rso/Base/Base.h>
#include <functional>


namespace rso
{
	namespace messenger
	{
		using namespace base;
		using namespace std;

		struct SFriend
		{
			wstring ID;
			wstring Nick;
		};

		using TCallbackAdd = function<void(const SFriend& Friend_)>;
		using TCallbackDel = function<void(const wstring& ID_)>;
		using TCallbackMsg = function<void(const wstring& ID_, const wstring& Msg_)>;

		enum class ERet
		{
			Ok,
			Max,
			Null
		};
	}
}