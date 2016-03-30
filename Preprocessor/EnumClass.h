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

#ifndef __Nimble_Preprocessor_EnumClass_H__
#define __Nimble_Preprocessor_EnumClass_H__

#define _NIMBLE_ENUM_TO_STRING( name )   case name: return NIMBLE_STRINGIFY( name );
#define _NIMBLE_ENUM_FROM_INT( name )    case name: return name;
#define _NIMBLE_ENUM_FROM_STRING( name ) if( strcmp( value, #name ) == 0 ) return name;
#define _NIMBLE_EMUM_VALUE( name )       values.push_back( name );

//! Macro to declare an enumeration class.
#define NIMBLE_DECLARE_ENUM( name, ... )                                                \
            struct name {                                                               \
                template<class T> friend class IsEnumClass;                             \
                enum Value {                                                            \
                    __VA_ARGS__,                                                        \
                    Total = NIMBLE_VA_NUM_ARGS(__VA_ARGS__)                             \
                };                                                                      \
                typedef Array<Value> Values;                                            \
                name( void ) : value( Total ) {}                                        \
                name( Value v ) : value( v ) {}                                         \
                name( const name& other ) : value( other.value ) {}                     \
                const name& operator = ( const name& other ) { value = other.value; }   \
                const name& operator = ( Value v ) { value = v; }                       \
                String toString( void ) const { return toString( value ); }             \
                static CString toString( Value value ) {                                \
                    switch( value ) {                                                   \
                    NIMBLE_EXPAND_ARGS( _NIMBLE_ENUM_TO_STRING, __VA_ARGS__ )           \
                    default: NIMBLE_NO_DEFAULT;                                         \
                    }                                                                   \
                    return "Total";                                                     \
                }                                                                       \
                static Value fromString( CString value ) {                              \
                    NIMBLE_EXPAND_ARGS( _NIMBLE_ENUM_FROM_STRING, __VA_ARGS__ )         \
                    return Total;                                                       \
                }                                                                       \
                static Values values( void ) {                                          \
                    Values values;                                                      \
                    NIMBLE_EXPAND_ARGS( _NIMBLE_EMUM_VALUE, __VA_ARGS__ )               \
                    return values;                                                      \
                }                                                                       \
                static Value valueAt( s32 index ) {                                     \
                    switch( index ) {                                                   \
                    NIMBLE_EXPAND_ARGS( _NIMBLE_ENUM_FROM_INT, __VA_ARGS__ )            \
                    default: NIMBLE_NO_DEFAULT;                                         \
                    }                                                                   \
                    return Total;                                                       \
                }                                                                       \
                static CString stringAt( s32 index ) {                                  \
                    return toString( valueAt( index ) );                                \
                }                                                                       \
            private:                                                                    \
                enum            __enumClassIndicator {};                                \
                Value           value;                                                  \
            };

#endif  /*  !__Nimble_Preprocessor_EnumClass_H__    */