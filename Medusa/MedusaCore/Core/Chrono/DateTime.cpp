// Copyright (c) 2015 fjz13. All rights reserved.
// Use of this source code is governed by a MIT-style
// license that can be found in the LICENSE file.
#include "MedusaCorePreCompiled.h"
#include "Core/Chrono/DateTime.h"
#include "Core/String/StringParser.h"
#include "Core/Log/Log.h"
#include "Core/String/Format/Format.h"

#ifdef MEDUSA_WINDOWS
#include <WinSock2.h>
#endif

MEDUSA_BEGIN;


const DateTime DateTime::StartDate = DateTime(std::chrono::system_clock::from_time_t(0));

DateTime::DateTime() 
	:DateTime(std::chrono::system_clock::from_time_t(0))
{

}

DateTime::DateTime(TimePoint seconds, bool isUTC/*=true*/) :mTimePoint(seconds), mIsUTC(isUTC)
{
	if (mIsUTC)
	{
		UpdateTimePointToUTCDate();
	}
	else
	{
		UpdateTimePointToLocalDate();
	}
}

DateTime::DateTime(uint year, uint month, uint day, uint hour/*=0*/, uint minute/*=0*/, uint second/*=0*/, bool isUTC/*=true*/)
{
	Init(year, month, day, hour, minute, second, isUTC);
}
DateTime::DateTime(const StringRef& strDateTime, bool isUTC/*=true*/)
{
	Init(strDateTime, "{}-{}-{} {}:{}:{}", isUTC);
}
DateTime::DateTime(const StringRef& strDateTime, const StringRef& format, bool isUTC/*=true*/)
{
	Init(strDateTime, format, isUTC);
}

DateTime::DateTime(const DateTime& dateTime)
	: mTimePoint(dateTime.mTimePoint), mDate(dateTime.mDate)
{

}

DateTime& DateTime::operator=(const DateTime& dateTime)
{
	mDate = dateTime.mDate;
	mTimePoint = dateTime.mTimePoint;
	return *this;
}

DateTime DateTime::Parse(const StringRef& strDateTime, bool isUTC/*=true*/)
{
	return DateTime(strDateTime, isUTC);
}
DateTime DateTime::Parse(const StringRef& strDateTime, const StringRef& format, bool isUTC/*=true*/)
{
	return DateTime(strDateTime, format, isUTC);
}

