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

//! Formats the input arguments to a string.
#define NIMBLE_LOGGER_FORMAT( format )                          \
            s8	    buffer[Log::MaxMessageLength];              \
            va_list ap;                                         \
            va_start( ap, format );                             \
            vsnprintf( buffer, sizeof( buffer ), format, ap );  \
            va_end( ap );

NIMBLE_BEGIN

    //! Abstract logger interface.
    class Log {
    public:

        //! Maximum message length constant
        enum { MaxMessageLength = 1024 };

        //! Available log levels.
        enum Level {
              Debug         //!< This log level is used for debug messages.
            , Verbose       //!< Used for a verbose messages.
            , Warning       //!< Used for warning messages.
            , Error         //!< Used for error messages.
            , Fatal         //!< Logs an internal error message.
        };

        //! Log message context.
        struct Context {
                            //! Constructs Context instance.
                            Context( CString function = "", CString file = "" )
                                : function( function ), file( file ), time( Time::localTime() ) {}

            CString         function;   //!< Parent function that issued the message.
            CString         file;       //!< Source file this message resides.
            Time::TimeValue time;       //!< The local time this message was issued.
        };

    public:

        //! Sets the standard logger interface.
        static void         setStandardLogger( void );

        //! Sets the custom logger interface.
        template<typename TLogger>
        static void         set( void );

        //! Formats and writes the message with specified log level to an output stream.
        static void         write( Log::Level level, CString tag, CString format, ... );

        //! Formats and writes the message with specified log level to an output stream.
        static void         write( const Context& ctx, Log::Level level, CString tag, CString format, ... );

    protected:

        //! Outputs the message to a log.
        virtual void        write( Level level, const Context& ctx, CString tag, CString text ) = 0;

    private:

        static AutoPtr<Log> s_instance; //!< The logger instance.
    };

    // ** Log::set
    template<typename TLogger>
    void Log::set( void )
    {
        s_instance = new TLogger;
    }

    // ** Log::write
    inline void Log::write( Log::Level level, CString tag, CString format, ... )
    {
        if( !s_instance.get() ) {
            return;
        }

        // Format the output message
        NIMBLE_LOGGER_FORMAT( format );

        // Write the message
        s_instance->write( level, Context(), tag, buffer );
    }

    // ** Log::write
    inline void Log::write( const Context& ctx, Log::Level level, CString tag, CString format, ... )
    {
        if( !s_instance.get() ) {
            return;
        }

        // Format the output message
        NIMBLE_LOGGER_FORMAT( format );

        // Write the message
        s_instance->write( level, ctx, tag, buffer );
    }

    //! Generic logger interface with various policies.
    template<typename TFilter, typename TFormatter, typename TWriter>
    class GenericLogger : public Log {
    protected:

        //! Uses all policies to output the message to a log.
        virtual void    write( Level level, const Context& ctx, CString tag, CString text ) NIMBLE_OVERRIDE;

    private:

        TFilter         m_filter;       //!< Log filtering policy.
        TFormatter      m_formatter;    //!< Message formatting policy.
        TWriter         m_writer;       //!< Writes message to an output.
    };

    // ** GenericLogger::write
    template<typename TFilter, typename TFormatter, typename TWriter>
    void GenericLogger<TFilter, TFormatter, TWriter>::write( Level level, const Context& ctx, CString tag, CString text )
    {
        // First ensure that messages passes through a filter
        if( !m_filter.filter( level, tag ) ) {
            return;
        }

        // Now format the message
        String message = m_formatter.format( level, ctx, tag, text );

        // Output message to a log
        m_writer.write( level, message );
    }

    //! Disables the all filtering.
    struct NoLogFiltering {
        bool filter( Log::Level level, CString tag ) { return true; }
    };

    //! Filters the log messages by log level.
    template<s32 TLevel>
    struct FilterLogByLevel {
        bool filter( Log::Level level, CString tag ) { return level >= TLevel; }
    };

    //! Formats the detailed log message.
    struct DetailedLogFormatter {
        String format( Log::Level level, const Log::Context& ctx, CString tag, CString text )
        {
            // Format the level
            CString levelFormatted = "";

            switch( level ) {
            case Log::Debug:    levelFormatted = "D"; break;
            case Log::Verbose:  levelFormatted = "V"; break;
            case Log::Warning:  levelFormatted = "W"; break;
            case Log::Error:    levelFormatted = "E"; break;
            case Log::Fatal:    levelFormatted = "F"; break;
            }

            // Extract the base name from file string
            String baseName = ctx.file;
            u32    idx      = baseName.find_last_of( "\\" );
            baseName        = baseName.substr( idx + 1 );

            // Format the tag
            String tagFormatted = toUpperCase( tag );

            // Perform the final formatting
            s8 formatted[Log::MaxMessageLength];
            if( level == Log::Fatal ) {
                _snprintf( formatted, sizeof( formatted ), "%s %-*s %s %s\n%*s %s (%s)\n", Time::timeString().c_str(), 8, toUpperCase( tag ).c_str(), levelFormatted, text, 41, "at", ctx.function, baseName.c_str() );
            } else {
                _snprintf( formatted, sizeof( formatted ), "%s %-*s %s %s", Time::timeString().c_str(), 8, toUpperCase( tag ).c_str(), levelFormatted, text );
            }
            

            return formatted;
        }
    };

    //! Writes the message to a VisualStudio output window
    extern void debugOutputToIde( CString text );

    //! Writes the log messsage to a stdout
    struct StandardWriter {
        void write( Log::Level level, const String& text )
        {
            printf( "%s", text.c_str() );
        }
    };

    //! Writes a colored message to a console.
    struct ColoredConsoleWriter {
        void write( Log::Level level, const String& text )
        {
        #ifdef NIMBLE_PLATFORM_WINDOWS
            static HANDLE handle = GetStdHandle( STD_OUTPUT_HANDLE );

            switch( level ) {
            case Log::Debug:            SetConsoleTextAttribute( handle, 8 );   break;
            case Log::Verbose:          SetConsoleTextAttribute( handle, 7 );   break;
            case Log::Warning:          SetConsoleTextAttribute( handle, 14 );  break;
            case Log::Error:            SetConsoleTextAttribute( handle, 12 );  break;
            case Log::Fatal:            SetConsoleTextAttribute( handle, 11 );  break;
            }
        #endif  /*  NIMBLE_PLATFORM_WINDOWS */
            printf( "%s", text.c_str() );
        }       
    };

    //! Writes the log message to an IDE output window
    struct IdeWriter {
        void write( Log::Level level, const String& text )
        {
            debugOutputToIde( text.c_str() );
        }
    };

    //! Appends the log message to a log file.
    struct FileWriter {
        void write( Log::Level level, const String& text )
        {
            static FILE* file = NULL;

            if( !file ) {
                 file = fopen( "LogFile.txt", "a+" );
                 NIMBLE_BREAK_IF( file == NULL );
                 fprintf( file, "-------------------------------------------------------------------------------------------------------------------------\n\n" );
            }
            
            fprintf( file, "%s", text.c_str() );
            fflush( file );
        }   
    };

    //! Generic composite writer policy to combine several writers.
    template<typename TPolicyA, typename TPolicyB>
    struct CompositeWriter {
        void write( Log::Level level, const String& text ) {
            TPolicyA().write( level, text );
            TPolicyB().write( level, text );
        }
    };

    //! A type definition for a debug writer that outputs messages to stdout and IDE.
    typedef CompositeWriter<ColoredConsoleWriter, IdeWriter> DebugWriter;

    //! Colored release writer outputs messages to console & file.
    typedef CompositeWriter<StandardWriter, FileWriter> ReleaseWriter;

    //! Colored release writer outputs messages to console & file.
    typedef CompositeWriter<ColoredConsoleWriter, FileWriter> ColoredReleaseWriter;

    //! Release logger.
    typedef GenericLogger<FilterLogByLevel<Log::Verbose>, DetailedLogFormatter, ReleaseWriter> ReleaseLogger;

    //! Colored release logger.
    typedef GenericLogger<FilterLogByLevel<Log::Verbose>, DetailedLogFormatter, ColoredReleaseWriter> ColoredReleaseLogger;

    //! Debug logger.
    typedef GenericLogger<FilterLogByLevel<Log::Debug>,   DetailedLogFormatter, DebugWriter> DebugLogger;

    // ** Log::setStandardLogger
    inline void Log::setStandardLogger( void )
    {
    #ifdef NIMBLE_DEBUG
        set<DebugLogger>();
    #else
        set<ColoredReleaseLogger>();
    #endif  /*  NIMBLE_DEBUG    */
    }

