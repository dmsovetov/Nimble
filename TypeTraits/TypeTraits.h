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

#ifndef __Nimble_TypeTraits_H__
#define __Nimble_TypeTraits_H__

#include "../Globals.h"

NIMBLE_BEGIN

    //! A helper template to define types with an integral constant inside.
    template<s32 TValue>
    struct IntegralConstant
    {
        enum {
            value = TValue
        };
    };

    //! Helper types used by SFINAE
    struct TrueType  : public IntegralConstant<1> {};
    struct FalseType : public IntegralConstant<0> {};

    //! Base class for all true/false types used by SFINAE
    template<bool>
    struct TrueFalseType : FalseType {};

    template<>
    struct TrueFalseType<true> : TrueType {};

    //! Base class for all yes/no types.
    struct YesNoType {
        typedef s8 Yes;
        typedef s8 No[2];
    };

    //! Template class to check if the specified type is a boolean.
    template<typename TValue>
    class IsBoolean : public FalseType {};
    template<> class IsBoolean<bool> : public TrueType {};

    //! Tests whether the type TFrom can be converted to a type TTo.
    template<typename TFrom, typename TTo>
    struct IsConvertible : public YesNoType {
        static Yes&   test( TTo );
        static No&    test( ... );
        static TFrom& create( void );

        enum { value = (sizeof( test( create() ) ) == sizeof( Yes )) };
    };

    //! Tests whether the type is a reference.
    template<typename T> struct IsReference         : FalseType {};
    template<typename T> struct IsReference<T&>     : TrueType  {};

    //! Tests whether the type is a pointer.
    template<typename T> struct IsPointer                   : FalseType {};
    template<typename T> struct IsPointer<T*>               : TrueType  {};
    template<typename T> struct IsPointer<const T>          : IsPointer<T> { };
    template<typename T> struct IsPointer<volatile T>       : IsPointer<T> { };
    template<typename T> struct IsPointer<const volatile T> : IsPointer<T> { };

    //! Tests whether the type is constant.
    template <typename T> struct IsConst            : FalseType {};
    template <typename T> struct IsConst<const T>   : TrueType  {};

    //! Tests whether type A is same as type B.
    template<typename A, typename B>
    struct TypeEquals : public FalseType {};

    template<typename A>
    struct TypeEquals<A, A> : public TrueType {};

    //! Static template checks
    NIMBLE_STATIC_ASSERT( !IsReference<s32>::value,         "s32 is not a reference type."      );
    NIMBLE_STATIC_ASSERT(  IsReference<s32&>::value,        "s32& is a reference type."         );
    NIMBLE_STATIC_ASSERT(  IsReference<const s32&>::value,  "const s32& is a reference type."   );

    NIMBLE_STATIC_ASSERT( !IsReference<s32>::value,         "s32 is not a pointer type."        );
    NIMBLE_STATIC_ASSERT(  IsPointer<s32*>::value,          "s32& is a pointer type."           );
    NIMBLE_STATIC_ASSERT(  IsPointer<const s32*>::value,    "const s32& is a pointer type."     );

    NIMBLE_STATIC_ASSERT( !IsConst<s32>::value,             "s32 is not a constant type."       );
    NIMBLE_STATIC_ASSERT(  IsConst<const s32>::value,       "const s32 is a constant type."     );

    NIMBLE_STATIC_ASSERT( !IsBoolean<f32>::value,           "f32 is no a boolean type."         );
    NIMBLE_STATIC_ASSERT(  IsBoolean<bool>::value,          "bool is a boolean type."           );

//    NIMBLE_STATIC_ASSERT(  TypeEquals<bool, bool>::value,   "bool and bool are same types."    );
//    NIMBLE_STATIC_ASSERT(  TypeEquals<bool, int>::value,    "bool and int are not same types." );

NIMBLE_END

#endif  /*  !__Nimble_TypeTraits_H__ */