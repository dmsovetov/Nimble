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

#ifndef __Nimble_Rgba_H__
#define __Nimble_Rgba_H__

#include "../Globals.h"

NIMBLE_BEGIN

    /*!
     Rgba color value.
     */
    class Rgba {
    public:

                        Rgba( void );
                        Rgba( f32 r, f32 g, f32 b, f32 a = 1.0f );
						Rgba( const f32 *v );
                        Rgba( const Rgb& other );

        bool            operator == ( const Rgba& other ) const;
        const Rgba&     operator += ( const Rgba& other );
        const Rgba&     operator *= ( const Rgba& other );
        const Rgba&     operator *= ( f32 scalar );
        Rgba            operator * ( f32 scalar ) const;
        Rgba            operator + ( const Rgba& other ) const;
        Rgba            operator * ( const Rgba& other ) const;
        Rgba            operator / ( f32 scalar ) const;

		//! Returns the color with modulated alpha by a given factor.
		Rgba			transparent( f32 factor ) const;

		//! Constructs Rgba color instance from bytes.
		static Rgba		fromBytes( u8 r, u8 g, u8 b, u8 a = 255 );

    public:

        f32           r, g, b, a;
    };

    // ** Rgba::Rgba
    inline Rgba::Rgba( void ) : r( 0.0f ), g( 0.0f ), b( 0.0f ), a( 1.0f )
    {

    }

    // ** Rgba::Rgba
    inline Rgba::Rgba( f32 r, f32 g, f32 b, f32 a ) : r( r ), g( g ), b( b ), a( a )
    {

    }

    // ** Rgba::Rgba
    inline Rgba::Rgba( const f32* v ) : r( v[0] ), g( v[1] ), b( v[2] ), a( v[3] )
    {

    }

    // ** Rgba::Rgba
    inline Rgba::Rgba( const Rgb& other ) : r( other.r ), g( other.g ), b( other.b ), a( 1.0f )
    {

    }

    // ** Rgba::operator ==
    inline bool Rgba::operator == ( const Rgba& other ) const {
        return r == other.r && g == other.g && b == other.b && a == other.a;
    }

    // ** Rgba::operator +=
    inline const Rgba& Rgba::operator += ( const Rgba& other ) {
        r += other.r; g += other.g; b += other.b; a += other.a;
        return *this;
    }

    // ** Rgba::operator *=
    inline const Rgba& Rgba::operator *= ( const Rgba& other ) {
        r *= other.r; g *= other.g; b *= other.b; a *= other.a;
        return *this;
    }

    // ** Rgba::operator *=
    inline const Rgba& Rgba::operator *= ( f32 scalar ) {
        r *= scalar; g *= scalar; b *= scalar; a *= scalar;
        return *this;
    }

    // ** Rgba::operator *
    inline Rgba Rgba::operator * ( f32 scalar ) const {
        return Rgba( r * scalar, g * scalar, b * scalar, a * scalar );
    }

    // ** Rgba::operator +
    inline Rgba Rgba::operator + ( const Rgba& other ) const {
        return Rgba( r + other.r, g + other.g, b + other.b, a + other.a );
    }

    // ** Rgba::operator *
    inline Rgba Rgba::operator * ( const Rgba& other ) const {
        return Rgba( r * other.r, g * other.g, b * other.b, a * other.a );
    }

    // ** Rgba::operator /
    inline Rgba Rgba::operator / ( f32 scalar ) const {
        return Rgba( r / scalar, g / scalar, b / scalar, a / scalar );
    }

	// ** Rgba::transparent
	inline Rgba Rgba::transparent( f32 factor ) const {
		return Rgba( r, g, b, a * factor );
	}

	// ** Rgba::fromBytes
	inline Rgba Rgba::fromBytes( u8 r, u8 g, u8 b, u8 a ) {
		return Rgba( r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f );
	}


    // ** Rgb::Rgb
    inline Rgb::Rgb( const Rgba& other ) : r( other.r ), g( other.g ), b( other.b )
    {

    }

NIMBLE_END

#endif  /*  !__Nimble_Rgba_H__  */
