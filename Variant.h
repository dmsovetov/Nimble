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

                        //! Constructs Variant value as a copy of another one.
                        Variant( const Variant& other );

                        //! Constructs an invalid Variant value.
                        Variant( void );

                        ~Variant( void );

        //! Copies this Variant value from another one.
        const Variant&  operator = ( const Variant& other );

        //! Returns the value type.
        const Type*     type( void ) const;

        //! Returns true if the Variant value is valid.
        bool            isValid( void ) const;

        //! Casts the Variant to a specified data type.
        template<typename TValue>
        TValue          as( void ) const;

		//! Converts the Variant to a specified integral type.
		template<typename TValue>
		NIMBLE_IF_INTEGRAL( TValue ) convert( void ) const;

		//! Converts the Variant to a specified floating type.
		template<typename TValue>
		NIMBLE_IF_FLOATING( TValue ) convert( void ) const;

		//! Converts the Variant to a String.
		template<typename TValue>
		NIMBLE_IF_STRING( TValue ) convert( void ) const;

		//! Converts the Variant to a specified type.
		template<typename TValue>
		NIMBLE_IF_CLASS( TValue ) convert( void ) const;

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

        //! Disposes the stored value and allocated memory.
        void            dispose( void );

    private:

        //! The constant size for a static value buffer.
        enum { MaxValueSize = 16 };

        const Type*     m_type;                 //!< The stored value type.
        union {
            void*       m_pointer;              //!< Allocated data pointer.
            u8          m_value[MaxValueSize];  //!< Values whom size is less than the maximum one are stored here.
        };
    };

    // ** Variant::fromValue
    template<typename TValue>
    Variant Variant::fromValue( const TValue& value )
    {
        return Variant( Type::fromClass<TValue>(), &value );
    }

    // ** Variant::Variant
    inline Variant::Variant( void ) : m_type( NULL )
    {
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
        return convert<TValue>();
    }

    // ** Variant::isValid
    inline bool Variant::isValid( void ) const
    {
        return m_type != NULL;
    }

	// ** Variant::convert
	template<typename TValue>
	NIMBLE_IF_INTEGRAL( TValue ) Variant::convert( void ) const
	{
		const Type* type = this->type();

		if( type->hasIntegerConversion() ) {
			return type->convertToInteger( pointer() );
		}

		return 0;
	}

	// ** Variant::convert
	template<typename TValue>
	NIMBLE_IF_FLOATING( TValue ) Variant::convert( void ) const
	{
		const Type* type = this->type();

		if( type->hasFloatConversion() ) {
			return type->convertToFloat( pointer() );
		}

		return 0.0f;
	}

	// ** Variant::convert
	template<typename TValue>
	NIMBLE_IF_STRING( TValue ) Variant::convert( void ) const
	{
		const Type* type = this->type();

		if( type->hasStringConversion() ) {
			return type->convertToString( pointer() );
		}

		return "";
	}

	// ** Variant::convert
	template<typename TValue>
	NIMBLE_IF_CLASS( TValue ) Variant::convert( void ) const
	{
		return TValue();
	}

    // ** Variant::Variant
    inline Variant::Variant( const Type* type, const void* copy ) : m_type( type )
    {
        m_type->construct( allocate(), copy );
    }

    // ** Variant::Variant
    inline Variant::Variant( const Variant& other ) : m_type( other.type() )
    {
        if( !other.isValid() ) {
            return;
        }

        m_type->construct( allocate(), other.pointer() );
    }

    // ** Variant::Variant
    inline Variant::~Variant( void )
    {
        dispose();
    }

    // ** Variant::operator =
    inline const Variant& Variant::operator = ( const Variant& other )
    {
        // Dispose the old value
        dispose();

        // Set the new type
        m_type = other.type();

        // Construct if type is valid
        if( other.isValid() ) {
            m_type->construct( allocate(), other.pointer() );
        }

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
        NIMBLE_ABORT_IF( type() == NULL, "variant with type 'void' could not be allocated" );

        // Get the value type size
        s32 size = type()->size();

        // The value fits the static buffer.
        if( size <= MaxValueSize ) {
            return m_value;
        }

        m_pointer = malloc( size );
        return m_pointer;
    }

    // ** Variant::dispose
    inline void Variant::dispose( void )
    {
        if( !isValid() ) {
            return;
        }

        // Destroy the stored value
        type()->destroy( pointer() );

        // Free the memory
        if( type()->size() > MaxValueSize ) {
            free( m_pointer );
        }
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