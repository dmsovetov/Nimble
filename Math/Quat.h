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

#ifndef __Nimble_Quat_H__
#define __Nimble_Quat_H__

#include "../Globals.h"

NIMBLE_BEGIN

    /*!
     Quaternion
     */
    class Quat {
    public:

                    //! Constructs a Quat instance.
                    Quat( void );

                    //! Constructs a Quat instance from coordinates.
                    Quat( f32 x, f32 y, f32 z, f32 w );

                    //! Constructs a Quat instance from vector and scalar.
                    Quat( const Vec3& v, f32 w );

                    //! Construxts a Quat instance from an array of 4 f32s.
                    Quat( const f32* v );

        //! Returns conjugated quaternion.
        Quat        operator - ( void ) const;

        //! Multiplies two quaternions.
        Quat        operator * ( const Quat& q ) const;

        //! Compares two quaternions
        bool        operator == ( const Quat& other ) const;

        //! Converts quaternion to an Euler angles vector, where X is bank, Y is heading and Z is attitude.
        Vec3        euler( void ) const;

        //! Calculates the local yaw element of this quaternion.
        f32         yaw( void ) const;

        //! Calculates the local pitch element of this quaternion.
        f32         pitch( void ) const;

        //! Calculates the local roll element of this quaternion.
        f32         roll( void ) const;

        //! Rotates point by a quaternion.
        Vec3        rotate( const Vec3& point ) const;

        //! Creates a rotation around axis quaternion.
        static Quat    rotateAroundAxis( f32 angle, const Vec3& axis );

    public:

        f32       x, y, z, w;
    };

    // ** Quat::Quat
    inline Quat::Quat( void ) : x( 0 ), y( 0 ), z( 0 ), w( 1 ) {

    }

    // ** Quat::Quat
    inline Quat::Quat( f32 x, f32 y, f32 z, f32 w ) : x( x ), y( y ), z( z ), w( w ) {

    }

    // ** Quat::Quat
    inline Quat::Quat( const Vec3& v, f32 w ) : x( v.x ), y( v.y ), z( v.z ), w( w ) {

    }

    // ** Quat::Quat
    inline Quat::Quat( const f32* v ) : x( v[0] ), y( v[1] ), z( v[2] ), w( v[3] ) {

    }

    // ** Quat::operator -
    inline Quat Quat::operator - ( void ) const {
        return Quat( -x, -y, -z, w );
    }

    // ** Quat::operator *
    inline Quat Quat::operator * ( const Quat& q ) const {
        return Quat( w * q.x + x * q.w + y * q.z - z * q.y,
                     w * q.y + y * q.w + z * q.x - x * q.z,
                     w * q.z + z * q.w + x * q.y - y * q.x,
                     w * q.w - x * q.x - y * q.y - z * q.z );
    }

    // ** Quat::operator ==
    inline bool Quat::operator == ( const Quat& other ) const
    {
        return x == other.x && y == other.y && z == other.z && w == other.w;
    }

    // ** Quat::euler
    inline Vec3 Quat::euler( void ) const
    {
        f32 x2 = x * x;
        f32 y2 = y * y;
        f32 z2 = z * z;

        return Vec3(  degrees( atan2( 2*x*w - 2*y*z, 1 - 2*x2 - 2*z2 ) )
                    , degrees( atan2( 2*y*w - 2*x*z, 1 - 2*y2 - 2*z2 ) )
                    , degrees( asin ( 2*x*y + 2*z*w ) ) );
    }

    // ** Quat::yaw
    inline f32 Quat::yaw( void ) const
    {
        f32 x2 = 2.0f * x;
        f32 y2 = 2.0f * y;
        f32 z2 = 2.0f * z;
        f32 wy = y2 * w;
        f32 xx = x2 * x;
        f32 xz = z2 * x;
        f32 yy = y2 * y;

        return degrees( atan2( xz + wy, 1.0f - (xx + yy) ) );
    }

    // ** Quat::pitch
    inline f32 Quat::pitch( void ) const
    {
        f32 x2  = 2.0f * x;
        f32 z2  = 2.0f * z;
        f32 wx = x2 * w;
        f32 xx = x2 * x;
        f32 yz = z2 * y;
        f32 zz = z2 * z;

        return degrees( atan2( yz + wx, 1.0f - (xx + zz) ) );
    }

    // ** Quat::roll
    inline f32 Quat::roll( void ) const
    {
        f32 y2  = 2.0f * y;
        f32 z2  = 2.0f * z;
        f32 wz = z2 * w;
        f32 xy = y2 * x;
        f32 yy = y2 * y;
        f32 zz = z2 * z;

        return degrees( atan2( xy + wz, 1.0f - (yy + zz) ) );
    }

    // ** Quat::rotate
    inline Vec3 Quat::rotate( const Vec3& v ) const
    {
        Quat q = (*this * Quat( v, 0.0f ) * -*this);
        return Vec3( q.x, q.y, q.z );
    }

    // ** Quat::rotateAroundAxis
    inline Quat Quat::rotateAroundAxis( f32 angle, const Vec3& axis )
    {
        f32 alpha = radians( angle );
        f32 sine    = sinf( alpha / 2.0f );

        return Quat( axis * sine, cosf( alpha / 2.0f ) );
    }

NIMBLE_END

#endif  /*  !__Nimble_Color_H__  */
