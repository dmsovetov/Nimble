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

#ifndef __Nimble_Hash_H__
#define __Nimble_Hash_H__

#include "Globals.h"

NIMBLE_BEGIN

    namespace HashFunction {
    
        /// MurmurHash2, by Austin Appleby
        ///
        /// @note
        /// This code makes a few assumptions about how your machine behaves
        ///
        /// 1. We can read a 4-byte value from any address without crashing
        /// 2. sizeof(int) == 4
        ///
        /// And it has a few limitations -
        ///
        /// 1. It will not work incrementally.
        /// 2. It will not produce the same results on little-endian and big-endian
        ///    machines.
        ///
        /// @note
        /// Borrowed here: https://github.com/taylor001/crown/blob/master/src/core/murmur.cpp

        //! Calculates the murmur hash value from data of specified length.
        template<typename T>
        T murmur( const void* input, u32 length, T seed = 0 )
        {
            NIMBLE_STATIC_ASSERT( false, "Hash::murmur is not implemented for this type" );
            NIMBLE_NOT_IMPLEMENTED;
            return T();
        }

        //! Calculates the 32-bit murmur hash value.
        template<s32>
        u32 murmur( const void* input, u32 length, s32 seed = 0 )
        {
            // 'm' and 'r' are mixing constants generated offline.
            // They're not really 'magic', they just happen to work well.
            const u32 m = 0x5bd1e995;
            const s32 r = 24;

            // Initialize the hash to a 'random' value
            u32 h = seed ^ len;

            // Mix 4 bytes at a time into the hash
            const u8* data = reinterpret_cast<const u8*>( key );

            while( len >= 4 ) {
                u32 k = *reinterpret_cast<u32*>( data );

                k *= m;
                k ^= k >> r;
                k *= m;

                h *= m;
                h ^= k;

                data += 4;
                len -= 4;
            }

            // Handle the last few bytes of the input array
            switch( len ) {
            case 3: h ^= data[2] << 16;
            case 2: h ^= data[1] << 8;
            case 1: h ^= data[0];
                    h *= m;
            };

            // Do a few final mixes of the hash to ensure the last few
            // bytes are well-incorporated.
            h ^= h >> 13;
            h *= m;
            h ^= h >> 15;

            return h;
        }

        //! Calculates the 64-bit murmur hash value.
        template<u64>
        u64 murmur( const void* input, u32 length, u64 seed = 0 )
        {
            const u64 m = 0xc6a4a7935bd1e995ull;
            const s32 r = 47;

            u64 h = seed ^ (len * m);

            const u64* data = reinterpret_cast<const u64*>( input );
            const u64* end = data + (len/8);

            while( data != end ) {
                u64 k = *data++;

                k *= m;
                k ^= k >> r;
                k *= m;

                h ^= k;
                h *= m;
            }

            const u8* data2 = reinterpret_cast<const u8*>( data );

            switch( len & 7 ) {
            case 7: h ^= u64(data2[6]) << 48;
            case 6: h ^= u64(data2[5]) << 40;
            case 5: h ^= u64(data2[4]) << 32;
            case 4: h ^= u64(data2[3]) << 24;
            case 3: h ^= u64(data2[2]) << 16;
            case 2: h ^= u64(data2[1]) << 8;
            case 1: h ^= u64(data2[0]);
                    h *= m;
            };

            h ^= h >> r;
            h *= m;
            h ^= h >> r;

            return h;
        }

        /// @note
        /// Borrowed here: http://www.cse.yorku.ca/~oz/hash.html

        //! Computes the djb2 hash value.
        NIMBLE_INLINE u64 djb2( const void* input, u32 length )
        {
            u64 hash = 5381;
            const s8* data = reinterpret_cast<const s8*>( input );

            for( u32 i = 0; i < length; i++ ) {
                hash = ((hash << 5) + hash) + data[i]; /* hash * 33 + c */
            }

            return hash;
        }

        //! Computes the sdbm hash value.
        NIMBLE_INLINE u64 sdbm( const void* input, u32 length )
        {
            u64 hash = 0;
            const s8* data = reinterpret_cast<const s8*>( input );

            for( u32 i = 0; i < length; i++ ) {
                hash = data[i] + (hash << 6) + (hash << 16) - hash;
            }

            return hash;
        }

        //! Computes the lose lose hash value.
        NIMBLE_INLINE u64 hash( const void* input, u32 length )
        {
	        u64 hash = 0;
            const s8* data = reinterpret_cast<const s8*>( input );

	        for( u32 i = 0; i < length; i++ ) {
	            hash += data[i];
            }

	        return hash;
        }

        //! Murmur hash predicate.
        template<typename T>
        struct MurmurHash {
            T operator()( CString input, s32 length ) const { return murmur<T>( input, length ); }
        };

        //! djb2 hash predicate.
        struct Djb2Hash {
            u64 operator()( CString input, s32 length ) const { return djb2( input, length ); }
        };

    } // namespace Hash

    //! Hashed string type.
    template<typename TValue, typename TPredicate>
    class HashedString {
    public:

        //! Alias the hash value type.
        typedef TValue      Type;

        //! Alias the hash predicate type.
        typedef TPredicate  Predicate;

                            //! Constructs a hash string with a zero value.
                            HashedString( void );

                            //! Constructs a hash string with a specified value.
        explicit            HashedString( TValue value );

                            //! Constructs a hash string from a pointer to a C string.
        explicit            HashedString( CString str );

                            //! Constructs a hash string from a pointer to a C string and length.
        explicit            HashedString( CString str, s32 length );

                            //! Returns the string hash value.
                            operator TValue( void ) const;

        //! Tests two hashed strings for an equality.
        bool                operator == ( const HashedString& other ) const;

        //! Tests two hashed strings for an inequality.
        bool                operator != ( const HashedString& other ) const;

        //! Tests two hashed strings.
        bool                operator < ( const HashedString& other ) const;

    private:

        TValue              m_value;    //!< Hash value.
    };

    // ** HashedString::HashedString
    template<typename TValue, typename TPredicate>
    HashedString<TValue, TPredicate>::HashedString( void )
        : m_value( 0 )
    {
    }

    // ** HashedString::HashedString
    template<typename TValue, typename TPredicate>
    HashedString<TValue, TPredicate>::HashedString( TValue value )
        : m_value( value )
    {
    }

    // ** HashedString::HashedString
    template<typename TValue, typename TPredicate>
    HashedString<TValue, TPredicate>::HashedString( CString str )
    {
        TPredicate predicate;
        m_value = predicate( str, strlen( str ) );
    }

    // ** HashedString::HashedString
    template<typename TValue, typename TPredicate>
    HashedString<TValue, TPredicate>::HashedString( CString str, s32 length )
    {
        TPredicate predicate;
        m_value = predicate( str, length );
    }

    // ** HashedString::operator T
    template<typename TValue, typename TPredicate>
    NIMBLE_INLINE HashedString<TValue, TPredicate>::operator TValue ( void ) const
    {
        return m_value;
    }

    // ** HashedString::operator <
    template<typename TValue, typename TPredicate>
    NIMBLE_INLINE bool HashedString<TValue, TPredicate>::operator == ( const HashedString& other ) const
    {
        return m_value == other.m_value;
    }

    // ** HashedString::operator <
    template<typename TValue, typename TPredicate>
    NIMBLE_INLINE bool HashedString<TValue, TPredicate>::operator != ( const HashedString& other ) const
    {
        return m_value != other.m_value;
    }

    // ** HashedString::operator <
    template<typename TValue, typename TPredicate>
    NIMBLE_INLINE bool HashedString<TValue, TPredicate>::operator < ( const HashedString& other ) const
    {
        return m_value < other.m_value;
    }

    //! Used by std::unordered_map to hash a string.
    template<typename THashedString>
    struct StringStdHasher {
        size_t operator () ( const THashedString& value ) const;
    };

    // ** StringStdHasher::operator ()
    template<typename THashedString>
    NIMBLE_INLINE size_t StringStdHasher<THashedString>::operator ()( const THashedString& value ) const {
        return std::hash<typename THashedString::Value>( value );
    }

    typedef HashedString<u32, HashFunction::Djb2Hash>   String32;
    typedef StringStdHasher<String32>                   String32Hasher;
    typedef HashedString<u64, HashFunction::Djb2Hash>   String64;
    typedef StringStdHasher<String64>                   String64Hasher;

NIMBLE_END

#endif  /*  !__Nimble_Hash_H__   */
