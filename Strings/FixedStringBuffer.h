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

#ifndef __Nimble_FixedStringBuffer_H__
#define __Nimble_FixedStringBuffer_H__

#include "../Globals.h"

NIMBLE_BEGIN

    //! A fixed string buffer containers a pointer to writable string and a maximum capacity of that string.
    class FixedStringBuffer
    {
    public:

        //! Constructs a string buffer instance from a pointer and maximum capacity.
        FixedStringBuffer(s8* buffer, s32 capacity);

        //! Stream write operator.
        FixedStringBuffer&  operator << (const s8* str);

        //! Writes a string view to this buffer.
        FixedStringBuffer&  operator << (const StringView& str);

        //! Stream write operator that copies a fixed string buffer.
        FixedStringBuffer&  operator << (const FixedStringBuffer& other);

        //! Returns a string buffer pointer.
        const s8*           cstr() const;

        //! Returns current string length.
        s32                 length() const;

        //! Returns available string buffer space.
        s32                 available() const;

        //! Returns initial string buffer capacity.
        s32                 capacity() const;

        //! Clears the string buffer.
        void                clear();

        //! Writes given string to a buffer.
        void                write(const s8* str);

        //! Writes formatted string to a buffer.
        void                writeWithFormat(const s8* format, ...);

        //! Copies bytes from a source buffer to an output one.
        void                write(const s8* str, s32 length);

    private:

        s8*                 m_buffer;   //!< An output buffer pointer.
        s8*                 m_output;   //!< A pointer to a writable buffer part.
        s32                 m_capacity; //!< Maximum string buffer capacity.
    };

    // ** FixedStringBuffer::FixedStringBuffer
    NIMBLE_INLINE FixedStringBuffer::FixedStringBuffer(s8* buffer, s32 capacity)
        : m_buffer(buffer)
        , m_output(buffer)
        , m_capacity(capacity)
    {
        clear();
    }

    // ** FixedStringBuffer::operator <<
    NIMBLE_INLINE FixedStringBuffer& FixedStringBuffer::operator << (const s8* str)
    {
        write(str);
        return *this;
    }

    // ** FixedStringBuffer::operator <<
    NIMBLE_INLINE FixedStringBuffer& FixedStringBuffer::operator << (const FixedStringBuffer& other)
    {
        if (this != &other)
        {
            write(other.cstr());
        }
        return *this;
    }

    // ** FixedStringBuffer::operator <<
    NIMBLE_INLINE FixedStringBuffer& FixedStringBuffer::operator << (const StringView& str)
    {
        write(str.value(), str.length());
        return *this;
    }

    // ** FixedStringBuffer::cstr
    NIMBLE_INLINE const s8* FixedStringBuffer::cstr() const
    {
        return m_buffer;
    }

    // ** FixedStringBuffer::length
    NIMBLE_INLINE s32 FixedStringBuffer::length() const
    {
        return m_output - m_buffer;
    }

    // ** FixedStringBuffer::available
    NIMBLE_INLINE s32 FixedStringBuffer::available() const
    {
        return capacity() - length();
    }

    // ** FixedStringBuffer::capacity
    NIMBLE_INLINE s32 FixedStringBuffer::capacity() const
    {
        return m_capacity;
    }

    // ** FixedStringBuffer::clear
    NIMBLE_INLINE void FixedStringBuffer::clear()
    {
        memset(m_buffer, 0, m_capacity);
        m_output = m_buffer;
    }

    // ** FixedStringBuffer::write
    NIMBLE_INLINE void FixedStringBuffer::write(const s8* str, s32 length)
    {
        NIMBLE_ABORT_IF(length >= available(), "string buffer overflow");
        memcpy(m_output, str, length);
        m_output += length;
    }

    // ** FixedStringBuffer::write
    NIMBLE_INLINE void FixedStringBuffer::write(const s8* str)
    {
        write(str, static_cast<s32>(strlen(str)));
    }

    // ** FixedStringBuffer::writeWithFormat
    NIMBLE_INLINE void FixedStringBuffer::writeWithFormat(const s8* format, ...)
    {
        NIMBLE_LOGGER_FORMAT(format);
        write(buffer, static_cast<s32>(strlen(buffer)));
    }

NIMBLE_END

#endif  /*    !__Nimble_FixedStringBuffer_H__    */
