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

#ifndef __Nimble_DFA_H__
#define __Nimble_DFA_H__

#include "Globals.h"

NIMBLE_BEGIN

    /*! DFA is a finite-state machine that accepts and rejects strings of symbols
     and only produces a unique computation of the automaton for each input string.
     */
    template<typename TSymbol, typename TStateType = u8>
    class DFA
    {
    public:
        
        //! A DFA state alias.
        typedef DFA<TSymbol, TStateType> State;
        
        //! An input symbol type alias.
        typedef TSymbol Symbol;
        
        //! A state type alias.
        typedef TStateType StateType;
        
        //! A function pointer type used as transition function.
        typedef bool (*Condition)(Symbol input, Symbol reference);
        
        //! A transition edge with transition function and a reference value passed to it.
        struct Edge
        {
            Condition       condition;  //!< A function to be used to search for matching transition by an input symbol.
            Symbol          symbol;     //!< A reference symbol passed to a transition function.
            DFA*            next;       //!< A target state to switch to by this edge.
        };

                            //! Constructs a DFA state with a maximum number of outgoing edges and a state type.
                            DFA(LinearAllocator& allocator, StateType type = 0, StateType subtype = 0);
        
        //! Processes an input string and returns a terminal state.
        const DFA*          accept(const Symbol* input, s32& length);
        
        //! Returns a state type.
        StateType           type() const;
        
        //! Sets a state type.
        void                setType(StateType value);
        
        //! Returns a state subtype.
        StateType           subtype() const;
        
        //! Addds a new transition that matches a specified symbol and returns a target state instance.
        DFA*                addEdge(Symbol symbol, StateType type = 0, StateType subtype = 0);
        
        //! Adds a new transition specified by a function and returns a target state instance.
        DFA*                addEdge(Condition condition, StateType type = 0, StateType subtype = 0);
    
        //! Adds a loop edge that matches a specified symbol.
        void                addLoop(Symbol symbol);
        
        //! Adds a loop edge that specified by a function.
        void                addLoop(Condition condition);
        
        //! Adds a new transition to target state specified by a function.
        void                addEdge(Condition condition, DFA* target);

    private:
        
        //! Allocates a new state instance.
        DFA*                newState(StateType type, StateType subtype);
        
        //! Allocates a new edge instance.
        Edge*               newEdge(Symbol symbol = 0);
        
        //! Searches for an edge that has a specified reference symbol.
        const Edge*         findEdgeBySymbol(Symbol symbol) const;
        
        //! Searches for an edge that matches an input symbol.
        const Edge*         findEdge(Symbol input) const;
        
        //! A transition function that is true if an input symbol matches the reference value.
        static bool equals(Symbol c, Symbol reference);

    private:
        
        LinearAllocator&    m_allocator;    //!< A linear allocator to create edges and states.
        Array<Edge*>        m_edges;        //!< All outgoing edges are stored here.
        StateType           m_type;         //!< A state type.
        StateType           m_subtype;      //!< A state subtype.
    };

    // ** DFA::DFA
    template<typename TSymbol, typename TStateType>
    DFA<TSymbol, TStateType>::DFA(LinearAllocator& allocator, StateType type, StateType subtype)
        : m_allocator(allocator)
        , m_type(type)
        , m_subtype(subtype)
    {
    }

    // ** DFA::accept
    template<typename TSymbol, typename TStateType>
    const DFA<TSymbol, TStateType>* DFA<TSymbol, TStateType>::accept(const Symbol* input, s32& length)
    {
        const DFA*    state   = this;
        const Symbol* current = input;
        
        do
        {
            // Find matching edge
            const Edge* edge = state->findEdge(*current);
            
            // Nothing found - probably a terminal state
            if (edge == NULL)
            {
                break;
            }
            
            // Switch to a next state.
            state = edge->next;
            current++;
        } while (*current);
        
        // Calculate a total number of characters read from an input string.
        length = static_cast<s32>(current - input);
        
        return state;
    }

    // ** DFA::type
    template<typename TSymbol, typename TStateType>
    TStateType DFA<TSymbol, TStateType>::type() const
    {
        return m_type;
    }

    // ** DFA::setType
    template<typename TSymbol, typename TStateType>
    void DFA<TSymbol, TStateType>::setType(StateType value)
    {
        m_type = value;
    }

    // ** DFA::subtype
    template<typename TSymbol, typename TStateType>
    TStateType DFA<TSymbol, TStateType>::subtype() const
    {
        return m_subtype;
    }

    // ** DFA::addEdge
    template<typename TSymbol, typename TStateType>
    DFA<TSymbol, TStateType>* DFA<TSymbol, TStateType>::addEdge(Symbol symbol, StateType type, StateType subtype)
    {
        if (const Edge* edge = findEdgeBySymbol(symbol))
        {
            return edge->next;
        }
        
        Edge* edge = newEdge(symbol);
        edge->next = newState(type, subtype);
        m_edges.insert(m_edges.begin(), edge);
        
        return edge->next;
    }

    // ** DFA::addEdge
    template<typename TSymbol, typename TStateType>
    DFA<TSymbol, TStateType>* DFA<TSymbol, TStateType>::addEdge(Condition condition, StateType type, StateType subtype)
    {
        Edge* edge      = newEdge();
        edge->condition = condition;
        edge->next      = newState(type, subtype);
        m_edges.push_back(edge);
        
        return edge->next;
    }

    // ** DFA::addLoop
    template<typename TSymbol, typename TStateType>
    void DFA<TSymbol, TStateType>::addLoop(Symbol symbol)
    {
        Edge* edge = newEdge(symbol);
        edge->next = this;
        m_edges.push_back(edge);
    }

    // ** DFA::addLoop
    template<typename TSymbol, typename TStateType>
    void DFA<TSymbol, TStateType>::addLoop(Condition condition)
    {
        addEdge(condition, this);
    }

    // ** DFA::addEdge
    template<typename TSymbol, typename TStateType>
    void DFA<TSymbol, TStateType>::addEdge(Condition condition, DFA* target)
    {
        Edge* edge      = newEdge();
        edge->condition = condition;
        edge->next      = target;
        m_edges.push_back(edge);
    }

    // ** DFA::newEdge
    template<typename TSymbol, typename TStateType>
    typename DFA<TSymbol, TStateType>::Edge* DFA<TSymbol, TStateType>::newEdge(Symbol symbol)
    {
        // Allocate new edge
        u8* pointer = m_allocator.allocate(sizeof(Edge));
        NIMBLE_ABORT_IF(pointer == NULL, "out of memory");
        
        // Construct allocated edge
        Edge* edge      = new (pointer) Edge;
        edge->condition = &equals;
        edge->symbol    = symbol;
        
        return edge;
    }

    // ** DFA::newState
    template<typename TSymbol, typename TStateType>
    DFA<TSymbol, TStateType>* DFA<TSymbol, TStateType>::newState(StateType type, StateType subtype)
    {
        // Allocate new edge
        u8* pointer = m_allocator.allocate(sizeof(DFA));
        NIMBLE_ABORT_IF(pointer == NULL, "out of memory");
        
        // Construct allocated edge
        DFA* state = new (pointer) DFA(m_allocator, type, subtype);
        return state;
    }

    // ** DFA::findEdgeBySymbol
    template<typename TSymbol, typename TStateType>
    const typename DFA<TSymbol, TStateType>::Edge* DFA<TSymbol, TStateType>::findEdgeBySymbol(Symbol symbol) const
    {
        for (size_t i = 0, n = m_edges.size(); i < n; i++)
        {
            const Edge* edge = m_edges[i];
            
            if (symbol == edge->symbol)
            {
                return edge;
            }
        }
        
        return NULL;
    }

    // ** DFA::findEdge
    template<typename TSymbol, typename TStateType>
    const typename DFA<TSymbol, TStateType>::Edge* DFA<TSymbol, TStateType>::findEdge(Symbol input) const
    {
        for (size_t i = 0, n = m_edges.size(); i < n; i++)
        {
            const Edge* edge = m_edges[i];
            
            if (edge->condition(input, edge->symbol))
            {
                return edge;
            }
        }
        
        return NULL;
    }

    // ** DFA::equals
    template<typename TSymbol, typename TStateType>
    bool DFA<TSymbol, TStateType>::equals(Symbol c, Symbol reference)
    {
        return c == reference;
    }

NIMBLE_END

#endif  /*    !__Nimble_DFA_H__    */
