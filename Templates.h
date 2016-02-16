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

    //! A helper template to define types with an integral constant inside.
    template<s32 TValue>
    struct IntegralConstant { enum { value = TValue }; };

    //! Helper types used by SFINAE
    struct TrueType  : public IntegralConstant<1> {};
    struct FalseType : public IntegralConstant<0> {};

    //! Base class for all true/false types used by SFINAE
    template<bool>
    struct TrueFalseType : FalseType {};

    template<>
    struct TrueFalseType<true> : TrueType {};

    //! Root SFINAE template class
    template<bool TExpression, typename T = void>
    struct EnableIf { typedef T value; };

    template<typename T>
    struct EnableIf<false, T> {};

    //! Template class to check if the specified type is integral
    template<typename TValue>
    class IsIntegral : public FalseType {};

    template<> class IsIntegral<char>			: public TrueType {};
    template<> class IsIntegral<unsigned char>	: public TrueType {};
    template<> class IsIntegral<short>			: public TrueType {};
    template<> class IsIntegral<unsigned short>	: public TrueType {};
    template<> class IsIntegral<int>			: public TrueType {};
    template<> class IsIntegral<unsigned int>	: public TrueType {};
    template<> class IsIntegral<long>			: public TrueType {};
    template<> class IsIntegral<unsigned long>	: public TrueType {};

    //! Template class to check if the specified type is a boolean.
    template<typename TValue>
    class IsBoolean : public FalseType {};
    template<> class IsBoolean<bool> : public TrueType {};

    //! Template class to check if the specified type is a floating point.
    template<typename TValue>
    class IsFloatingPoint : public FalseType {};

    template<> class IsFloatingPoint<float>  : public TrueType {};
    template<> class IsFloatingPoint<double> : public TrueType {};

    //! Template type to check if the specified type is an arithmetic type.
    template<typename TValue>
    class IsArithmetic : public TrueFalseType<IsFloatingPoint<TValue>::value || IsIntegral<TValue>::value || IsBoolean<TValue>::value> {};

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