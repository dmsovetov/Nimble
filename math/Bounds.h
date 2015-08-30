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

#ifndef __Foo_Bounds_H__
#define __Foo_Bounds_H__

FOO_BEGIN_NAMESPACE

    /*!
     A bounding box class.
     */
    class Bounds {
    public:

        //! Constructs a Bounds instance
        /*!
         \param min A bounds minimum point.
         \param max A bounds maximum point.
         */
        Bounds( Vec3 min = Vec3( FLT_MAX, FLT_MAX, FLT_MAX ), Vec3 max = Vec3( -FLT_MAX, -FLT_MAX, -FLT_MAX ) );

        //! Adds a new point to a bounding box (extends a bounds if needed).
        const Bounds&   operator += ( const Vec3& point );

        //! Adds two bounding volumes.
        const Bounds&   operator += ( const Bounds& other );

        //! Returns a bounds volume.
        float           volume( void ) const;

        //! Returns a minimum bound point.
        const Vec3&     min( void ) const;

        //! Returns a maximum bound point.
        const Vec3&     max( void ) const;

        //! Returns bounds width.
        float           width( void ) const;

        //! Returns bounds height.
        float           height( void ) const;

        //! Returns bounds depth.
        float           depth( void ) const;

        //! Returns a random point in bounding box.
        Vec3            randomPointInside( void ) const;

    private:

        //! Lower corner coordinate.
        Vec3            m_min;

        //! Upper corner coordinate.
        Vec3            m_max;
    };

    // ** Bounds::Bounds
    inline Bounds::Bounds( Vec3 min, Vec3 max ) : m_min( min ), m_max( max ) {

    }

    // ** Bounds::randomPointInside
    inline Vec3 Bounds::randomPointInside( void ) const
    {
        return Vec3(  lerp( m_min.x, m_max.x, rand0to1() )
                    , lerp( m_min.y, m_max.y, rand0to1() )
                    , lerp( m_min.z, m_max.z, rand0to1() ) );
    }

    // ** Bounds::volume
    inline float Bounds::volume( void ) const {
        return (m_max.x - m_min.x) * (m_max.y - m_min.y) * (m_max.z - m_min.z);
    }

    // ** Bounds::min
    inline const Vec3& Bounds::min( void ) const {
        return m_min;
    }

    // ** Bounds::max
    inline const Vec3& Bounds::max( void ) const {
        return m_max;
    }

    // ** Bounds::width
    inline float Bounds::width( void ) const {
        return fabsf( m_max.x - m_min.x );
    }

    // ** Bounds::height
    inline float Bounds::height( void ) const {
        return fabsf( m_max.y - m_min.y );
    }

    // ** Bounds::depth
    inline float Bounds::depth( void ) const {
        return fabsf( m_max.z - m_min.z );
    }

    // ** Bounds::operator +=
    inline const Bounds& Bounds::operator += ( const Vec3& point ) {
        for( int i = 0; i < 3; i++ ) {
            m_min[i] = min2( m_min[i], point[i] );
            m_max[i] = max2( m_max[i], point[i] );
        }
        return *this;
    }

    // ** Bounds::operator +=
    inline const Bounds& Bounds::operator += ( const Bounds& other ) {
        *this += other.m_min;
        *this += other.m_max;
        return *this;
    }

	//! A 2d bounding rectangle class.
	class Rect {
	public:

						//! Constructs a Rect from corner points.
						Rect( const Vec2& min = Vec2(), const Vec2& max = Vec2() );

						//! Constructs a Rect instance from it's coordinates.
						Rect( f32 x1, f32 y1, f32 x2, f32 y2 );

		//! Returns the upper left bounding rect corner.
		const Vec2&		min( void ) const;

		//! Returns the lower right bounding rect corner.
		const Vec2&		max( void ) const;

		//! Returns rectangle width.
		f32				width( void ) const;

		//! Returns rectangle height.
		f32				height( void ) const;

	private:

		Vec2			m_min;	//!< Upper left bounding rect corner.
		Vec2			m_max;	//!< Lower right bounding rect corner.
	};

	// ** Rect::Rect
	inline Rect::Rect( const Vec2& min, const Vec2& max )
		: m_min( min ), m_max( max )
	{
	}

	// ** Rect::Rect
	inline Rect::Rect( f32 x1, f32 y1, f32 x2, f32 y2 )
		: m_min( x1, y1 ), m_max( x2, y2 )
	{
	}

	// ** Rect::min
	inline const Vec2& Rect::min( void ) const
	{
		return m_min;
	}

	// ** Rect::max
	inline const Vec2& Rect::max( void ) const
	{
		return m_max;
	}

	// ** Rect::min
	inline f32 Rect::width( void ) const
	{
		return m_max.x - m_min.x;
	}

	// ** Rect::max
	inline f32 Rect::height( void ) const
	{
		return m_max.y - m_min.y;
	}

FOO_END_NAMESPACE

#endif  /*  !defined( __Foo_Bounds_H__ )  */
