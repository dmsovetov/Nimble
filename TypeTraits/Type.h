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

#ifndef __Nimble_Type_H__
#define __Nimble_Type_H__

#include "../Globals.h"

NIMBLE_BEGIN

    //! Type instance stores info about a C++ type.
    class Type {
    public:

        //! Returns the type for a specified C++ class.
        template<typename TValue>
        static const Type*  fromClass( void );

        //! Returns true if the data type matches the specified class.
        template<typename TValue>
        bool                is( void ) const;

        //! Returns true if the type is integral.
        bool                isIntegral( void ) const;

        //! Returns true if the type is floating point.
        bool                isFloatingPoint( void ) const;

        //! Returns true if the type is arithmetic.
        bool                isArithmetic( void ) const;

        //! Returns true if the type can be converted to an integer.
        bool                hasIntegerConversion( void ) const;

        //! Returns true if the type can be converted to a floating point.
        bool                hasFloatConversion( void ) const;

        //! Returns true if the type can be converted to a string.
        bool                hasStringConversion( void ) const;

        //! Returns a type id.
        u32                 id( void ) const;

        //! Returns the type size.
        s32                 size( void ) const;

        //! Returns the type name.
        CString             name( void ) const;

        //! Constructs the type instance.
        void                construct( void* instance, const void* copy = NULL ) const;

        //! Destroys the type instance.
        void                destroy( void* instance ) const;

        //! Converts the value to an integer.
        s64                 convertToInteger( const void* instance ) const;

        //! Converts the value to an float.
        f64                 convertToFloat( const void* instance ) const;

        //! Converts the value to a string.
        String              convertToString( const void* instance ) const; 

    private:

        //! Alias for an integral type cast function.
        typedef TypeConvertionFunction<s64>::Signature IntegerCast;

        //! Alias for a floating point type cast function.
        typedef TypeConvertionFunction<f64>::Signature FloatCast;

        //! Alias for a string type cast function.
        typedef TypeConvertionFunction<String>::Signature StringCast;

        //! Type traits structure.
        struct Traits {
                                        //! Constructs Traits instance.
                                        Traits( void )
                                            : size( 0 ), features( 0 ), constructor( NULL ), destructor( NULL ), integerCast( NULL ), floatCast( NULL ), toString( NULL ) {}

            s32                         size;           //!< The type size.
            FlagSet8                    features;       //!< Type features bitset.
            TypeConstructorSignature    constructor;    //!< Construction function.
            TypeDestructorSignature     destructor;     //!< Destruction function.       
            IntegerCast                 integerCast;    //!< Function to cast value to an integer.
            FloatCast                   floatCast;      //!< Function to cast value to an floating point value.
            StringCast                  toString;       //!< Function to convert value to a string using a toString function.
        };

                            //! Constructs the Type instance.
                            Type( u32 id = 0, const String& name = "", const Traits& traits = Traits() );

    private:

        //! Type info bit masks.
        enum {
              Integral    = BIT( 0 ) //!< The type is integral.
            , Floating    = BIT( 1 ) //!< The type is floating point.
            , Arithmetic  = BIT( 2 ) //!< The type is arithmetic.
        };

        u32                 m_id;       //!< Unique type id.
        String              m_name;     //!< The type name.
        Traits              m_traits;   //!< The type traits.
    };

    // ** Type::Type
    inline Type::Type( u32 id, const String& name, const Traits& traits )
        : m_id( id )
        , m_name( name )
        , m_traits( traits )
    {
    }

    // ** Type::isIntegral
    inline bool Type::isIntegral( void ) const
    {
        return m_traits.features.is( Integral );
    }

    // ** Type::isFloatingPoint
    inline bool Type::isFloatingPoint( void ) const
    {
        return m_traits.features.is( Floating );
    }

    // ** Type::isArithmetic
    inline bool Type::isArithmetic( void ) const
    {
        return m_traits.features.is( Arithmetic );
    }

    // ** Type::hasIntegerConversion
    inline bool Type::hasIntegerConversion( void ) const
    {
        return m_traits.integerCast != NULL;
    }

    // ** Type::hasFloatConversion
    inline bool Type::hasFloatConversion( void ) const
    {
        return m_traits.floatCast != NULL;
    }

    // ** Type::hasStringConversion
    inline bool Type::hasStringConversion( void ) const
    {
        return m_traits.toString != NULL;
    }

    // ** Type::id
    inline u32 Type::id( void ) const
    {
        return m_id;
    }

    // ** Type::size
    inline s32 Type::size( void ) const
    {
        return m_traits.size;
    }

    // ** Type::name
    inline CString Type::name( void ) const
    {
        return m_name.c_str();
    }

    // ** Type::construct
    inline void Type::construct( void* instance, const void* copy ) const
    {
        NIMBLE_ABORT_IF( m_traits.constructor == NULL, "type should has a constructor" );
        m_traits.constructor( instance, copy );
    }

    // ** Type::destroy
    inline void Type::destroy( void* instance ) const
    {
        NIMBLE_ABORT_IF( m_traits.destructor == NULL, "type should has a destructor" );
        m_traits.destructor( instance );
    }

    // ** Type::convertToInteger
    inline s64 Type::convertToInteger( const void* instance ) const
    {
        NIMBLE_ABORT_IF( m_traits.integerCast == NULL, "casting function should be valid" );
        return m_traits.integerCast( instance );
    }

    // ** Type::convertToFloat
    inline f64 Type::convertToFloat( const void* instance ) const
    {
        NIMBLE_ABORT_IF( m_traits.floatCast == NULL, "casting function should be valid" );
        return m_traits.floatCast( instance );
    }

    // ** Type::convertToFloat
    inline String Type::convertToString( const void* instance ) const
    {
        NIMBLE_ABORT_IF( m_traits.toString == NULL, "casting function should be valid" );
        return m_traits.toString( instance );
    }

    // ** Type::fromClass
    template<typename TValue>
    const Type* Type::fromClass( void )
    {
        static Type  instance;
        static Type* type = NULL;

        if( type ) {
            return type;
        }

        // Setup type traits
        Traits traits;

        // Initialize type features
        traits.features.set( Floating,   TypeTraits<TValue>::Floating );
        traits.features.set( Integral,   TypeTraits<TValue>::Integral );
        traits.features.set( Arithmetic, TypeTraits<TValue>::Arithmetic );

        // The type size
        traits.size = TypeTraits<TValue>::Size;

        // Type constructor & destructor
        traits.constructor = TypeTraits<TValue>::constructor();
        traits.destructor  = TypeTraits<TValue>::destructor();

        // Type conversion functions
        traits.integerCast = TypeTraits<TValue>::template staticCast<s64>();
        traits.floatCast   = TypeTraits<TValue>::template staticCast<f64>();
        traits.toString    = TypeTraits<TValue>::toStringConverter();

        // Initialize the data type
        instance = Type( GroupedTypeIndex<TValue, Type>::idx(), TypeInfo<TValue>::qualifiedName(), traits );

        // Save the shared pointer
        type = &instance;

        return type;
    }

    // ** Type::is
    template<typename TValue>
    bool Type::is( void ) const
    {
        return this == fromClass<TValue>();
    }

NIMBLE_END

#endif  /*  !__Nimble_Type_H__  */
