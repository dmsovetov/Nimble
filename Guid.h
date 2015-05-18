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

#ifndef __Foo_Guid_H__
#define __Foo_Guid_H__

FOO_BEGIN_NAMESPACE

	//! 12-byte globaly unique identifier.
	struct Guid {
	public:

					enum { Size = 12 };

					//! Constructs an empty Guid identifier.
					Guid( void );

					//! Constructs Guid identifier from bytes.
					Guid( const u8* id );

		//! Converts Guid to a string representation.
		String		toString( void ) const;

		//! Returns GUID bytes.
		const u8*	bytes( void ) const;

		//! Compares two Entity identifiers.
		bool		operator < ( const Guid& other ) const ;

		//! Compares two Entity identifiers.
		bool		operator == ( const Guid& other ) const;

		//! Compares two Entity identifiers.
		bool		operator != ( const Guid& other ) const;

	private:

		//! GUID bytes.
		u8			m_id[Size];
	};

	// ** Guid::Guid
	inline Guid::Guid( void )
	{
		memset( m_id, 0, Size );
	}

	// ** Guid::Guid
	inline Guid::Guid( const u8* id )
	{
		memcpy( m_id, id, Size );
	}

	// ** Guid::toString
	inline String Guid::toString( void ) const
	{
		static const s8 digits[] = "0123456789abcdef";

		String result = "";

		for( s32 i = 0; i < Size; i++ ) {
			result += digits[(m_id[i] >> 4) & 0xf];
			result += digits[(m_id[i] >> 0) & 0xf];
		}

		DC_BREAK_IF( result.length() != 24 );

		return result;
	}

	// ** Guid::bytes
	inline const u8* Guid::bytes( void ) const
	{
		return m_id;
	}

	// ** Guid::operator <
	inline bool Guid::operator < ( const Guid& other ) const
	{
		return memcmp( m_id, other.m_id, Size ) < 0;
	}

	// ** Guid::operator ==
	inline bool Guid::operator == ( const Guid& other ) const
	{
		return memcmp( m_id, other.m_id, Size ) == 0;
	}

	// ** Guid::operator !=
	inline bool Guid::operator != ( const Guid& other ) const
	{
		return memcmp( m_id, other.m_id, Size ) != 0;
	}

FOO_END_NAMESPACE

#endif	/*	!__Foo_Guid_H__	*/
