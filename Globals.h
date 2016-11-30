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

#ifndef __Nimble_Globals_H__
#define __Nimble_Globals_H__

#include "Config.h"
#include "Namespace.h"
#include "Types.h"

#ifdef NIMBLE_PLATFORM_MACOS
    #define _strdup strdup
    #define _snprintf snprintf
    #define FLT_MAX     __FLT_MAX__
    #define FLT_MIN     __FLT_MIN__
#endif  /*  #ifdef NIMBLE_PLATFORM_MACOS    */

NIMBLE_BEGIN

    namespace Internal {

        //! Outputs the message to a log (used by assertions).
        void message( int level, CString function, CString file, CString tag, CString prefix, CString format, ... );

    } // namespace Internal

    //! Returns random item from list
    template<typename T>
    const T& randomItem( const List<T>& items )
    {
        u32 idx = rand() % items.size();

        typename List<T>::const_iterator i = items.begin();

        while( idx-- ) {
            ++i;
        }

        return *i;
    }

    //! Returns random item from set
    template<typename T>
    const T& randomItem( const Set<T>& items )
    {
        u32 idx = rand() % items.size();

        typename Set<T>::const_iterator i = items.begin();

        while( idx-- ) {
            ++i;
        }

        return *i;
    }

    //! Returns random item from array
    template<typename T>
    const T& randomItem( const Array<T>& items )
    {
        return items[rand() % items.size()];
    }

    //! Forward declaration of lerp function
    template<typename TValue>
    TValue lerp( TValue a, TValue b, f32 scalar );

    //! Returns the random value in specified range
    template<typename TValue>
    TValue randomValue( const TValue& min, const TValue& max )
    {
        return lerp( min, max, rand() / f32( RAND_MAX ) );
    }

    //! Converts string to an upper case.
    inline String toUpperCase( const String& str )
    {
        String input = str;
        std::transform( input.begin(), input.end(), input.begin(), ::toupper );
        return input;
    }

    //! Converts string to a lower case.
    inline String toLowerCase( const String& str )
    {
        String input = str;
        std::transform( input.begin(), input.end(), input.begin(), ::tolower );
        return input;
    }

    //! Splits the string with a specified separator.
    inline Array<String> split( const String& input, CString delimeters )
    {
        Array<String> result;

        // Duplicate the string before processing
        s8* source = _strdup( input.c_str() );
        s8* line   = strtok( source, delimeters );

        // Tokenize the string
        while( line ) {
            result.push_back( line );
            line = strtok( NULL, delimeters );
        }

        // Cleanup temporary string buffer
        free( source );

        return result;
    }

    //! Converts value to a string.
    template< typename T>
    String toString( const T& n )
    {
        std::ostringstream stm;
        stm << n ;
        return stm.str();
    }

    //! Converts value to a bit string.
    template<typename TValue>
    String toBitString( TValue x )
    {
        String result = "";

        for( s32 i = sizeof( TValue ) * 8 - 1; i >= 0; i-- ) {
            bool set = x & (static_cast<TValue>( 1 ) << i) ? true : false;
            if( !set && result.empty() ) {
                continue;
            }

            result += set ? "1" : "0";
        }

        return result;
    }

    //! Trims zeroes from a right side of a bitset.
    template<typename TValue>
    TValue trimBits( TValue x )
    {
        while( (x & 1) == 0 ) {
            x = x >> 1;
        }

        return x;
    }

    //! Convert the byte value to a string.
    inline String toByteString( u8 value )
    {
        static const s8 digits[] = "0123456789abcdef";

        String result = "";

        result += digits[(value >> 4) & 0xf];
        result += digits[(value >> 0) & 0xf];

        return result;
    }

    const f32 Pi        = 3.1415926535897932f;
    const f32 Epsilon    = 0.0001f;

    //! Calculates the next power of two of a given number.
    inline u32 nextPowerOf2( u32 n )
    {
        u32 count = 0;

        /* First n in the below condition is for the case where n is 0*/
        if( n && !(n & (n - 1)) ) {
            return n;
        }

        while( n != 0 )
        {
            n >>= 1;
            count += 1;
        }
    
        return 1 << count;
    }

    //! Generates a random value in a [0, 1] range.
    inline f32 rand0to1( void ) {
        static f32 invRAND_MAX = 1.0f / RAND_MAX;
        return rand() * invRAND_MAX;
    }

    //! Does a linear interpolation between two values.
    template<typename TValue>
    TValue lerp( TValue a, TValue b, f32 scalar )
    {
        return static_cast<TValue>( a * (1.0f - scalar) + b * scalar );
    }

    //! Same as lerp but makes sure the values interpolate correctly when they wrap around 360 degrees.
    template<typename TValue>
    TValue lerpDegrees( TValue a, TValue b, f32 scalar )
    {
		if( abs( b - a ) > 180.0f )
		{
			if( b > a ) {
                a += 360.0f;
            } else {
                b += 360.0f;
            }
		}

		f32 value = lerp( a, b, scalar );

		while( value <   0.0f ) value += 360.0f;
		while( value > 360.0f ) value -= 360.0f;

		return value;
    }

    //! Returns true if three f32 values are equal.
    inline f32 equal3( f32 a, f32 b, f32 c, f32 eps = 0.001f )
    {
        return fabs( a - b ) <= eps && fabs( b - c ) <= eps && fabs( c - a ) <= eps;
    }

    //! Rounds the f32ing point number to a specified decimal place.
    inline f32 round( f32 a, u32 decimal )
    {
        f32 multipler = powf( 10.0f, ( f32 )decimal );
        return floor( a * multipler ) / multipler;
    }

    //! Returns a minimum value of two.
    template<typename TValue>
    TValue min2( const TValue& a, const TValue& b ) {
        return a < b ? a : b;
    }

    //! Returns a minimum value of three.
    template<typename TValue>
    TValue min3( const TValue& a, const TValue& b, const TValue& c ) {
        return min2( a, min2( b, c ) );
    }

    //! Returns a maximum value of two.
    template<typename TValue>
    TValue max2( const TValue& a, const TValue& b ) {
        return a > b ? a : b;
    }

    //! Returns a maximum value of three.
    template<typename TValue>
    TValue max3( const TValue& a, const TValue& b, const TValue& c ) {
        return max2( a, max2( b, c ) );
    }

    //! Clamps the value to a specified range.
    template<typename TValue>
    TValue clamp( const TValue& value, const TValue& min, const TValue& max ) {
        return min2( max2( value, min ), max );
    }

    //! Converts degrees to radians
    inline f32 radians( f32 degrees ) {
        return degrees / 180.0f * Pi;
    }

    //! Converts radians to degrees
    inline f32 degrees( f32 radians ) {
        return radians * 180.0f / Pi;
    }

    //! Returns true if an argument is not a number.
    inline bool isNaN( f32 value )
    {
    #ifdef WIN32
        return _isnan( value ) ? true : false;
    #else
        return isnan( value );
    #endif
    }

    //! Helper struct to hold a numeric range.
    struct Range {
                Range( f32 min = -FLT_MAX, f32 max = FLT_MAX )
                    : min( min ), max( max ) {}

        f32     min;    //! The minimum range value.
        f32     max;    //! The maximum range value.

        //! Clamps the input value to a range.
        f32        clamp( f32 value ) const { return :: NIMBLE_NS clamp( value, min, max ); }

        //! Returns true if the value is inside the range.
        bool    contains( f32 value ) const { return value >= min && value <= max; }
    };

NIMBLE_END

#endif  /*  !__Nimble_Globals_H__   */
