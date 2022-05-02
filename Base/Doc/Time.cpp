#include "Time.h"
#include <ctime>


namespace rso
{
    namespace core
    {
        using namespace std;

        TInt CTime::_GetY(TInt& rRemain_o, bool& rbLeapYear_o) const
        {
            bool bLeapYear = true;
            TInt Year = ((m_Date + 693961) / 146097) * 400;
            TInt Remain = (m_Date + 693961) % 146097;

            if (Remain >= 36525)
            {
                Year += 100;
                Remain -= 36525;
                bLeapYear = false;

                if (Remain >= 36524)
                {
                    Year += (Remain / 36524) * 100;
                    Remain = Remain % 36524;
                }
            }

            if (bLeapYear == true)
            {
                if (Remain >= 1461)
                {
                    Year += (Remain / 1461) * 4;
                    Remain = Remain % 1461;
                }
            }
            else
            {
                if (Remain >= 1460)
                {
                    Year += 4;
                    Remain -= 1460;
                    bLeapYear = true;

                    if (Remain >= 1461)
                    {
                        Year += (Remain / 1461) * 4;
                        Remain = Remain % 1461;
                    }
                }
            }

            if (bLeapYear == true)
            {
                if (Remain >= 366)
                {
                    ++Year;
                    Remain -= 366;
                    bLeapYear = false;

                    if (Remain >= 365)
                    {
                        Year += (Remain / 365);
                        Remain = Remain % 365;
                    }
                }
            }
            else
            {
                if (Remain >= 365)
                {
                    Year += (Remain / 365);
                    Remain = Remain % 365;
                }
            }


            rRemain_o = Remain;
            rbLeapYear_o = bLeapYear;

            return Year;
        }
        TInt CTime::_GetM(bool bLeapYear_, TInt& rRemain_io) const
        {
            TInt Month = 1;
            if (rRemain_io >= 31) { ++Month; rRemain_io -= 31; }
            else return Month;

            TInt FebDay = bLeapYear_ ? 29 : 28;

            if (rRemain_io >= FebDay) { ++Month; rRemain_io -= FebDay; }
            else { return Month; }
            if (rRemain_io >= 31) { ++Month; rRemain_io -= 31; }
            else { return Month; }
            if (rRemain_io >= 30) { ++Month; rRemain_io -= 30; }
            else { return Month; }
            if (rRemain_io >= 31) { ++Month; rRemain_io -= 31; }
            else { return Month; }
            if (rRemain_io >= 30) { ++Month; rRemain_io -= 30; }
            else { return Month; }
            if (rRemain_io >= 31) { ++Month; rRemain_io -= 31; }
            else { return Month; }
            if (rRemain_io >= 31) { ++Month; rRemain_io -= 31; }
            else { return Month; }
            if (rRemain_io >= 30) { ++Month; rRemain_io -= 30; }
            else { return Month; }
            if (rRemain_io >= 31) { ++Month; rRemain_io -= 31; }
            else { return Month; }
            if (rRemain_io >= 30) { ++Month; rRemain_io -= 30; }
            else { return Month; }

            return Month;
        }
        void CTime::_RefreshOverDay(void)
        {
            if (m_Time >= c_DayMSec)
            {
                m_Date += (m_Time / c_DayMSec);
                m_Time %= c_DayMSec;
            }
            else if (m_Time < 0)
            {
                if (m_Time <= -c_DayMSec)
                {
                    m_Date += (m_Time / c_DayMSec);
                    m_Time %= c_DayMSec;
                }

                --m_Date;
                if (m_Date < 0)
                {
                    m_Date = 0;
                    m_Time = 0;
                    return;
                }
                m_Time += c_DayMSec;
                return;
            }
        }
        TInt CTime::GetLDay(TInt Y_, TInt M_)
        {
            switch (M_)
            {
            case 1: return 31;
            case 2:
                if (IsLYear(Y_) == true) return 29;
                return 28;
            case 3: return 31;
            case 4: return 30;
            case 5: return 31;
            case 6: return 30;
            case 7: return 31;
            case 8: return 31;
            case 9: return 30;
            case 10: return 31;
            case 11: return 30;
            case 12: return 31;
            default: return 0;
            }
        }
        bool CTime::IsLYear(TInt Y_)
        {
            if (Y_ % 400 == 0) return true;
            if (Y_ % 100 == 0) return false;
            if (Y_ % 4 == 0) return true;

            return false;
        }
        inline bool CTime::IsValidYear(TInt Y_)
        {
            return (Y_ >= 1900 && Y_ <= 9999);
        }
        inline bool CTime::IsValidMonth(TInt M_)
        {
            return (M_ >= 1 && M_ <= 12);
        }
        inline bool CTime::IsValidDay(TInt Y_, TInt M_, TInt D_)
        {
            return (D_ > 0 && D_ <= GetLDay(Y_, M_));
        }
        inline bool CTime::IsValidDate(TInt Y_, TInt M_, TInt D_)
        {
            return IsValidDay(Y_, M_, D_);
        }
        inline bool CTime::IsValidHour(TInt H_)
        {
            return (H_ >= 0 && H_ <= 23);
        }
        inline bool CTime::IsValidMinute(TInt Mn_)
        {
            return (Mn_ >= 0 && Mn_ <= 59);
        }
        inline bool CTime::IsValidSecond(TInt S_)
        {
            return (S_ >= 0 && S_ <= 59);
        }
        inline bool CTime::IsValidMSecond(TInt Ms_)
        {
            return (Ms_ >= 0 && Ms_ <= 999);
        }
        bool CTime::IsValidTime(TInt H_, TInt Mn_, TInt S_, TInt Ms_)
        {
            if (!IsValidHour(H_) ||
                !IsValidMinute(Mn_) ||
                !IsValidSecond(S_) ||
                !IsValidMSecond(Ms_))
                return false;

            return true;
        }
        bool CTime::IsValidDateTime(TInt Y_, TInt M_, TInt D_, TInt H_, TInt Mn_, TInt S_, TInt Ms_)
        {
            if (!IsValidDate(Y_, M_, D_) ||
                !IsValidTime(H_, Mn_, S_, Ms_))
                return false;

            return true;
        }
        TInt CTime::GetDaysToMonthOfYear(TInt Year_, TInt Month_, TInt Day_)
        {
            TInt DOY = 0;
            for (TInt m = 1; m < Month_; ++m)
                DOY += GetLDay(Year_, m);

            DOY += Day_;

            return DOY;
        }
        bool CTime::GetValidFutureFromNowByYear(TInt& Y_, TInt M_, TInt D_)
        {
            auto ToYear = Y_ + c_MaxCheckYear;

            for (; Y_ < ToYear; ++Y_)
            {
                if (IsValidDate(Y_, M_, D_))
                    return true;
            }

            return false;
        }
        bool CTime::GetValidFutureFromNowByMonth(TInt& Y_, TInt& M_, TInt D_)
        {
            auto ToYear = Y_ + c_MaxCheckYear;

            for (; Y_ < ToYear; ++Y_)
            {
                for (; M_ <= 12; ++M_)
                {
                    if (IsValidDate(Y_, M_, D_))
                        return true;
                }
                M_ = 1;
            }

            return false;
        }
        bool CTime::GetValidFutureFromNowByDay(TInt& Y_, TInt& M_, TInt& D_)
        {
            auto ToYear = Y_ + c_MaxCheckYear;

            for (; Y_ < ToYear; ++Y_)
            {
                for (; M_ <= 12; ++M_)
                {
                    for (; D_ <= 31; ++D_)
                    {
                        if (IsValidDate(Y_, M_, D_))
                            return true;
                    }
                    D_ = 1;
                }
                M_ = 1;
            }

            return false;
        }
        CTime::CTime() :
            m_Date(c_Date_Null),
            m_Time(0)
        {
        }
        CTime::CTime(nowtime_t, TInt Month_, TInt Day_, TInt Hour_, TInt Minute_, TInt Second_)
        {
            SetNow();

            auto Year = GetYear();
            if (!GetValidFutureFromNowByYear(Year, Month_, Day_))
                THROWEX();

            auto Now(*this);

            if (!SetDateTime(Year, Month_, Day_, Hour_, Minute_, Second_))
                THROWEX();

            if (*this < Now)
            {
                if (!SetNextYear())
                    THROWEX();
            }
        }
        CTime::CTime(nowtime_t, TInt Day_, TInt Hour_, TInt Minute_, TInt Second_)
        {
            SetNow();

            auto Year = GetYear();
            auto Month = GetMonth();
            if (!GetValidFutureFromNowByMonth(Year, Month, Day_))
                THROWEX();

            auto Now(*this);

            if (!SetDateTime(Year, Month, Day_, Hour_, Minute_, Second_))
                THROWEX();

            if (*this < Now)
            {
                if (!SetNextMonth())
                    THROWEX();
            }
        }
        CTime::CTime(nowtime_t, EDayOfWeek DayOfWeek_, TInt Hour_, TInt Minute_, TInt Second_)
        {
            SetNow();

            auto Year = GetYear();
            auto Month = GetMonth();
            auto Day = GetDay();
            if (!GetValidFutureFromNowByDay(Year, Month, Day))
                THROWEX();

            auto Now(*this);

            if (!SetDateTime(Year, Month, Day, Hour_, Minute_, Second_))
                THROWEX();

            if (*this < Now)
                AddDay(1);

            while (GetDayOfWeek() != DayOfWeek_)
                AddDay(1);
        }
        CTime::CTime(nowtime_t, TInt Hour_, TInt Minute_, TInt Second_)
        {
            SetNow();

            auto Year = GetYear();
            auto Month = GetMonth();
            auto Day = GetDay();
            if (!GetValidFutureFromNowByDay(Year, Month, Day))
                THROWEX();

            auto Now(*this);

            if (!SetDateTime(Year, Month, Day, Hour_, Minute_, Second_))
                THROWEX();

            if (*this < Now)
                AddDay(1);
        }
        CTime::CTime(nowtime_t, TInt Minute_, TInt Second_)
        {
            SetNow();

            auto Year = GetYear();
            auto Month = GetMonth();
            auto Day = GetDay();
            auto Hour = GetHour();
            if (!GetValidFutureFromNowByDay(Year, Month, Day))
                THROWEX();

            auto Now(*this);

            if (!SetDateTime(Year, Month, Day, Hour, Minute_, Second_))
                THROWEX();

            if (*this < Now)
                AddHour(1);
        }
        CTime::CTime(nowtime_t, TInt Second_)
        {
            SetNow();

            auto Year = GetYear();
            auto Month = GetMonth();
            auto Day = GetDay();
            auto Hour = GetHour();
            auto Minute = GetMinute();
            if (!GetValidFutureFromNowByDay(Year, Month, Day))
                THROWEX();

            auto Now(*this);

            if (!SetDateTime(Year, Month, Day, Hour, Minute, Second_))
                THROWEX();

            if (*this < Now)
                AddMinute(1);
        }
        CTime::CTime(nowtime_t)
        {
            SetNow();
        }
        CTime::CTime(const TInt DateValue_, const TInt TimeValue_) :
            m_Date(DateValue_),
            m_Time(TimeValue_)
        {
        }
        CTime::CTime(TInt Year_, TInt Month_, TInt Day_, TInt Hour_, TInt Minute_, TInt Second_, TInt MSecond_)
        {
            if (!SetDateTime(Year_, Month_, Day_, Hour_, Minute_, Second_, MSecond_))
                THROWEX();
        }
        CTime::CTime(time_t TimeT_)
        {
            tm Time;
            if (localtime_s(&Time, &TimeT_) != 0)
                THROWEX();

            SetDateTime(Time.tm_year + 1900, Time.tm_mon + 1, Time.tm_mday, Time.tm_hour, Time.tm_min, Time.tm_sec);
        }
        bool CTime::operator == (const CTime& rVal_) const
        {
            if (m_Date == rVal_.m_Date  &&
                m_Time == rVal_.m_Time)
                return true;

            return false;
        }
        bool CTime::operator != (const CTime& rVal_) const
        {
            if (*this == rVal_)
                return false;

            return true;
        }
        bool CTime::operator < (const CTime& rVal_) const
        {
            if (m_Date < rVal_.m_Date)
                return true;
            else if (m_Date > rVal_.m_Date)
                return false;

            if (m_Time < rVal_.m_Time)
                return true;

            return false;
        }
        bool CTime::operator > (const CTime& rVal_) const
        {
            if (m_Date > rVal_.m_Date)
                return true;
            else if (m_Date < rVal_.m_Date)
                return false;

            if (m_Time > rVal_.m_Time)
                return true;

            return false;
        }
        bool CTime::operator <= (const CTime& rVal_) const
        {
            if (*this > rVal_)
                return false;

            return true;
        }
        bool CTime::operator >= (const CTime& rVal_) const
        {
            if (*this < rVal_)
                return false;

            return true;
        }
        time_t CTime::ToTimeT(void) const
        {
            tm TM(ToTM());
            return mktime(&TM);
        }
        void CTime::SetNow(void)
        {
            struct timeb NowTimeB;

            ftime(&NowTimeB);

            struct tm NowTm;
            if (localtime_s(&NowTm, &NowTimeB.time) == NULL)
            {
                SetDateTime(
                    NowTm.tm_year + 1900,
                    NowTm.tm_mon + 1,
                    NowTm.tm_mday,
                    NowTm.tm_hour,
                    NowTm.tm_min,
                    NowTm.tm_sec,
                    NowTimeB.millitm);
            }
        }
        bool CTime::SetDateTime(TInt Year_, TInt Month_, TInt Day_, TInt Hour_, TInt Minute_, TInt Second_, TInt MSecond_)
        {
            if (!SetDate(Year_, Month_, Day_) ||
                !SetTime(Hour_, Minute_, Second_, MSecond_))
                return false;

            return true;
        }
        bool CTime::SetDate(TInt Year_, TInt Month_, TInt Day_)
        {
            if (!IsValidDate(Year_, Month_, Day_))
                return false;

            m_Date = (Year_ * 365) + (Year_ - 1) / 4 - (Year_ - 1) / 100 + (Year_ - 1) / 400 - c_DaysTo1900 + GetDaysToMonthOfYear(Year_, Month_, Day_) - 1;

            return true;
        }
        bool CTime::SetDate(TInt Date_)
        {
            if (Date_ <= c_DaysTo1900)
                return false;

            m_Date = Date_;

            return true;
        }
        bool CTime::SetTime(TInt Hour_, TInt Minute_, TInt Second_, TInt MSecond_)
        {
            if (!IsValidTime(Hour_, Minute_, Second_, MSecond_))
                return false;

            m_Time =
                (Hour_ * c_HourMSec) +
                (Minute_ * c_MinuteMSec) +
                (Second_ * c_SecondMSec) +
                MSecond_;

            return true;
        }
        bool CTime::SetTime(TInt Time_)
        {
            if (Time_ < 0 || Time_ >= c_DayMSec)
                return false;

            m_Time = Time_;

            return true;
        }
        TInt CTime::GetYear(void) const
        {
            TInt Remain = 0;
            bool bLeapYear = false;

            return _GetY(Remain, bLeapYear);
        }
        TInt CTime::GetMonth(void) const
        {
            TInt Remain = 0;
            bool bLeapYear = false;
            _GetY(Remain, bLeapYear);

            return _GetM(bLeapYear, Remain);
        }
        TInt CTime::GetDay(void) const
        {
            TInt Remain = 0;
            bool bLeapYear = false;

            _GetY(Remain, bLeapYear);
            _GetM(bLeapYear, Remain);

            return (Remain + 1);
        }
        TInt CTime::GetHour(void) const
        {
            return (m_Time / c_HourMSec);
        }
        TInt CTime::GetMinute(void) const
        {
            return ((m_Time % c_HourMSec) / c_MinuteMSec);
        }
        TInt CTime::GetSecond(void) const
        {
            return ((m_Time % c_MinuteMSec) / c_SecondMSec);
        }
        TInt CTime::GetMSecond(void) const
        {
            return (m_Time % c_SecondMSec);
        }
        TInt CTime::GetWeek(void) const
        {
            return (GetDay() - 1 + GetDayOfWeek()) / 7;
        }
        TInt CTime::GetFullWeek(void) const
        {
            return (GetDay() - 1) / 7;
        }
        EDayOfWeek CTime::GetDayOfWeek(void) const
        {
            return EDayOfWeek((m_Date + 1) % 7);
        }
        bool CTime::IsLeapYear(void) const
        {
            TInt Year = GetYear();

            if (Year % 400 == 0) return true;
            if (Year % 100 == 0) return false;
            if (Year % 4 == 0) return true;

            return false;
        }
        bool CTime::IsLastDay(void) const
        {
            CTime NextDay;
            NextDay.m_Date = m_Date + 1;

            if (NextDay.GetDay() < GetDay())
                return true;

            return false;
        }
        bool CTime::IsLastWeek(void) const
        {
            CTime ChkTime;
            ChkTime.SetDate(GetYear(), GetMonth(), GetLastDay());

            if (GetWeek() == ChkTime.GetWeek())
                return true;

            return false;
        }
        bool CTime::IsLastFullWeek(void) const
        {
            CTime ChkTime;
            ChkTime.SetDate(GetYear(), GetMonth(), GetLastDay());

            if (GetFullWeek() == ChkTime.GetFullWeek())
                return true;

            return false;
        }
        TInt CTime::GetLastDay(void) const
        {
            CTime ScanDay;
            ScanDay = *this;

            TInt OldDay = ScanDay.GetDay();

            for (TInt i = 0; i < c_MaxDaysOfMonth; ++i)
            {
                ScanDay.AddDay();
                if (ScanDay.GetDay() < OldDay) return OldDay;

                OldDay = ScanDay.GetDay();
            }

            return 0; // Can't find
        }
        TInt CTime::GetAge(void) const
        {
            CTime Now(nowtime);

            if (*this >= Now) return 0;

            TInt Age = Now.GetYear() - GetYear();

            if (Age <= 0) return 0;

            --Age;

            if (Now.GetMonth() < GetMonth()) return Age;
            else if (Now.GetMonth() > GetMonth()) return ++Age;
            else if (Now.GetDay() < GetDay()) return Age;
            else if (Now.GetDay() > GetDay()) return ++Age;
            else if (Now.GetHour() < GetHour()) return Age;
            else if (Now.GetHour() > GetHour()) return ++Age;
            else if (Now.GetMinute() < GetMinute()) return Age;
            else if (Now.GetMinute() > GetMinute()) return ++Age;
            else if (Now.GetSecond() < GetSecond()) return Age;
            else if (Now.GetSecond() > GetSecond()) return ++Age;

            return ++Age;
        }
        bool CTime::SetNextYear(void)
        {
            auto Year = GetYear() + 1;
            auto Month = GetMonth();
            auto Day = GetDay();
            auto Hour = GetHour();
            auto Minute = GetMinute();
            auto Second = GetSecond();
            auto MSecond = GetMSecond();

            auto ToYear = Year + c_MaxCheckYear;
            for (; Year < ToYear; ++Year)
            {
                if (SetDateTime(Year, Month, Day, Hour, Minute, Second, MSecond))
                    return true;
            }

            return false;
        }
        bool CTime::SetNextMonth(void)
        {
            auto Year = GetYear();
            auto Month = GetMonth();
            auto Day = GetDay();
            auto Hour = GetHour();
            auto Minute = GetMinute();
            auto Second = GetSecond();
            auto MSecond = GetMSecond();

            if (Month < 12)
            {
                ++Month;
            }
            else
            {
                ++Year;
                Month = 1;
            }

            auto ToYear = Year + c_MaxCheckYear;

            for (; Year < ToYear; ++Year)
            {
                for (; Month <= 12; ++Month)
                {
                    if (SetDateTime(Year, Month, Day, Hour, Minute, Second, MSecond))
                        return true;
                }
                Month = 1;
            }

            return false;
        }
        void CTime::AddDay(TInt Day_)
        {
            if (m_Date + Day_ < 0) return;

            m_Date += Day_;
        }
        void CTime::AddHour(TInt Hour_)
        {
            if (Hour_ >= c_DayHour)
            {
                m_Date += (Hour_ / c_DayHour);
                Hour_ %= c_DayHour;
            }
            else if (Hour_ <= -c_DayHour)
            {
                m_Date += (Hour_ / c_DayHour);
                if (m_Date < 0)
                {
                    m_Date = 0;
                    m_Time = 0;
                    return;
                }
                Hour_ %= c_DayHour;
            }

            m_Time += (Hour_*c_HourMSec);
            _RefreshOverDay();
        }
        void CTime::AddMinute(TInt Minute_)
        {
            if (Minute_ >= c_DayMin)
            {
                m_Date += (Minute_ / c_DayMin);
                Minute_ %= c_DayMin;
            }
            else if (Minute_ <= -c_DayMin)
            {
                m_Date += (Minute_ / c_DayMin);
                if (m_Date < 0)
                {
                    m_Date = 0;
                    m_Time = 0;
                    return;
                }
                Minute_ %= c_DayMin;
            }

            m_Time += (Minute_*c_MinuteMSec);
            _RefreshOverDay();
        }
        void CTime::AddSecond(TInt Second_)
        {
            if (Second_ >= c_DaySec)
            {
                m_Date += (Second_ / c_DaySec);
                Second_ %= c_DaySec;
            }
            else if (Second_ <= -c_DaySec)
            {
                m_Date += (Second_ / c_DaySec);
                if (m_Date < 0)
                {
                    m_Date = 0;
                    m_Time = 0;
                    return;
                }
                Second_ %= c_DaySec;
            }

            m_Time += (Second_*c_SecondMSec);
            _RefreshOverDay();
        }
        void CTime::AddMSecond(TInt MSecond_)
        {
            m_Time += MSecond_;
            _RefreshOverDay();
        }
        SDateTime CTime::ToDateTime(void) const
        {
            return SDateTime(
                GetYear(),
                GetMonth(),
                GetDay(),
                GetHour(),
                GetMinute(),
                GetSecond()
            );
        }
        tm CTime::ToTM(void) const
        {
            return tm{
            GetSecond(),
            GetMinute(),
            GetHour(),
            GetDay(),
            GetMonth() - 1,
            GetYear() - 1900
            };
        }
        wstring CTime::ToString(const wchar_t* pFormat_) const
        {
            tm TM(ToTM());

            wchar_t Str[1024 + 1] = {};
            if (wcsftime(Str, 1024, pFormat_, &TM) == 0)
                return wstring();

            return wstring(Str);
        }

