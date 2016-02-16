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

#ifndef __Nimble_Vec2_H__
#define __Nimble_Vec2_H__

#include "../Globals.h"

NIMBLE_BEGIN

    /*!
     2-component euclidean space vector.
     */
    class Vec2 {
    public:

                Vec2( void );
				Vec2( f32 value );
                Vec2( f32 x, f32 y );
				Vec2( int x, int y );
                Vec2( const f32* v );
				Vec2( const double* v );

        f32&  operator[]( int index );
        f32   operator[]( int index ) const;
        bool    operator == ( const Vec2& other ) const;

		const Vec2& operator += ( const Vec2& other );
		const Vec2&	operator *= ( f32 scalar );

        f32		operator * ( const Vec2& other ) const;
        Vec2		operator - ( const Vec2& other ) const;
        Vec2		operator + ( const Vec2& other ) const;
		Vec2		operator / ( const Vec2& other ) const;
        Vec2		operator * ( f32 scalar ) const;
		Vec2		operator - ( void ) const;

		//! Normalizes the vector.
		f32	normalize( void );

		//! Returns the vector length.
		f32	length( void ) const;

		//! Returns the squared vector length.
		f32	lengthSqr( void ) const;

        //! Returns a perpendicular vector to a this one.
        Vec2    perp( void ) const;

		//! Returns the normalized vector.
		static Vec2	normalized( const Vec2& v );

		//! Returns the random direction vector.
		static Vec2 randDirection( void );

		//! Returns the vector by a clock-wise angle
		static Vec2 fromAngle( f32 angle );

    public:

        f32   x, y;
    };

    // ** Vec2::Vec2
    inline Vec2::Vec2( void ) : x( 0.0f ), y( 0.0f )
    {

    }

    // ** Vec2::Vec2
    inline Vec2::Vec2( f32 value ) : x( value ), y( value )
    {

    }

    // ** Vec2::Vec2
    inline Vec2::Vec2( f32 x, f32 y ) : x( x ), y( y )
    {

    }

    // ** Vec2::Vec2
    inline Vec2::Vec2( int x, int y ) : x( ( f32 )x ), y( ( f32 )y )
    {

    }

    // ** Vec2::Vec2
    inline Vec2::Vec2( const f32* v ) : x( v[0] ), y( v[1] )
    {

    }

    // ** Vec2::Vec2
    inline Vec2::Vec2( const double* v ) : x( ( f32 )v[0] ), y( ( f32 )v[1] )
    {

    }

    // ** Vec2::operator[]
    inline f32 Vec2::operator[]( int index ) const {
        switch( index ) {
        case 0: return x;
        case 1: return y;
        }

        NIMBLE_BREAK
        return 0;
    }

    // ** Vec2::operator[]
    inline f32& Vec2::operator[]( int index ) {
        switch( index ) {
        case 0: return x;
        case 1: return y;
        }

        NIMBLE_BREAK
        return x;
    }

    // ** Vec2::operator ==
    inline bool Vec2::operator == ( const Vec2& other ) const {
        return x == other.x && y == other.y;
    }

    // ** Vec2::operator +=
    inline const Vec2& Vec2::operator += ( const Vec2& other )
	{
        x += other.x;
		y += other.y;
		return *this;
    }

    // ** Vec2::operator *=
    inline const Vec2& Vec2::operator *= ( f32 scalar )
	{
        x *= scalar;
		y *= scalar;
		return *this;
    }

    // ** Vec2::operator *
    inline f32 Vec2::operator * ( const Vec2& other ) const {
        return x * other.x + y * other.y;
    }

    // ** Vec2::operator *
    inline Vec2 Vec2::operator / ( const Vec2& other ) const {
        return Vec2( x / other.x, y / other.y );
    }

    // ** Vec2::operator -
    inline Vec2 Vec2::operator - ( const Vec2& other ) const {
        return Vec2( x - other.x, y - other.y );
    }

    // ** Vec2::operator +
    inline Vec2 Vec2::operator + ( const Vec2& other ) const {
        return Vec2( x + other.x, y + other.y );
    }

    // ** Vec2::operator *
    inline Vec2 Vec2::operator * ( f32 scalar ) const {
        return Vec2( x * scalar, y * scalar );
    }

    // ** Vec2::operator -
    inline Vec2 Vec2::operator - ( void ) const {
        return Vec2( -x, -y );
    }

	// ** Vec2::normalize
	inline f32 Vec2::normalize( void )
	{
		f32 len = length();

		if( len == 0.0f ) {
			return len;
		}

		x /= len;
		y /= len;

		return len;
	}

	// ** Vec2::length
	inline f32 Vec2::length( void ) const
	{
		return sqrtf( lengthSqr() );
	}

	// ** Vec2::lengthSqr
	inline f32 Vec2::lengthSqr( void ) const
	{
		return x * x + y * y;
	}

    // ** Vec2::perp
    inline Vec2 Vec2::perp( void ) const
    {
        return Vec2( y, -x );
    }

	// ** Vec2::normalized
	inline Vec2 Vec2::normalized( const Vec2& v )
	{
		Vec2 result = v;
		result.normalize();
		return result;
	}

    // ** Vec2::randDirection
    inline Vec2 Vec2::randDirection( void )
    {
        f32 angle = f32( rand() % 360 );
        return fromAngle( angle );
    }

    // ** Vec2::fromAngle
    inline Vec2 Vec2::fromAngle( f32 angle )
    {
        f32 r = radians( angle );
        return Vec2( sinf( r ), cosf( r ) );
    }

    // ** operator *
    inline Vec2 operator * ( const f32 scalar, const Vec2& v )
    {
		return Vec2( v.x * scalar, v.y * scalar );
	}

NIMBLE_END

#endif  /*  !__Nimble_Vec2_H__  */
