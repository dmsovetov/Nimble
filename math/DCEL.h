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

#ifndef __Foo_DCEL_H__
#define __Foo_DCEL_H__

#include "Config.h"

#include <vector>
#include <list>
#include <map>

FOO_BEGIN_NAMESPACE

namespace dcel {

	class Edge;

	//! Mesh vertex index type.
	typedef unsigned int Index;

	//! Mesh vertex.
	class Vertex {
	public:

						//! Constructs Vertex instance.
						Vertex( Index index );

		//! Returns vertex index.
		Index			index( void ) const;

		//! Returns edge.
		Edge*			edge( void ) const;

	private:

		Index			m_index;	//!< Vertex index.
		Edge*			m_edge;		//!< An arbitrary edge that has this vertex as an origin.
	};

	// ** Vertex::Vertex
	inline Vertex::Vertex( Index index ) : m_index( index ), m_edge( NULL )
	{
	}

	// ** Vertex::index
	inline Index Vertex::index( void ) const
	{
		return m_index;
	}

	// ** Vertex::edge
	inline Edge* Vertex::edge( void ) const
	{
		return m_edge;
	}

	//! Edge bounds a single face and represents a linked list of edges around the face.
	class Edge {
	public:

						//! Constructs Edge instance.
						Edge( void );

		//! Returns the edge origin vertex.
		Vertex*			vertex( void ) const;

		//! Sets the origin vertex.
		void			setVertex( Vertex* value );

		//! Returns the next edge in list.
		Edge*			next( void ) const;

		//! Sets the next edge in list.
		void			setNext( Edge* value );

		//! Returns a twin edge.
		Edge*			twin( void ) const;

		//! Sets a twin edge.
		void			setTwin( Edge* value );

	private:

		Vertex*			m_vertex;	//!< The origin vertex.
		Edge*           m_next;     //!< The next edge around the face.
		Edge*           m_twin;     //!< The pair edge.
	};

	// ** Edge::Edge
	inline Edge::Edge( void ) : m_vertex( NULL ), m_next( NULL ), m_twin( NULL )
	{
	}

	// ** Edge::vertex
	inline Vertex* Edge::vertex( void ) const
	{
		return m_vertex;
	}

	// ** Edge::setVertex
	inline void Edge::setVertex( Vertex* value )
	{
		m_vertex = value;
	}

	// ** Edge::next
	inline Edge* Edge::next( void ) const
	{
		assert( m_next != NULL );
		return m_next;
	}

	// ** Edge::setNext
	inline void Edge::setNext( Edge* value )
	{
		m_next = value;
	}

	// ** Edge::twin
	inline Edge* Edge::twin( void ) const
	{
		return m_twin;
	}

	// ** Edge::setTwin
	inline void Edge::setTwin( Edge* value )
	{
		m_twin = value;
		value->m_twin = this;
	}

    //! DCEL mesh is a doubly connected edge list data structure to simplify access to a triangular mesh topology.
    template<typename TIndex>
    class Mesh {
    public:

		//! Container to store mesh edges.
		typedef std::vector<Edge*>	Edges;

		//! Container to store mesh vertices.
		typedef std::vector<Vertex*> Vertices;

		//! Creates a new vertex.
		Vertex*				addVertex( void );

		//! Returns vertex by index.
		Vertex*				vertex( TIndex index );

		//! Returns the edge count.
		const Edges&		edges( void ) const;

        //! Constructs a half edge struct from a triangle mesh.
        static Mesh			create( const TIndex* indices, unsigned int count );

    private:

                            //! Constructs a Mesh instance.
                            Mesh( void );

    private:

        Edges				m_edges;        //!< Mesh edges.
		Vertices			m_vertices;		//!< Mesh vertices.
    };

    // ** Mesh::Mesh
    template<typename TIndex>
    Mesh<TIndex>::Mesh( void )
    {
    }

    // ** Mesh::edge
    template<typename TIndex>
    const typename Mesh<TIndex>::Edges& Mesh<TIndex>::edges( void ) const
    {
		return m_edges;
    }

    // ** Mesh::addVertex
    template<typename TIndex>
	Vertex* Mesh<TIndex>::addVertex( void )
    {
		Vertex* vertex = new Vertex( m_vertices.size() );
		m_vertices.push_back( vertex );
		return vertex;
    }

	// ** Mesh::vertex
    template<typename TIndex>
	Vertex* Mesh<TIndex>::vertex( TIndex index )
    {
		return index < m_vertices.size() ? m_vertices[index] : NULL;
    }

    // ** Mesh::create
    template<typename TIndex>
    Mesh<TIndex> Mesh<TIndex>::create( const TIndex* indexBuffer, unsigned int count )
    {
		Mesh<TIndex> mesh;

		// ** Create edges.
		for( int i = 0; i < count; i++ ) {
			mesh.m_edges.push_back( new Edge );
		}

		// ** Create vertices
		for( int i = 0; i < count; i++ ) {
			if( indexBuffer[i] >= mesh.m_vertices.size() ) {
				mesh.addVertex();
			}
		}

		// ** Create edges.
		for( TIndex face = 0; face < count / 3; face++ ) {
			Edge**		  edges   = &mesh.m_edges[face * 3];
			const TIndex* indices = &indexBuffer[face * 3];

			for( int i = 0; i < 3; i++ ) {
				edges[i]->setVertex( mesh.vertex( indices[i] ) );
				edges[i]->setNext( edges[(i + 1) % 3] );
			}
		}

		return mesh;
	/*
        typedef std::pair<TIndex, TIndex>           EdgeVertices;
        typedef std::multimap<EdgeVertices, Edge*>  EdgeCache;

        int       faceCount  = count / 3;
        int       edgeCount  = faceCount * 3;
		Edges	  edges( edgeCount );
        EdgeCache cache;



		for( TIndex face = 0; face < faceCount; face++ ) {
			for( int i = 0; i < 3; i++ ) {
				edges.push_back( )
			}
		}

	/*
		for( int i = 0; i < edgeCount; i++ ) {
			edges[i] = new Edge;
		}

        for( TIndex face = 0; face < faceCount; face++ ) {
			Edge**		  faceEdges   = &edges[face * 3];
            const TIndex* faceIndices = &indices[face * 3];

            for( int i = 0; i < 3; i++ ) {
                Edge* edge = faceEdges[i];

                edge->m_face   = face;
                edge->m_vertex = faceIndices[i];
                edge->m_next   = faceEdges[(i + 1) % 3];

                cache.insert( std::pair<EdgeVertices, Edge*>( EdgeVertices( edge->m_vertex, edge->m_next->m_vertex ), edge ) );
            }
        }

        for( int i = 0; i < edgeCount; i++ ) {
            Edge* e = edges[i];

            EdgeCache::iterator z = cache.find( EdgeVertices( e->m_next->m_vertex, e->m_vertex ) );

			if( z != cache.end() ) {
				z->second->m_twin = e;
				e->m_twin  = z->second;
			}
        }

        return Mesh<TIndex>( edges );
	*/
    }

} // namespace dcel

FOO_END_NAMESPACE

#endif  /*  !defined( __Foo_Mesh_H__ )  */
