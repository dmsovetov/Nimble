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

#ifndef __Nimble_EventEmitter_H__
#define __Nimble_EventEmitter_H__

#include "Globals.h"

NIMBLE_BEGIN

    namespace detail {
        
        //! Event listener subscribtion
        class Listener {
        public:
            
            virtual         ~Listener( void ) {}
            
            //! Emits event to listener.
            virtual void    notify( const void* e ) = 0;
        };
        
        //! Event listener with a type.
        template<typename T>
        class EventListener : public Listener {
        public:
            
            //! Callback function type.
            typedef cClosure<void(const T&)> Callback;
            
                            //! Constructs EventListener instance.
                            EventListener( const Callback& callback )
                                : m_callback( callback ) {}

			//! Compares this listener with a callback.
			bool			operator == ( const Callback& callback ) const { return m_callback == callback; }
            
            //! Calls a callback function with a casted event.
            virtual void    notify( const void* e ) { m_callback( *reinterpret_cast<const T*>( e ) ); }
            
        private:
            
            //! Callback function.
            Callback        m_callback;
        };
    } // namespace detail

	//! Event emitter class is used for dispatching strong typed global events.
	class EventEmitter {
	public:

        virtual	~EventEmitter( void ) {}

		//! Callback type wrapper.
		template<typename TEvent>
		struct Callback {
			//! Callback type alias.
			typedef typename detail::EventListener<TEvent>::Callback Type;
		};

		//! Subscribes to an event of type TEvent.
		template<typename TEvent>
        void subscribe( const typename Callback<TEvent>::Type& callback );

		//! Unsubscribes from an event of type TEvent.
		template<typename TEvent>
		void unsubscribe( const typename Callback<TEvent>::Type& callback );

		//! Emits a global event.
		template<typename TEvent>
		void notify( const TEvent& e );

	#ifdef NIMBLE_CPP11_ENABLED
        //! Constructs and emits a new event instance.
		template<typename TEvent, typename ... TArgs>
		void notify( const TArgs& ... args );
	#endif  /*  NIMBLE_CPP11_ENABLED    */

	private:

		//! Array of listeners.
		typedef Array< AutoPtr<detail::Listener> > Listeners;

		//! Listener container type.
		typedef Map<TypeIdx, Listeners> Subscribers;

		//! Array of array of listeners.
		Subscribers	m_subscribers;
	};

	// ** EventEmitter::subscribe
	template<typename TEvent>
	inline void EventEmitter::subscribe( const typename Callback<TEvent>::Type& callback )
	{
		TypeIdx idx = TypeIndex<TEvent>::idx();

		if( m_subscribers.count( idx ) == 0 ) {
			m_subscribers[idx] = Listeners();
		}

        m_subscribers[idx].push_back( new detail::EventListener<TEvent>( callback ) );
	}

	// ** EventEmitter::unsubscribe
	template<typename TEvent>
	inline void EventEmitter::unsubscribe( const typename Callback<TEvent>::Type& callback )
	{
		TypeIdx idx = TypeIndex<TEvent>::idx();

		if( m_subscribers.count( idx ) == 0 ) {
			return;
		}

		typedef detail::EventListener<TEvent> EventListenerType;
		Listeners& listeners = m_subscribers[idx];

		for( Listeners::iterator i = listeners.begin(); i != listeners.end(); )
		{
			EventListenerType& listener = *static_cast<EventListenerType*>( i->get() );
			if( listener == callback ) {
				i = listeners.erase( i );
			} else {
				++i;
			}
		}
	}

	// ** EventEmitter::notify
	template<typename TEvent>
	inline void EventEmitter::notify( const TEvent& e )
	{
		TypeIdx idx = TypeIndex<TEvent>::idx();
		Subscribers::iterator i = m_subscribers.find( idx );

		if( i == m_subscribers.end() ) {
			return;
		}

		for( u32 j = 0, n = ( u32 )i->second.size(); j < n; j++ ) {
            i->second[j]->notify( &e );
		}
	}

#ifdef NIMBLE_CPP11_ENABLED

	// ** EventEmitter::notify
	template<typename TEvent, typename ... TArgs>
	inline void EventEmitter::notify( const TArgs& ... args )
	{
		TEvent e( args... );
		notify( e );
	}

