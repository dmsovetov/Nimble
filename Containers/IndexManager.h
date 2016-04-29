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

#ifndef __Nimble_Containers_IndexManager_H__
#define __Nimble_Containers_IndexManager_H__

#include "../Globals.h"

NIMBLE_BEGIN

    //! Manages unique integer identifiers that can be resused after being released.
    template<typename TValue>
    class IndexManager {
    public:

                        //! Constructs an IndexManager instance.
                        IndexManager( void );

                        //! Cleans up an IndexManager instance.
                        ~IndexManager( void );

        //! Reallocates an index manager.
        void            resize( s32 capacity );

        //! Returns a next free index.
        TValue          acquire( void );

        //! Releases a used index.
        void            release( TValue index );

        //! Returns a maximum capacity.
        s32             maximumCapacity( void ) const;

        //! Returns a total number of allocated indices.
        s32             allocatedCount( void ) const;

        //! Returns true if there are any free indices left.
        bool            hasFreeIndices( void ) const;

        //! Returns an array of indices.
        const TValue*   indices( void ) const;

    private:

        TValue*         m_indices;          //!< An array of allocated indices.
        s32             m_maximumCapacity;  //!< A maximum number of indices that can be allocated.
        s32             m_allocatedCount;   //!< A total number of allocated indices.
        TValue          m_firstFree;        //!< A first free index.
    };

    // ** IndexManager::IndexManager
    template<typename TValue>
    IndexManager<TValue>::IndexManager( void )
        : m_indices( NULL )
        , m_maximumCapacity( 0 )
        , m_allocatedCount( 0 )
        , m_firstFree( 0 )
    {
    }

    // ** IndexManager::~IndexManager
    template<typename TValue>
    IndexManager<TValue>::~IndexManager( void )
    {
        delete[]m_indices;
    }

    // ** IndexManager::resize
    template<typename TValue>
    void IndexManager<TValue>::resize( s32 capacity )
    {
        // Allocate a new array of indices
        TValue* indices = new TValue[capacity];

        // Copy all allocated indices to an array
        if( m_allocatedCount ) {
            memcpy( indices, m_indices, sizeof( TValue ) * m_allocatedCount );
        }

        // Add new indices to a free list.
        for( s32 i = m_allocatedCount; i < capacity; i++ ) {
            indices[i] = i + 1;
        }

        // Free previous indices
        delete[]m_indices;
        m_indices = indices;

        // Update a capacity
        m_maximumCapacity = capacity;
    }

    // ** IndexManager::acquire
    template<typename TValue>
    TValue IndexManager<TValue>::acquire( void )
    {
        // Resize an array of indices if no more left
        if( !hasFreeIndices() ) {
            resize( maximumCapacity() + 1 );
        }

        // Allocate a next index
        TValue index = m_firstFree;
        m_firstFree  = m_indices[index];
        m_allocatedCount++;

        // And return it's value
        return index;
    }

    // ** IndexManager::release
    template<typename TValue>
    void IndexManager<TValue>::release( TValue index )
    {
        NIMBLE_ABORT_IF( m_allocatedCount == 0, "no indices allocated" );

        m_indices[index] = m_firstFree;
        m_firstFree      = index;
        m_allocatedCount--;
    }

    // ** IndexManager::hasFreeIndices
    template<typename TValue>
    bool IndexManager<TValue>::hasFreeIndices( void ) const
    {
        return allocatedCount() < maximumCapacity();
    }

    // ** IndexManager::maximumCapacity
    template<typename TValue>
    s32 IndexManager<TValue>::maximumCapacity( void ) const
    {
        return m_maximumCapacity;
    }

    // ** IndexManager::allocatedCount
    template<typename TValue>
    s32 IndexManager<TValue>::allocatedCount( void ) const
    {
        return m_allocatedCount;
    }

    // ** IndexManager::indices
    template<typename TValue>
    const TValue* IndexManager<TValue>::indices( void ) const
    {
        return m_indices;
    }

NIMBLE_END

#endif    /*    !__Nimble_Containers_IndexManager_H__    */