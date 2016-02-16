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

#ifndef __Nimble_Logger_H__
#define __Nimble_Logger_H__

#include "Globals.h"

NIMBLE_BEGIN

    //! Abstract logger interface.
    class Log {
    public:

        //! Maximum message length constant
        enum { MaxMessageLength = 1024 };

        //! Available log levels
        enum Level {
              Debug     //!< This log level is used for debug messages.
            , Verbose   //!< Used for a verbose messages.
            , Warning   //!< Used for warning messages.
            , Error     //!< Used for error messages.
        };

    protected:

        //! Outputs the message to a log.
        virtual void    write( Level level, tm* time, CString tag, CString text ) = 0;
    };

    //! Generic log interface.
    template<typename TLog>
    class LogInterface {
    public:

                        //! Constructs the LogInterface instance.
                        LogInterface( CString tag )
                            : m_tag( tag ) {}

        //! Outputs the log message.
        static void     write( Log::Level level, CString format, ... );

    private:

        TLog            m_log;  //!< Actual log implementation.
        CString         m_tag;  //!< Log tag.
    };

    // ** LogInterface::write
    template<typename TLog>
    inline void LogInterface<TLog>::write( Log::Level level, CString format, ... )
    {
        // Format the output message
        s8 text[MaxMessageLength];
        va_list ap;
        va_start( ap, format );
        vsnprintf( text, MaxMessageLength, format, ap );
        va_end( ap );

        // Get the local time
        time_t timestamp = time( NULL );
        tm*    time      = localtime( &timestamp );

        // Write the message
        m_log.write( level, time, m_tag, text );
    }

    //! Generic logger interface with various policies.
    template<typename TFilter, typename TFormatter, typename TWriter>
    class GenericLogger : public Log {
    protected:

        //! Uses all policies to output the message to a log.
        virtual void    write( Level level, tm* time, CString tag, CString text ) NIMBLE_OVERRIDE;

    private:

        TFilter         m_filter;       //!< Log filtering policy.
        TFormatter      m_formatter;    //!< Message formatting policy.
        TWriter         m_writer;       //!< Writes message to an output.
    };

    // ** GenericLogger::write
    template<typename TFilter, typename TFormatter, typename TWriter>
    void GenericLogger<TFilter, TFormatter, TWriter>::write( Level level, tm* time, CString tag, CString text )
    {
        // First ensure that messages passes through a filter
        if( !m_filter.filter( level, tag ) ) {
            return;
        }

        // Now format the message
        String message = m_formatter.format( level, time, tag, text );

        // Output message to a log
        m_writer.write( message );
    }

    //! Disables the all filtering.
    struct NoLogFiltering {
        bool filter( Log::Level level, CString tag ) { return true; }
    };

    //! Filters the log messages by log level.
    template<typename TLevel>
    struct FilterLogByLevel {
        bool filter( Log::Level level, CString tag ) { return level >= TLevel; }
    };

    //! Formats the detailed log message.
    struct DetailedLogFormatter {
        String format( Log::Level level, tm* time, CString tag, CString text )
        {
            // Format the time
            s8 timeFormatted[80];
            strftime( timeFormatted, 80, "%d-%m-%Y %I:%M:%S", time );
            
            // Format the level
            CString levelFormatted = "";

            switch( level ) {
            case Log::Debug:    levelFormatted = "D"; break;
            case Log::Verbose:  levelFormatted = "V"; break;
            case Log::Warning:  levelFormatted = "W"; break;
            case Log::Error:    levelFormatted = "E"; break;
            }

            // Format the tag
            String tagFormatted = toUpperCase( tag );

            // Construct the final message
            return String( timeFormatted ) + " " + levelFormatted + " " + tagFormatted + "  " + text;
        }
    };

NIMBLE_END

#endif  /*  !__Nimble_Logger_H__   */
