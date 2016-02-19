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

//! Declare the debugging breakpoint macroses
#if defined( NIMBLE_DEBUG )
    #if defined( NIMBLE_PLATFORM_WINDOWS )
        #if defined( _WIN64 )
            #include <crtdbg.h>
            #define NIMBLE_BREAK_IF( condition ) if( condition ) { _CrtDbgBreak(); }
        #else
            #define NIMBLE_BREAK_IF( condition ) if( condition ) { __asm { int 3 } }
        #endif

        #define NIMBLE_CHECK_MEMORY NIMBLE_BREAK_IF( _CrtCheckMemory() == 0 )
    #elif defined( NIMBLE_PLATFORM_IOS ) || defined( DC_PLATFORM_MACOS )
        #if defined __arm__ || defined __thumb__
            #define NIMBLE_BREAK_IF( condition ) if( condition ) { asm( "trap" ); }
        #elif defined(__i386__) || defined(__x86_64__)
            #define NIMBLE_BREAK_IF( condition ) if( condition ) { asm( "int $0x3" ); }
        #else
            #define NIMBLE_BREAK_IF( condition ) assert( !(condition) );
        #endif

        #define NIMBLE_CHECK_MEMORY
    #elif defined( NIMBLE_PLATFORM_ANDROID )
        #define NIMBLE_BREAK_IF( condition )    \
            if( condition )  {                  \
                char buffer[1024];              \
                sprintf( buffer, "Debug break at %s, line %d\nExpression: %s\n", __FILE__, __LINE__, #condition );    \
                __android_log_print( ANDROID_LOG_WARN, "DreemchestNative", buffer );    \
            }

        #define NIMBLE_CHECK_MEMORY
    #elif defined( NIMBLE_PLATFORM_LINUX )
        #if defined(__i386__) || defined(__x86_64__)
            #define NIMBLE_BREAK_IF( condition ) if( condition ) { asm("int $3"); }
        #else
            #include <cassert>
            #define NIMBLE_BREAK_IF( condition ) assert( !(condition) );
        #endif

        #define NIMBLE_CHECK_MEMORY
    #endif

    #define NIMBLE_DEBUG_ONLY( ... )		__VA_ARGS__
    #define NIMBLE_ASSERT( condition )		NIMBLE_BREAK_IF( !(condition) )
    #define NIMBLE_BREAK                    NIMBLE_BREAK_IF( true )
#else
    #define NIMBLE_BREAK_IF( condition )
    #define NIMBLE_BREAK
    #define NIMBLE_ASSERT( condition )
    #define NIMBLE_CHECK_MEMORY
    #define NIMBLE_DEBUG_ONLY( ... )
#endif

//! Preprocessor stub to mark unimplemented code
#define NIMBLE_NOT_IMPLEMENTED NIMBLE_BREAK

//! Preprocessor stub to mark deprecated code
#define NIMBLE_DEPRECATED NIMBLE_BREAK

#endif  /*  !__Nimble_Preprocessor_Debug_H__    */