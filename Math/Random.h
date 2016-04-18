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

#ifndef __Nimble_Random_H__
#define __Nimble_Random_H__

#include "../Globals.h"

NIMBLE_BEGIN

    //! Pseudo-random number generator.
    class Random {
    public:

                    //! Constructs a Random instance.
                    Random( s32 seed );

        //! Returns a next pseudo-random number in a sequence.
        s32         generate( void );

        //! Returns current seed value.
        s32         seed( void ) const;

        //! Sets current seed value.
        void        setSeed( s32 value );

    private:

        mutable s32 m_seed;     //!< Generator seed.
    };

    // ** Random::Random
    NIMBLE_INLINE Random::Random( s32 seed )
        : m_seed( seed )
    {
    }

    // ** Random::setSeed
    NIMBLE_INLINE s32 Random::seed( void ) const
    {
        return m_seed;
    }

    // ** Random::setSeed
    NIMBLE_INLINE void Random::setSeed( s32 value )
    {
        m_seed = value;
    }

    // ** Random::Random
    NIMBLE_INLINE s32 Random::generate( void )
    {
	    m_seed = 214013 * m_seed + 13737667;
	    return (m_seed >> 16) & 0x7fff;
    }

NIMBLE_END

#endif  /*  !__Nimble_Random_H__  */
