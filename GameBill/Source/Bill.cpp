#include "Bill.h"

namespace rso
{
	namespace gamebill
	{
		CBill::CBill(const SDBOption& DBOption_, TDBCallback Callback_) :
			_DB(DBOption_, Callback_)
		{
			CMSSQL MSSQL(DBOption_);

			if (!MSSQL.Connect())
				THROWEX();

			const wstring TableNameReceipt = L"sys_tbl_Receipt";
			const wstring SPNamePurchase = L"sys_sp_Purchase";
			wstring Query;

			////////////////////////////////////////////////////////////////////////////////////////////////////////////
			// CREATE Table If not exists
			Query = LR"(
IF NOT EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[)" + TableNameReceipt + LR"(]') AND type in (N'U'))
BEGIN
CREATE TABLE [dbo].[)" + TableNameReceipt + LR"(](
	[UID] [bigint] NOT NULL,
	[OrderID] [varchar]()" + to_wstring(_GetOrderIDLengthMax()) + LR"() NOT NULL,
	[OS] [tinyint] NOT NULL,
	[CountryCode] [char]()" + to_wstring(_GetCountryCodeLengthMax()) + LR"() NOT NULL,
	[PurchaseTime] [bigint] NOT NULL,
	[PurchaseType] [int] NOT NULL,
	[ProductID] [varchar]()" + to_wstring(_GetProductIDLengthMax()) + LR"() NOT NULL,
	[Stream] [varbinary](max) NOT NULL,
 CONSTRAINT [PK_)" + TableNameReceipt + LR"(] PRIMARY KEY CLUSTERED 
(
	[UID] ASC,
	[OrderID] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
) ON [PRIMARY] TEXTIMAGE_ON [PRIMARY]
END)";
			if (!SQL_SUCCEEDED(MSSQL.ExecuteDirect(Query)))
				THROWEX();

			////////////////////////////////////////////////////////////////////////////////////////////////////////////
			// CREATE SP If not exists
			Query = LR"(
IF NOT EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[)" + SPNamePurchase + LR"(]') AND type in (N'P', N'PC'))
BEGIN
EXEC dbo.sp_executesql @statement = N'CREATE PROCEDURE [dbo].[)" + SPNamePurchase + LR"(] AS' 
END)";
			if (!SQL_SUCCEEDED(MSSQL.ExecuteDirect(Query)))
				THROWEX();

			// ALTER
			Query = LR"(
ALTER PROCEDURE [dbo].[)" + SPNamePurchase + LR"(]
	@UID_ BIGINT
,	@OrderID_ VARCHAR()" + to_wstring(_GetOrderIDLengthMax()) + LR"()
,	@OS_ TINYINT
,	@CountryCode_ CHAR()" + to_wstring(_GetCountryCodeLengthMax()) + LR"()
,	@PurchaseTime_ BIGINT
,	@PurchaseType_ INT
,	@ProductID_ VARCHAR()" + to_wstring(_GetProductIDLengthMax()) + LR"()
,	@Stream_ VARBINARY(MAX)
AS
BEGIN
	SET XACT_ABORT ON
	SET NOCOUNT ON

	INSERT	dbo.)" + TableNameReceipt + LR"( ([UID], OrderID, OS, CountryCode, PurchaseTime, PurchaseType, ProductID, Stream)
	VALUES (@UID_, @OrderID_, @OS_, @CountryCode_, @PurchaseTime_, @PurchaseType_, @ProductID_, @Stream_)

	RETURN 0

END)";

			_DB.AddCmd<SPurchaseIn, SPurchaseOut>(0, L"[dbo]." + SPNamePurchase, true);
		}
		void CBill::Start(void)
		{
			_DB.Start(5);

			while (!_DB.Connected())
				this_thread::sleep_for(milliseconds(100));
		}
		void CBill::Proc(void)
		{
			_DB.Proc();
		}
	}
}
