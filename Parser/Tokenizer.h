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

#ifndef __Nimble_Tokenizer_H__
#define __Nimble_Tokenizer_H__

#include "DFA.h"
#include "Token.h"

NIMBLE_BEGIN

    /*!
     A class to break an expression stream of text up into identifiers, numbers and operators.
     */
    class ExpressionTokenizer
    {
    public:
        
        //! An alias for DFA type.
        typedef DFA<s8, u8>     DFA;
        
        //! An alias for DFA transition function.
        typedef DFA::Condition  Condition;
        
                                //! Constructs an expression tokenizer instance.
                                ExpressionTokenizer(LinearAllocator& allocator);
        
        //! Sets number token identifier and subtypes for integral and decimal numbers.
        void                    setNumber(u8 value, u8 integral = 0, u8 decimal = 0);
        
        //! Set space token identifiers.
        void                    setWhitespace(u8 space, u8 tab);
        
        //! Sets new line token identifier.
        void                    setNewLine(u8 value);
        
        //! Sets an identifier token type.
        void                    setIdentifier(u8 value);

        //! Adds a new operator to a tokenizer.
        void                    addToken(const s8* sequence, u8 type);
        
        //! Adds a new keyword to a tokenizer.
        void                    addKeyword(const s8* sequence, u8 type);

        //! Adds an identifier token to tokenizer.
        void                    addIdentifier(Condition start, DFA::Condition middle, u8 type);
        
        //! Reads next token from an input stream.
        Token                   read(const s8* input);
        
    private:
        
        //! Returns true if an input symbol is a digit.
        static bool             digit(s8 symbol, s8 reference);
        
        //! Returns true if an input symbol is a digit or a letter.
        static bool             digitOrLetter(s8 c, s8 reference);
        
        //! Returns true if an input symbol is a letter.
        static bool             letter(s8 c, s8 reference);

    private:

        DFA                     m_dfa;          //!< A DFA to be used to read tokens.
        DFA::State*             m_identifier;   //!< An identifier state.
        u8                      m_newLine;      //!< A new line token type.
        int                     m_line;         //!< A current line number.
    };

    // ** ExpressionTokenizer::ExpressionTokenizer
    NIMBLE_INLINE ExpressionTokenizer::ExpressionTokenizer(LinearAllocator& allocator)
        : m_dfa(allocator)
        , m_line(1)
    {
    }

    // ** ExpressionTokenizer:setNumberToken
    NIMBLE_INLINE void ExpressionTokenizer::setNumber(u8 value, u8 integral, u8 decimal)
    {
        DFA::State* numberState = m_dfa.addEdge(digit, value, integral);
        numberState->addLoop(digit);
        
        DFA::State* decimalPoint = numberState->addEdge('.');
        DFA::State* decimalState = decimalPoint->addEdge(digit, value, decimal);
        decimalState->addLoop(digit);
    }

    // ** ExpressionTokenizer:setWhitespace
    NIMBLE_INLINE void ExpressionTokenizer::setWhitespace(u8 space, u8 tab)
    {
        m_dfa.addEdge(' ', space);
        m_dfa.addEdge('\t', tab);
    }

    // ** ExpressionTokenizer::setNewLine
    NIMBLE_INLINE void ExpressionTokenizer::setNewLine(u8 value)
    {
        m_newLine = value;
        
        DFA::State* newLineState = m_dfa.addEdge('\r');
        newLineState->addEdge('\n', value);
        m_dfa.addEdge('\n', value);
    }

    // ** ExpressionTokenizer::addToken
    NIMBLE_INLINE void ExpressionTokenizer::addToken(const s8* sequence, u8 type)
    {
        DFA::State* state = &m_dfa;
        
        for (const char* symbol = sequence; *symbol; symbol++)
        {
            state = state->addEdge(*symbol);
        }
        state->setType(type);
    }

    // ** ExpressionTokenizer::setIdentifier
    NIMBLE_INLINE void ExpressionTokenizer::setIdentifier(u8 value)
    {
        m_identifier = m_dfa.addEdge(letter, value);
        m_identifier->addLoop(digitOrLetter);
    }

    // ** ExpressionTokenizer::addKeyword
    NIMBLE_INLINE void ExpressionTokenizer::addKeyword(const s8* sequence, u8 type)
    {
        DFA::State* state = &m_dfa;
        
        for (const char* symbol = sequence; *symbol; symbol++)
        {
            state = state->addEdge(*symbol, m_identifier->type());
            state->addEdge(digitOrLetter, m_identifier);
        }
        state->setType(type);
    }

    // ** ExpressionTokenizer::read
    NIMBLE_INLINE Token ExpressionTokenizer::read(const s8* input)
    {
        s32 length;
        const DFA::State* state = m_dfa.accept(input, length);
        NIMBLE_ABORT_IF(state == NULL, "failed to read token");

        if (state->type() == m_newLine)
        {
            m_line++;
        }
        
        return Token(StringView(input, length), state->type(), state->subtype(), m_line);
    }

    // ** ExpressionTokenizer::digit
    NIMBLE_INLINE bool ExpressionTokenizer::digit(s8 symbol, s8 reference)
    {
        return symbol >= '0' && symbol <= '9';
    }
       
    // ** ExpressionTokenizer::digitOrLetter
    NIMBLE_INLINE bool ExpressionTokenizer::digitOrLetter(s8 c, s8 reference)
    {
        return digit(c, reference) || letter(c, reference);
    }
     
    // ** ExpressionTokenizer::letter
    NIMBLE_INLINE bool ExpressionTokenizer::letter(s8 c, s8 reference)
    {
        return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
    }

NIMBLE_END

#endif  /*  !__Nimble_Tokenizer_H__ */
