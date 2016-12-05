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

#ifndef __Nimble_Preprocessor_CplusplusFeatures_H__
#define __Nimble_Preprocessor_CplusplusFeatures_H__

/*!
    Inspired by:
    http://blog.molecular-matters.com/2011/07/12/a-plethora-of-macros/
*/

//! Enable the C++11 support macro.
#if __cplusplus > 199711L && !defined( NIMBLE_CPP11_DISABLED )
    #define NIMBLE_CPP11_ENABLED
#endif  /* NIMBLE_CPP11_ENABLED */

//! Just a workaround
#if !defined( NIMBLE_CPP11_DISABLED )
    #define NIMBLE_CPP11_ENABLED (1)
#endif  /*  !NIMBLE_CPP11_DISABLED  */

//! Define the macroses for C++11 keywords
#if defined( NIMBLE_CPP11_ENABLED )
    #define NIMBLE_OVERRIDE         override
    #define NIMBLE_FINAL            final
    #define NIMBLE_ABSTRACT         = 0
    #define NIMBLE_STATIC_ASSERT( expression, message ) static_assert( expression, message )
#else
     //!< Just empty preprocessor stubs for backward compatibility.
    #define NIMBLE_OVERRIDE
    #define NIMBLE_FINAL
    #define NIMBLE_ABSTRACT         = 0
    #define NIMBLE_STATIC_ASSERT( expression, message )
#endif  /*  NIMBLE_CPP11_ENABLED    */

//! Macro definition to mark functions as inline
#ifdef NIMBLE_PLATFORM_WINDOWS
    #define NIMBLE_INLINE           __forceinline
    #define NIMBLE_NO_INLINE        __declspec( noinline )
    #define NIMBLE_HINT( hint )     __assume( hint )
    #define NIMBLE_PRAGMA( pragma ) __pragma( pragma )
    #define NIMBLE_RESTRICT         __restrict
    #define NIMBLE_RESTRICT_RV      __declspec( restrict )
    #define NIMBLE_NO_ALIAS         __declspec( noalias )
#else
    #define NIMBLE_INLINE           inline
    #define NIMBLE_NO_INLINE
    #define NIMBLE_HINT( hint )
    #define NIMBLE_PRAGMA( pragma )
    #define NIMBLE_RESTRICT
    #define NIMBLE_RESTRICT_RV
    #define NIMBLE_NO_ALIAS
#endif  /*  NIMBLE_PLATFORM_WINDOWS */

#define NIMBLE_NO_DEFAULT       NIMBLE_HINT( 0 )

#endif  /*  !__Nimble_Preprocessor_CplusplusFeatures_H__    */
