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
            s8	    buffer[Logger::MaxMessageLength];           \
            va_list ap;                                         \
            va_start( ap, format );                             \
            vsnprintf( buffer, sizeof( buffer ), format, ap );  \
            va_end( ap );

NIMBLE_BEGIN

    //! Writes the message to a VisualStudio output window
    extern void debugOutputToIde( CString text );

    //! Shared logger interface.
    class Logger {
    public:

        //! Maximum message length constant
        enum { MaxMessageLength = 1024 };

        //! Available log levels.
        enum Level {
              Debug         //!< This log level is used for debug messages.
            , Verbose       //!< Used for a verbose messages.
            , Milestone     //!< The milestone message.
            , Warning       //!< Used for warning messages.
            , Error         //!< Used for error messages.
            , Fatal         //!< Logs the fatal error message.
            , Internal      //!< Logs an internal error message.
        };

        //! Log message context.
        struct Context {
                                    //! Constructs Context instance.
                                    Context( CString function = "", CString file = "" )
                                        : function( function ), file( file ), time( Time::localTime() ) {}

            CString                 function;   //!< Parent function that issued the message.
            CString                 file;       //!< Source file this message resides.
            Time::TimeValue         time;       //!< The local time this message was issued.
        };

        //! Log filtering policy base class.
        struct Filter {
            //! This function should return true if message passes the filter.
            virtual bool            filter( Level level, CString tag, CString prefix ) const = 0;
        };

        //! Filters the log messages by log level.
        struct FilterByLevel : public Filter {
                                    //! Constructs FilterByLevel instance.
                                    FilterByLevel( Level level ) : m_level( level ) {}

            //! Any message that has greater or equal level passes this filter.
            virtual bool            filter( Level level, CString tag, CString prefix ) const NIMBLE_OVERRIDE;

            Level                   m_level;    //!< Log level.
        };

        //! Filters the log messages by a prefix. Any message that has a prefix contained in list passes the filter.
        struct FilterByPrefix : public Filter {
                                    //! Constructs FilterByPrefix instance.
                                    FilterByPrefix( const StringList& prefixes, Level level = Fatal, bool inverse = false ) : prefixes( prefixes ), level( level ), inverse( inverse ) {}

            //! Any message that has a prefix contained in list passes the filter.
            virtual bool            filter( Level level, CString tag, CString prefix ) const NIMBLE_OVERRIDE;

            StringList              prefixes;   //!< List of prefixes.
            Level                   level;      //!< The message level required to ignore this filter.
            bool                    inverse;    //!< Inverse the filter.
        };

        //! Formats the log message to a string.
        struct Formatter {
            virtual String          format( Level level, const Context& ctx, CString tag, CString prefix, CString text ) const = 0;
        };

        //! Formatter base class contains utility formatting functions.
        struct BaseFormatter : public Formatter {
            //! Formats the tag string.
            String                  formatTag( CString value ) const;

            //! Formats the level to a string.
            String                  formatLevel( Level value ) const;

            //! Extracts the base name from a file string.
            String                  baseFileName( CString value ) const;
        };

        //! Outputs the detailed log message with date, time, tag, prefix.
        struct DetailedFormatter : public BaseFormatter {
            String                  format( Logger::Level level, const Logger::Context& ctx, CString tag, CString prefix, CString text ) const NIMBLE_OVERRIDE;
        };

        //! Outputs the short message with time, tag, level and prefix.
        struct ShortFormatter : public BaseFormatter {
            String                  format( Logger::Level level, const Logger::Context& ctx, CString tag, CString prefix, CString text ) const NIMBLE_OVERRIDE;
        };

        //! Outputs the log message.
        struct Writer {
            virtual void            write( Level level, const String& text ) const = 0;  
        };

        //! Writes the log messsage to a stdout
        struct StandardWriter : public Writer {
            virtual void            write( Level level, const String& text ) const NIMBLE_OVERRIDE;
        };

        //! Outputs the colored log message to a console.
        struct ColoredConsoleWriter : public StandardWriter {
            virtual void            write( Level level, const String& text ) const NIMBLE_OVERRIDE;
        };

        //! Outputs the log message to an IDE output window
        struct IdeWriter : public Writer {
            virtual void            write( Level level, const String& text ) const NIMBLE_OVERRIDE;
        };

        //! Opens the log file in append mode and outputs the log message.
        struct FileWriter : public Writer {
                                    //! Constructs the FileWriter instance.
                                    FileWriter( const String& fileName = "LogFile.txt" ) : fileName( fileName ) {}
            virtual void            write( Level level, const String& text ) const NIMBLE_OVERRIDE;
            String                  fileName; //!< Log file name.
        };

        //! Composite writer policy to combine two writers in a single one.
        template<typename T, typename U>
        struct CompositeWriter : public Writer {
                                    //! Constructs CompositeWriter instance.
                                    CompositeWriter( void ) : first( new T ), second( new U ) {}
            virtual void            write( Level level, const String& text ) const NIMBLE_OVERRIDE;
            AutoPtr<T>              first;    //!< First writer.
            AutoPtr<U>              second;   //!< Second writer.
        };

        //! Composite filter policy to combine two filters in a single one.
        struct CompositeFilter : public Filter {
                                     //! Constructs CompositeFilter instance.
                                    CompositeFilter( AutoPtr<Filter> first, AutoPtr<Filter> second )
                                        : first( first ), second( second ) {}
            virtual bool            filter( Level level, CString tag, CString prefix ) const NIMBLE_OVERRIDE;
            AutoPtr<Filter>         first;    //!< First filter.
            AutoPtr<Filter>         second;   //!< Second filter.       
        };

        //! A type definition for a debug writer that outputs messages to stdout and IDE.
        typedef CompositeWriter<ColoredConsoleWriter, IdeWriter> DebugWriter;

        //! Colored release writer outputs messages to console & file.
        typedef CompositeWriter<StandardWriter, FileWriter> ReleaseWriter;

        //! Colored release writer outputs messages to console & file.
        typedef CompositeWriter<ColoredConsoleWriter, FileWriter> ColoredReleaseWriter;

    public:

        //! Sets the standard logger interface.
        static void                 setStandardLogger( void );

        //! Sets the custom log filter.
        static void                 setFilter( AutoPtr<Filter> value );

        //! Sets the custom log writer.
        static void                 setFormatter( AutoPtr<Formatter> value );

        //! Sets the custom log writer.
        static void                 setWriter( AutoPtr<Writer> value );

        //! Formats and writes the message with specified log level to an output stream.
        static void                 write( Logger::Level level, CString tag, CString prefix, CString format, ... );

        //! Formats and writes the message with specified log level to an output stream.
        static void                 write( const Context& ctx, Logger::Level level, CString tag, CString prefix, CString format, ... );

    protected:

        //! Outputs the message to a log.
        virtual void                write( Level level, const Context& ctx, CString tag, CString prefix, CString text );

    private:

        static Logger               s_instance;     //!< Shared logger instance.
        AutoPtr<Filter>             m_filter;       //!< Filtering policy.
        AutoPtr<Formatter>          m_formatter;    //!< Message formatting policy.
        AutoPtr<Writer>             m_writer;       //!< Log writer policy.
    };

    // ** Logger::setFilter
    inline void Logger::setFilter( AutoPtr<Filter> value )
    {
        s_instance.m_filter = value;
    }

    // ** Logger::setFormatter
    inline void Logger::setFormatter( AutoPtr<Formatter> value )
    {
        s_instance.m_formatter = value;
    }

    // ** Logger::setWriter
    inline void Logger::setWriter( AutoPtr<Writer> value )
    {
        s_instance.m_writer = value;
    }

    // ** Logger::write
    inline void Logger::write( Logger::Level level, CString tag, CString prefix, CString format, ... )
    {
        // Format the output message
        NIMBLE_LOGGER_FORMAT( format );

        // Write the message
        s_instance.write( level, Context(), tag, prefix, buffer );
    }

    // ** Logger::write
    inline void Logger::write( const Context& ctx, Logger::Level level, CString tag, CString prefix, CString format, ... )
    {
        // Format the output message
        NIMBLE_LOGGER_FORMAT( format );

        // Write the message
        s_instance.write( level, ctx, tag, prefix, buffer );
    }

    // ** Logger::write
    inline void Logger::write( Level level, const Context& ctx, CString tag, CString prefix, CString text )
    {
        // Ignore all messages if there is no formatter or writer set
        if( !m_formatter.get() || !m_writer.get() ) {
            return;
        }

        // First ensure that messages passes through a filter
        if( m_filter.get() && !m_filter->filter( level, tag, prefix ) ) {
            return;
        }

        // Now format the message
        String message = m_formatter->format( level, ctx, tag, prefix, text );

        // Output message to a log
        m_writer->write( level, message );
    }

    // ** Logger::StandardWriter::write
    inline void Logger::StandardWriter::write( Logger::Level level, const String& text ) const
    {
        printf( "%s", text.c_str() );
    }

    // ** Logger::FilterByLevel::filter
    inline bool Logger::FilterByLevel::filter( Level level, CString tag, CString prefix ) const
    {
        return level >= m_level;
    }

    // ** Logger::FilterByLevel::filter
    inline bool Logger::FilterByPrefix::filter( Level level, CString tag, CString prefix ) const
    {
        if( level >= this->level ) {
            return true;
        }

        bool contains = prefixes.contains( prefix );
        return inverse ? !contains : contains;
    }

    // ** Logger::BaseFormatter::formatTag
    inline String Logger::BaseFormatter::formatTag( CString value ) const
    {
        return toUpperCase( value );
    }

    // ** Logger::BaseFormatter::formatLevel
    inline String Logger::BaseFormatter::formatLevel( Level value ) const
    {
        switch( value ) {
        case Logger::Debug:     return "D";
        case Logger::Verbose:   return "V";
        case Logger::Milestone: return "M";
        case Logger::Warning:   return "W";
        case Logger::Error:     return "E";
        case Logger::Fatal:     return "F";
        case Logger::Internal:  return "I";
        default:                NIMBLE_BREAK_IF( true, "unexpected log level" );
        }

        return "";
    }

    // ** Logger::BaseFormatter::baseFileName
    inline String Logger::BaseFormatter::baseFileName( CString value ) const
    {
        String baseName = value;
        u32    idx      = baseName.find_last_of( "\\" );
        return baseName.substr( idx + 1 );
    }

    // ** Logger::DetailedFormatter::format
    inline String Logger::DetailedFormatter::format( Logger::Level level, const Context& ctx, CString tag, CString prefix, CString text ) const
    {
        // Format the level
        String _level = formatLevel( level );

        // Format the tag
        String _tag = formatTag( tag );

        // Format the date string
        String _date = Time::formatTimeString( "%Y-%m-%d %I:%M:%S " ) + Time::timeZoneString();

        // Perform the final formatting
        s8 formatted[Logger::MaxMessageLength];
        if( level == Logger::Fatal || level == Logger::Internal ) {
            String baseName = baseFileName( ctx.file );
            _snprintf( formatted, sizeof( formatted ), "%s %-*s %s [%s] %s\n%*s %s (%s)\n", _date.c_str(), 8, _tag.c_str(), _level.c_str(), prefix, text, 41, "at", ctx.function, baseName.c_str() );
        } else {
            _snprintf( formatted, sizeof( formatted ), "%s %-*s %s [%s] %s", _date.c_str(), 8, _tag.c_str(), _level.c_str(), prefix, text );
        }

        return formatted;
    }

    // ** Logger::ShortFormatter::format
    inline String Logger::ShortFormatter::format( Logger::Level level, const Context& ctx, CString tag, CString prefix, CString text ) const
    {
        // Format the level
        String _level = formatLevel( level );

        // Format the tag
        String _tag = formatTag( tag );

        // Format the date string
        String _date = Time::formatTimeString( "%I:%M:%S" );

        // Perform the final formatting
        s8 formatted[Logger::MaxMessageLength];
        if( level == Logger::Fatal || level == Logger::Internal ) {
            String baseName = baseFileName( ctx.file );
            _snprintf( formatted, sizeof( formatted ), "%s %-*s %s [%s] %s\n%*s %s (%s)\n", _date.c_str(), 8, _tag.c_str(), _level.c_str(), prefix, text, 26, "at", ctx.function, baseName.c_str() );
        } else {
            _snprintf( formatted, sizeof( formatted ), "%s %-*s %s [%s] %s", _date.c_str(), 8, _tag.c_str(), _level.c_str(), prefix, text );
        }

        return formatted;
    }

    // ** Logger::ColoredConsoleWriter::write
    inline void Logger::ColoredConsoleWriter::write( Level level, const String& text ) const
    {
    #ifdef NIMBLE_PLATFORM_WINDOWS
        static HANDLE handle = GetStdHandle( STD_OUTPUT_HANDLE );

        switch( level ) {
        case Logger::Debug:             SetConsoleTextAttribute( handle, 8 );   break;
        case Logger::Verbose:           SetConsoleTextAttribute( handle, 7 );   break;
        case Logger::Milestone:         SetConsoleTextAttribute( handle, 10 );  break;
        case Logger::Warning:           SetConsoleTextAttribute( handle, 14 );  break;
        case Logger::Error:             SetConsoleTextAttribute( handle, 12 );  break;
        case Logger::Fatal:             SetConsoleTextAttribute( handle, 11 );  break;
        case Logger::Internal:          SetConsoleTextAttribute( handle, 13 );  break;
        }
    #endif  /*  NIMBLE_PLATFORM_WINDOWS */

        StandardWriter::write( level, text );

    #ifdef NIMBLE_PLATFORM_WINDOWS
        SetConsoleTextAttribute( handle, 7 );
    #endif  /*  NIMBLE_PLATFORM_WINDOWS */
    }       

    // ** Logger::IdeWriter::write
    inline void Logger::IdeWriter::write( Level level, const String& text ) const
    {
        debugOutputToIde( text.c_str() );
    }

    // ** Logger::IdeWriter::write
    inline void Logger::FileWriter::write( Level level, const String& text ) const
    {
        static FILE* file = NULL;

        if( !file ) {
            file = fopen( fileName.c_str(), "a+" );
            NIMBLE_BREAK_IF( file == NULL, "failed to open the log file" );
            fprintf( file, "-------------------------------------------------------------------------------------------------------------------------\n\n" );
        }
            
        fprintf( file, "%s", text.c_str() );
        fflush( file );
    }

    // ** Logger::CompositeWriter::write
    template<typename T, typename U>
    void Logger::CompositeWriter<T, U>::write( Level level, const String& text ) const
    {
        first->write( level, text );
        second->write( level, text );
    }

    // ** Logger::CompositeFilter::filter
    inline bool Logger::CompositeFilter::filter( Level level, CString tag, CString prefix ) const
    {
        return first->filter( level, tag, prefix ) && second->filter( level, tag, prefix );
    }

    // ** Logger::setStandardLogger
    inline void Logger::setStandardLogger( void )
    {
    #ifdef NIMBLE_DEBUG
        setFilter( new FilterByLevel( Debug ) );
        setFormatter( new ShortFormatter );
        setWriter( new DebugWriter );
    #else
        setFilter( new FilterByLevel( Verbose ) );
        setFormatter( new DetailedFormatter );
        setWriter( new ColoredReleaseWriter );
    #endif  /*  NIMBLE_DEBUG    */
    }

	namespace Internal {
	
        // ** message
		inline void message( int level, const char* function, const char* file, const char* tag, const char* prefix, const char* format, ... )
		{
			NIMBLE_LOGGER_FORMAT( format );
			Logger::write( Logger::Context( function, file ), static_cast<Logger::Level>( level ), tag, prefix, buffer );
		}

	} // namespace Internal

