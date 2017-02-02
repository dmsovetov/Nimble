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

#ifndef __Nimble_Preprocessor_Platform_H__
#define __Nimble_Preprocessor_Platform_H__

//! Preprocessor platform identifier constants
#define NIMBLE_WINDOWS      (5001)
#define NIMBLE_MACOS        (5002)
#define NIMBLE_LINUX        (5003)
#define NIMBLE_IOS          (5004)
#define NIMBLE_ANDROID      (5005)
#define NIMBLE_JAVASCRIPT   (5006)  //!< When built with Emscripten

//! Declare the platform preprocessor variable
#if defined( WIN32 )
    #define NIMBLE_PLATFORM NIMBLE_WINDOWS
    #define NIMBLE_PLATFORM_WINDOWS
#endif  /*  WIN32   */

#if defined( __APPLE__ )
    #include "TargetConditionals.h"

    #if TARGET_OS_OSX
        #define NIMBLE_PLATFORM NIMBLE_MACOS
        #define NIMBLE_PLATFORM_MACOS
        #error osx!
    #elif TARGET_OS_IOS
        #define NIMBLE_PLATFORM NIMBLE_IOS
        #define NIMBLE_PLATFORM_IOS
    #endif  /*  TARGET_OS_OSX   */
#endif  /*  __APPLE__ */

//! Check that platform preprocessor variable is selected
#if !defined( NIMBLE_PLATFORM )
    #error Nimble: unknown platform
#endif  /*  !NIMBLE_PLATFORM    */

#endif  /*  !__Nimble_Preprocessor_Platform_H__    */