uint DateTime::DaysInMonth(uint year, uint month)
{
	static const uint day_of_month[13] = { 31, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	uint m = (month > 11) ? 0 : month;
	if (m == 2 && (((year % 4 == 0) && (year % 100 != 0)) || (year % 400 == 0)))
	{
		return 29;
	}
	else
	{
		return day_of_month[m];
	}
}

void DateTime::AddYears(int years)
{
	mDate.tm_year = mDate.tm_year + (int)years;
	UpdateUTCDateToTimePoint();
}
void DateTime::AddMonths(int months)
{
	int a = (int)((mDate.tm_mon + months) / 12);
	mDate.tm_year = mDate.tm_year + a;
	mDate.tm_mon = mDate.tm_mon + (int)((mDate.tm_mon + months) % 12) - 1;
	UpdateUTCDateToTimePoint();
}

void DateTime::AddSeconds(int val)
{
	mTimePoint += std::chrono::seconds(val);
	UpdateTimePointToUTCDate();
}

HeapString DateTime::ToString(const StringRef& formaterStr) const
{
	char s[256];
	strftime(s, sizeof(s), formaterStr.c_str(), &mDate);
	return HeapString(s);
}
HeapString DateTime::ToString() const
{
	return ToString("%Y-%m-%d %H:%M:%S");
}
HeapString DateTime::ToShortDateString() const
{
	return ToString("%Y-%m-%d");
}

HeapString DateTime::ToTotalSecondsString() const
{
	return String::Format("{}", TotalSeconds());
}

HeapString DateTime::ToLongString() const
{
	HeapString result(256);
	auto len = strftime(result.c_str(), result.Size(), "%Y-%m-%d %H:%M:%S", &mDate);
	result.ForceSetLength(len);

	auto val=std::chrono::duration_cast<std::chrono::milliseconds>(mTimePoint.time_since_epoch());
	auto totalMilliseconds = val.count();
	result.AppendFormat(":{:d03}", totalMilliseconds % 1000);
	return result;
}

void DateTime::Init(uint year, uint month, uint day, uint hour, uint minute, uint second, bool isUTC/*=true*/)
{
	assert(year >= 1900);
	assert(month <= 11);
	assert(day >= 1 && day <= 31);
	assert(hour <= 23);
	assert(minute <= 59);
	assert(second <= 59);

	Memory::ClearZero(&mDate);

	mDate.tm_year = static_cast<int>(year - 1900);
	mDate.tm_mon = static_cast<int>(month);
	mDate.tm_mday = static_cast<int>(day);
	mDate.tm_hour = static_cast<int>(hour);
	mDate.tm_min = static_cast<int>(minute);
	mDate.tm_sec = static_cast<int>(second);

	mIsUTC = isUTC;
	if (mIsUTC)
	{
		UpdateUTCDateToTimePoint();
	}
	else
	{
		UpdateLocalDateToTimePoint();
	}
}

void DateTime::Init(const StringRef& dateTimeStr, const StringRef& formaterStr, bool isUTC/*=true*/)
{
	uint year, month, day, hour, minutes, seconds;
#ifdef MEDUSA_WINDOWS
	sscanf_s(dateTimeStr.c_str(), formaterStr.c_str(), &year, &month, &day, &hour, &minutes, &seconds);
#else
	sscanf(dateTimeStr.c_str(), formaterStr.c_str(), &year, &month, &day, &hour, &minutes, &seconds);
#endif
	Init(year, month - 1, day, hour, minutes, seconds, isUTC);
}

DateTime* DateTime::TryParse(const StringRef& strDateTime, bool isUTC/*=true*/)
{
	return TryParse(strDateTime, "{}-{}-{} {}:{}:{}", isUTC);
}

DateTime* DateTime::TryParse(const StringRef& strDateTime, const StringRef& format, bool isUTC/*=true*/)
{
	uint year, month, day, hour, minute, second;
#ifdef WIN32
	sscanf_s(strDateTime.c_str(), format.c_str(), &year, &month, &day, &hour, &minute, &second);
#else
	sscanf(strDateTime.c_str(), format.c_str(), &year, &month, &day, &hour, &minute, &second);
#endif
	if ((year >= 1900) && (month <= 11) && (day >= 1 && day <= 31) && (hour <= 23) && (minute <= 59) && (second <= 59))
	{
		return new DateTime(strDateTime, format, isUTC);
	}

	return nullptr;
}

size_t DateTime::Ticks()
{
#ifdef MEDUSA_WINDOWS
	return ::GetTickCount();
#else
	return 0;
#endif
}


void DateTime::SetTimeZone(StringRef timezone)
{
	//set TZ=tzn[+|-]hh[:mm[:ss]][dzn] 
	/*
	tzn
	Three-letter time-zone name, such as PST. You must specify the correct offset from local time to UTC.
	hh
	Difference in hours between UTC and local time. Sign (+) optional for positive values.
	mm
	Minutes. Separated from hh by a colon (:).
	ss
	Seconds. Separated from mm by a colon (:).
	dzn
	Three-letter daylight-saving-time zone such as PDT. If daylight saving time is never in effect in the locality, set TZ without a value for dzn. The C run-time library assumes the United States' rules for implementing the calculation of daylight saving time (DST).
	*/
#ifdef MEDUSA_WINDOWS
	_putenv_s("TZ", timezone.c_str());
	_tzset();
#else
	setenv("TZ", timezone.c_str(), 1);
	tzset();
#endif

}

void DateTime::SetTimeZoneToUTC()
{
	SetTimeZone("PST0");
}

DateTime DateTime::Now()
{
	return DateTime(std::chrono::system_clock::now(), false);
}

DateTime DateTime::UTCNow()
{
	auto result = DateTime(std::chrono::system_clock::now(), false);
	result.ConvertToUTC();
	return result;
}

bool DateTime::IsDayLight()
{
#ifdef MEDUSA_WINDOWS
	//return _daylight!=0;

	int daylight;
	_get_daylight(&daylight);
	return daylight != 0;
#else
	return daylight;

	/*time_t rawtime;
	time ( &rawtime );

	tm localDate = localtime(&rawtime);
	return localDate.tm_isdst;*/
#endif

}

long DateTime::GetTimeZoneSeconds()
{
#ifdef MEDUSA_WINDOWS
	//return _timezone;
	long timezone;
	_get_timezone(&timezone);
	return timezone;
#else
	return timezone;

	/*time_t rawtime;
	time ( &rawtime );

	tm localDate = localtime(&rawtime);
	tm gmDate = gmtime( &rawtime );
	return (localDate.tm_hour  - gmDate->tm_hour)*3600;*/
#endif

}

HeapString DateTime::GetTimeZoneName()
{
#ifdef MEDUSA_WINDOWS
	//return _tzname[0];
	size_t s;
	char tzname[100];
	_get_tzname(&s, tzname, sizeof(tzname), 0);
	return HeapString(tzname, s - 1);
#else
	return HeapString(tzname[0]);

	/*size_t s;
	char tzname[100];
	get_tzname( &s, tzname, sizeof(tzname), 0 );
	return HeapString(tzname,s-1);*/
#endif

}

void DateTime::UpdateUTCDateToTimePoint()
{
#ifdef MEDUSA_WINDOWS
	auto val = _mkgmtime(&mDate);
#else
	auto val = mktime(&mDate) - timezone;
#endif
	mTimePoint = std::chrono::system_clock::from_time_t(val);

}

void DateTime::UpdateTimePointToUTCDate()
{
	auto val = std::chrono::system_clock::to_time_t(mTimePoint);

#ifdef MEDUSA_WINDOWS
	gmtime_s(&mDate, &val);
#else
	mDate = *gmtime(&val);
#endif // WIN32
}

void DateTime::UpdateLocalDateToTimePoint()
{

#ifdef MEDUSA_WINDOWS
	auto val = mktime(&mDate);
#else
	auto val = mktime(&mDate);
#endif
	mTimePoint = std::chrono::system_clock::from_time_t(val);
}

void DateTime::UpdateTimePointToLocalDate()
{
	auto val = std::chrono::system_clock::to_time_t(mTimePoint);
#ifdef MEDUSA_WINDOWS
	localtime_s(&mDate, &val);
#else
	mDate = *localtime(&val);
#endif // WIN32
}


void DateTime::PrintTimeZone()
{
	HeapString timeZoneName = GetTimeZoneName();
	Log::FormatInfo("IsDayLight:{}\tTimeZone:{}\tTimeZoneName:{}", StringParser::ToString(IsDayLight()).c_str(), GetTimeZoneSeconds(), timeZoneName.c_str());
}


time_t DateTime::TotalSeconds() const
{
	return std::chrono::system_clock::to_time_t(mTimePoint);
}

void DateTime::ConvertToUTC()
{
	RETURN_IF_TRUE(mIsUTC);
	mIsUTC = true;
	UpdateTimePointToUTCDate();

}

void DateTime::ConvertToLocal()
{
	RETURN_IF_FALSE(mIsUTC);
	mIsUTC = false;
	UpdateTimePointToLocalDate();
}

timespec DateTime::ToAbsoluteTime(int64 millisecondsFromNow)
{
#ifdef MEDUSA_WINDOWS
	struct timespec abstime;
	struct timeval tv;

	SYSTEMTIME wtm;
	GetLocalTime(&wtm);
	struct tm tm;
	tm.tm_year = wtm.wYear - 1900;
	tm.tm_mon = wtm.wMonth - 1;
	tm.tm_mday = wtm.wDay;
	tm.tm_hour = wtm.wHour;
	tm.tm_min = wtm.wMinute;
	tm.tm_sec = wtm.wSecond;
	tm.tm_isdst = -1;
	time_t clock = mktime(&tm);
	tv.tv_sec = (long)clock;
	tv.tv_usec = wtm.wMilliseconds * 1000;

	abstime.tv_sec = tv.tv_sec + millisecondsFromNow / 1000;
	abstime.tv_nsec = tv.tv_usec * 1000 + (millisecondsFromNow % 1000) * 1000000;
	if (abstime.tv_nsec >= 1000000000)
	{
		abstime.tv_nsec -= 1000000000;
		abstime.tv_sec++;
	}
	return abstime;
#else
	struct timespec abstime;
	struct timeval tv;
	gettimeofday(&tv, nullptr);

	abstime.tv_sec = tv.tv_sec + millisecondsFromNow / 1000;
	abstime.tv_nsec = tv.tv_usec * 1000 + (millisecondsFromNow % 1000) * 1000000;
	if (abstime.tv_nsec >= 1000000000)
	{
		abstime.tv_nsec -= 1000000000;
		abstime.tv_sec++;
	}
	return abstime;
#endif
}



MEDUSA_END;