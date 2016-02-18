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

//! Declares a member detector class. https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Member_Detector
#define NIMBLE_DECLARE_MEMBER_DETECTOR( name )                                                  \
            template<typename T>                                                                \
            class Has_ ## name : public YesNoType {                                             \
                struct Fallback { int name; };                                                  \
                struct Derived : T, Fallback { };                                               \
                template<typename U, U> struct Check;                                           \
                template<typename U>    static No&  test( Check<int Fallback::*, &U::name>* );  \
                template<typename U>    static Yes& test( ... );                                \
            public:                                                                             \
                enum { value = sizeof( test<Derived>(0) ) == sizeof( Yes ) };                   \
            };

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

    //! Tests whether a type T is a class or union.
    template<typename T>
    struct IsClassOrUnion : public YesNoType {
    private:

        template<typename U> static Yes& test( void (U::*)() );
        template<typename U> static No&  test( ... );

    public:

        enum { value = sizeof( test<T>( 0 ) ) == sizeof( Yes ) };
    };
    
    NIMBLE_DECLARE_MEMBER_DETECTOR( toString )

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

    NIMBLE_STATIC_ASSERT( (TypeEquals<bool, bool>::value),   "bool and bool are same types."    );
    NIMBLE_STATIC_ASSERT( (!TypeEquals<bool, int>::value),   "bool and int are not same types." );


    //! Generic type to declare the type convertion function signature.
    template<typename TTo>
    struct TypeConvertionFunction {
        typedef TTo ( *Signature )( const void* value );
    };

    //! Type construction function signature. Construction function performs a placement new to call a constructor of type.
    typedef void ( *TypeConstructorSignature )( void* instance, const void* copy );

    //! A destruction function should call a destructor for a valid type instance.
    typedef void ( *TypeDestructorSignature )( void* instance );

    //! Generic type traits to extract all needed info about a specified type.
    template<typename T>
    class TypeTraits {
    public:

        enum {
              Integral   = IsIntegral<T>::value         //!< Indicates that a type is integral.
            , Floating   = IsFloatingPoint<T>::value    //!< Indicates that a type is floating point.
            , Arithmetic = IsArithmetic<T>::value       //!< Indicates that a type is arithmetic.
            , Size       = sizeof( T )                  //!< Stores the type size in bytes.
        };

        //! A helper struct to declare the static type conversion function.
        template<typename U>
        struct TypeConverter {
            //! Converts the value (expected to be of type T) to value of type U.
            static U thunk( const void* value ) { return static_cast<U>( *reinterpret_cast<const T*>( value ) ); }
        };

        //! Returns the type conversion function if the type cast from type T to a type U is valid.
        template<typename U>
        static typename EnableIf<IsConvertible<T, U>::value, typename TypeConvertionFunction<U>::Signature>::value typeCast( void )
        {
            return &TypeConverter<U>::thunk;
        }
    
        //! Returns a NULL pointer if the type cast from type T to a type U is not valid.
        template<typename U>
        static typename EnableIf<!IsConvertible<T, U>::value, typename TypeConvertionFunction<U>::Signature>::value typeCast( void )
        {
            return NULL;
        }

        //! Returns the type conversion function if the type T has a toString method.
        template<typename U>
        static typename EnableIf<Has_toString<U>::value, typename TypeConvertionFunction<String>::Signature>::value detectToString( void )
        {
            struct ToString {
                //! Constructs an instance of type T.
                static String thunk( const void* instance )
                {
                    return reinterpret_cast<const U*>( instance )->toString();
                }
            };

            return &ToString::thunk;       
        }

        //! Returns a NULL pointer if the type T does not have a toString method.
        template<typename U>
        static typename EnableIf<!Has_toString<U>::value, typename TypeConvertionFunction<String>::Signature>::value detectToString( void )
        {
            return NULL;
        }

        //! Wraps the primitive type to a string conversion function.
        struct PrimitiveToString {
            template<typename U>
            static TypeConvertionFunction<String>::Signature detectToString( void )
            {
                struct ToString {
                    //! Just calls a toString function from a global scope.
                    static String thunk( const void* instance ) { return toString( *reinterpret_cast<const U*>( instance ) ); }
                };

                return &ToString::thunk;
            }   
        };

        //! Returns a type to string converter.
        static TypeConvertionFunction<String>::Signature toStringConverter( void )
        {
            return TypeSelector<IsClassOrUnion<T>::value, TypeTraits<T>, PrimitiveToString>::type::detectToString<T>();
        }

        //! Returns a type constructor.
        static TypeConstructorSignature constructor( void )
        {
            //! A helper struct to declare the type construction function.
            struct Constructor {
                //! Constructs an instance of type T.
                static void thunk( void* instance, const void* copy )
                {
                    if( copy ) {
                        new( instance ) T( *reinterpret_cast<const T*>( copy ) );
                    } else {
                        new( instance ) T;
                    }
                }
            };

            return &Constructor::thunk;
        }

        //! Returns a type destructor.
        static TypeDestructorSignature destructor( void )
        {
            //! A helper struct to declare the type destruction function.
            struct Destructor {
                //! Constructs an instance of type T.
                static void thunk( void* instance )
                {
                    reinterpret_cast<T*>( instance )->~T();
                }
            };

            return &Destructor::thunk;
        }
    };

NIMBLE_END

#endif  /*  !__Nimble_TypeTraits_H__ */