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

#ifndef __Foo_Vec3_H__
#define __Foo_Vec3_H__

#include "Config.h"

FOO_BEGIN_NAMESPACE

    /*!
     Euclidean 3-dimensinal vector.
     */
    class Vec3 {
    public:

                    Vec3( void );
                    Vec3( float x, float y, float z );
                    Vec3( const float* v );

        bool        operator == ( const Vec3& other ) const;
        float&      operator[]( int index );
        float       operator[]( int index ) const;
        Vec3        operator - ( void ) const;
        Vec3        operator + ( const Vec3& other ) const;
        Vec3        operator - ( const Vec3& other ) const;
        float       operator * ( const Vec3& other ) const;
        Vec3        operator * ( float scalar ) const;
        Vec3        operator / ( float scalar ) const;
        Vec3        operator % ( const Vec3& other ) const;

        //! Normalizes vector.
        float       normalize( void );

        //! Returns a vector length.
        float       length( void ) const;

        //! Rotates around an axis.
        static Vec3 rotateAroundAxis( const Vec3& axis, float theta, const Vec3& point );

        //! Returns a random point in sphere.
        static Vec3 randomInSphere( const Vec3& center, float radius );

        //! Returns a random direction.
        static Vec3 randomDirection( void );

        //! Returns a random direction on hemisphere.
        static Vec3 randomHemisphereDirection( const Vec3& point, const Vec3& normal );

        //! Returns a normalized vector.
        static Vec3 normalize( const Vec3& v );

    public:

        float   x, y, z;
    };

    // ** Vec3::Vec3
    inline Vec3::Vec3( void ) : x( 0.0f ), y( 0.0f ), z( 0.0f )
    {

    }

    // ** Vec3::Vec3
    inline Vec3::Vec3( float x, float y, float z ) : x( x ), y( y ), z( z )
    {

    }

    // ** Vec3::Vec3
    inline Vec3::Vec3( const float* v ) : x( v[0] ), y( v[1] ), z( v[2] )
    {

    }

    // ** Vec3::operator[]
    inline float Vec3::operator[]( int index ) const {
        switch( index ) {
        case 0: return x;
        case 1: return y;
        case 2: return z;
        }

        assert( false );
        return 0;
    }

    // ** Vec3::operator[]
    inline float& Vec3::operator[]( int index ) {
        switch( index ) {
        case 0: return x;
        case 1: return y;
        case 2: return z;
        }

        assert( false );
        return x;
    }

    // ** Vec3::operator ==
    inline bool Vec3::operator == ( const Vec3& other ) const {
        return x == other.x && y == other.y && z == other.z;
    }

    // ** Vec3::operator *
    inline Vec3 Vec3::operator * ( float scalar ) const {
        return Vec3( x * scalar, y * scalar, z * scalar );
    }

    // ** Vec3::operator /
    inline Vec3 Vec3::operator / ( float scalar ) const {
        return Vec3( x / scalar, y / scalar, z / scalar );
    }

    // ** Vec3::operator *
    inline float Vec3::operator * ( const Vec3& other ) const {
        return x * other.x + y * other.y + z * other.z;
    }

    // ** Vec3::operator +
    inline Vec3 Vec3::operator + ( const Vec3& other ) const {
        return Vec3( x + other.x, y + other.y, z + other.z );
    }

    // ** Vec3::operator -
    inline Vec3 Vec3::operator - ( const Vec3& other ) const {
        return Vec3( x - other.x, y - other.y, z - other.z );
    }

    // ** Vec3::operator %
    inline Vec3 Vec3::operator % ( const Vec3& other ) const {
        return Vec3( y*other.z - z*other.y, z*other.x - x*other.z, x*other.y - y*other.x );
    }

    inline Vec3 Vec3::operator - ( void ) const {
        return Vec3( -x, -y, -z );
    }

    // ** Vec3::length
    inline float Vec3::length( void ) const {
        return sqrt( x*x + y*y + z*z ) ;
    }

    // ** Vec3::normalize
    inline float Vec3::normalize( void ) {
        float len = length();
        if( len ) {
            x /= len ; y /= len ; z /= len;
        }
        
        return len;
    }

    // ** Vec3::rotateAroundAxis
    inline Vec3 Vec3::rotateAroundAxis( const Vec3& axis, float theta, const Vec3& point )
    {
        Vec3 rotationAxis = axis;
        rotationAxis.normalize();

        double common_factor = sin( theta * 0.5 );

        double a = cos( theta * 0.5 );
        double b = rotationAxis.x * common_factor;
        double c = rotationAxis.y * common_factor;
        double d = rotationAxis.z * common_factor;

        double mat[9] = {
            a*a+b*b-c*c-d*d, 2*(b*c-a*d),     2*(b*d+a*c),
            2*(b*c+a*d),     a*a-b*b+c*c-d*d, 2*(c*d-a*b),
            2*(b*d-a*c),     2*(c*d+a*b),     a*a-b*b-c*c+d*d
        };

        Vec3 result;

        for(int i = 0; i < 3; i++){
            for( int j = 0; j < 3; j++ ) result[i] += static_cast<float>( mat[i*3+j] * point[j] );
        }

        assert( !isNaN( result.x ) );
        assert( !isNaN( result.y ) );
        assert( !isNaN( result.z ) );

        return result;
    }

    // ** Vec3::randomInSphere
    inline Vec3 Vec3::randomInSphere( const Vec3& center, float radius )
    {
        return center + Vec3::randomDirection() * radius;
    }

    // ** Vec3::randomDirection
    inline Vec3 Vec3::randomDirection( void )
    {
        Vec3 result( rand0to1() * 2 - 1, rand0to1() * 2 - 1, rand0to1() * 2 - 1 );
        result.normalize();

        return result;
    }

    // ** Vec3::randomHemisphereDirection
    inline Vec3 Vec3::randomHemisphereDirection( const Vec3& point, const Vec3& normal )
    {
    /*
        float theta = acosf( sqrtf( rand0to1() ) );
        float phi   = rand0to1() * 6.28318531f;	// ** TwoPi

        // temporary created rondom direction
        Vec3 temp = Vec3( sinf( theta ) * cosf( phi ), sinf( theta ) * sinf( phi ), cosf( theta ) );

        // rotate "temp" such that z-axis used for sampling is aligned with "normal"
        float angle_between	= acosf( normal.z );
        float d = fabs( 1 - normal.z );
        if( 1 - fabs( normal.z ) <= 0.01f ) {
            angle_between = normal.z > 0 ? 0.0f : -M_PI;
        }

        assert( !isnan( angle_between ) );

        Vec3 rotation_axis;
        if( fabs( normal.x ) < 0.0001f && fabs( normal.y ) < 0.0001f ) {
            rotation_axis = Vec3( -normal.y, normal.z, 0.0 );
        } else {
            rotation_axis = Vec3( -normal.y, normal.x, 0.0 );
        }

        rotation_axis.normalize();

        return Vec3::rotateAroundAxis( rotation_axis, angle_between, temp );
    */
        Vec3 dir = randomDirection();

        if( dir * normal < 0 ) {
            dir = -dir;
        }

        return dir;
    }

    // ** Vec3::normalize
    inline Vec3 Vec3::normalize( const Vec3& v ) {
        Vec3 result = v;
        result.normalize();
        return result;
    }

FOO_END_NAMESPACE

#endif  /*  !defined( __Foo_Vec3_H__ )  */
