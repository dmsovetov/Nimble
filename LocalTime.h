/**************************************************************************

 The MIT License (MIT)

 Copyright (c) 2015 Dmitry Sovetov

 https://github.com/dmsovetov

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.

 **************************************************************************/

#ifndef __Nimble_LocalTime_H__
#define __Nimble_LocalTime_H__

#include "Globals.h"

#ifdef NIMBLE_PLATFORM_WINDOWS
    #ifndef WIN32_LEAN_AND_MEAN
        #define WIN32_LEAN_AND_MEAN
        #define NIMBLE_RESET_LEAN_AND_MEAN
    #endif  /*  !WIN32_LEAN_AND_MEAN    */

    #ifndef NOMINMAX
        #define NOMINMAX
        #define NIMBLE_RESET_NOMINMAX
    #endif  /*  !NOMINMAX   */

    #include <Windows.h>
    #include <timeapi.h>

    #ifdef NIMBLE_RESET_LEAN_AND_MEAN
        #undef WIN32_LEAN_AND_MEAN
    #endif  /*  NIMBLE_RESET_LEAN_AND_MEAN  */

    #ifdef NIMBLE_RESET_NOMINMAX
        #undef NOMINMAX
    #endif  /*  NIMBLE_RESET_NOMINMAX  */

    #ifdef near
        #undef near
    #endif  /*  near    */

    #ifdef far
        #undef far
    #endif  /*  far */

    #ifdef NEAR
        #undef NEAR
        #define NEAR
    #endif  /*  NEAR    */

    #ifdef FAR
        #undef FAR
        #define FAR
    #endif  /*  FAR    */
#endif  /*  NIMBLE_PLATFORM_WINDOWS */

NIMBLE_BEGIN

    //! Time class contains all time-related stuff.
    class Time {
    public:

        //! Contains the current time value.
        struct TimeValue {
            time_t          tv_sec;     //!< Time value, in seconds.
            time_t          tv_usec;    //!< Time value, in microseconds (that are not a multiple of seconds).
        };

        //! Returns current time in milliseconds.
        static u32          current( void );

        //! Returns the current time of day.
        static TimeValue    localTime( void );

        //! Returns the time zone.
        static s32          timeZone( void );

        //! Returns the formatted time string.
        static String       timeString( void );

        //! Returns the time zone string.
        static String       timeZoneString( void );
    };

    // ** Time::timeZone
    inline s32 Time::timeZone( void )
    {
    #ifdef NIMBLE_PLATFORM_WINDOWS
        TIME_ZONE_INFORMATION tz;
        GetTimeZoneInformation( &tz );
        return -tz.Bias / 60;
    #else
        NIMBLE_BREAK;
        return 0;
    #endif  /*  NIMBLE_PLATFORM_WINDOWS */
    }

    // ** Time::current
    inline u32 Time::current( void )
    {
    #ifdef NIMBLE_PLATFORM_WINDOWS
        return timeGetTime();
    #else
        NIMBLE_BREAK;
        return 0;
    #endif  /*  NIMBLE_PLATFORM_WINDOWS */
    }

    // ** Time::localTime
    inline Time::TimeValue Time::localTime( void )
    {
        TimeValue result;

    #ifdef NIMBLE_PLATFORM_WINDOWS
        // Source: http://stackoverflow.com/questions/10905892/equivalent-of-gettimeday-for-windows/26085827#26085827
        // Note: some broken versions only have 8 trailing zero's, the correct epoch has 9 trailing zero's
        static const u64 EPOCH = static_cast<u64>( 116444736000000000ULL );

        SYSTEMTIME  system_time;
        FILETIME    file_time;
        u64         time;

        GetSystemTime( &system_time );
        SystemTimeToFileTime( &system_time, &file_time );
        time =  static_cast<u64>( file_time.dwLowDateTime );
        time += static_cast<u64>( file_time.dwHighDateTime ) << 32;

        result.tv_sec  = static_cast<time_t>( (time - EPOCH) / 10000000L );
        result.tv_usec = static_cast<time_t>( system_time.wMilliseconds * 1000 );
    #else
        timeval tv;
        gettimeofday( &tv, NULL );
        result.tv_sec = tv.tv_sec;
        result.tv_usec = tv.tv_usec;
    #endif  /*  NIMBLE_PLATFORM_WINDOWS */

        return result;
    }

    // ** Time::timeString
    inline String Time::timeString( void )
    {
        // Get the local time
        TimeValue local = localTime();

        // Convert time value to a tm
        time_t timestamp = local.tv_sec;
        tm*    time      = localtime( &timestamp );

        // Format the time
        s8 timeFormatted[80];
        strftime( timeFormatted, sizeof( timeFormatted ), "%Y-%m-%d %I:%M:%S", time );

        // Build final buffer
        s8 formatted[100];
        _snprintf( formatted, sizeof( formatted ), "%s.%03d", timeFormatted, local.tv_usec / 1000 );

        return String( formatted ) + " " + timeZoneString();
    }

    // ** Time::timeZoneString
    inline String Time::timeZoneString( void )
    {
        s8 buffer[16];
        _snprintf( buffer, sizeof( buffer ), "UTC%+04d", timeZone() );
        return buffer;
    }

NIMBLE_END

#endif  /*  !__Nimble_LocalTime_H__  */