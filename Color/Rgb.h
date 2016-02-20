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

#ifndef __Nimble_Rgb_H__
#define __Nimble_Rgb_H__

#include "../Globals.h"

NIMBLE_BEGIN

    class Rgba;

    /*!
     Double LDR pixel data.
    */
    struct DoubleLdr {
                        //! Constructs the DoubleLdr instance.
                        DoubleLdr( unsigned char r, unsigned char g, unsigned char b )
                            : r( r ), g( g ), b( b ) {}

        unsigned char   r;
        unsigned char   g;
        unsigned char   b;
    };

    /*!
     RGBm LDR pixel data.
    */
    struct RgbmLdr {
                        //! Constructs the RgbmLdr instance.
                        RgbmLdr( unsigned char r, unsigned char g, unsigned char b, unsigned char m )
                            : r( r ), g( g ), b( b ), m( m ) {}

        unsigned char   r;
        unsigned char   g;
        unsigned char   b;
        unsigned char   m;
    };

    /*!
     Rgb color value
     */
    class Rgb {
    public:

                        Rgb( void );
                        Rgb( f32 r, f32 g, f32 b );
                        Rgb( const f32* v );
        explicit        Rgb( const Rgba& other );

        bool            operator == ( const Rgb& other ) const;
        const Rgb&      operator += ( const Rgb& other );
        const Rgb&      operator *= ( const Rgb& other );
        const Rgb&      operator *= ( f32 scalar );
        Rgb             operator * ( f32 scalar ) const;
        Rgb             operator + ( const Rgb& other ) const;
        Rgb             operator * ( const Rgb& other ) const;
        Rgb             operator / ( const Rgb& other ) const;
        Rgb             operator / ( f32 scalar ) const;
        Rgb             operator + ( f32 scalar ) const;
        Rgb             operator - ( f32 scalar ) const;

		//! Returns the color hash string.
		String			hashString( void ) const;

        //! Calculates color luminance,
        f32           luminance( void ) const;

        //! Converts color to double LDR encoded value.
        DoubleLdr       doubleLdr( void ) const;

        //! Converts color to RGBM LDR encoded value.
        RgbmLdr         rgbm( void ) const;

		//! Constructs Rgb color instance from bytes.
		static Rgb		fromBytes( u8 r, u8 g, u8 b );

		//! Constructs Rgb color instance from hash string.
		static Rgb		fromHashString( const String& value );

    public:

        f32           r, g, b;
    };

    // ** Rgb::Rgb
    inline Rgb::Rgb( void ) : r( 0.0f ), g( 0.0f ), b( 0.0f )
    {

    }

    // ** Rgb::Rgb
    inline Rgb::Rgb( f32 r, f32 g, f32 b ) : r( r ), g( g ), b( b )
    {

    }

    // ** Rgb::Rgb
    inline Rgb::Rgb( const f32* v ) : r( v[0] ), g( v[1] ), b( v[2] )
    {

    }

	// ** Rgb::hashString
	inline String Rgb::hashString( void ) const
	{
		return "#" + toByteString( static_cast<u8>( r * 255 ) ) + toByteString( static_cast<u8>( g * 255 ) ) + toByteString( static_cast<u8>( b * 255 ) );
	}

    // ** Rgb::luminance
    inline f32 Rgb::luminance( void ) const {
        return 0.2126f * r + 0.7152f * g + 0.0722f * b;
    }

    // ** Rgb::doubleLdr
    inline DoubleLdr Rgb::doubleLdr( void ) const
    {
        return DoubleLdr( static_cast<unsigned char>( min2( 255.0f, r * 0.5f * 255.0f ) )
                        , static_cast<unsigned char>( min2( 255.0f, g * 0.5f * 255.0f ) )
                        , static_cast<unsigned char>( min2( 255.0f, b * 0.5f * 255.0f ) )
                        );
    }

    // ** Rgb::rgbm
    inline RgbmLdr Rgb::rgbm( void ) const
    {
        f32 m = 255.0f / max3( r, g, b );

        return RgbmLdr( static_cast<unsigned char>( min2( 255.0f, r / m * 255.0f ) )
                      , static_cast<unsigned char>( min2( 255.0f, g / m * 255.0f ) )
                      , static_cast<unsigned char>( min2( 255.0f, b / m * 255.0f ) )
                      , static_cast<unsigned char>( min2( 255.0f, m * 255.0f ) )
                      );
    }

    // ** Rgb::operator ==
    inline bool Rgb::operator == ( const Rgb& other ) const {
        return r == other.r && g == other.g && b == other.b;
    }

    // ** Rgb::operator +=
    inline const Rgb& Rgb::operator += ( const Rgb& other ) {
        r += other.r; g += other.g; b += other.b;
        return *this;
    }

    // ** Rgb::operator *=
    inline const Rgb& Rgb::operator *= ( const Rgb& other ) {
        r *= other.r; g *= other.g; b *= other.b;
        return *this;
    }

    // ** Rgb::operator *=
    inline const Rgb& Rgb::operator *= ( f32 scalar ) {
        r *= scalar; g *= scalar; b *= scalar;
        return *this;
    }

    // ** Rgb::operator *
    inline Rgb Rgb::operator * ( f32 scalar ) const {
        return Rgb( r * scalar, g * scalar, b * scalar );
    }

    // ** Rgb::operator +
    inline Rgb Rgb::operator + ( const Rgb& other ) const {
        return Rgb( r + other.r, g + other.g, b + other.b );
    }

    // ** Rgb::operator *
    inline Rgb Rgb::operator * ( const Rgb& other ) const {
        return Rgb( r * other.r, g * other.g, b * other.b );
    }

    // ** Rgb::operator /
    inline Rgb Rgb::operator / ( const Rgb& other ) const {
        return Rgb( r / other.r, g / other.g, b / other.b );
    }

    // ** Rgb::operator /
    inline Rgb Rgb::operator / ( f32 scalar ) const {
        return Rgb( r / scalar, g / scalar, b / scalar );
    }

    // ** Rgb::operator +
    inline Rgb Rgb::operator + ( f32 scalar ) const {
        return Rgb( r + scalar, g + scalar, b + scalar );
    }

    // ** Rgb::operator -
    inline Rgb Rgb::operator - ( f32 scalar ) const {
        return Rgb( r - scalar, g - scalar, b - scalar );
    }

	// ** Rgb::fromBytes
	inline Rgb Rgb::fromBytes( u8 r, u8 g, u8 b ) {
		return Rgb( r / 255.0f, g / 255.0f, b / 255.0f );
	}

	// ** Rgb::fromHashString
	inline Rgb Rgb::fromHashString( const String& value )
	{
		NIMBLE_ASSERT( value[0] == '#', "Should start with #" );

		s8* error;
		u64 hex = strtoul( value.c_str() + 1, &error, 16 );

		if( *error ) {
			NIMBLE_BREAK;
			return Rgb();
		}

		u8 r = (hex >> 16) & 0xFF;
		u8 g = (hex >>  8) & 0xFF;
		u8 b = (hex >>  0) & 0xFF;

		return Rgb::fromBytes( r, g, b );
	}

NIMBLE_END

#endif  /*  !__Nimble_Rgb_H__  */
