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

#ifndef __Nimble_Templates_H__
#define __Nimble_Templates_H__

#include "Globals.h"

NIMBLE_BEGIN

    //! Root SFINAE template class
    template<bool TExpression, typename T = void>
    struct EnableIf { typedef T value; };

    template<typename T>
    struct EnableIf<false, T> {};

    //! Performs a static type selection.
    template<bool TCondition, typename A, typename B> struct TypeSelector              { typedef A type; };
    template<typename A, typename B>                  struct TypeSelector<false, A, B> { typedef B type; };

    // Tupple index builder
    // http://habrahabr.ru/post/101430/

    template<s32 ... Idxs>
    struct IndexesTuple {};

    template<s32 Num, typename Tp = IndexesTuple<>> 
    struct IndexTupleBuilder;

    template<s32 Num, s32 ... Idxs>  
    struct IndexTupleBuilder<Num, IndexesTuple<Idxs ...>> : IndexTupleBuilder<Num - 1, IndexesTuple<Idxs ..., sizeof ... (Idxs)>> {};

    template<s32 ... Idxs> 
    struct IndexTupleBuilder<0, IndexesTuple<Idxs ...>> 
    { 
	    typedef IndexesTuple<Idxs...> Indexes; 
    };

NIMBLE_END

#endif  /*  !__Nimble_Templates_H__ */