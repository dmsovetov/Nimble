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
 
#ifndef	__Nimble_AbstractFactory_H__
#define	__Nimble_AbstractFactory_H__

#include "../Globals.h"

NIMBLE_BEGIN

	//! Abstract factory is used for object construction.
	template<typename TBase, typename TTypeId>
	class AbstractFactory {
	public:

		//! Registers the new object factory.
		template<typename TDerived>
		bool declare( const TTypeId& id )
		{
			if( m_factories.count( id ) ) {
				return false;
			}

			m_factories[id] = new TypeFactory<TDerived>;
			return true;
		}

		//! Constructs a new object instance.
		TBase* construct( const TTypeId& id ) const
		{
			typename Factories::const_iterator i = m_factories.find( id );

			if( i == m_factories.end() ) {
				return NULL;
			}

			return i->second->create();
		}

	protected:

		//! Object factory base.
		struct Factory {
			//! Creates new object instance.
			virtual TBase*	create( void ) const = 0;
		};

		//! Generic object factory.
		template<typename TDerived>
		struct TypeFactory : public Factory {
			//! Creates a new instance of TDerived type.
			virtual TBase* create( void ) const { return new TDerived; }
		};

		//! Factory strong pointer type.
		typedef AutoPtr<Factory>		    FactoryPtr;

		//! Container type to store object factories.
		typedef Map<TTypeId, FactoryPtr>	Factories;

		//! Registered object factories.
		Factories							m_factories;
	};

	//! Type index based abstract factory.
	template<typename TBase>
	class IndexedAbstractFactory {
	public:

		//! Constructs the object by type id.
		TBase* construct( TypeIdx id ) const
		{
			return m_factory.construct( id );
		}

		//! Registers the new object factory.
		template<typename TDerived>
		bool declare( void )
		{
			return m_factory.template declare<TDerived>( generateTypeId<TDerived>() );
		}

		//! Constructs a new object instance.
		template<typename TDerived>
		TDerived* construct( void ) const
		{
			return m_factory.construct( generateTypeId<TDerived>() );
		}

	private:

		//! Generates the type id from type.
		template<typename T>
		static TypeIdx generateTypeId( void )
		{
			return GroupedTypeIndex<T, TBase>::idx();
		}

	private:

		//! Abstract factory used to construct objects.
		AbstractFactory<TBase, TypeIdx>	m_factory;
	};

	//! Type name based abstract factory.
	template<typename TBase>
	class NamedAbstractFactory {
	public:

		//! Constructs the object by type id.
		TBase* construct( const String& id ) const
		{
			return m_factory.construct( id );
		}

		//! Registers the new object factory.
		template<typename TDerived>
		bool declare( void )
		{
			return m_factory.template declare<TDerived>( generateTypeName<TDerived>() );
		}

		//! Constructs a new object instance.
		template<typename TDerived>
		TBase* construct( void ) const
		{
			return m_factory.construct( generateTypeName<TDerived>() );
		}

	private:

		//! Generates the type name from type.
		template<typename T>
		static String generateTypeName( void )
		{
			return TypeInfo<T>::name();
		}

	private:

		//! Abstract factory used to construct objects.
		AbstractFactory<TBase, String>	m_factory;
	};

NIMBLE_END

#endif		/*	!__Nimble_AbstractFactory_H__	*/
