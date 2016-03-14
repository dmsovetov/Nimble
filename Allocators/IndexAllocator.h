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

#ifndef __Nimble_IndexAllocator_H__
#define __Nimble_IndexAllocator_H__

#include "../Globals.h"

NIMBLE_BEGIN

    //! Allocates objects of type T from an array and returns their indices.
    template<typename T>
    class IndexAllocator {
	public:

					    //! Constructs IndexAllocator instance
					    IndexAllocator( s32 size, f32 growth = 0.1f );

        //! Returns the allocated item by it's index.
        const T&        operator[] ( s32 index ) const;
        T&              operator[] ( s32 index );

        //! Returns allocated items.
        const Array<T>& items( void ) const;
        Array<T>&       items( void );

		//! Clears an array of allocated objects.
		void		    reset( void );

		//! Allocates the new object.
		s32			    allocate( void );

		//! Returns the total number of allocated objects.
		s32			    allocatedCount( void ) const;

		//! Returns the maximum capacity.
		s32			    maximumCapacity( void ) const;

	private:

		Array<T>	    m_array;            //!< An array of allocated objects.
        f32             m_growth;           //!< When an array is full it's size will be increased by this factor.
		s32			    m_capacity;         //!< The maximum number of objects that can be allocated.
		s32			    m_allocatedCount;	//!< The total number of allocated objects.
    };

    // ** IndexAllocator::IndexAllocator
    template<typename T>
    IndexAllocator<T>::IndexAllocator( s32 size, f32 growth )
        : m_growth( growth )
        , m_capacity( size )
        , m_allocatedCount( 0 )
    {
        m_array.resize( size );
    }

    // ** IndexAllocator::items
    template<typename T>
    const Array<T>& IndexAllocator<T>::items( void ) const
    {
        return m_array;
    }

    // ** IndexAllocator::items
    template<typename T>
    Array<T>& IndexAllocator<T>::items( void )
    {
        return m_array;
    }

	// ** IndexAllocator::reset
	template<typename T>
	void IndexAllocator<T>::reset( void )
	{
        for( s32 i = 0, n = allocatedCount(); i < n; i++ ) {
            m_array[i].~T();
        }

		m_allocatedCount = 0;
	}

    // ** IndexAllocator::operator[]
    template<typename T>
    const T& IndexAllocator<T>::operator[]( s32 index ) const
    {
        NIMBLE_ABORT_IF( index < 0 || index >= allocatedCount(), "index is out of range" );
        return m_array[index];
    }

    // ** IndexAllocator::operator[]
    template<typename T>
    T& IndexAllocator<T>::operator[]( s32 index )
    {
        NIMBLE_ABORT_IF( index < 0 || index >= allocatedCount(), "index is out of range" );
        return m_array[index];
    }

	// ** IndexAllocator::allocate
	template<typename T>
	s32 IndexAllocator<T>::allocate( void )
	{
        // Resize an array when maximum capacity reached
		if( m_allocatedCount >= maximumCapacity() ) {
            m_capacity = maximumCapacity() + min2( 1, static_cast<s32>( maximumCapacity() * m_growth ) );
			m_array.resize( m_capacity );
		}

        s32 idx = m_allocatedCount++;
		T* allocated = &m_array[idx];
		new( allocated ) T;

		return idx;
	}

	// ** IndexAllocator::allocatedCount
	template<typename T>
	s32 IndexAllocator<T>::allocatedCount( void ) const
	{
		return m_allocatedCount;
	}

	// ** IndexAllocator::maximumCapacity
	template<typename T>
	s32 IndexAllocator<T>::maximumCapacity( void ) const
	{
		return m_capacity;
	}

NIMBLE_END

#endif    /*    !__Nimble_IndexAllocator_H__    */
