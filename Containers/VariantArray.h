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

#ifndef __Nimble_VariantArray_H__
#define __Nimble_VariantArray_H__

#include "../Globals.h"

NIMBLE_BEGIN

    //! Wraps the Array<Variant> type and adds the stream operator. 
    class VariantArray {
    public:

        //! Alias the container type.
        typedef Array<Variant>  Container;

                                //! Converts the VariantArray to a underlying container type.
                                operator const Container& ( void ) const { return m_array; }

        //! Appends a new value of specified type as variant to an array.
        template<typename TValue>
        VariantArray&           operator << ( const TValue& value );

        //! Appends an array of values as variants to an array.
        template<typename TValue>
        VariantArray&            operator << ( const Array<TValue>& values );

        //! Appends a new string value to variant array.
        VariantArray&           operator << ( CString value );

        //! Appends a new vartiant value to an array.
        VariantArray&           operator << ( const Variant& value );

        //! Appends a new KeyValue from a KvBuilder instance to an array.
        VariantArray&           operator << ( const KvBuilder& value );

    private:

        Container               m_array;    //!< underlying variant container.
    };

    // ** VariantArray::operator <<
    template<typename TValue>
    VariantArray& VariantArray::operator << ( const TValue& value )
    {
        m_array.push_back( Variant::fromValue<TValue>( value ) );
        return *this;
    }

    // ** KvBuilder::operator <<
    template<typename TValue>
    VariantArray& VariantArray::operator << ( const Array<TValue>& value )
    {
        VariantArray items;

        for( u32 i = 0, n = ( u32 )value.size(); i < n; i++ ) {
            items << value[i];
        }

        return *this << items;
    }

    // ** VariantArray::operator <<
    inline VariantArray& VariantArray::operator << ( CString value )
    {
        m_array.push_back( Variant::fromValue<String>( value ) );
        return *this;
    }

    // ** VariantArray::operator <<
    inline VariantArray& VariantArray::operator << ( const Variant& value )
    {
        m_array.push_back( value );
        return *this;
    }

    // ** VariantArray::operator <<
    inline VariantArray& VariantArray::operator <<  ( const KvBuilder& value )
    {
        m_array.push_back( static_cast<Variant>( value ) );
        return *this;
    }

    // ** operator >>
    template<typename T>
    void operator >> ( const Array<T>& value, Variant& variant )
    {
        VariantArray array;
        
        for( s32 i = 0, n = static_cast<s32>( value.size() ); i < n; i++ ) {
            Variant item;
            value[i] >> item;
            array << item;
        }
        
        variant = Variant::fromValue<VariantArray>( array );
    }

    // ** operator <<
    template<typename T>
    void operator << ( Array<T>& value, const Variant& variant )
    {
        if( !variant.type()->is<VariantArray>() ) {
            return;
        }
        
        const VariantArray::Container& items = variant.expect<VariantArray>();
        
        for( s32 i = 0, n = static_cast<s32>( items.size() ); i < n; i++ ) {
            value.push_back( items[i].as<T>() );
        }
    }

    // ** KvBuilder::operator <<
    template<typename TValue>
    KvBuilder& KvBuilder::operator << ( const Array<TValue>& value )
    {
        VariantArray items;
        
        for( u32 i = 0, n = ( u32 )value.size(); i < n; i++ ) {
            items << value[i];
        }
        
        return *this << items;
    }

NIMBLE_END

#endif  /*  !__Nimble_VariantArray_H__    */