NIMBLE_END

//! Constructs the logger context instance
#define NIMBLE_LOGGER_CONTEXT   \
            Log::Context( __FUNCTION__, NIMBLE_FILE_LINE( __LINE__ ) )

//! This should be placed once somewhere in your code.
#define NIMBLE_LOGGER_STATIC( ... )         \
            NIMBLE_BEGIN                    \
            AutoPtr<Log> Log::s_instance;   \
            NIMBLE_IMPLEMENT_OUTPUT_IDE     \
            NIMBLE_END

//! Declares a namespace with global functions that output log messages with specified tag.
#define NIMBLE_LOGGER_TAG( tag )            \
            namespace log {                 \
                NIMBLE_IMPORT               \
                inline void warn( const Log::Context& ctx, CString format, ... )    { NIMBLE_LOGGER_FORMAT( format ); Log::write( ctx, Log::Warning, #tag, buffer ); }  \
                inline void verbose( const Log::Context& ctx, CString format, ... ) { NIMBLE_LOGGER_FORMAT( format ); Log::write( ctx, Log::Verbose, #tag, buffer ); }  \
                inline void debug( const Log::Context& ctx, CString format, ... )   { NIMBLE_LOGGER_FORMAT( format ); Log::write( ctx, Log::Debug,   #tag, buffer ); }  \
                inline void error( const Log::Context& ctx, CString format, ... )   { NIMBLE_LOGGER_FORMAT( format ); Log::write( ctx, Log::Error,   #tag, buffer ); }  \
                inline void fatal( const Log::Context& ctx, CString format, ... )   { NIMBLE_LOGGER_FORMAT( format ); Log::write( ctx, Log::Fatal,   #tag, buffer ); }  \
            }

//! Private implementation of an debugOutputToIde function.
#ifdef NIMBLE_PLATFORM_WINDOWS
    #define NIMBLE_IMPLEMENT_OUTPUT_IDE void debugOutputToIde( CString text ) { OutputDebugString( text ); }
#else
    #define NIMBLE_IMPLEMENT_OUTPUT_IDE void debugOutputToIde( CString text ) { printf( "%s\n", text ); }
#endif  /*  NIMBLE_PLATFORM_WINDOWS */

#endif  /*  !__Nimble_Logger_H__   */
