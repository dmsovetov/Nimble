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

#ifndef __Nimble_UniquePtr_H__
#define __Nimble_UniquePtr_H__

#include "../Globals.h"

NIMBLE_BEGIN

    template<typename T>
    class UPtr {
    protected:

        T*                  m_pointer;

    public:

                            UPtr( T *pointer = NULL );
                            UPtr( const UPtr<T>& other );
                            ~UPtr( void );

        T*                  get( void ) const;
        T*                  drop( void );

        T*                  operator -> ( void );
        const T*            operator -> ( void ) const;
        T&                  operator *    ( void );
        const T&            operator *    ( void ) const;
        UPtr&            operator =    ( T *pointer );
        bool                operator ==    ( const UPtr<T>& other ) const;
        bool                operator != ( const UPtr<T>& other ) const;
        const UPtr<T>&   operator =  ( const UPtr<T>& other );
    };

    // ** UPtr::UPtr
    template<typename T>
    UPtr<T>::UPtr( T *pointer ) {
        m_pointer = pointer;
    }

    template<typename T>
    UPtr<T>::UPtr( const UPtr<T>& other ) {
        m_pointer = other.m_pointer;
        const_cast< UPtr<T>& >( other ).m_pointer = NULL;
    }

    template<typename T>
    UPtr<T>::~UPtr( void ) {
        if( m_pointer ) {
            delete m_pointer;
            m_pointer = NULL;
        }
    }

    // ** UPtr::operator ->
    template<typename T>
    T* UPtr<T>::operator -> ( void ) {
        NIMBLE_ABORT_IF( m_pointer == NULL, "dereferencing NULL pointer" );
        return m_pointer;
    }

    template<typename T>
    const T* UPtr<T>::operator -> ( void ) const {
        NIMBLE_ABORT_IF( m_pointer == NULL, "dereferencing NULL pointer" );
        return m_pointer;
    }

    // ** UPtr::operator *
    template<typename T>
    T& UPtr<T>::operator * ( void ) {
        NIMBLE_ABORT_IF( m_pointer == NULL, "dereferencing NULL pointer" );
        return *m_pointer;
    }

    template<typename T>
    UPtr<T>& UPtr<T>::operator = ( T *pointer ) {
        if( m_pointer ) {
            delete m_pointer;
        }

        m_pointer = pointer;
        return *this;
    }


    // ** UPtr::operator =
    template<typename T>
    const UPtr<T>& UPtr<T>::operator = ( const UPtr<T>& other ) {
        if( m_pointer ) {
            delete m_pointer;
        }

        m_pointer = other.m_pointer;
        const_cast<UPtr<T>&>( other ).m_pointer = NULL;

        return *this;
    }

    // ** UPtr::operator ==
    template<typename T>
    bool UPtr<T>::operator == ( const UPtr<T>& other ) const {
        return ( m_pointer == other.m_pointer );
    }

    // ** UPtr::operator !=
    template<typename T>
    bool UPtr<T>::operator != ( const UPtr<T>& other ) const {
        return ( m_pointer != other.m_pointer );
    }

    // ** UPtr::get
    template<typename T>
    T* UPtr<T>::get( void ) const {
        return m_pointer;
    }

    // ** UPtr::drop
    template<typename T>
    T* UPtr<T>::drop( void ) {
        T * tmp        = m_pointer;
        m_pointer    = NULL;
        return tmp;
    }

NIMBLE_END

#endif  /*    !__Nimble_UniquePtr_H__    */