NIMBLE_END

//! Constructs the logger context instance
#define NIMBLE_LOGGER_CONTEXT   \
            Logger::Context( __FUNCTION__, NIMBLE_FILE_LINE( __LINE__ ) )

//! This should be placed once somewhere in your code.
#define NIMBLE_LOGGER_STATIC( ... )         \
            NIMBLE_BEGIN                    \
            Logger Logger::s_instance;      \
            NIMBLE_IMPLEMENT_OUTPUT_IDE     \
            NIMBLE_END

//! Declares a namespace with global functions that output log messages with specified tag.
#define NIMBLE_LOGGER_TAG( tag )            \
            namespace Log {                 \
                NIMBLE_IMPORT               \
                inline void warn( const Logger::Context& ctx, CString prefix, CString format, ... )    { NIMBLE_LOGGER_FORMAT( format ); Logger::write( ctx, Logger::Warning, #tag, prefix, buffer ); }  \
                inline void verbose( const Logger::Context& ctx, CString prefix, CString format, ... ) { NIMBLE_LOGGER_FORMAT( format ); Logger::write( ctx, Logger::Verbose, #tag, prefix, buffer ); }  \
                inline void milestone( const Logger::Context& ctx, CString prefix, CString format, ... ) { NIMBLE_LOGGER_FORMAT( format ); Logger::write( ctx, Logger::Milestone, #tag, prefix, buffer ); }  \
                inline void debug( const Logger::Context& ctx, CString prefix, CString format, ... )   { NIMBLE_LOGGER_FORMAT( format ); Logger::write( ctx, Logger::Debug,   #tag, prefix, buffer ); }  \
                inline void error( const Logger::Context& ctx, CString prefix, CString format, ... )   { NIMBLE_LOGGER_FORMAT( format ); Logger::write( ctx, Logger::Error,   #tag, prefix, buffer ); }  \
                inline void fatal( const Logger::Context& ctx, CString prefix, CString format, ... )   { NIMBLE_LOGGER_FORMAT( format ); Logger::write( ctx, Logger::Fatal,   #tag, prefix, buffer ); }  \
                inline void internalError( const Logger::Context& ctx, CString prefix, CString format, ... )   { NIMBLE_LOGGER_FORMAT( format ); Logger::write( ctx, Logger::Internal, #tag, prefix, buffer ); }  \
            }

//! Private implementation of an debugOutputToIde function.
#ifdef NIMBLE_PLATFORM_WINDOWS
    #define NIMBLE_IMPLEMENT_OUTPUT_IDE void debugOutputToIde( CString text ) { OutputDebugString( text ); }
#else
    #define NIMBLE_IMPLEMENT_OUTPUT_IDE void debugOutputToIde( CString text ) { printf( "%s\n", text ); }
#endif  /*  NIMBLE_PLATFORM_WINDOWS */

#endif  /*  !__Nimble_Logger_H__   */
