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

#ifndef __Nimble_ExpressionParser_H__
#define __Nimble_ExpressionParser_H__

#include "ExpressionTokenizer.h"

NIMBLE_BEGIN

    //! Recursive descent parser used to process complex expressions.
    class ExpressionParser
    {
    public:
        
                                //! Constructs an ExpressionParser
                                ExpressionParser(ExpressionTokenizer& tokenizer, s32 lookAhead = 0);
        
    protected:
        
        //! An index of a first user-defined token.
        enum { UserDefinedToken = ExpressionTokenizer::TotalTokenTypes };
        
        //! Returns true if current token is not EOF.
        bool                    hasTokens() const;
        
        //! Prepares an input stream for reading.
        void                    prepare(const s8* input);
        
        //! Returns a next token from a stream at specified index.
        const Token&            look(u32 index) const;
        
        //! Checks if the current token matches the specified one and if so goes to a next one and returns true.
        bool                    parse(u8 type);
        
        //! Returns true if the current token matches the specified one.
        bool                    check(u8 type, s32 lookAhead = 0) const;
        
        //! Expects to read from stream a specified token.
        void                    expect(u8 type);
        
        //! Reads the next token from an input stream.
        const Token&            next();
        
        //! Returns a current token.
        const Token&            current() const;
        
        //! Returns a previous token.
        const Token&            previous() const;
        
        //! Reads a next token a returns the one that was current before this call.
        const Token&            read();
        
    private:
        
        //! Reads a next token from an input.
        void                    read(Token& token);
        
    private:
        
        ExpressionTokenizer&    m_tokenizer;    //!< An expression tokenizer.
        Array<Token>            m_stream;       //!< An input token stream.
        Token                   m_previous;     //!< The previous token that was read.
        const s8*               m_input;        //!< An input text.
    };

    // ** ExpressionParser::ExpressionParser
    NIMBLE_INLINE ExpressionParser::ExpressionParser(ExpressionTokenizer& tokenizer, s32 lookAhead)
        : m_tokenizer(tokenizer)
        , m_input(NULL)
    {
        m_stream.resize(lookAhead + 1);
    }

    // ** ExpressionParser::hasTokens
    NIMBLE_INLINE bool ExpressionParser::hasTokens() const
    {
        return current().type() != ExpressionTokenizer::TokenEOF;
    }

    // ** ExpressionParser::prepare
    NIMBLE_INLINE void ExpressionParser::prepare(const s8* input)
    {
        m_input = input;
        
        for (size_t i = 0, n = m_stream.size(); i < n; i++)
        {
            read(m_stream[i]);
        }
    }

    // ** ExpressionParser::look
    NIMBLE_INLINE const Token& ExpressionParser::look(u32 index) const
    {
        NIMBLE_ABORT_IF(index > m_stream.size(), "index is out of range");
        return m_stream[index];
    }

    // ** ExpressionParser::parse
    NIMBLE_INLINE bool ExpressionParser::parse(u8 type)
    {
        if (check(type))
        {
            next();
            return true;
        }
        
        return false;
    }

    // ** ExpressionParser::check
    NIMBLE_INLINE bool ExpressionParser::check(u8 type, s32 lookAhead) const
    {
        return look(lookAhead) == type;
    }

    // ** ExpressionParser::next
    NIMBLE_INLINE const Token& ExpressionParser::next()
    {
        size_t n = m_stream.size();
        
        // Save previous token
        m_previous = current();
        
        // Shift token stream
        for (size_t i = 0; i < n - 1; i++)
        {
            m_stream[i] = m_stream[i + 1];
        }
        
        // Read the next one
        read(m_stream[n - 1]);

        return current();
    }

    // ** ExpressionParser::current
    NIMBLE_INLINE const Token& ExpressionParser::current() const
    {
        NIMBLE_ABORT_IF(m_stream.empty(), "input stream is empty");
        return look(0);
    }

    // ** ExpressionParser::previous
    NIMBLE_INLINE const Token& ExpressionParser::previous() const
    {
        return m_previous;
    }

    // **  ExpressionParser::read
    NIMBLE_INLINE const Token& ExpressionParser::read()
    {
        next();
        return previous();
    }

    // **  ExpressionParser::read
    NIMBLE_INLINE void ExpressionParser::read(Token& token)
    {
        do
        {
            token = m_tokenizer.read(m_input);
            m_input += token.length();
        } while (   token.type() == ExpressionTokenizer::TokenSpace
                 || token.type() == ExpressionTokenizer::TokenTab
                 || token.type() == ExpressionTokenizer::TokenNewLine);
    }

NIMBLE_END

#endif  /*  !__Nimble_ExpressionParser_H__ */
