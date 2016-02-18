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

#ifndef __Nimble_Variant_H__
#define __Nimble_Variant_H__

#include "Globals.h"

NIMBLE_BEGIN

    //! The Variant class acts like a union for the most common data types.
    class Variant {
    public:

                        ~Variant( void );

                        //! Constructs Variant value as a copy of another one.
                        Variant( const Variant& other );

        //! Copies this Variant value from another one.
        const Variant&  operator = ( const Variant& other );

        //! Returns the value type.
        const Type*     type( void ) const;

        //! Casts the Value to a specified data type.
        template<typename TValue>
        TValue          as( void ) const;

        //! Constructs Value instance from data.
        template<typename TValue>
        static Variant  fromValue( const TValue& value );

    private:

                        //! Constructs Variant instance.
                        Variant( const Type* type, const void* copy );

        //! Returns the allocated data pointer.
        const void*     pointer( void ) const;
        void*           pointer( void );

        //! Allocates the data if the value size exceeds the maxium value.
        void*           allocate( void );

    private:

        //! The constant size for a static value buffer.
        enum { MaxValueSize = 16 };

        const Type*     m_type;                 //!< The stored value type.
        union {
            void*       m_pointer;              //!< Allocated data pointer.
            u8          m_value[MaxValueSize];  //!< Values whom size is less than the maximum one are stored here.
        };
    };

    //! Converts variant value to an integer if possible.
    template<typename TValue>
    struct VariantToIntegerConverter {
        static bool convert( TValue& value, const Type* type, const void* instance ) {
            if( !type->hasIntegerConversion() ) {
                return false;
            }

            value = static_cast<TValue>( type->convertToInteger( instance ) );
            return true;
        }
    };

    //! Converts variant value to an float if possible.
    template<typename TValue>
    struct VariantToFloatConverter {
        static bool convert( TValue& value, const Type* type, const void* instance ) {
            if( !type->hasFloatConversion() ) {
                return false;
            }

            value = static_cast<TValue>( type->convertToFloat( instance ) );
            return true;
        }
    };

    //! Converts variant value to an string if possible.
    template<typename TValue>
    struct VariantToStringConverter {
        static bool convert( TValue& value, const Type* type, const void* instance ) {
            if( !type->hasStringConversion() ) {
                return false;
            }

            value = static_cast<TValue>( type->convertToString( instance ) );
            return true;
        }
    };

    //! Variant conversion is no possible.
    template<typename TValue>
    struct NoConversion { static bool convert( TValue& value, const Type* type, const void* instance ) { return false; } };

    // ** Variant::fromValue
    template<typename TValue>
    Variant Variant::fromValue( const TValue& value )
    {
        return Variant( Type::fromClass<TValue>(), &value );
    }

    // ** Variant::as
    template<typename TValue>
    TValue Variant::as( void ) const
    {
        // Get the value type
        const Type* type = this->type();

        // Get the instance pointer
        const void* instance = pointer();

        // Just return a value if types match
        if( type->is<TValue>() ) {
            return *reinterpret_cast<const TValue*>( instance );
        }

        // Otherwise try to perform a type cast
        TValue result = TValue();

        if( TypeSelector< IsIntegral<TValue>::value, VariantToIntegerConverter<TValue>, NoConversion<TValue> >::type::convert( result, type, instance ) ) {
            return result;
        }
        if( TypeSelector< IsFloatingPoint<TValue>::value, VariantToFloatConverter<TValue>, NoConversion<TValue> >::type::convert( result, type, instance ) ) {
            return result;
        }
        if( TypeSelector< TypeEquals<TValue, String>::value, VariantToStringConverter<TValue>, NoConversion<TValue> >::type::convert( result, type, instance ) ) {
            return result;
        }

        return result;

        //// Try the integer conversion
        //if( TypeTraits<TValue>::Integral && type->hasIntegerConversion() ) {
        //    return static_cast<TValue>( type->convertToInteger( pointer() ) );
        //}

        //// Try the float convertion
        //if( TypeTraits<TValue>::Floating && type->hasFloatConversion() ) {
        //    return static_cast<TValue>( type->convertToFloat( pointer() ) );
        //}

        //return TValue();
    }

    // ** Variant::Variant
    inline Variant::Variant( const Type* type, const void* copy ) : m_type( type )
    {
        m_type->construct( allocate(), copy );
    }

    // ** Variant::Variant
    inline Variant::Variant( const Variant& other ) : m_type( other.type() )
    {
        m_type->construct( allocate(), other.pointer() );
    }

    // ** Variant::Variant
    inline Variant::~Variant( void )
    {
        if( type()->size() > MaxValueSize ) {
            free( m_pointer );
        }
    }

    // ** Variant::operator =
    inline const Variant& Variant::operator = ( const Variant& other )
    {
        m_type = other.type();
        m_type->construct( allocate(), other.pointer() );
        return *this;
    }

    // ** Variant::type
    inline const Type* Variant::type( void ) const
    {
        return m_type;
    }

    // ** Variant::allocate
    inline void* Variant::allocate( void )
    {
        NIMBLE_BREAK_IF( !type() );

        // Get the value type size
        s32 size = type()->size();

        // The value fits the static buffer.
        if( size <= MaxValueSize ) {
            return m_value;
        }

        m_pointer = malloc( size );
        return m_pointer;
    }

    // ** Variant::pointer
    inline const void* Variant::pointer( void ) const
    {
        return const_cast<Variant*>( this )->pointer();
    }

    // **  Variant::pointer
    inline void* Variant::pointer( void )
    {
        if( type()->size() > MaxValueSize ) {
            return m_pointer;
        }
    
        return m_value;
    }

NIMBLE_END

#endif  /*  !__Nimble_Variant_H__    */