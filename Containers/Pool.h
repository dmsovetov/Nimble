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

#ifndef __Nimble_Containers_Pool_H__
#define __Nimble_Containers_Pool_H__

#include "../Globals.h"

NIMBLE_BEGIN

    //! Opaque handle type, stores array index & generation.
    template<s32 TBitsIndex, s32 TBitsGeneration>
    class OpaqueHandle {
    public:

        //! Expose the number of bits used to represent an index.
        enum { Bits = TBitsIndex };

                                //! Constructs OpaqueHandle instance.
                                OpaqueHandle( u32 index = ~0, u32 generation = 0 )
                                    : m_index( index ), m_generation( generation ) {}

                                //! Copies an OpaqueHandle instance.
                                OpaqueHandle( const OpaqueHandle& other )
                                    : m_index( other.m_index ), m_generation( other.m_generation ) {}

                                //! Casts this OpaqueHandle to an integer value.
                                operator u32( void ) const;

        //! Compares two handles.
        bool                    operator == ( const OpaqueHandle& other ) const { return m_index == other.m_index && m_generation == other.m_generation; }

        //! Returns true if this handle is valid.
        inline bool             isValid( void ) const;

        //! Returns opaque handle generation.
        inline u32              generation( void ) const;

    private:

        u32                     m_index      : TBitsIndex;       //!< Slot index that is referenced by this handle.
        u32                     m_generation : TBitsGeneration;  //!< Generation of a slot at a time when this handle was issued.
    };

    // ** OpaqueHandle::operator u32
    template<s32 TBitsIndex, s32 TBitsGeneration>
    NIMBLE_INLINE OpaqueHandle<TBitsIndex, TBitsGeneration>::operator u32 ( void ) const
    {
        return m_index;
    }

    // ** OpaqueHandle::isValid
    template<s32 TBitsIndex, s32 TBitsGeneration>
    NIMBLE_INLINE bool OpaqueHandle<TBitsIndex, TBitsGeneration>::isValid( void ) const
    {
        static u32 Invalid = static_cast<u32>( ~0 ) >> (32 - TBitsIndex);
        return m_index != Invalid;
    }

    // ** OpaqueHandle::generation
    template<s32 TBitsIndex, s32 TBitsGeneration>
    NIMBLE_INLINE u32 OpaqueHandle<TBitsIndex, TBitsGeneration>::generation( void ) const
    {
        return m_generation;
    }

    //! Container issues opaque handles to it's slots that act as weak references.
    template<typename TValue, typename THandle>
    class Pool {
    public:

        typedef TValue          Value;  //!< Store the value type.
        typedef THandle         Handle; //!< Store the handle type.

                                //! Constructs Pool instance.
                                Pool( void );

        //! Adds the value to a container and returns it's ID.
        Handle                  add( const Value& value );

        //! Reserves the slot inside a container.
        Handle                  reserve( void );

        //! Removes item from a container with specified handle. Returns true if the removal succeed, otherwise returns false.
        bool                    remove( const Handle& handle );

        //! Returns true if the specified handle is valid.
        bool                    has( const Handle& handle ) const;

        //! Returns the value referenced by specified handle.
        NIMBLE_INLINE const Value&  get( const Handle& handle ) const;
        NIMBLE_INLINE Value&        get( const Handle& handle );

        //! Returns the total number of used slots.
        s32                     size( void ) const;

        //! Returns data at specified index.
        const TValue&           dataAt( s32 index ) const;
        TValue&                 dataAt( s32 index );

        //! Returns handle at specified index.
        Handle                  handleAt( s32 index ) const;

        //! Returns the maxium capacity.
        s32                     capacity( void ) const;

        //! Returns the total number of free handles.
        s32                     freeCount( void ) const;

    #ifdef NIMBLE_DEBUG
        //! Used for debuging, traces list of free slots.
        void                    traceFreeSlots( void ) const;
    #endif  /*  NIMBLE_DEBUG    */

    private:

        //! Expands the opaque handle list by count elements.
        void                    expand( s32 count );

        //! Returns next free hande.
        Handle                  allocate( void );

    private:

        Array<Value>            m_data;         //!< Actual slot data.
        Array<Handle>           m_slots;        //!< Array of avaible slots.
        u32                     m_head;         //!< Head of a free list.
        s32                     m_count;        //!< The total number of allocated handles.
        s32                     m_capacity;     //!< The maximum capacity.
    };

    // ** Pool::Pool
    template<typename TValue, typename THandle>
    Pool<TValue, THandle>::Pool( void )
         : m_head( 0 )
         , m_count( 0 )
         , m_capacity( 0 )
    {
    }

    // ** Pool::size
    template<typename TValue, typename THandle>
    s32 Pool<TValue, THandle>::size( void ) const
    {
        return m_count;
    }

    // ** Pool::add
    template<typename TValue, typename THandle>
    THandle Pool<TValue, THandle>::add( const TValue& value )
    {
        Handle handle = reserve();
        m_data[handle] = value;
        return handle;
    }

    // ** Pool::reserve
    template<typename TValue, typename THandle>
    THandle Pool<TValue, THandle>::reserve( void )
    {
        // Maximum capacity reached - expand
        if( freeCount() == 0 ) {
            s32 count = static_cast<s32>( capacity() * 0.25f );
            expand( count <= 0 ? 4 : count );
        }

        // Reserve the slot
        Handle handle = allocate();
        NIMBLE_ABORT_IF( !handle.isValid(), "failed to allocate handle" );

        return handle;
    }

    // ** Pool::has
    template<typename TValue, typename THandle>
    bool Pool<TValue, THandle>::has( const THandle& handle ) const
    {
        if( !handle.isValid() ) {
            return false;
        }

        NIMBLE_ABORT_IF( handle >= static_cast<u32>( capacity() ), "handle index is out of range" );
        return handle.generation() == m_slots[handle].generation();
    }

    // ** Pool::get
    template<typename TValue, typename THandle>
    NIMBLE_INLINE const TValue& Pool<TValue, THandle>::get( const THandle& handle ) const
    {
        NIMBLE_ABORT_IF( !has( handle ), "Handle is not valid" );
        return m_data[handle];
    }

    // ** Pool::get
    template<typename TValue, typename THandle>
    NIMBLE_INLINE TValue& Pool<TValue, THandle>::get( const THandle& handle )
    {
        NIMBLE_ABORT_IF( !has( handle ), "Handle is not valid" );
        return m_data[handle];
    }

    // ** Pool::expand
    template<typename TValue, typename THandle>
    void Pool<TValue, THandle>::expand( s32 count )
    {
        // Save previous capacity.
        s32 oldCapacity = capacity();
        m_capacity = oldCapacity + count;

        // Resize the block of slots
        m_slots.resize( capacity() );

        // Resize data
        m_data.resize( capacity() );

        // Add new handles to a free list.
        for( s32 i = oldCapacity; i < oldCapacity + count; i++ ) {
            m_slots[i] = i + 1;
        }
    }

    // ** Pool::allocate
    template<typename TValue, typename THandle>
    THandle Pool<TValue, THandle>::allocate( void )
    {
        // Return invalid handle if no more slots left.
        if( size() >= capacity() ) {
            return 0;
        }

        // Get the first free handle.
        THandle free = m_slots[m_head];

        // Save next free slot index.
        u32 next = free;

        // Fix the index.
        free = THandle( m_head, free.generation() );

        // Remove it from a list.
        m_head = next;

        // Increase the size of a container.
        m_count++;

        return free;
    }

    // ** SlotHandleList::remove
    template<typename TValue, typename THandle>
    bool Pool<TValue, THandle>::remove( const THandle& handle )
    {
        if( !has( handle ) ) {
            return false;
        }

        u32 idx = handle;

        // Place this handle to a free list and increase the generation counter to invalidate all living handles.
        m_slots[idx] = THandle( m_head, handle.generation() + 1 );
        m_head       = idx;

        // Decrease the allocated counter.
        m_count--;

        return true;
    }

    // ** SlotHandleList::freeCount
    template<typename TValue, typename THandle>
    s32 Pool<TValue, THandle>::freeCount( void ) const
    {
        return capacity() - size();
    }

    // ** SlotHandleList::capacity
    template<typename TValue, typename THandle>
    s32 Pool<TValue, THandle>::capacity( void ) const
    {
        return m_capacity;
    }

    // ** Pool::dataAt
    template<typename TValue, typename THandle>
    const TValue& Pool<TValue, THandle>::dataAt( s32 index ) const
    {
        NIMBLE_ABORT_IF( index < 0 || index >= size(), "index is out of range" );
        return m_data[index];
    }

    // ** Pool::dataAt
    template<typename TValue, typename THandle>
    TValue& Pool<TValue, THandle>::dataAt( s32 index )
    {
        NIMBLE_ABORT_IF( index < 0 || index >= size(), "index is out of range" );
        return m_data[index];
    }

    // ** Pool::handleAt
    template<typename TValue, typename THandle>
    THandle Pool<TValue, THandle>::handleAt( s32 index ) const
    {
        return THandle( index, m_slots[index].generation() );
    }

#ifdef NIMBLE_DEBUG
    // ** Pool::traceFreeSlots
    template<typename TValue, typename THandle>
    void Pool<TValue, THandle>::traceFreeSlots( void ) const
    {
        printf( "Free slots: " );

        for( s32 idx = m_head; idx < capacity(); ) {
            printf( "%d ", idx );
            idx = indexFromId( m_handles[idx] );
        }

        printf( "\n" );
    }
#endif  /*  NIMBLE_DEBUG    */

    //! Used by std::unordered_map to hash opaque handle.
    template<typename THandle>
    struct OpaqueHandleHasher {
        size_t operator () ( const THandle& handle ) const;
    };

    // ** OpaqueHandleHasher::operator ()
    template<typename THandle>
    NIMBLE_INLINE size_t OpaqueHandleHasher<THandle>::operator ()( const THandle& handle ) const {
        return static_cast<u32>( handle );
    }

NIMBLE_END

#endif  /*  !__Nimble_Containers_Pool_H__    */
