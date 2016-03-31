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

#ifndef __Nimble_TypeName_H__
#define __Nimble_TypeName_H__

#include "../Globals.h"

NIMBLE_BEGIN

    //! Data type for type id.
    typedef u32 TypeId;

    namespace Detail {

        //! Parses the type name from a preprocessor-generated string.
        class TypeNameParser {
        public:

            //! Returns a qualified class name (with namespace).
            static String qualifiedName( CString str )
            {
            #if __GNUC__
                String name = str;

		        // ** Parse a GCC __PRETTY_FUNCTION__
		        u32 eq = name.find_first_of( "=" );

		        if( eq != String::npos ) {
			        eq += 2;
			        u32 end = name.find_first_of( ";", eq );
			        return name.substr( eq, end - eq );
		        }
            #else
		        String name = str;
            #endif  /*  __GNUC__    */

		        // ** Get the template argument
		        u32 start = name.find_first_of( "<" );
		        u32 end   = name.find_first_of( ">" );
		        name = name.substr( start + 1, end - start - 1 ); 

		        // ** Remove the 'struct ' from the beginning.
		        start = name.find_first_of( " " );
		        if( start != String::npos ) {
			        name = name.substr( start + 1 );
		        }

		        return name;
            }

            //! Returns just a name of a type without a namespace.
            static String name( CString str )
            {
		        String name = qualifiedName( str );

		        u32 start = name.find_last_of( ":" );
		        if( start != String::npos ) {
			        name = name.substr( start + 1 );
		        }

		        return name;
            }
        };

    } // namespace detail

    //! TypeInfo class helps to generate a universally unique id & extract a type name.
    template<typename T>
    class TypeInfo {
    public:

	    //! Returns a qualified class name (with namespace).
	    static CString qualifiedName( void )
	    {
		    static String value = Detail::TypeNameParser::qualifiedName( NIMBLE_PRETTY_FUNCTION );
		    return value.c_str();
	    }

	    //! Returns a class name without a namespace.
	    static CString name( void )
	    {
		    static String value = Detail::TypeNameParser::name( NIMBLE_PRETTY_FUNCTION );
		    return value.c_str();
	    }

	    //! Returns a unique class id.
	    static TypeId id( void )
	    {
		    static String32 value = String32( name() );
		    return value;
	    }
    };

NIMBLE_END

#endif  /*  !__Nimble_TypeName_H__  */