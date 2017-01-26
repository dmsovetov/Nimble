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

#ifndef __Nimble_H__
#define __Nimble_H__

#include "Config.h"

#include "Preprocessor/Preprocessor.h"

#include "Patterns/Singleton.h"

#include "Pointers/Ptr.h"
#include "Pointers/WeakPtr.h"
#include "Pointers/UniquePtr.h"

#include "Templates.h"
#include "Hash.h"
#include "Bitset.h"
#include "FlagSet.h"
#include "Guid.h"

#include "Hashing/Base64.h"

#include "Closure/Closure.h"

#include "Allocators/LinearAllocator.h"
#include "Allocators/IndexAllocator.h"

#include "Containers/Pool.h"
#include "Containers/StringList.h"
#include "Containers/FixedArray.h"
#include "Containers/IndexCache.h"
#include "Containers/IndexManager.h"
#include "Containers/BidHashMap.h"
#include "Containers/BidMap.h"

#include "Color/Rgb.h"
#include "Color/Rgba.h"

#include "Math/Random.h"
#include "Math/Vector.h"
#include "Math/Vec2.h"
#include "Math/Vec3.h"
#include "Math/Vec4.h"
#include "Math/Quat.h"
#include "Math/Curve.h"
#include "Math/FixedPointNumber.h"
#include "Math/Graph.h"
#include "Math/LinearRegression.h"
#include "Math/Matrix.h"
#include "Math/Matrix4.h"
#include "Math/Samples.h"

#include "Bv/Bounds.h"
#include "Bv/Frustum.h"

#include "RectanglePacker.h"

#include "Math/Plane.h"
#include "Math/Ray.h"

#include "Math/Mesh.h"

#include "TypeTraits/NumericTraits.h"
#include "TypeTraits/TypeIndex.h"
#include "TypeTraits/TypeName.h"
#include "TypeTraits/Type.h"

#include "LocalTime.h"
#include "EventEmitter.h"
#include "Composition.h"
#include "Patterns/AbstractFactory.h"
#include "Variant.h"
#include "KeyValue.h"
#include "Debug/Breadcrumb.h"
#include "Debug/Logger.h"

#include "Strings/FixedString.h"

#include "Containers/VariantArray.h"

namespace std {
    template<typename T>
    struct hash< NIMBLE_NS Ptr<T> > {
        size_t operator()( const NIMBLE_NS Ptr<T>& val ) const {
            return ::std::hash<T*>()( val.get() );
        }
    };

    template<typename T>
    struct hash< NIMBLE_NS WeakPtr<T> > {
        size_t operator()( const NIMBLE_NS WeakPtr<T>& val ) const {
            return ::std::hash<T*>()( val.get() );
        }
    };

    //! Used by std::unordered_map to hash a string.
    template<>
    struct hash< NIMBLE_NS String32 > {
        size_t operator () ( const NIMBLE_NS String32& value ) const {
            return hash<typename NIMBLE_NS String32::Type>()( value );
        }
    };

    //! Used by std::unordered_map to hash a string.
    template<>
    struct hash< NIMBLE_NS String64 > {
        size_t operator () ( const NIMBLE_NS String64& value ) const {
            return hash<typename NIMBLE_NS String64::Type>()( value );
        }
    };
}

#endif  /*  !__Nimble_H__   */