#endif	/*	NIMBLE_CPP11_ENABLED	*/

	//! Base class for ref counted event emitter classes.
	class RefCountedEventEmitter : public RefCounted {
	public:

		//! Subscribes to an event of type TEvent.
		template<typename TEvent>
		void						subscribe( const typename EventEmitter::Callback<TEvent>::Type& callback ) { m_eventEmitter.subscribe<TEvent>( callback ); }

		//! Unsubscribes from an event of type TEvent.
		template<typename TEvent>
		void						unsubscribe( const typename EventEmitter::Callback<TEvent>::Type& callback ) { m_eventEmitter.unsubscribe<TEvent>( callback ); }

	#ifdef NIMBLE_CPP11_ENABLED
		//! Constructs and emits a new event instance.
		template<typename TEvent, typename ... TArgs>
		void						notify( const TArgs& ... args ) { m_eventEmitter.notify<TEvent, TArgs...>( args... ); }
	#endif	/*	NIMBLE_CPP11_ENABLED	*/

	protected:

		EventEmitter				m_eventEmitter;	//!< Event emitter instance.
	};

#ifdef NIMBLE_CPP11_ENABLED
    //! Event emitter class used for injection.
    template<typename TBase>
    class InjectedEventEmitter : public TBase {
	public:

                                    //! Constructs InjectedEventEmitter instance by passing arguments to TBase's constructor.
                                    template<typename ... TArgs>
                                    InjectedEventEmitter( TArgs ... args )
                                        : TBase( args... ) {}

		//! Subscribes to an event of type TEvent.
		template<typename TEvent>
		void						subscribe( const typename EventEmitter::Callback<TEvent>::Type& callback ) { m_eventEmitter.subscribe<TEvent>( callback ); }

		//! Unsubscribes from an event of type TEvent.
		template<typename TEvent>
		void						unsubscribe( const typename EventEmitter::Callback<TEvent>::Type& callback ) { m_eventEmitter.unsubscribe<TEvent>( callback ); }

		//! Constructs and emits a new event instance.
		template<typename TEvent, typename ... TArgs>
		void						notify( const TArgs& ... args ) { m_eventEmitter.notify<TEvent, TArgs...>( args... ); }
	
	protected:

		EventEmitter				m_eventEmitter;	//!< Event emitter instance.
    };
#endif	/*	NIMBLE_CPP11_ENABLED	*/

	//namespace detail {

	//	//! Event interface
	//	class IQueuedEvent {
	//	public:

	//		virtual			~IQueuedEvent( void ) {}
	//		virtual void	emit( EventEmitter& emitter ) = 0;
	//	};

	//	//! Typed event.
	//	template<typename T>
	//	class QueuedEvent : public IQueuedEvent {
	//	public:

	//						//! Constructs a QueuedEvent instance.
	//						QueuedEvent( const T& e )
	//							: m_event( e ) {}

	//		//! Emits an event.
	//		virtual void	emit( EventEmitter& emitter ) { emitter.emit( m_event ); }

	//	private:

	//		//! Stored event data.
	//		T				m_event;
	//	};

	//	//! Event queue is used for cross-thread event emitting.
	//	class EventQueue {
	//	public:

	//		//! Queues a new event.
	//		template<typename T>
	//		void			queue( const T& e );

	//		//! Emits all queued events.
	//		void			emit( EventEmitter& emitter );

	//	private:

	//		//! Queued events.
	//		Array<IQueuedEvent*>	m_events;
	//	};

	//	// ** EventQueue::queue
	//	template<typename T>
	//	inline void EventQueue::queue( const T& e )
	//	{
	//	//	m_events.push_back( new QueuedEvent( e ) );
	//	}

	//	// ** EventQueue::emit
	//	inline void EventQueue::emit( EventEmitter& emitter )
	//	{
	//		for( u32 i = 0, n = m_events.size(); i < n; i++ ) {
	//			m_events[i]->emit( emitter );
	//			delete m_events[i];
	//		}

	//		m_events.clear();
	//	}

	//} // namespace detail

NIMBLE_END

#endif	/*	!__Nimble_EventEmitter_H__	*/