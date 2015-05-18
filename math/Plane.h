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

#ifndef __Foo_Plane_H__
#define __Foo_Plane_H__

FOO_BEGIN_NAMESPACE

    /*!
     A plane in a 3-dimensinal space.
     */
    class Plane {
    public:

                        //! Constructs a Plane instance.
                        /*!
                         \param normal Plane normal.
                         \param point Point on plane.
                         */
                        Plane( const Vec3& normal = Vec3( 0, 0, 0 ), float distance = 0.0f )
                            : m_normal( normal ), m_distance( distance ) {}

        //! Projects a point onto this plane
        Vec3            operator * ( const Vec3& point ) const;

        //! Create plane from point and normal.
        static Plane    calculate( const Vec3& normal, const Vec3& point );

    private:

        //! Plane normal.
        Vec3            m_normal;

        //! Plane distance to origin.
        float           m_distance;
    };

    // ** Plane::calculate
    inline Plane Plane::calculate( const Vec3& normal, const Vec3& point) {
        return Plane( normal, -(normal * point) );
    }

    // ** Plane::operator *
    inline Vec3 Plane::operator * ( const Vec3& point ) const {
        float distanceToPoint = m_normal * point + m_distance;
        return point - m_normal * distanceToPoint;
    }

FOO_END_NAMESPACE

#endif  /*  !defined( __Foo_Plane_H__ )  */
