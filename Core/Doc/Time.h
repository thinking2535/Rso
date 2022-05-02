#pragma once


#include "Core.h"
#include <chrono>
#include <sys/timeb.h>
#include <time.h>


namespace rso
{
    namespace core
    {
        using namespace std;
        using namespace chrono;

        struct SDateTime;
        class CTime
        {
        public:
            static const TInt c_Date_Null = -1;

            static const TInt c_MaxDaysOfYear = 366;
            static const TInt c_MaxDaysOfMonth = 31;
            static const TInt c_MaxCheckYear = 100;

            static const TInt c_SecondMSec = 1000;
            static const TInt c_MinuteMSec = 60000;
            static const TInt c_HourMSec = 3600000;
            static const TInt c_DayMSec = 86400000;
            static const TInt c_DaySec = 86400;
            static const TInt c_DayMin = 1440;
            static const TInt c_DayHour = 24;
            static const TInt c_DaysTo1900 = 693960;

        private:
            TInt m_Date;
            TInt m_Time;

            TInt _GetY(TInt& rRemain_o, bool& rbLeapYear_o) const;
            TInt _GetM(bool bLeapYear_, TInt& rRemain_io) const;
            void _RefreshOverDay(void);

        public:
            static bool IsLYear(TInt Y_);
            static TInt GetLDay(TInt Y_, TInt M_);
            static bool IsValidYear(TInt Y_);
            static bool IsValidMonth(TInt M_);
            static bool IsValidDay(TInt Y_, TInt M_, TInt D_);
            static bool IsValidDate(TInt Y_, TInt M_, TInt D_);
            static bool IsValidHour(TInt H_);
            static bool IsValidMinute(TInt Mn_);
            static bool IsValidSecond(TInt S_);
            static bool IsValidMSecond(TInt Ms_);
            static bool IsValidTime(TInt H_, TInt Mn_, TInt S_, TInt Ms_);
            static bool IsValidDateTime(TInt Y_, TInt M_, TInt D_, TInt H_, TInt Mn_, TInt S_, TInt Ms_);
            static TInt GetDaysToMonthOfYear(TInt Year_, TInt Month_, TInt Day_);
            static bool GetValidFutureFromNowByYear(TInt& Y_, TInt M_, TInt D_);
            static bool GetValidFutureFromNowByMonth(TInt& Y_, TInt& M_, TInt D_);
            static bool GetValidFutureFromNowByDay(TInt& Y_, TInt& M_, TInt& D_);
            CTime();
            CTime(nowtime_t, TInt Month_, TInt Day_, TInt Hour_, TInt Minute_, TInt Second_);
            CTime(nowtime_t, TInt Day_, TInt Hour_, TInt Minute_, TInt Second_);
            CTime(nowtime_t, EDayOfWeek DayOfWeek_, TInt Hour_, TInt Minute_, TInt Second_);
            CTime(nowtime_t, TInt Hour_, TInt Minute_, TInt Second_);
            CTime(nowtime_t, TInt Minute_, TInt Second_);
            CTime(nowtime_t, TInt Second_);
            CTime(nowtime_t);
            CTime(const TInt DateValue_, const TInt TimeValue_);
            CTime(TInt Year_, TInt Month_, TInt Day_, TInt Hour_, TInt Minute_, TInt Second_, TInt MSecond_ = 0);
            CTime(time_t TimeT_);

