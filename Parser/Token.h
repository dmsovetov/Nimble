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

#ifndef __Nimble_Token_H__
#define __Nimble_Token_H__

NIMBLE_BEGIN

    /*!
     A single token parsed from an input stream.
     Token contains a pointer to it's text, length and a line number.
     */
    class Token
    {
    public:
        
                                //! Constructs an invalid Token instance.
                                Token();
        
                                //! Constructs a Token instance.
                                Token(const StringView& text, u8 type, u8 subtype, s32 line, u16 column);
        
                                //! Returns true if the token is valid (has valid type or subtype).
                                operator bool() const;
        
        //! Returns true if token type matches the specified one.
        bool                    operator == (u8 type) const;
        
        //! Returns true if token type does not match the specified one.
        bool                    operator != (u8 type) const;

        //! Returns true if token text matches the specified one.
        bool                    operator == (const s8* text) const;
        
        //! Returns token text.
        const StringView&       text() const;
        
        //! Returns a token text string.
        String                  str() const;
        
        //! Returns token length.
        s32                     length() const;
        
        //! Returns token line.
        s32                     line() const;
        
        //! Returns token column number.
        s32                     column() const;
        
        //! Returns token type.
        u8                      type() const;
        
        //! Returns token subtype.
        u8                      subtype() const;
        
    private:
        
        StringView              m_text;     //!< A token text.
        u8                      m_type;     //!< A token type.
        u8                      m_subtype;  //!< A token subtype.
        s32                     m_line;     //!< A line number.
        u16                     m_column;   //!< A token column number.
    };

    // ** Token::Token
    NIMBLE_INLINE Token::Token()
        : m_type(0)
        , m_subtype(0)
        , m_line(0)
        , m_column(0)
    {
    }

    // ** Token::Token
    NIMBLE_INLINE Token::Token(const StringView& text, u8 type, u8 subtype, s32 line, u16 column)
        : m_text(text)
        , m_type(type)
        , m_subtype(subtype)
        , m_line(line)
        , m_column(column)
    {
        
    }

    // ** Token::operator bool
    NIMBLE_INLINE Token::operator bool () const
    {
        return m_type || m_subtype;
    }

    // ** Token::operator ==
    NIMBLE_INLINE bool Token::operator == (u8 type) const
    {
        return m_type == type;
    }

    // ** Token::operator !=
    NIMBLE_INLINE bool Token::operator != (u8 type) const
    {
        return m_type != type;
    }

    // ** Token::operator ==
    NIMBLE_INLINE bool Token::operator == (const s8* text) const
    {
        return m_text == text;
    }

    // ** Token::text
    NIMBLE_INLINE const StringView& Token::text() const
    {
        return m_text;
    }

    // ** Token::str
    NIMBLE_INLINE String Token::str() const
    {
        String result;
        const s8* text = m_text;
        
        for (s32 i = 0; i < length(); i++)
        {
            result += text[i];
        }
        
        return result;
    }

    // ** Token::length
    NIMBLE_INLINE s32 Token::length() const
    {
        return m_text.length();
    }

    // ** Token::line
    NIMBLE_INLINE s32 Token::line() const
    {
        return m_line;
    }

    // ** Token::column
    NIMBLE_INLINE s32 Token::column() const
    {
        return m_column;
    }

    // ** Token::type
    NIMBLE_INLINE u8 Token::type() const
    {
        return m_type;
    }

    // ** Token::subtype
    NIMBLE_INLINE u8 Token::subtype() const
    {
        return m_subtype;
    }

NIMBLE_END

#endif  /*  !__Nimble_Token_H__ */
