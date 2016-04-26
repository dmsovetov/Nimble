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

#ifndef __Nimble_FixedString_H__
#define __Nimble_FixedString_H__

#include "../Globals.h"

NIMBLE_BEGIN

    //! Fixed string wraps a C string that should not be deleted in a runtime.
    class FixedString {
    public:

                            //! Constructs an empty FixedString instance.
                            FixedString( void );

                            //! Constructs a FixedString from a pointer and length.
                            FixedString( CString value, s32 length = 0 );

                            //! Constructs a FixedString instance from another one.
                            FixedString( const FixedString& other );

                            //! Returns a stored C string.
                            operator CString ( void ) const;

        //! Copies a fixed string from another one.
        const FixedString&  operator = ( const FixedString& other );

        //! Tests two fixed strings for an equality.
        bool                operator == ( const FixedString& other ) const;

        //! Compares two fixed strings.
        bool                operator < ( const FixedString& other ) const;

        //! Returns a string length.
        s32                 length( void ) const;

        //! Returns a string value.
        CString             value( void ) const;

        //! Returns a hashed string value.
        const String32&     hash( void ) const;

    private:

        CString             m_value;    //!< Actual string value.
        s32                 m_length;   //!< A string length.
        String32            m_hash;     //!< A string hash value.
    };

    // ** FixedString::FixedString
    NIMBLE_INLINE FixedString::FixedString( void )
        : m_value( NULL )
        , m_length( 0 )
    {
    }

    // ** FixedString::FixedString
    NIMBLE_INLINE FixedString::FixedString( CString value, s32 length )
        : m_value( value )
        , m_length( length ? length : strlen( value ) )
    {
        m_hash = String32( m_value, m_length );
    }

    // ** FixedString::FixedString
    NIMBLE_INLINE FixedString::FixedString( const FixedString& other )
        : m_value( other.m_value )
        , m_length( other.m_length )
        , m_hash( other.m_hash )
    {
    }

    // ** FixedString::operator CString
    NIMBLE_INLINE FixedString::operator CString ( void ) const
    {
        return m_value;
    }

    // ** FixedString::operator =
    NIMBLE_INLINE const FixedString& FixedString::operator = ( const FixedString& other )
    {
        m_value  = other.m_value;
        m_length = other.m_length;
        m_hash   = other.m_hash;
        return *this;
    }

    // ** FixedString::operator ==
    NIMBLE_INLINE bool FixedString::operator == ( const FixedString& other ) const
    {
        if( m_length != other.m_length ) {
            return false;
        }

        if( m_hash != other.m_hash ) {
            return false;
        }

        return strncmp( m_value, other.m_value, m_length ) == 0;
    }

    // ** FixedString::operator <
    NIMBLE_INLINE bool FixedString::operator < ( const FixedString& other ) const
    {
        if( m_length != other.m_length ) {
            return m_length < other.m_length;
        }

        return strncmp( m_value, other.m_value, m_length ) < 0;
    }

    // ** FixedString::length
    NIMBLE_INLINE s32 FixedString::length( void ) const
    {
        return m_length;
    }

    // ** FixedString::value
    NIMBLE_INLINE CString FixedString::value( void ) const
    {
        return m_value;
    }

    // ** FixedString::hash
    NIMBLE_INLINE const String32& FixedString::hash( void ) const
    {
        return m_hash;
    }

NIMBLE_END

#endif  /*    !__Nimble_FixedString_H__    */
