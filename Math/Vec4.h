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

#ifndef __Nimble_Vec4_H__
#define __Nimble_Vec4_H__

#include "../Globals.h"

NIMBLE_BEGIN

    /*!
     Euclidean homogeneous coordinate vector.
     */
    class Vec4 {
    public:

                    Vec4( void );
                    Vec4( f32 x, f32 y, f32 z, f32 w = 1.0f );
                    Vec4( const f32* v );

					//! Converts homogeneous coordinate to a 3-component vector.
					operator Vec3( void ) const;

		//! Compares two Vec4 instances.
        bool        operator == ( const Vec4& other ) const;

		//! Returns a vector component by index.
        f32&      operator[]( int index );

		//! Returns a vector component by index.
        f32       operator[]( int index ) const;

		//! Multiplies a vector by scalar value.
		Vec4		operator * ( f32 scalar ) const;

		//! Adds two vectors.
		Vec4		operator + ( const Vec4& other ) const;

    public:

        f32		x, y, z, w;
    };

    // ** Vec4::Vec4
    inline Vec4::Vec4( void ) : x( 0.0f ), y( 0.0f ), z( 0.0f ), w( 1.0f )
    {

    }

    // ** Vec4::Vec4
    inline Vec4::Vec4( f32 x, f32 y, f32 z, f32 w ) : x( x ), y( y ), z( z ), w( w )
    {

    }

    // ** Vec4::Vec4
    inline Vec4::Vec4( const f32* v ) : x( v[0] ), y( v[1] ), z( v[2] ), w( v[3] )
    {

    }

	// ** Vec4::operator Vec3
	inline Vec4::operator Vec3( void ) const
	{
		return Vec3( x, y, z );
	}

    // ** Vec4::operator *
    inline Vec4 Vec4::operator * ( f32 scalar ) const {
        return Vec4( x * scalar, y * scalar, z * scalar, w );
    }

    // ** Vec4::operator +
    inline Vec4 Vec4::operator + ( const Vec4& other ) const {
        return Vec4( x * other.x, y * other.y, z * other.z, w );
    }

    // ** Vec4::operator[]
    inline f32 Vec4::operator[]( int index ) const
    {
        NIMBLE_ABORT_IF( index < 0 && index >= 4, "index is out of range" );

        switch( index ) {
        case 0: return x;
        case 1: return y;
        case 2: return z;
		case 3: return w;
        }

        return 0;
    }

    // ** Vec4::operator[]
    inline f32& Vec4::operator[]( int index )
    {
        NIMBLE_ABORT_IF( index < 0 && index >= 4, "index is out of range" );

        switch( index ) {
        case 0: return x;
        case 1: return y;
        case 2: return z;
		case 3: return w;
        }

        return x;
    }

    // ** Vec4::operator ==
    inline bool Vec4::operator == ( const Vec4& other ) const {
        return x == other.x && y == other.y && z == other.z && w == other.w;
    }

NIMBLE_END

#endif  /*  !__Nimble_Vec4_H__  */
