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

#ifndef __Nimble_Preprocessor_Debug_H__
#define __Nimble_Preprocessor_Debug_H__

#include "Platform.h"

//! Declare the debugging macro
#if (defined( NIMBLE_PLATFORM_WINDOWS ) && !defined( NDEBUG )) || defined( _DEBUG ) 
    #define NIMBLE_DEBUG
#endif  /*  NIMBLE_DEBUG   */

//! Aborts the application execution
#if NIMBLE_EXIT_ON_ASSERT
    #define NIMBLE_ABORT( code )	exit( code )
#else
    #define NIMBLE_ABORT( code )    // Do nothing
#endif  /*  NIMBLE_EXIT_ON_ASSERT   */

//! Declare the debugging breakpoint macroses
#if defined( NIMBLE_DEBUG )
    #if defined( NIMBLE_PLATFORM_WINDOWS )
        #include <crtdbg.h>
        #define NIMBLE_BREAK        _CrtDbgBreak()
        #define NIMBLE_CHECK_MEMORY NIMBLE_ASSERT( _CrtCheckMemory(), "memory corruption detected" )
    #elif defined( NIMBLE_PLATFORM_IOS ) || defined( DC_PLATFORM_MACOS )
        #if defined __arm__ || defined __thumb__
            #define NIMBLE_BREAK asm( "trap" )
        #elif defined(__i386__) || defined(__x86_64__)
            #define NIMBLE_BREAK asm( "int $0x3" );
        #else
            #define NIMBLE_BREAK
        #endif

        #define NIMBLE_CHECK_MEMORY
    #elif defined( NIMBLE_PLATFORM_ANDROID )
        #define NIMBLE_BREAK
        #define NIMBLE_CHECK_MEMORY
    #elif defined( NIMBLE_PLATFORM_LINUX )
        #if defined(__i386__) || defined(__x86_64__)
            #define NIMBLE_BREAK asm("int $3");
        #else
            #define NIMBLE_BREAK
        #endif

        #define NIMBLE_CHECK_MEMORY
    #endif

    #define NIMBLE_DEBUG_ONLY( ... ) __VA_ARGS__

    //! This macro expects that given expression is TRUE, otherwise it ouputs the fatal error message and quits an application.
	#define NIMBLE_ABORT_IF( expression, ... )	\
                if( expression ) {              \
				    Internal::message( 6, __FUNCTION__, NIMBLE_FILE_LINE( __LINE__ ), "Nimble", "assert", (__VA_ARGS__ " (" NIMBLE_STRINGIFY( expression ) ")") ); \
				    NIMBLE_BREAK;               \
                }
#else
    #define NIMBLE_BREAK
    #define NIMBLE_CHECK_MEMORY
    #define NIMBLE_DEBUG_ONLY( ... )

    //! This macro expects that given expression is TRUE, otherwise it ouputs the fatal error message and quits an application.
	#define NIMBLE_ABORT_IF( expression, ... )	\
                if( expression ) {              \
				    Internal::message( 5, __FUNCTION__, NIMBLE_FILE_LINE( __LINE__ ), "Nimble", "assert", (__VA_ARGS__ " (" NIMBLE_STRINGIFY( expression ) ")") ); \
				    NIMBLE_ABORT( -1 );         \
                }
#endif

//! This macro expects that given expression is TRUE, otherwise it outputs the warning message.
#define NIMBLE_BREAK_IF( expression, ... ) \
            if( expression ) { \
    	        Internal::message( 6, __FUNCTION__, NIMBLE_FILE_LINE( __LINE__ ), "Nimble", "expect", (__VA_ARGS__ " (" NIMBLE_STRINGIFY( expression ) ")\n") ); \
			    NIMBLE_BREAK;    \
            }

//! Preprocessor stub to mark unimplemented code
#define NIMBLE_NOT_IMPLEMENTED \
            Internal::message( 6, __FUNCTION__, NIMBLE_FILE_LINE( __LINE__ ), "Nimble", "assert", "Feature is not implemented\n" ); \
            NIMBLE_BREAK;

//! Preprocessor stub to mark deprecated code
#define NIMBLE_DEPRECATED NIMBLE_BREAK

//! This block of preprocessing disables the debugging macroses
#if NIMBLE_NO_DEBUG
    #undef NIMBLE_ABORT_IF
    #define NIMBLE_ABORT_IF( ... )
    #undef NIMBLE_BREAK_IF
    #define NIMBLE_BREAK_IF( ... )
#endif  /*  NIMBLE_NO_DEBUG */

#endif  /*  !__Nimble_Preprocessor_Debug_H__    */