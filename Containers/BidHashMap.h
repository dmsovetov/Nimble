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

#ifndef __Nimble_Containers_BidHashMap_H__
#define __Nimble_Containers_BidHashMap_H__

#include "../Globals.h"

NIMBLE_BEGIN

    //! A simple bidirectional hash map wrapper.
    template<typename TKey, typename TValue>
    class BidHashMap {
    public:

        typedef HashMap<TKey, TValue>       KeyToValue;     //!< A container type to map from key to value.
        typedef HashMap<TValue, TKey>       ValueToKey;     //!< A container type to map from value to key.

        //! Inserts a bidirectional key to value mapping.
        void                                insert( const TKey& key, const TValue& value );

        //! Removes a bidirection mapping between key and value.
        void                                remove( const TKey& key, const TValue& value );

        //! Removes a bidirectional mapping by a key.
        void                                remove( const TKey& key );

        //! Removes a bidirectional mapping by a value.
        void                                remove( const TValue& value );

        //! Returns a key that is mapped to a specified value.
        typename ValueToKey::const_iterator find( const TValue& value ) const;

        //! Returns a value that is mapped from a specified key.
        typename KeyToValue::const_iterator find( const TKey& key ) const;

        //! Returns true if bidirectional mapping with a specified value exists.
        bool                                has( const TValue& value ) const;

        //! Returns true if bidirectional mapping with a specified key exists.
        bool                                has( const TKey& key ) const;

    private:

        KeyToValue                          m_keyToValue;   //!< Maps from a key to a value.
        ValueToKey                          m_valueToKey;   //!< Maps from a value to a key.
    };

    // ** BidHashMap::insert
    template<typename TKey, typename TValue>
    void BidHashMap<TKey, TValue>::insert( const TKey& key, const TValue& value )
    {
        m_keyToValue[key]   = value;
        m_valueToKey[value] = key;
    }

    // ** BidHashMap::remove
    template<typename TKey, typename TValue>
    void BidHashMap<TKey, TValue>::remove( const TKey& key, const TValue& value )
    {
        m_keyToValue.erase( find( key ) );
        m_valueToKey.erase( find( value ) );
    }

    // ** BidHashMap::remove
    template<typename TKey, typename TValue>
    void BidHashMap<TKey, TValue>::remove( const TKey& key )
    {
        typename KeyToValue::const_iterator i = find( key );
        m_valueToKey.erase( find( i->second ) );
        m_keyToValue.erase( i );
    }

    // ** BidHashMap::remove
    template<typename TKey, typename TValue>
    void BidHashMap<TKey, TValue>::remove( const TValue& value )
    {
        typename ValueToKey::const_iterator i = find( value );
        m_keyToValue.erase( find( i->second ) );
        m_valueToKey.erase( i );
    }

    // ** BidHashMap::find
    template<typename TKey, typename TValue>
    typename BidHashMap<TKey, TValue>::ValueToKey::const_iterator BidHashMap<TKey, TValue>::find( const TValue& value ) const
    {
        return m_valueToKey.find( value );
    }

    // ** BidHashMap::find
    template<typename TKey, typename TValue>
    typename BidHashMap<TKey, TValue>::KeyToValue::const_iterator BidHashMap<TKey, TValue>::find( const TKey& key ) const
    {
        return m_keyToValue.find( key );
    }

    // ** BidHashMap::has
    template<typename TKey, typename TValue>
    bool BidHashMap<TKey, TValue>::has( const TValue& value ) const
    {
        return m_valueToKey.count( value ) > 0;
    }

    // ** BidHashMap::has
    template<typename TKey, typename TValue>
    bool BidHashMap<TKey, TValue>::has( const TKey& key ) const
    {
        return m_keyToValue.count( key ) > 0;
    }

NIMBLE_END

#endif  /*  !__Nimble_Containers_BidHashMap_H__    */