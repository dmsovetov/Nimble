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

//! Wraps the EnableIf template class to simplify the code.
#define NIMBLE_STATIC_IF( Condition, Type )        \
            typename EnableIf<Condition::value, typename Type>::value

//! Wraps the EnableIf template with IsConvertible expression.
#define NIMBLE_IF_CONVERTIBLE( From, To, Type )    \
            typename EnableIf<IsConvertible<typename From, typename To>::value, typename Type>::value

//! Wraps the EnableIf template with !IsConvertible expression.
#define NIMBLE_IFNOT_CONVERTIBLE( From, To, Type )    \
            typename EnableIf<!IsConvertible<typename From, typename To>::value, typename Type>::value

//! Wraps the EnableIf template with IsIntegral expression.
#define NIMBLE_IF_INTEGRAL_TYPE( Input, Output )    \
            typename EnableIf<IsIntegral<typename Input>::value, typename Output>::value

//! Wraps the EnableIf template with IsFloatingPoint expression.
#define NIMBLE_IF_FLOATING_TYPE( Input, Output )    \
            typename EnableIf<IsFloatingPoint<typename Input>::value, typename Output>::value

//! Wraps the EnableIf template with TypeEquals<Input, String> expression.
#define NIMBLE_IF_STRING_TYPE( Input, Output )    \
            typename EnableIf<TypeEquals<typename Input, String>::value, typename Output>::value

//! Wraps the EnableIf template with IsClassOrUnion expression.
#define NIMBLE_IF_CLASS_TYPE( Input, Output )    \
            typename EnableIf<IsClassOrUnion<typename Input>::value && !TypeEquals<typename Input, String>::value, typename Output>::value

//! Wraps the EnableIf template with IsIntegral expression.
#define NIMBLE_IF_INTEGRAL( Input )    \
            typename EnableIf<IsIntegral<typename Input>::value, typename Input>::value

//! Wraps the EnableIf template with IsFloatingPoint expression.
#define NIMBLE_IF_FLOATING( Input )    \
            typename EnableIf<IsFloatingPoint<typename Input>::value, typename Input>::value

//! Wraps the EnableIf template with TypeEquals<Input, String> expression.
#define NIMBLE_IF_STRING( Input )    \
            typename EnableIf<TypeEquals<typename Input, String>::value, typename Input>::value

//! Wraps the EnableIf template with TypeEquals<Input, bool> expression.
#define NIMBLE_IF_BOOL( Input )    \
            typename EnableIf<TypeEquals<typename Input, bool>::value, typename Input>::value

//! Wraps the EnableIf template with IsClassOrUnion expression.
#define NIMBLE_IF_CLASS( Input )    \
            typename EnableIf<IsClassOrUnion<typename Input>::value && !TypeEquals<typename Input, String>::value, typename Input>::value

//! Wraps the EnableIf template with IsEnum expression.
#define NIMBLE_IF_ENUM( Input )    \
            typename EnableIf<IsEnum<typename Input>::value, typename Input>::value

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