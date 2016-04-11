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

#ifndef __Nimble_Preprocessor_H__
#define __Nimble_Preprocessor_H__

#include "Platform.h"
#include "CplusplusFeatures.h"
#include "Debug.h"
#include "EnumClass.h"

//! Macro definition to simplify bitmasks declarations
#define BIT( bit ) ( 1 << (bit) )

//! Deletes the pointer and sets it to NULL
#define NIMBLE_DELETE( pointer ) delete (pointer); (pointer) = NULL;

//! Deletes the array pointer and sets it to NULL
#define NIMBLE_DELETE_ARRAY( pointer ) delete[](pointer); (pointer) = NULL;

//! A preprocessor variable to wrap __FUNCTION__ & __PRETTY_FUNCTION__ variables
#if __GNUC__
    #define NIMBLE_PRETTY_FUNCTION __PRETTY_FUNCTION__
#else
    #define NIMBLE_PRETTY_FUNCTION __FUNCTION__
#endif

//! Converts the preprocessor macro argument to a string
#define NIMBLE_STRINGIFY( ... ) #__VA_ARGS__

//! Constructs the file name and line string
#define NIMBLE_FILE_LINE( line ) __FILE__ ":" NIMBLE_STRINGIFY( line )

//! Macro definition to disable copy constructor.
#define NIMBLE_DISABLE_COPY( T )            \
    private:                                \
        T( const T& );                      \
        T& operator = ( const T& );

//! Macro definition to disable heap allocation of this class.
#define NIMBLE_DISABLE_HEAP_ALLOC()         \
    private:                                \
        void* operator new( size_t size );    \
        void* operator new[]( size_t size );

/*
    From here:
    http://blog.molecular-matters.com/2011/07/22/an-improved-assert/
    http://blog.molecular-matters.com/2011/07/12/a-plethora-of-macros/
*/

#define NIMBLE_JOIN( x, y )     _NIMBLE_JOIN( x, y )
#define _NIMBLE_JOIN( x, y )    __NIMBLE_JOIN( x, y )
#define __NIMBLE_JOIN( x, y )   x##y

// VS 2010 still has this compiler bug which treats a __VA_ARGS__ argument as being one single parameter:
// https://connect.microsoft.com/VisualStudio/feedback/details/521844/variadic-macro-treating-va-args-as-a-single-parameter-for-other-macros#details
#if _MSC_VER >= 1400
#    define _NIMBLE_VA_NUM_ARGS_HELPER(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, N, ...)    N
#    define _NIMBLE_VA_NUM_ARGS_REVERSE_SEQUENCE            10, 9, 8, 7, 6, 5, 4, 3, 2, 1
#    define _NIMBLE_LEFT_PARENTHESIS (
#    define _NIMBLE_RIGHT_PARENTHESIS )
#    define NIMBLE_VA_NUM_ARGS(...)                        _NIMBLE_VA_NUM_ARGS_HELPER _NIMBLE_LEFT_PARENTHESIS __VA_ARGS__, _NIMBLE_VA_NUM_ARGS_REVERSE_SEQUENCE _NIMBLE_RIGHT_PARENTHESIS
#else
#    define NIMBLE_VA_NUM_ARGS(...)                        _NIMBLE_NUM_ARGS(__VA_ARGS__, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1)
#    define _NIMBLE_VA_NUM_ARGS(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, N, ...)    N
#endif

#if _MSC_VER >= 1400
#    define NIMBLE_PASS_VA(...)                            _NIMBLE_LEFT_PARENTHESIS __VA_ARGS__ _NIMBLE_RIGHT_PARENTHESIS
#else
#    define NIMBLE_PASS_VA(...)                            (__VA_ARGS__)
#endif

#define _NIMBLE_EXPAND_ARGS_1( op, a1 )                                 op(a1)
#define _NIMBLE_EXPAND_ARGS_2( op, a1, a2 )                             op(a1) op(a2)
#define _NIMBLE_EXPAND_ARGS_3( op, a1, a2, a3 )                         op(a1) op(a2) op(a3)
#define _NIMBLE_EXPAND_ARGS_4( op, a1, a2, a3, a4 )                     op(a1) op(a2) op(a3) op(a4)
#define _NIMBLE_EXPAND_ARGS_5( op, a1, a2, a3, a4, a5 )                 op(a1) op(a2) op(a3) op(a4) op(a5)
#define _NIMBLE_EXPAND_ARGS_6( op, a1, a2, a3, a4, a5, a6 )             op(a1) op(a2) op(a3) op(a4) op(a5) op(a6)
#define _NIMBLE_EXPAND_ARGS_7( op, a1, a2, a3, a4, a5, a6, a7 )         op(a1) op(a2) op(a3) op(a4) op(a5) op(a6) op(a7)
#define _NIMBLE_EXPAND_ARGS_8( op, a1, a2, a3, a4, a5, a6, a7, a8 )     op(a1) op(a2) op(a3) op(a4) op(a5) op(a6) op(a7) op(a8)
#define _NIMBLE_EXPAND_ARGS_9( op, a1, a2, a3, a4, a5, a6, a7, a8, a9 ) op(a1) op(a2) op(a3) op(a4) op(a5) op(a6) op(a7) op(a8) op(a9)
 
// variadic macro "dispatching" the arguments to the correct macro.
#define NIMBLE_EXPAND_ARGS( op, ... ) NIMBLE_JOIN(_NIMBLE_EXPAND_ARGS_, NIMBLE_VA_NUM_ARGS(__VA_ARGS__)) NIMBLE_PASS_VA(op, __VA_ARGS__)

#endif  /*    !__Nimble_Preprocessor_H__    */
