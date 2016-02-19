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

#ifndef __Nimble_StringList_H__
#define __Nimble_StringList_H__

#include "../Globals.h"

NIMBLE_BEGIN

    //! A simple container to store a list of strings, nothing more, nothing less.
    class StringList {
    public:

        //! Alias the string list container type.
        typedef List<String> Container;

        //! Appends new string to a list.
        StringList&         operator << ( const String& value );

        //! Appends new string to a list.
        StringList&         operator << ( CString value );

        //! Appends other string list to a list.
        StringList&         operator << ( const StringList& other );

        //! Returns the total number of strings in list.
        s32                 count( void ) const;

        //! Returns the last string in list.
        const String&       last( void ) const;

        //! Returns actual string list container.
        const Container&    container( void ) const;

        //! Pops the last string in list.
        String              pop( void );

        //! Returns true if the string list is empty.
        bool                isEmpty( void ) const;

        //! Returns true if a string list contains the specified string.
        bool                contains( const String& value ) const;

    private:

        Container           m_strings;  //!< Actual string container.
    };

    // ** StringList::operator <<
    inline StringList& StringList::operator << ( const String& value )
    {
        m_strings.push_back( value );
        return *this;
    }

    // ** StringList::operator <<
    inline StringList& StringList::operator << ( CString value )
    {
        m_strings.push_back( value );
        return *this;
    }

    // ** StringList::operator <<
    inline StringList& StringList::operator << ( const StringList& other )
    {
        m_strings.insert( m_strings.end(), other.m_strings.begin(), other.m_strings.end() );
        return *this;
    }

    // ** StringList::count
    inline s32 StringList::count( void ) const
    {
        return static_cast<s32>( m_strings.size() );
    }

    // ** StringList::isEmpty
    inline bool StringList::isEmpty( void ) const
    {
        return m_strings.empty();
    }

    // ** StringList::contains
    inline bool StringList::contains( const String& value ) const
    {
        return std::find( m_strings.begin(), m_strings.end(), value ) != m_strings.end();
    }

    // ** StringList::pop
    inline String StringList::pop( void )
    {
        NIMBLE_BREAK_IF( isEmpty() );
        String result = last();
        m_strings.pop_back();
        return result;
    }

    // ** StringList::last
    inline const String& StringList::last( void ) const
    {
        return m_strings.back();
    }

    // ** StringList::container
    inline const StringList::Container& StringList::container( void ) const
    {
        return m_strings;
    }

NIMBLE_END

#endif  /*  !__Nimble_StringList_H__    */