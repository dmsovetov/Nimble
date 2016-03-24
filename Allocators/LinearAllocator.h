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

#ifndef __Nimble_LinearAllocator_H__
#define __Nimble_LinearAllocator_H__

#include "../Globals.h"

NIMBLE_BEGIN

    //! Alocates bytes from an array.
    class LinearAllocator {
    public:

                    //! Constructs LinearObjectAllocator instance
                    LinearAllocator( s32 size )
                        : m_size( size )
                        , m_allocated( 0 )
                    {
                        reset();
                    }

		//! Clears an array of allocated bytes.
		void		reset( void );

        //! Performs a reallocation of internal buffer.
        void        resize( s32 size );

		//! Allocates the specified number of bytes.
		u8*			allocate( s32 size );

		//! Returns the total number of allocated bytes.
		s32			allocated( void ) const;

		//! Returns the maximum allocator capacity.
		u32			size( void ) const;

        //! Returns a pointer to an allocated data.
        const u8*   data( void ) const;

    private:

        s32         m_size;         //!< The maximum number of bytes that can be allocated.
        s32         m_allocated;    //!< The total number of bytes that is allocated.
        Array<u8>   m_bytes;        //!< Allocated bytes.
    };

	// ** LinearAllocator::reset
	inline void LinearAllocator::reset( void )
	{
		m_allocated = 0;
		resize( m_size );
	}

    // ** LinearAllocator::resize
    inline void LinearAllocator::resize( s32 size )
    {
        m_bytes.resize( size );
        m_size = size;
    }

	// ** LinearAllocator::allocate
	inline u8* LinearAllocator::allocate( s32 size )
	{
		if( (m_allocated + size) >= m_size ) {
			return NULL;
		}

		u8* allocated = &m_bytes[m_allocated];
		m_allocated += size;

		return allocated;
	}

	// ** LinearAllocator::allocated
	inline s32 LinearAllocator::allocated( void ) const
	{
		return m_allocated;
	}

	// ** LinearAllocator::size
	inline u32 LinearAllocator::size( void ) const
	{
		return m_size;
	}

    // ** LinearAllocator::data
    inline const u8* LinearAllocator::data( void ) const
    {
        return &m_bytes[0];
    }

    //! Allocates objects of type T from an array.
    template<typename T>
    class LinearObjectAllocator {
	public:

					//! Constructs LinearObjectAllocator instance
					LinearObjectAllocator( u32 size )
						: m_size( size ), m_count( 0 ) { reset(); }

		//! Clears an array of allocated objects.
		void		reset( void );

		//! Allocates the new object.
		T*			allocate( void );

		//! Returns the total number of allocated objects.
		u32			allocatedCount( void ) const;

		//! Returns the maximum capacity.
		u32			size( void ) const;

	private:

		Array<T>	m_array;	//!< An array of allocated objects.
		u32			m_size;		//!< The maximum number of objects that can be allocated.
		u32			m_count;	//!< The total number of allocated objects.
    };

	// ** LinearObjectAllocator::reset
	template<typename T>
	void LinearObjectAllocator<T>::reset( void )
	{
		m_count = 0;
		m_array.resize( m_size );
	}

	// ** LinearObjectAllocator::allocate
	template<typename T>
	T* LinearObjectAllocator<T>::allocate( void )
	{
		if( m_count >= m_size ) {
			return NULL;
		}

		T* allocated = &m_array[m_count++];
		new( allocated ) T;

		return allocated;
	}

	// ** LinearObjectAllocator::allocatedCount
	template<typename T>
	u32 LinearObjectAllocator<T>::allocatedCount( void ) const
	{
		return m_count;
	}

	// ** LinearObjectAllocator::size
	template<typename T>
	u32 LinearObjectAllocator<T>::size( void ) const
	{
		return m_size;
	}

NIMBLE_END

#endif    /*    !__Nimble_LinearAllocator_H__    */