        SDateTime::SDateTime()
        {
        }
        SDateTime::SDateTime(
            TSqlSmallInt Year_,
            TSqlUSmallInt Month_,
            TSqlUSmallInt Day_,
            TSqlUSmallInt Hour_,
            TSqlUSmallInt Minute_,
            TSqlUSmallInt Second_,
            TSqlUInt MSecond_) :
            year(Year_),
            month(Month_),
            day(Day_),
            hour(Hour_),
            minute(Minute_),
            second(Second_),
            fraction(MSecond_ * 1000000)
        {}
        SDateTime::SDateTime(const TTime& TimePoint_)
        {
            auto TimeT = system_clock::to_time_t(TimePoint_);
            tm Tm;
            localtime_s(&Tm, &TimeT);
            year = TSqlSmallInt(Tm.tm_year + 1900);
            month = TSqlUSmallInt(Tm.tm_mon + 1);
            day = TSqlUSmallInt(Tm.tm_mday);
            hour = TSqlUSmallInt(Tm.tm_hour);
            minute = TSqlUSmallInt(Tm.tm_min);
            second = TSqlUSmallInt(Tm.tm_sec);
            fraction = duration_cast<milliseconds>(TimePoint_.time_since_epoch()).count() % 1000 * 1000000;
        }
        SDateTime::SDateTime(const CTime& Time_)
        {
            year = (TSqlSmallInt)Time_.GetYear();
            month = (TSqlUSmallInt)Time_.GetMonth();
            day = (TSqlUSmallInt)Time_.GetDay();
            hour = (TSqlUSmallInt)Time_.GetHour();
            minute = (TSqlUSmallInt)Time_.GetMinute();
            second = (TSqlUSmallInt)Time_.GetSecond();
            fraction = (TSqlUInt)Time_.GetMSecond() * 1000000;
        }
        SDateTime& SDateTime::operator = (const CTime& Time_)
        {
            year = (TSqlSmallInt)Time_.GetYear();
            month = (TSqlUSmallInt)Time_.GetMonth();
            day = (TSqlUSmallInt)Time_.GetDay();
            hour = (TSqlUSmallInt)Time_.GetHour();
            minute = (TSqlUSmallInt)Time_.GetMinute();
            second = (TSqlUSmallInt)Time_.GetSecond();
            fraction = (TSqlUInt)Time_.GetMSecond() * 1000000;

            return *this;
        }
        bool SDateTime::operator == (const SDateTime& rTS_)
        {
            if (year == rTS_.year  &&
                month == rTS_.month  &&
                day == rTS_.day  &&
                hour == rTS_.hour  &&
                minute == rTS_.minute  &&
                second == rTS_.second  &&
                fraction == rTS_.fraction) return true;

            return false;
        }
        bool SDateTime::operator != (const SDateTime& rTS_)
        {
            if (*this == rTS_) return false;

            return true;
        }
        bool SDateTime::operator < (const SDateTime& TS_) const
        {
            if (year < TS_.year) return true;
            if (year > TS_.year) return false;
            if (month < TS_.month) return true;
            if (month > TS_.month) return false;
            if (day < TS_.day) return true;
            if (day > TS_.day) return false;
            if (hour < TS_.hour) return true;
            if (hour > TS_.hour) return false;
            if (minute < TS_.minute) return true;
            if (minute > TS_.minute) return false;
            if (second < TS_.second) return true;
            if (second > TS_.second) return false;
            if (fraction < TS_.fraction) return true;

            return false;
        }
        bool SDateTime::operator > (const SDateTime& TS_) const
        {
            return (TS_ < *this);
        }
        bool SDateTime::operator <= (const SDateTime& TS_) const
        {
            return !(*this > TS_);
        }
        bool SDateTime::operator >= (const SDateTime& TS_) const
        {
            return !(*this < TS_);
        }
        bool SDateTime::IsNull(void) const
        {
            if (year == 1900 &&
                month == 1 &&
                day == 1 &&
                hour == 0 &&
                minute == 0 &&
                second == 0 &&
                fraction == 0)
                return true;

            return false;
        }
        CTime SDateTime::ToTime(void) const
        {
            return CTime(year, month, day, hour, minute, second);
        }
        time_t SDateTime::ToTimeT(void) const
        {
            auto TM(ToTM());
            return mktime(&TM);
        }
        TTime SDateTime::ToTimePoint(void) const
        {
            return GetTimePoint(year, month, day, hour, minute, second, fraction / 1000000);
        }
        tm SDateTime::ToTM(void) const
        {
            return tm{
            second,
            minute,
            hour,
            day,
            month - 1,
            year - 1900
            };
        }
        bool SDateTime::IsSameDay(const SDateTime& TimeStamp_) const
        {
            if (year == TimeStamp_.year &&
                month == TimeStamp_.month &&
                day == TimeStamp_.day)
                return true;

            return false;
        }
        SDateTime SDateTime::AddMonth(TSqlUSmallInt Month_) const
        {
            SDateTime Time = *this;
            Time.year += ((Time.month - 1 + Month_) / 12);
            Time.month = (Time.month - 1 + Month_) % 12 + 1;

            auto NextLastDayOfMonth = LastDayOfMonth(Time.year, Time.month);
            if (Time.day > NextLastDayOfMonth)
                Time.day = NextLastDayOfMonth;

            return Time;
        }
        __int32 SDateTime::GetMonthDiff(const SDateTime& Subtrahend_) const
        {
            __int32 Result = (year > Subtrahend_.year ? (year - Subtrahend_.year) : -(__int32)(Subtrahend_.year - year)) * 12;
            Result += (month > Subtrahend_.month ? (month - Subtrahend_.month) : -(__int32)(Subtrahend_.month - month));
            return Result;
        }
        TSqlUSmallInt SDateTime::GetFullWeek(void) const
        {
            return (day - 1) / 7;
        }
        wstring SDateTime::ToString(const wchar_t* pFormat_) const
        {
            tm TM(ToTM());

            wchar_t Str[1024 + 1] = {};
            if (wcsftime(Str, 1024, pFormat_, &TM) == 0)
                return wstring();

            return wstring(Str);
        }
    }
}
