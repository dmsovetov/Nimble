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

        //! Returns true if the type is integral.
	    bool                isIntegral( void ) const;

        //! Returns true if the type is floating point.
	    bool                isFloatingPoint( void ) const;

        //! Returns true if the type is arithmetic.
	    bool                isArithmetic( void ) const;

        //! Returns the type size.
	    s32                 size( void ) const;

        //! Returns the type name.
        CString             name( void ) const;

    private:

                            //! Constructs the Type instance.
                            Type( const String& name = "", u8 features = 0, s32 size = 0 );

    private:

        //! Type info bit masks.
        enum {
              Integral    = BIT( 0 ) //!< The type is integral.
            , Floating    = BIT( 1 ) //!< The type is floating point.
            , Arithmetic  = BIT( 2 ) //!< The type is arithmetic.
        };

        String              m_name;     //!< The type name.
        s32                 m_size;     //!< The type size.
        u8                  m_features; //!< Type features bitset.
    };

    // ** Type::Type
    inline Type::Type( const String& name, u8 features, s32 size ) : m_name( name ), m_features( features ), m_size( size )
    {
    }

    // ** Type::isIntegral
	inline bool Type::isIntegral( void ) const
    {
        return m_features & Integral ? true : false;
    }

    // ** Type::isFloatingPoint
	inline bool Type::isFloatingPoint( void ) const
    {
        return m_features & Floating ? true : false;
    }

    // ** Type::isArithmetic
	inline bool Type::isArithmetic( void ) const
    {
        return m_features & Arithmetic ? true : false;
    }

    // ** Type::size
	inline s32 Type::size( void ) const
    {
        return m_size;
    }

    // ** Type::name
	inline CString Type::name( void ) const
    {
        return m_name.c_str();
    }

    // ** Type::fromClass
    template<typename TValue>
    static const Type* Type::fromClass( void )
    {
        static Type  instance;
        static Type* type = NULL;

        if( type ) {
            return type;
        }

        // Initialize type features
        u8 features = 0;
        features |= IsFloatingPoint<TValue>::value  ? Floating   : 0;
        features |= IsIntegral<TValue>::value       ? Integral   : 0;
        features |= IsArithmetic<TValue>::value     ? Arithmetic : 0;

        // Initialize the data type
        instance = Type( TypeInfo<TValue>::name(), features, sizeof( TValue ) );

        // Save the shared pointer
        type = &instance;

		return type;
    }

NIMBLE_END

#endif  /*  !__Nimble_Type_H__  */