            bool operator == (const CTime& rVal_) const;
            bool operator != (const CTime& rVal_) const;
            bool operator < (const CTime& rVal_) const;
            bool operator > (const CTime& rVal_) const;
            bool operator <= (const CTime& rVal_) const;
            bool operator >= (const CTime& rVal_) const;
            time_t ToTimeT(void) const;
            bool IsNull(void) const { return (m_Date == c_Date_Null); }
            void SetNull(void) { m_Date = c_Date_Null; }
            void SetNow(void);
            TInt GetDays(void) const { return m_Date; }
            TInt GetDaySeconds(void) const { return (m_Time / c_SecondMSec); }
            TInt GetDayMSeconds(void) const { return m_Time; }
            bool SetDateTime(TInt Year_, TInt Month_, TInt Day_, TInt Hour_, TInt Minute_, TInt Second_, TInt MSecond_ = 0);
            bool SetDate(TInt Year_, TInt Month_, TInt Day_);
            bool SetDate(TInt Date_);
            bool SetTime(TInt Hour_, TInt Minute_, TInt Second_, TInt MSecond_ = 0);
            bool SetTime(TInt Time_);
            TInt GetYear(void) const;
            TInt GetMonth(void) const;
            TInt GetDay(void) const;
            TInt GetHour(void) const;
            TInt GetMinute(void) const;
            TInt GetSecond(void) const;
            TInt GetMSecond(void) const;
            TInt GetWeek(void) const; // 몇번째 주인가?
            TInt GetFullWeek(void) const; // 몇번째 주인가? ( 만으로 )
            EDayOfWeek GetDayOfWeek(void) const;
            bool IsLeapYear(void) const;
            bool IsLastDay(void) const;
            bool IsLastWeek(void) const;
            bool IsLastFullWeek(void) const;
            TInt GetLastDay(void) const;
            TInt GetAge(void) const;
            TInt64 GetTotalSec(void) const { return ((TInt64)m_Date*c_DaySec + (TInt64)m_Time / 1000); }
            bool SetNextYear(void); // 현재 Month, Day 를 가지는 다음 Year
            bool SetNextMonth(void); // 현재 Day 를 가지는 다음 Month
            void AddDay(TInt Day_ = 1);
            void AddHour(TInt Hour_ = 1);
            void AddMinute(TInt Minute_ = 1);
            void AddSecond(TInt Second_ = 1);
            void AddMSecond(TInt MSecond_ = 1);
            SDateTime ToDateTime(void) const;
            tm ToTM(void) const;
            wstring ToString(const wchar_t* pFormat_ = L"%Y-%m-%d %H:%M:%S") const;
        };

        struct SDateTime
        {
            TSqlSmallInt year{ 1900 };
            TSqlUSmallInt month{ 1 };
            TSqlUSmallInt day{ 1 };
            TSqlUSmallInt hour{ 0 };
            TSqlUSmallInt minute{ 0 };
            TSqlUSmallInt second{ 0 };
            TSqlUInt fraction{ 0 };

            SDateTime();
            SDateTime(
                TSqlSmallInt Year_,
                TSqlUSmallInt Month_,
                TSqlUSmallInt Day_,
                TSqlUSmallInt Hour_,
                TSqlUSmallInt Minute_,
                TSqlUSmallInt Second_,
                TSqlUInt MSecond_ = 0);
            SDateTime(const TTime& TimePoint_);
            explicit SDateTime(const CTime& Time_);
            SDateTime& operator = (const CTime& Time_);
            bool operator == (const SDateTime& rTS_);
            bool operator != (const SDateTime& rTS_);
            bool operator < (const SDateTime& TS_) const;
            bool operator > (const SDateTime& TS_) const;
            bool operator <= (const SDateTime& TS_) const;
            bool operator >= (const SDateTime& TS_) const;
            bool IsNull(void) const;
            CTime ToTime(void) const;
            time_t ToTimeT(void) const;
            TTime ToTimePoint(void) const;
            tm ToTM(void) const;
            bool IsSameDay(const SDateTime& TimeStamp_) const;
            SDateTime AddMonth(TSqlUSmallInt Month_) const;
            __int32 GetMonthDiff(const SDateTime& Subtrahend_) const;
            TSqlUSmallInt GetFullWeek(void) const;
            wstring ToString(const wchar_t* pFormat_ = L"%Y-%m-%d %H:%M:%S") const;
        };
    }
}
