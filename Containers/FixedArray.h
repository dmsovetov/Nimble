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

#ifndef __Nimble_Containers_FixedArray_H__
#define __Nimble_Containers_FixedArray_H__

#include "../Globals.h"

NIMBLE_BEGIN

    //! This container type wraps an Array class and implements the swap-on-remove behaviour.
    template<typename T>
    class FixedArray {
    public:

        //! Alias the container type.
        typedef Array<T>    Container;

                            //! Constructs the FixedArray of specified size.
                            FixedArray( s32 size = 0 );

        //! Returns an item at specified index.
        const T&            operator []( s32 index ) const;

        //! Returns the total number of items inside an array.
        s32                 count( void ) const;

        //! Returns the maximum number of items that can be stored inside an array without resizing.
        s32                 maximumCapacity( void ) const;

        //! Removes element from an array at specified index.
        void                remove( s32 index );

        //! Adds new element to the end of an array and returns it's index.
        s32                 push( const T& value );

        //! Resizes an array.
        void                resize( s32 size );

    private:

        Container           m_items;    //!< Actual data container.
        s32                 m_count;    //!< The total number of items stored in array.
    };

    // ** FixedArray::FixedArray
    template<typename T>
    FixedArray<T>::FixedArray( s32 size )
        : m_count( 0 )
    {
        resize( size );
    }

    // ** FixedArray::operator []
    template<typename T>
    NIMBLE_INLINE const T& FixedArray<T>::operator []( s32 index ) const
    {
        NIMBLE_BREAK_IF( index < 0 || index >= count(), "index is out of range" );
        return m_items[index];
    }

    // ** FixedArray::count
    template<typename T>
    NIMBLE_INLINE s32 FixedArray<T>::count( void ) const
    {
        return m_count;
    }

    // ** FixedArray::maximumCapacity
    template<typename T>
    s32 FixedArray<T>::maximumCapacity( void ) const
    {
        return static_cast<s32>( m_items.size() );
    }

    // ** FixedArray::remove
    template<typename T>
    void FixedArray<T>::remove( s32 index )
    {
        NIMBLE_BREAK_IF( index < 0 || index >= count(), "index is out of range" );
        m_items[index] = m_items[m_count - 1];
        m_count--;
    }

    // ** FixedArray::push
    template<typename T>
    s32 FixedArray<T>::push( const T& value )
    {
        if( count() < maximumCapacity() ) {
            m_items[m_count] = value;
        } else {
            m_items.push_back( value );
        }
        m_count++;

        return m_count - 1;
    }

    // ** FixedArray::resize
    template<typename T>
    void FixedArray<T>::resize( s32 size )
    {
        if( size ) {
            m_items.resize( size );
        } else {
            m_items.clear();
        }

        m_count = min2( size, m_count );
    }

NIMBLE_END

#endif  /*  !__Nimble_Containers_FixedArray_H__    */