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

#ifndef __Foo_Exception_H__
#define __Foo_Exception_H__

#include <string.h>

#pragma warning( disable : 4996 )

FOO_BEGIN_NAMESPACE

    // ** class Exception
    class Exception {
    public:

        enum { MaxMessageLength = 4096 };

        char			message[MaxMessageLength];
                        Exception( const char *string = "" ) { strncpy( message, string, MaxMessageLength ); }

    public:

        static void		Error( const char *message, ... );
    };

    // ** Exception::Error
    inline void Exception::Error( const char *message, ... ) {
    #ifdef DC_EXCEPTIONS_ENABLED
        va_list		ap;
        char		formated[MaxMessageLength];

        va_start( ap, message );
        vsnprintf( formated, MaxMessageLength, message, ap );
        va_end( ap );
        
        throw Exception( formated );
    #endif
    }

FOO_END_NAMESPACE

#endif  /*  !defined( __Foo_Exception_H__ )  */
