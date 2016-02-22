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
 
#ifndef	__Nimble_KeyValue_H__
#define	__Nimble_KeyValue_H__

#include "Globals.h"

NIMBLE_BEGIN

    //! Key-value storage type.
    template<typename TKey>
    class Kv {
    public:

    #ifdef NIMBLE_CPP11_ENABLED
        //! Alias the property container type.
        typedef std::unordered_map<TKey, Variant>   Properties;
    #else
        //! Alias the property container type.
        typedef Map<TKey, Variant>                  Properties;
    #endif  /*  NIMBLE_CPP11_ENABLED    */

        //! Returns the value with specified key.
        const Variant&      operator [] ( const TKey& key ) const;

        //! Returns stored KeyValue properties.
        const Properties&   properties( void ) const;

        //! Sets the value with specified key.
        void                setValueAtKey( const TKey& key, const Variant& value );

        //! Returns the value with specified key.
        const Variant&      valueAtKey( const TKey& key ) const;

        //! Returns true if value with specified key exists.
        bool                has( const TKey& key ) const;

        //! Generic method to set value with specified key.
        template<typename TValue>
        void set( const TKey& key, const TValue& value )
        {
            setValueAtKey( key, Variant::fromValue<TValue>( value ) );
        }

        //! Generic method to get value with specified key.
        template<typename TValue>
        TValue get( const TKey& key, const TValue& defaultValue = TValue() ) const
        {
            return has( key ) ? valueAtKey( key ).as<TValue>() : defaultValue;
        }

    private:

        Properties          m_properties;   //!< Actual key-value pairs.
    };

    // ** Kv::operator [] 
    template<typename TKey>
    const Variant& Kv<TKey>::operator [] ( const TKey& key ) const
    {
        NIMBLE_BREAK_IF( !has( key ) );
        return valueAtKey( key );
    }

    // ** Kv::properties
    template<typename TKey>
    const typename Kv<TKey>::Properties& Kv<TKey>::properties( void ) const
    {
        return m_properties;
    }

    // ** Kv::setValueAtKey
    template<typename TKey>
    void Kv<TKey>::setValueAtKey( const TKey& key, const Variant& value )
    {
        m_properties[key] = value;
    }

    // ** Kv::valueAtKey
    template<typename TKey>
    const Variant& Kv<TKey>::valueAtKey( const TKey& key ) const
    {
        static Variant invalid;
        Properties::const_iterator i = m_properties.find( key );
        return i != m_properties.end() ? i->second : invalid;
    }

    // ** Kv::has
    template<typename TKey>
    bool Kv<TKey>::has( const TKey& key ) const
    {
        return m_properties.count( key ) > 0;
    }

NIMBLE_END

#endif  /*  !__Nimble_KeyValue_H__  */