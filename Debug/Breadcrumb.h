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

#ifndef __Nimble_Breadcrumb_H__
#define __Nimble_Breadcrumb_H__

#include "../Globals.h"

NIMBLE_BEGIN

    //! Breadcrumb contains the latest system state.
    class Breadcrumb : public Singleton<Breadcrumb> {
    public:

        //! Alias the Kv type to store variables.
        typedef Kv<String>      Variables;

        //! All context data is stored inside this helper structure.
        struct Context {
                                //! Constructs the Context instance.
                                Context( const String& name = "" )
                                    : name( name ) {}
            String              name;   //!< Context name.
            Variables           vars;   //!< Context variables.
        };

        //! Pushes a new context onto the stack.
        void                    push( const String& name );

        //! Pops the context from a stack.
        void                    pop( void );

        //! Returns the total number of pushed contexts.
        s32                     contextCount( void ) const;

        //! Returns the context by index.
        const Context&          context( s32 index ) const;

        //! Returns breadcrumb global variables.
        const Variables&        globals( void ) const;

        //! Converts the breadcrumb to a string.
        String                  format( s32 ident ) const;

        //! Static method to set the global breadcrumb varible.
        template<typename TValue>
        static void             global( const String& name, const TValue& value );

        //! Static method to set the local (assigned to an active context) breadcrumb varible.
        template<typename TValue>
        static void             local( const String& name, const TValue& value );

    private:

        //! Sets the global breadcrumb variable.
        template<typename TValue>
        void                    setGlobalVariable( const String& name, const TValue& value );

        //! Sets the breadcrumb variable inside the current context (if there is no current contexts sets the global variable).
        template<typename TValue>
        void                    setLocalVariable( const String& name, const TValue& value );

        //! Formats variables with a specified ident.
        String                  formatVariables( s32 ident, const Variables& variables ) const;

    private:

        Variables               m_globals;  //!< Global variables.
        Array<Context>          m_stack;    //!< Stack of contexts.
    };

    // ** Breadcrumb::global
    template<typename TValue>
    void Breadcrumb::global( const String& name, const TValue& value )
    {
        instance().setGlobalVariable<TValue>( name, value );
    }

    // ** Breadcrumb::local
    template<typename TValue>
    void Breadcrumb::local( const String& name, const TValue& value )
    {
        instance().setLocalVariable<TValue>( name, value );
    }

    // ** Breadcrumb::setGlobalVariable
    template<typename TValue>
    void Breadcrumb::setGlobalVariable( const String& name, const TValue& value )
    {
        m_globals.setValueAtKey( name, Variant::fromValue( value ) );
    }

    // ** Breadcrumb::setLocalVariable
    template<typename TValue>
    void Breadcrumb::setLocalVariable( const String& name, const TValue& value )
    {
        Variables& vars = m_stack.empty() ? m_globals : m_stack.back().vars;
        vars.setValueAtKey( name, Variant::fromValue( value ) );
    }

    // ** Breadcrumb::globals
    inline const Breadcrumb::Variables& Breadcrumb::globals( void ) const
    {
        return m_globals;
    }

    // ** Breadcrumb::push
    inline void Breadcrumb::push( const String& name )
    {
        m_stack.push_back( Context( name ) );
    }

    // ** Breadcrumb::pop
    inline void Breadcrumb::pop( void )
    {
        NIMBLE_ABORT_IF( contextCount() == 0, "no contexts are pushed onto the stack" );
        m_stack.pop_back();
    }

    // ** Breadcrumb::contextCount
    inline s32 Breadcrumb::contextCount( void ) const
    {
        return static_cast<s32>( m_stack.size() );
    }

    // ** Breadcrumb::context
    inline const Breadcrumb::Context& Breadcrumb::context( s32 index ) const
    {
        NIMBLE_ABORT_IF( index < 0 || index >= contextCount(), "index is out of range" );
        return m_stack[index];
    }

    // ** Breadcrumb::formatVariables
    inline String Breadcrumb::formatVariables( s32 ident, const Variables& variables ) const
    {
        // Get the key-value properties
        const Variables::Properties& items = variables.properties();

        // If no variables - just return an empty string
        if( items.empty() ) {
            return "";
        }

        // Format all variables
        String formatted;
        s8     buffer[256];

        // Find the maximum length of a variable name
        s32 offset = 0;

        for( Variables::Properties::const_iterator i = items.begin(), end = items.end(); i != end; ++i ) {
            offset = max2<s32>( offset, i->first.length() );
        }

        // Format all variables
        for( Variables::Properties::const_iterator i = items.begin(), end = items.end(); i != end; ++i ) {
            _snprintf( buffer, sizeof( buffer ), "%*s%-*s = %s\n", ident, "", offset, i->first.c_str(), i->second.as<String>().c_str() );
            formatted += buffer;
        }
    
        return formatted;
    }

    // ** Breadcrumb::format
    inline String Breadcrumb::format( s32 ident ) const
    {
        // No contexts pushed - return an empty string
        if( m_stack.empty() ) {
            return "";
        }

        // Declare variables used for formatting
        String formatted = "";
        s8     buffer[256];

        // Format the breadcrumb header
        _snprintf( buffer, sizeof( buffer ), "\n%*sBREADCRUMB:\n", ident, "" );
        formatted += buffer;

        // Compose the global variables
        String variables = formatVariables( ident, m_globals );
        if( variables != "" ) {
            formatted += "\n" + variables;
        }

        // Compose the context stack string.
        for( s32 i = contextCount() - 1; i >= 0; i-- ) {
            const Context& ctx = context( i );
            _snprintf( buffer, sizeof( buffer ), "\n%*s[%2d] %s\n", ident, "", i, ctx.name.c_str() );

            formatted += buffer;

            // Format variables
            String variables = formatVariables( ident + 5, ctx.vars );
            if( variables != "" ) {
                formatted += (variables + "\n");
            }
        }

        return formatted;
    }

    //! A helper struct to mark the breadcrumb context scope.
    struct BreadcrumbScope {
        //! Pushes the context upon construction.
        BreadcrumbScope( const String& name ) { Breadcrumb::instance().push( name ); }

        //! Pops the context upon destruction.
        ~BreadcrumbScope( void ) { Breadcrumb::instance().pop(); }
    };

NIMBLE_END

//! Macro definition to push the breadcrumb context with a specified name
#define NIMBLE_BREADCRUMB( name ) ::NIMBLE_NS BreadcrumbScope __breadcrumbScope( name )

//! Macro definition to push the breadcrumb context with current function as name
#define NIMBLE_BREADCRUMB_F ::NIMBLE_NS BreadcrumbScope __breadcrumbScope( NIMBLE_PRETTY_FUNCTION )

#endif  /*  !__Nimble_Breadcrumb_H__    */