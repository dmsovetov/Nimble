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

#ifndef __Nimble_NumericTraits_H__
#define __Nimble_NumericTraits_H__

#include "TypeTraits.h"

NIMBLE_BEGIN

    //! Static template checks
    NIMBLE_STATIC_ASSERT(  IsIntegral<u8>::value,           "u8 is an integral type."           );
    NIMBLE_STATIC_ASSERT(  IsIntegral<s8>::value,           "s8 is an integral type."           );
    NIMBLE_STATIC_ASSERT(  IsIntegral<u16>::value,          "u16 is an integral type."          );
    NIMBLE_STATIC_ASSERT(  IsIntegral<s16>::value,          "s16 is an integral type."          );
    NIMBLE_STATIC_ASSERT(  IsIntegral<u32>::value,          "u32 is an integral type."          );
    NIMBLE_STATIC_ASSERT(  IsIntegral<s32>::value,          "s32 is an integral type."          );
    NIMBLE_STATIC_ASSERT(  IsIntegral<u64>::value,          "u64 is an integral type."          );
    NIMBLE_STATIC_ASSERT(  IsIntegral<s64>::value,          "s64 is an integral type."          );

    NIMBLE_STATIC_ASSERT(  IsIntegral<const u8>::value,     "const u8 is an integral type."     );
    NIMBLE_STATIC_ASSERT(  IsIntegral<const s8>::value,     "const s8 is an integral type."     );
    NIMBLE_STATIC_ASSERT(  IsIntegral<const u16>::value,    "const u16 is an integral type."    );
    NIMBLE_STATIC_ASSERT(  IsIntegral<const s16>::value,    "const s16 is an integral type."    );
    NIMBLE_STATIC_ASSERT(  IsIntegral<const u32>::value,    "const u32 is an integral type."    );
    NIMBLE_STATIC_ASSERT(  IsIntegral<const s32>::value,    "const s32 is an integral type."    );
    NIMBLE_STATIC_ASSERT(  IsIntegral<const u64>::value,    "const u64 is an integral type."    );
    NIMBLE_STATIC_ASSERT(  IsIntegral<const s64>::value,    "const s64 is an integral type."    );

    NIMBLE_STATIC_ASSERT(  IsIntegral<volatile u8>::value,     "volatile u8 is an integral type."     );
    NIMBLE_STATIC_ASSERT(  IsIntegral<volatile s8>::value,     "volatile s8 is an integral type."     );
    NIMBLE_STATIC_ASSERT(  IsIntegral<volatile u16>::value,    "volatile u16 is an integral type."    );
    NIMBLE_STATIC_ASSERT(  IsIntegral<volatile s16>::value,    "volatile s16 is an integral type."    );
    NIMBLE_STATIC_ASSERT(  IsIntegral<volatile u32>::value,    "volatile u32 is an integral type."    );
    NIMBLE_STATIC_ASSERT(  IsIntegral<volatile s32>::value,    "volatile s32 is an integral type."    );
    NIMBLE_STATIC_ASSERT(  IsIntegral<volatile u64>::value,    "volatile u64 is an integral type."    );
    NIMBLE_STATIC_ASSERT(  IsIntegral<volatile s64>::value,    "volatile s64 is an integral type."    );

    NIMBLE_STATIC_ASSERT(  IsIntegral<const volatile u8>::value,     "const volatile u8 is an integral type."     );
    NIMBLE_STATIC_ASSERT(  IsIntegral<const volatile s8>::value,     "const volatile s8 is an integral type."     );
    NIMBLE_STATIC_ASSERT(  IsIntegral<const volatile u16>::value,    "const volatile u16 is an integral type."    );
    NIMBLE_STATIC_ASSERT(  IsIntegral<const volatile s16>::value,    "const volatile s16 is an integral type."    );
    NIMBLE_STATIC_ASSERT(  IsIntegral<const volatile u32>::value,    "const volatile u32 is an integral type."    );
    NIMBLE_STATIC_ASSERT(  IsIntegral<const volatile s32>::value,    "const volatile s32 is an integral type."    );
    NIMBLE_STATIC_ASSERT(  IsIntegral<const volatile u64>::value,    "const volatile u64 is an integral type."    );
    NIMBLE_STATIC_ASSERT(  IsIntegral<const volatile s64>::value,    "const volatile s64 is an integral type."    );

    NIMBLE_STATIC_ASSERT( !IsIntegral<f32>::value, "f32 is not an integral type."   );

    NIMBLE_STATIC_ASSERT( IsFloatingPoint<f32>::value, "f32 is a floating point type." );
    NIMBLE_STATIC_ASSERT( IsFloatingPoint<f64>::value, "f64 is a floating point type." );
    NIMBLE_STATIC_ASSERT( IsFloatingPoint<long double>::value, "long double is a floating point type." );

NIMBLE_END

#endif  /*  !__Nimble_NumericTraits_H__ */
