#pragma once

#include "Base.h"
#include <Rso/MobileUtil/ReceiptCheck.h>
#include <Rso/MSSQL/StoredProcedure.h>

namespace rso
{
	namespace gamebill
	{
		using namespace mobileutil;
		using namespace mssql;

		class CBill
		{
			using _TSP = CStoredProcedure<TUID>;

		public:
			using TOrder = CReceiptCheck::SOrder;
			using TOutObject = typename _TSP::SOutObject;
			using TDBCallback = _TSP::TCallback;

		private:
			_TSP _DB;

			inline constexpr size_t _GetOrderIDLengthMax(void) const { return 50; }
			inline constexpr size_t _GetCountryCodeLengthMax(void) const { return 2; }
			inline constexpr size_t _GetProductIDLengthMax(void) const { return 50; }
		public:
			CBill(const SDBOption& DBOption_, TDBCallback Callback_);
			void Start(void);
			template<typename... TParams>
			bool Push(const SPurchaseIn& In_, const TParams&... Params_)
			{
				return _DB.Push<SPurchaseIn>(0, In_.UID, In_, Params_...);
			}
			void Proc(void);
		};
	}
}
