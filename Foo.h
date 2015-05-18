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

#ifndef __Foo_H__
#define __Foo_H__

#define FOO_INCLUDED

#ifdef FOO_NAMESPACE
    #define FOO_BEGIN_NAMESPACE namespace FOO_NAMESPACE {
    #define FOO_END_NAMESPACE   }
    #define USING_FOO using namespace FOO_NAMESPACE;
#else
    #define FOO_BEGIN_NAMESPACE
    #define FOO_END_NAMESPACE
    #define USING_FOO
#endif

#include <math.h>
#include <stdlib.h>
#include <assert.h>
#include <float.h>

#include "Preprocessor.h"
#include "Exception.h"
#include "Logger.h"
#include "StringHash.h"
#include "Format.h"
#include "Classes.h"
#include "UserData.h"
#include "Guid.h"
#include "Composition.h"

FOO_BEGIN_NAMESPACE
    #include "delegate/Closure.h"
FOO_END_NAMESPACE

#include "memory/WeakPtr.h"
#include "memory/StrongPtr.h"
#include "memory/AutoPtr.h"

#include "Types.h"
#include "Variant.h"
#include "Image.h"

#include "math/FooMath.h"

#include "Bitset.h"

#endif  /*  !defined( __Foo_H__ ) */
