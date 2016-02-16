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

#ifndef __Nimble_Ptr_H__
#define __Nimble_Ptr_H__

#include "RefCounted.h"

NIMBLE_BEGIN

	template<typename T> class WeakPtr;

    //! A reference counted pointer type.
    template<typename T>
    class Ptr {
    public:

        typedef Ptr<T> ThisType;

                        Ptr( const ThisType& other );
                        Ptr( T *pointer = NULL );
                        ~Ptr( void );

                        template <class R>
                        Ptr( const Ptr<R>& other ) : m_ptr( other.get() ) {
                            if( m_ptr ) {
                                static_cast<RefCounted*>( m_ptr )->retain();
                            }
                        }

						Ptr( const WeakPtr<T>& other );

        T*              get( void ) const;
		bool			valid( void ) const;
        T*              operator -> ( void );
        const T*        operator -> ( void ) const;
        T&              operator *  ( void );
        const T&        operator *  ( void ) const;
        ThisType&       operator =  ( T* pointer );
        ThisType&       operator =  ( const ThisType& other );
        bool            operator == ( const ThisType& other ) const;
        bool            operator != ( const ThisType& other ) const;
        bool            operator <  ( const ThisType& other ) const;

    private:

        void            setPointer( RefCounted *pointer );

    private:

        T*              m_ptr;
    };

    // ** Ptr::Ptr
    template<typename T>
    Ptr<T>::Ptr( T* pointer ) : m_ptr( pointer ) {
        if( m_ptr ) {
            static_cast<RefCounted*>( m_ptr )->retain();
        }
    }

    // ** Ptr::Ptr
    template<typename T>
    Ptr<T>::Ptr( const ThisType& other ) : m_ptr( other.m_ptr ) {
        if( m_ptr ) {
            static_cast<RefCounted*>( m_ptr )->retain();
        }
    }

    // ** Ptr::Ptr
    template<typename T>
    Ptr<T>::Ptr( const WeakPtr<T>& pointer ) : m_ptr( NULL ) {
        setPointer( pointer.get() );
    }

    // ** Ptr::~Ptr
    template<typename T>
    Ptr<T>::~Ptr( void ) {
        if( m_ptr ) {
            m_ptr->release();
            m_ptr = NULL;
        }
    }

    // ** Ptr::get
    template<typename T>
    T* Ptr<T>::get( void ) const  {
        return ( T* )m_ptr;
    }

    // ** Ptr::valid
    template<typename T>
    bool Ptr<T>::valid( void ) const  {
        return m_ptr != NULL;
    }

    // ** Ptr::operator ->
    template<typename T>
    T* Ptr<T>::operator -> ( void ) {
        DC_BREAK_IF( m_ptr == NULL );
        return static_cast<T*>( m_ptr );
    }

    template<typename T>
    const T* Ptr<T>::operator -> ( void ) const {
        DC_BREAK_IF( m_ptr == NULL );
        return static_cast<const T*>( m_ptr );
    }

    // ** Ptr::operator *
    template<typename T>
    T& Ptr<T>::operator * ( void ) {
        DC_BREAK_IF( m_ptr == NULL );
        return *static_cast<T*>( m_ptr );
    }

    // ** Ptr::operator *
    template<typename T>
    const T& Ptr<T>::operator * ( void ) const {
        DC_BREAK_IF( m_ptr == NULL );
        return *static_cast<const T*>( m_ptr );
    }

    // ** Ptr::operator =
    template<typename T>
    typename Ptr<T>::ThisType& Ptr<T>::operator = ( const ThisType& other ) {
        setPointer( other.m_ptr );
        return *this;
    }

    template<typename T>
    typename Ptr<T>::ThisType& Ptr<T>::operator = ( T *pointer ) {
        setPointer( pointer );
        return *this;
    }

    // ** Ptr::operator ==
    template<typename T>
    bool Ptr<T>::operator == ( const ThisType& other ) const {
        return ( m_ptr == other.m_ptr );
    }

    // ** Ptr::operator !=
    template<typename T>
    bool Ptr<T>::operator != ( const ThisType& other ) const {
        return ( m_ptr != other.m_ptr );
    }

    // ** Ptr::operator <
    template<typename T>
    bool Ptr<T>::operator < ( const ThisType& other ) const {
        return ( m_ptr < other.m_ptr );
    }

    // ** Ptr::setPointer
    template<typename T>
    void Ptr<T>::setPointer( RefCounted *pointer ) {
        if( m_ptr ) {
            m_ptr->release();
        }

        if( (m_ptr = pointer) ) {
            m_ptr->retain();
        }
    }

NIMBLE_END

#endif    /*    !__Nimble_Ptr_H__    */
