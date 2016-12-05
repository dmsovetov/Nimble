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
 
#ifndef    __Nimble_KeyValue_H__
#define    __Nimble_KeyValue_H__

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

        //! Removes a value with specified key and returns true if value existed before deletion.
        bool                removeValueAtKey( const TKey& key );

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
            return has( key ) ? valueAtKey( key ).template as<TValue>() : defaultValue;
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

    // ** Kv::removeValueAtKey
    template<typename TKey>
    bool Kv<TKey>::removeValueAtKey( const TKey& key )
    {
        typename Properties::iterator i = m_properties.find( key );

        if( i != m_properties.end() ) {
            m_properties.erase( i );
            return true;
        }

        return false;
    }

    // ** Kv::valueAtKey
    template<typename TKey>
    const Variant& Kv<TKey>::valueAtKey( const TKey& key ) const
    {
        static Variant invalid;
        typename Properties::const_iterator i = m_properties.find( key );
        return i != m_properties.end() ? i->second : invalid;
    }

    // ** Kv::has
    template<typename TKey>
    bool Kv<TKey>::has( const TKey& key ) const
    {
        return m_properties.count( key ) > 0;
    }

    //! Builds the Kv<String> instance with help of streaming operators.
    class KvBuilder {
    public:

        //! Alias the KeyValue type
        typedef Kv<String>  Result;

                            //! Converts the KvBuilder to underlying KeyValue.
                            operator const Result& ( void ) const { return m_result; }

                            //! Converts the KvBuilder to a Variant with a KeyValue inside.
                            operator Variant ( void ) const { return Variant::fromValue( m_result ); }

        //! Appends a new value to a dictionary
        template<typename TValue>
        KvBuilder&          operator << ( const TValue& value );

        //! Appends an array of values to a dictionary.
        template<typename TValue>
        KvBuilder&            operator << ( const Array<TValue>& values );

        //! Appends a new CString value to a dictionary.
        KvBuilder&          operator << ( CString value );

        //! Appends the KeyValue from a KvBuilder instance to a dictionary.
        KvBuilder&          operator << ( const KvBuilder& value );

    private:

        //! Appends a new Variant value with a specified key.
        void                appendKey( const String& key, const Variant& value );

    private:

        String              m_key;      //!< Active dictionary key.
        Result              m_result;   //!< Resulting key-value object.
    };

    // ** KvBuilder::operator <<
    template<typename TValue>
    KvBuilder& KvBuilder::operator << ( const TValue& value )
    {
        appendKey( m_key, Variant::fromValue<TValue>( value ) );
        return *this;
    }

    // ** KvBuilder::operator <<
    inline KvBuilder& KvBuilder::operator << ( CString value )
    {
        if( m_key != "" ) {
            appendKey( m_key, Variant::fromValue<String>( value ) );
        } else {
            m_key = value;
        }
        return *this;
    }

    // ** KvBuilder::operator <<
    inline KvBuilder& KvBuilder::operator << ( const KvBuilder& value )
    {
        appendKey( m_key, static_cast<Variant>( value ) );
        return *this;
    }

    // ** KvBuilder::appendKey
    inline void KvBuilder::appendKey( const String& key, const Variant& value )
    {
        NIMBLE_BREAK_IF( key == "" );
        m_result.setValueAtKey( key, value );
        m_key = "";
    }

NIMBLE_END

#endif  /*  !__Nimble_KeyValue_H__  */
