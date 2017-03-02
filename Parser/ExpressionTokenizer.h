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

#ifndef __Nimble_ExpressionTokenizer_H__
#define __Nimble_ExpressionTokenizer_H__

#include "DFA.h"
#include "Token.h"

NIMBLE_BEGIN

    /*!
     A class to break an expression stream of text up into identifiers, numbers and operators.
     */
    class ExpressionTokenizer
    {
    public:
        
        //! Token types produced by expression tokenizer.
        enum TokenType
        {
              TokenNewLine = 1  //!< A new line token.
            , TokenSpace        //!< A whitespace token.
            , TokenTab          //!< A tab token.
            , TokenKeyword      //!< A keyword token.
            , TokenPunctuation  //!< A punctuation token.
            , TokenOperator     //!< An operator token.
            , TokenIdentifier   //!< An identifier token.
            , TokenNumber       //!< A number token.
            , TokenEOF          //!< The end of file token.
            , TotalTokenTypes   //!< A total number of tokens.
        };
        
        //! Number token subtypes.
        enum NumberTokenSubtype
        {
              NumberInteger = 1 //!< An integer constant in form 1, 2, 3.
            , NumberFloat       //!< A floating point constant in form 1.0f
            , NumberDouble      //!< A double value in form 1.0 or scientific form.
        };
        
        //! An alias for DFA type.
        typedef DFA<s8, u8>     DFA;
        
        //! An alias for DFA transition function.
        typedef DFA::Condition  Condition;
        
                                //! Constructs an expression tokenizer instance.
                                ExpressionTokenizer(LinearAllocator& allocator);
    

        //! Adds a new operator to a tokenizer.
        void                    addOperator(const s8* sequence, u8 type);
        
        //! Adds a new keyword to a tokenizer.
        void                    addKeyword(const s8* sequence, u8 type, u8 subtype = TokenKeyword);
        
        //! Adds a new punctuation symbol to a tokenizer.
        void                    addPunctuation(const s8* sequence, u8 type);

        //! Reads next token from an input stream.
        Token                   read(const s8* input);
        
    private:
        
        //! Returns true if an input symbol is a digit.
        static bool             digit(s8 symbol, s8 reference);
        
        //! Returns true if an input symbol is a digit or a letter.
        static bool             digitOrLetter(s8 c, s8 reference);
        
        //! Returns true if an input symbol is a letter.
        static bool             letter(s8 c, s8 reference);
        
        //! Adds an exponential number format.
        void                    addExponentialNumbers(DFA::State* initial, s8 suffix);

    private:

        DFA                     m_dfa;          //!< A DFA to be used to read tokens.
        DFA::State*             m_identifier;   //!< An identifier state.
        s32                     m_line;         //!< A current line number.
        u16                     m_column;       //!< A current column number.
    };

    // ** ExpressionTokenizer::ExpressionTokenizer
    NIMBLE_INLINE ExpressionTokenizer::ExpressionTokenizer(LinearAllocator& allocator)
        : m_dfa(allocator)
        , m_line(1)
        , m_column(1)
    {
        // Construct whitespace and tabs token states
        m_dfa.addEdge(' ', TokenSpace);
        m_dfa.addEdge('\t', TokenTab);
        
        // Add new line token
        DFA::State* newLine = m_dfa.addEdge('\r');
        newLine->addEdge('\n', TokenNewLine);
        m_dfa.addEdge('\n', TokenNewLine);
        
        // Construct a number token states
        DFA::State* number = m_dfa.addEdge(digit, TokenNumber, NumberInteger);
        number->addLoop(digit);
        
        DFA::State* decimalPoint = number->addEdge('.');
        DFA::State* decimal = decimalPoint->addEdge(digit, TokenNumber, NumberDouble);
        decimal->addLoop(digit);
        
        DFA::State* suffix = decimal->addEdge(letter, TokenNumber, NumberFloat);
        suffix->addLoop(letter);
        
        addExponentialNumbers(decimal, 'e');
        addExponentialNumbers(decimal, 'E');

        // Add identifiers
        m_identifier = m_dfa.addEdge(letter, TokenIdentifier);
        m_identifier->addLoop(digitOrLetter);
    }

    NIMBLE_INLINE void ExpressionTokenizer::addExponentialNumbers(DFA::State* initial, s8 suffix)
    {
        DFA::State* exponential = initial->addEdge(suffix);
        
        DFA::State* plus = exponential->addEdge('+');
        plus->addEdge(digit, TokenNumber, NumberDouble)->addLoop(digit);
        
        DFA::State* minus = exponential->addEdge('-');
        minus->addEdge(digit, TokenNumber, NumberDouble)->addLoop(digit);
    }

    // ** ExpressionTokenizer::addOperator
    NIMBLE_INLINE void ExpressionTokenizer::addOperator(const s8* sequence, u8 type)
    {
        DFA::State* state = &m_dfa;
        
        for (const char* symbol = sequence; *symbol; symbol++)
        {
            state = state->addEdge(*symbol);
        }
        state->setType(TokenOperator);
        state->setSubtype(type);
    }

    // ** ExpressionTokenizer::addKeyword
    NIMBLE_INLINE void ExpressionTokenizer::addKeyword(const s8* sequence, u8 type, u8 subtype)
    {
        DFA::State* state = &m_dfa;
        
        for (const char* symbol = sequence; *symbol; symbol++)
        {
            state = state->addEdge(*symbol, m_identifier->type());
            state->addEdge(digitOrLetter, m_identifier);
        }
        state->setType(type);
        state->setSubtype(subtype);
    }

    // ** ExpressionTokenizer::addPunctuation
    NIMBLE_INLINE void ExpressionTokenizer::addPunctuation(const s8* sequence, u8 type)
    {
        DFA::State* state = &m_dfa;
        
        for (const char* symbol = sequence; *symbol; symbol++)
        {
            state = state->addEdge(*symbol);
        }
        state->setType(type);
        state->setSubtype(TokenPunctuation);
    }

    // ** ExpressionTokenizer::read
    NIMBLE_INLINE Token ExpressionTokenizer::read(const s8* input)
    {
        if (*input == 0)
        {
            return Token(StringView(input, 0), TokenEOF, 0, m_line, m_column);
        }
        
        s32 length;
        s32 column = m_column;
        s32 line = m_line;
        const DFA::State* state = m_dfa.accept(input, length);
        NIMBLE_ABORT_IF(state == NULL, "failed to read token");

        m_column += length;
        
        if (state->type() == TokenNewLine)
        {
            m_line++;
            m_column = 1;
        }
        
        return Token(StringView(input, length), state->type(), state->subtype(), line, column);
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

#endif  /*  !__Nimble_ExpressionTokenizer_H__ */
