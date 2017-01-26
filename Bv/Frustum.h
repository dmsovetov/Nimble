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

#ifndef __Nimble_Frustum_H__
#define __Nimble_Frustum_H__

#include "../Globals.h"

NIMBLE_BEGIN

    //! A set of frustum vertices.
    class FrustumVertices
    {
    public:
        
                        //! Constructs frustum vertices.
                        FrustumVertices();
        
                        //! Constructs frustum vertices.
                        FrustumVertices(const Vec3 vertices[8]);
        
        //! Returns a frustum vertex at specified index.
        const Vec3&     operator[] (s32 index) const;
        
        //! Returns a frustum vertex at specified index.
        Vec3&           operator[] (s32 index);
        
        //! Returns a bounding box around these frustum vertices.
        Bounds          box() const;
        
        //! Returns a bounding sphere around these frustum vertices.
        Sphere          sphere() const;
        
        //! Returns an array of frustum points.
        const Vec3*     points() const;
        
    private:
        
        Vec3            m_vertices[8];  //!< Actual vertex coordinates.
    };

    // ** FrustumVertices::FrustumVertices
    NIMBLE_INLINE FrustumVertices::FrustumVertices()
    {
    }

    // ** FrustumVertices::FrustumVertices
    NIMBLE_INLINE FrustumVertices::FrustumVertices(const Vec3 vertices[8])
    {
        memcpy(m_vertices, vertices, sizeof(Vec3) * 8);
    }

    // ** FrustumVertices::operator[]
    NIMBLE_INLINE const Vec3& FrustumVertices::operator[] (s32 index) const
    {
        NIMBLE_ABORT_IF(index < 0 || index >= 8, "index is out of range");
        return m_vertices[index];
    }

    // ** FrustumVertices::operator[]
    NIMBLE_INLINE Vec3& FrustumVertices::operator[] (s32 index)
    {
        NIMBLE_ABORT_IF(index < 0 || index >= 8, "index is out of range");
        return m_vertices[index];
    }

    // ** FrustumVertices::box
    NIMBLE_INLINE Bounds FrustumVertices::box() const
    {
        return Bounds::fromPoints(m_vertices, 8);
    }

    // ** FrustumVertices::sphere
    NIMBLE_INLINE Sphere FrustumVertices::sphere() const
    {
        return Sphere::fromPoints(m_vertices, 8);
    }

    // ** FrustumVertices::points
    NIMBLE_INLINE const Vec3* FrustumVertices::points() const
    {
        return m_vertices;
    }

    //! A frustum is the portion of a pyramid that lies between two parallel planes (near and far) cutting it.
    class Frustum
    {
    public:
        
                        //! Constructs an invalid frustum instance.
                        Frustum();
        
                        //! Constructs a frustum instance.
                        Frustum(f32 fov, f32 aspectRatio, f32 near, f32 far);
        
        //! Returns a frustum aspect ratio.
        f32             aspectRatio() const;
        
        //! Returns a frustum field of view.
        f32             fov() const;
        
        //! Returns a frustum near clipping plane.
        f32             near() const;
        
        //! Returns a frustum far clipping plane.
        f32             far() const;
        
        //! Splits a frustum into an array of smaller frustums.
        Array<Frustum>  split(s32 count, f32 lambda) const;
        
        //! Returns a frustum vertices.
        FrustumVertices vertices() const;
        
    private:
        
        f32             m_aspectRatio;  //!< A frustum aspect ratio.
        f32             m_fov;          //!< A frustum vertical field of view.
        f32             m_near;         //!< A near clipping plane.
        f32             m_far;          //!< A far clipping plane.
    };

    // ** Frustum::Frustum
    NIMBLE_INLINE Frustum::Frustum()
        : m_aspectRatio(0.0f)
        , m_fov(0.0f)
        , m_near(0.0f)
        , m_far(0.0f)
    {
    }

    // ** Frustum::Frustum
    NIMBLE_INLINE Frustum::Frustum(f32 fov, f32 aspectRatio, f32 near, f32 far)
        : m_aspectRatio(aspectRatio)
        , m_fov(fov)
        , m_near(near)
        , m_far(far)
    {
    }

    // ** Frustum::aspectRatio
    NIMBLE_INLINE f32 Frustum::aspectRatio() const
    {
        return m_aspectRatio;
    }

    // ** Frustum::fov
    NIMBLE_INLINE f32 Frustum::fov() const
    {
        return m_fov;
    }

    // ** Frustum::near
    NIMBLE_INLINE f32 Frustum::near() const
    {
        return m_near;
    }

    // ** Frustum::far
    NIMBLE_INLINE f32 Frustum::far() const
    {
        return m_far;
    }

    // ** Frustum::split
    NIMBLE_INLINE Array<Frustum> Frustum::split(s32 count, f32 lambda) const
    {
        Array<Frustum> frustums;
    
        f32 n = m_near;
        frustums.resize(count);
        for (s32 i = 0; i < count; i++)
        {
            f32 k   = (i + 1) / static_cast<f32>(count);
            f32 CLi = m_near * powf(m_far / m_near, k);
            f32 CUi = m_near + (m_far - m_near) * k;
            
            f32 f = lerp(CUi, CLi, lambda);
            frustums[i] = Frustum(m_fov, m_aspectRatio, n, f);
            n = f;
        }
        
        return frustums;
    }

    // ** Frustum::vertices
    NIMBLE_INLINE FrustumVertices Frustum::vertices() const
    {
        // Calculate a tangents from a FOV and aspect ratio
        f32 tanHalfVFOV = tanf(radians(m_fov * 0.5f));
        f32 tanHalfHFOV = tanf(radians(m_fov * 0.5f)) * m_aspectRatio;
        
        // Calculate dimensions of a split far and near faces
        f32 xn = m_near * tanHalfHFOV;
        f32 xf = m_far  * tanHalfHFOV;
        f32 yn = m_near * tanHalfVFOV;
        f32 yf = m_far  * tanHalfVFOV;
        
        FrustumVertices vertices;
        
        // Construct a near frustum plane in a local space
        vertices[0] = Vec3(-xn, -yn, -m_near);
        vertices[1] = Vec3( xn, -yn, -m_near);
        vertices[2] = Vec3( xn,  yn, -m_near);
        vertices[3] = Vec3(-xn,  yn, -m_near);
        
        // Construct a far frustum plane in a local space
        vertices[4] = Vec3(-xf, -yf, -m_far);
        vertices[5] = Vec3( xf, -yf, -m_far);
        vertices[6] = Vec3( xf,  yf, -m_far);
        vertices[7] = Vec3(-xf,  yf, -m_far);
        
        return vertices;
    }

NIMBLE_END

#endif  /*  !__Nimble_Frustum_H__  */
