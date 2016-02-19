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

#endif  /*    !__Nimble_Preprocessor_H__    */
