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

        //! Returns the color hash string.
        String          hashString( void ) const;

        //! Returns the color with modulated alpha by a given factor.
        Rgba            transparent( f32 factor ) const;

        //! Returns the color with RGB values multiplied by a given factor.
        Rgba            lighter( f32 factor ) const;

        //! Returns the color with RGB values divided by a given factor.
        Rgba            darker( f32 factor ) const;

        //! Encodes a color to an integer.
        u32             toInteger( void ) const;

        //! Constructs Rgba color instance from bytes.
        static Rgba     fromBytes( u8 r, u8 g, u8 b, u8 a = 255 );

        //! Constructs Rgba color instance from hash string.
        static Rgba     fromHashString( const String& value );

        //! Decodes a color from an integer.
        static Rgba     fromInteger( u32 value );

    public:

        f32             r, g, b, a;
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

    // ** Rgba::toInteger
    inline u32 Rgba::toInteger( void ) const
    {
        return   (static_cast<u32>( a * 255 ) << 24)
               | (static_cast<u32>( b * 255 ) << 16)
               | (static_cast<u32>( g * 255 ) <<  8)
               | (static_cast<u32>( r * 255 ) <<  0) 
               ;
    }

    // ** Rgba::fromInteger
    inline Rgba Rgba::fromInteger( u32 value )
    {
        u8 a = (value >> 24) & 0xFF;
        u8 b = (value >> 16) & 0xFF;
        u8 g = (value >>  8) & 0xFF;
        u8 r = (value >>  0) & 0xFF;

        return fromBytes( r, g, b, a );
    }

    // ** Rgba::hashString
    inline String Rgba::hashString( void ) const
    {
        return "#" + toByteString( static_cast<u8>( a * 255 ) ) + toByteString( static_cast<u8>( r * 255 ) ) + toByteString( static_cast<u8>( g * 255 ) ) + toByteString( static_cast<u8>( b * 255 ) );
    }

    // ** Rgba::transparent
    inline Rgba Rgba::transparent( f32 factor ) const {
        return Rgba( r, g, b, a * factor );
    }

    // ** Rgba::lighter
    inline Rgba Rgba::lighter( f32 factor ) const
    {
        return Rgba( r * factor, g * factor, b * factor, a );
    }

    // ** Rgba::darker
    inline Rgba Rgba::darker( f32 factor ) const
    {
        return Rgba( r / factor, g / factor, b / factor, a );
    }

    // ** Rgba::fromBytes
    inline Rgba Rgba::fromBytes( u8 r, u8 g, u8 b, u8 a ) {
        return Rgba( r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f );
    }

    // ** Rgba::fromHashString
    inline Rgba Rgba::fromHashString( const String& value )
    {
        NIMBLE_BREAK_IF( value[0] != '#', "should start with #" );

        u64 hex = strtoul( value.c_str() + 1, NULL, 16 );

        u8 a = (hex >> 24) & 0xFF;
        u8 r = (hex >> 16) & 0xFF;
        u8 g = (hex >>  8) & 0xFF;
        u8 b = (hex >>  0) & 0xFF;

        return Rgba::fromBytes( r, g, b, a );
    }

    // ** Rgb::Rgb
    inline Rgb::Rgb( const Rgba& other ) : r( other.r ), g( other.g ), b( other.b )
    {

    }

NIMBLE_END

#endif  /*  !__Nimble_Rgba_H__  */
