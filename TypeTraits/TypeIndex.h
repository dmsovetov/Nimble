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

#ifndef __Nimble_TypeIndex_H__
#define __Nimble_TypeIndex_H__

#include "../Globals.h"

NIMBLE_BEGIN

    //! Type for class index.
    typedef u16 TypeIdx;

    //! Type for global type indices.
    struct GlobalTypeIndex {};

    //! TypeIndexGenerator is a base class for TypeIndex and used to store a global class counter.
    template<typename T>
    class TypeIndexGenerator {
    protected:

        static TypeIdx  generateNextIdx( void ) { static TypeIdx nextTypeIdx = 1; return nextTypeIdx++; }
    };

    //! TypeIndex class helps to generate a unique integer index for any class.
    /*!
     Note: TypeIndex doesn't guaranty that a same index is generated for a same class all the time.
     */
    template<typename T>
    class TypeIndex : public TypeIndexGenerator<GlobalTypeIndex> {
    public:

        //! Generates a type index.
        static TypeIdx      idx( void ) { static TypeIdx idx = TypeIndexGenerator::generateNextIdx(); return idx; }
    };

    //! GroupedTypeIndex is used for generating a separate sets of indices.
    template<typename T, typename U>
    class GroupedTypeIndex : public TypeIndexGenerator<U> {
    public:

	    //! Generates a type index.
	    static TypeIdx      idx( void ) { static TypeIdx idx = TypeIndexGenerator<U>::generateNextIdx(); return idx; }
    };

NIMBLE_END

#endif  /*  !__Nimble_TypeIndex_H__  */