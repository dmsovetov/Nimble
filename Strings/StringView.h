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

#ifndef __Nimble_StringView_H__
#define __Nimble_StringView_H__

#include "../Globals.h"

NIMBLE_BEGIN

    //! A string view wraps a pointer to a C-string and it's length.
    class StringView
    {
    public:
        
                            //! Constructs an empty StringView instance.
                            StringView();
        
                            //! Constructs a StringView from a pointer and length.
                            StringView(const s8* value, s32 length = 0);
        
                            //! Constructs a FixedString instance from another one.
                            StringView(const StringView& other);
        
                            //! Returns stored C string.
                            operator const s8* () const;
        
        //! Copies a string view from another one.
        const StringView&   operator = (const StringView& other);
        
        //! Tests two string views for an equality.
        bool                operator == (const StringView& other) const;
        
        //! Tests two string views for an equality.
        bool                operator == (const s8* text) const;
        
        //! Compares two string views.
        bool                operator < (const StringView& other) const;
        
        //! Returns a string length.
        s32                 length() const;
        
        //! Returns a string value.
        const s8*           value() const;

        //! Returns a string object.
        String              str() const;
        
    private:
        
        const s8*           m_value;    //!< Actual string value.
        s32                 m_length;   //!< A string length.
    };

    // ** StringView::StringView
    NIMBLE_INLINE StringView::StringView()
        : m_value( NULL )
        , m_length( 0 )
    {
    }

    // ** StringView::StringView
    NIMBLE_INLINE StringView::StringView(const s8* value, s32 length)
        : m_value( value )
        , m_length( 0 )
    {
        if( m_value == NULL )
        {
            return;
        }
        
        m_length = static_cast<s32>(length ? length : strlen(m_value));
    }

    // ** StringView::StringView
    NIMBLE_INLINE StringView::StringView(const StringView& other)
        : m_value(other.m_value)
        , m_length(other.m_length)
    {
    }

    // ** StringView::operator const s8*
    NIMBLE_INLINE StringView::operator const s8* () const
    {
        return m_value;
    }

    // ** StringView::operator =
    NIMBLE_INLINE const StringView& StringView::operator = (const StringView& other)
    {
        m_value  = other.m_value;
        m_length = other.m_length;
        return *this;
    }

    // ** StringView::operator ==
    NIMBLE_INLINE bool StringView::operator == (const StringView& other) const
    {
        if(m_length != other.m_length)
        {
            return false;
        }

        return strncmp(m_value, other.m_value, m_length) == 0;
    }

    // ** StringView::operator ==
    NIMBLE_INLINE bool StringView::operator == (const s8* text) const
    {
        return strncmp(m_value, text, m_length) == 0;
    }

    // ** StringView::operator <
    NIMBLE_INLINE bool StringView::operator < (const StringView& other) const
    {
        if(m_length != other.m_length)
        {
            return m_length < other.m_length;
        }
        
        return strncmp(m_value, other.m_value, m_length) < 0;
    }

    // ** StringView::length
    NIMBLE_INLINE s32 StringView::length() const
    {
        return m_length;
    }

    // ** StringView::value
    NIMBLE_INLINE const s8* StringView::value() const
    {
        return m_value;
    }

    // ** StringView::str
    NIMBLE_INLINE String StringView::str() const
    {
        String result = "";

        for (s32 i = 0; i < m_length; i++)
        {
            result += m_value[i];
        }

        return result;
    }

NIMBLE_END

#endif  /*    !__Nimble_StringView_H__    */
