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

#ifndef __Nimble_Containers_IndexCache_H__
#define __Nimble_Containers_IndexCache_H__

#include "../Globals.h"

NIMBLE_BEGIN

    //! Manages a mapping from a resource to it's integer identifier and returns a resource by it's identifier.
    template<typename TValue, typename THasher = std::hash<TValue> >
    class IndexCache {
    public:

        //! Adds a new resource to a cache and returns it's identifier.
        s32                             add( const TValue& value );

        //! Returns a resource by it's identifier.
        const TValue&                   resolve( s32 identifier ) const;

        //! Returns a resource cache size.
        s32                             size( void ) const;

    private:

        //! Container type to map from a resource to it's identifier.
        typedef HashMap<TValue, s32, THasher>   ValueIdentifiers;

        Array<TValue>                   m_cache;    //!< An array of cached items.
        ValueIdentifiers                m_ids;      //!< Maps a resource instance to an associated iteger identifier.
    };

    // ** IndexCache::size
    template<typename TValue, typename THasher>
    NIMBLE_INLINE s32 IndexCache<TValue, THasher>::size( void ) const
    {
        return static_cast<s32>( m_cache.size() );
    }

    // ** IndexCache::add
    template<typename TValue, typename THasher>
    s32 IndexCache<TValue, THasher>::add( const TValue& value )
    {
        // First lookup a resource inside an identifiers map.
        typename ValueIdentifiers::iterator i = m_ids.find( value );

        // Found - just return a stored id
        if( i != m_ids.end() ) {
            return i->second;
        }

        // Allocate new identifier
        s32 id = size() + 1;

        // Push value to a cache
        m_cache.push_back( value );

        // Register identifier
        m_ids[value] = id;

        return id;
    }

    // ** IndexCache::add
    template<typename TValue, typename THasher>
    NIMBLE_INLINE const TValue& IndexCache<TValue, THasher>::resolve( s32 identifier ) const
    {
        identifier = identifier - 1;
        NIMBLE_ABORT_IF( identifier < 0 || identifier >= size(), "invalid resource identifier" );
        return m_cache[identifier];
    }

NIMBLE_END

#endif    /*    !__Nimble_Containers_IndexCache_H__    */