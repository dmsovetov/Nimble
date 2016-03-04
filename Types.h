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

#ifndef __Nimble_Types_H__
#define __Nimble_Types_H__

#include "Namespace.h"

#include <string>
#include <vector>
#include <set>
#include <map>
#include <list>
#include <stack>
#include <queue>
#include <algorithm>
#include <sstream>
#include <functional>

#if NIMBLE_CPP11_ENABLED
    #include <unordered_map>
#endif  /*  NIMBLE_CPP11_ENABLED    */

#include <time.h>
#include <stdarg.h>

//! A shortcut for a static assert of a type size.
#define NIMBLE_EXPECTED_TYPE_SIZE( type, size ) \
            NIMBLE_STATIC_ASSERT( sizeof( type ) == size, "Unexpected " #type " type size, expected " #size )

NIMBLE_BEGIN

	template<typename T>
	class Array : public std::vector<T> {
	public:

		//! Constructs an empty Array instance.
		Array( void ) {}

		//! Constructs an Array instance with a single item inside.
		Array( const T& value ) { this->push_back( value ); }

		//! Appends a new value to an array.
		Array& operator << ( const T& value ) { this->push_back( value ); return *this; }
	};

	template<typename T>
	class Stack : public std::stack<T> {};

	template<typename T>
	class List : public std::list<T> {};

    template< typename T, typename Compare = std::less<T> >
    class Set : public std::set<T, Compare> {};

    template<typename K, typename V, typename P = std::less<K> >
    class Map : public std::map<K, V, P> {};

	struct Void { template<typename T> void read( const T& ) {} };

    typedef double                  f64;
    typedef float                   f32;
    typedef char                    s8;
    typedef signed long long        s64;
    typedef signed int              s32;
    typedef unsigned long long      u64;
    typedef unsigned int            u32;
    typedef short                   s16;
    typedef unsigned short          u16;
    typedef unsigned char           u8;
    typedef const char*             CString;
    typedef std::string             String;
    typedef Array<String>			StringArray;
	typedef Array<class Guid>		GuidArray;
	typedef Array<u8>				ByteArray;

    NIMBLE_EXPECTED_TYPE_SIZE( f32, 4 );
    NIMBLE_EXPECTED_TYPE_SIZE( f64, 8 );
    NIMBLE_EXPECTED_TYPE_SIZE( u8, 1 );
    NIMBLE_EXPECTED_TYPE_SIZE( s8, 1 );
    NIMBLE_EXPECTED_TYPE_SIZE( u16, 2 );
    NIMBLE_EXPECTED_TYPE_SIZE( s16, 2 );
    NIMBLE_EXPECTED_TYPE_SIZE( u32, 4 );
    NIMBLE_EXPECTED_TYPE_SIZE( s32, 4 );
    NIMBLE_EXPECTED_TYPE_SIZE( u64, 8 );
    NIMBLE_EXPECTED_TYPE_SIZE( s64, 8 );

NIMBLE_END

#endif  /*  !__Nimble_Namespace_H__   */
