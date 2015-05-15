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

#ifndef __Foo_Mesh_H__
#define __Foo_Mesh_H__

#include "Config.h"
#include "Vec2.h"
#include "Vec3.h"

#include <vector>
#include <list>
#include <map>

FOO_BEGIN_NAMESPACE

    //! DCEL data struct to simplify access to a triangular mesh topology.
    template<typename TIndex = unsigned short>
    class DCEL {
    public:

        //! Index buffer type.
        typedef std::vector<TIndex> IndexBuffer;

        //! The edge struct.
        struct Edge {
            TIndex          m_face;     //!< The parent face.
            TIndex          m_vertex;   //!< The first edge vertex.
            Edge*           m_next;     //!< The next edge around the face.
            Edge*           m_twin;     //!< The pair edge.

            //! Returns true if this edge is a boundary.
            bool isBoundary( void ) const
            {
                return m_twin == NULL;
            }

            //! Returns edge twin.
            Edge* twin( void ) const
            {
                return m_twin;
            }

            //! Sets the edge twin.
            void setTwin( Edge* value )
            {
                m_twin        = value;
                value->m_twin = this;
            }
        };

                            ~DCEL( void );

        //! Returns edge count.
        int                 edgeCount( void ) const;

        //! Returns edge by index.
        const Edge*         edge( int index ) const;

        //! Constructs a half edge struct from a triangle mesh.
        static DCEL         create( const IndexBuffer& indexBuffer );

    private:

                            //! Constructs a HalfEdge instance.
                            DCEL( Edge* edges, int edgeCount );

    private:

        Edge*               m_edges;        //!< Mesh edges.
        int                 m_edgeCount;    //!< The total number of edges.
    };

    // ** DCEL::DCEL
    template<typename TIndex>
    DCEL<TIndex>::DCEL( Edge* edges, int edgeCount ) : m_edges( edges ), m_edgeCount( edgeCount )
    {
    }

    // ** DCEL::DCEL
    template<typename TIndex>
    DCEL<TIndex>::~DCEL( void )
    {
    //    delete[]m_edges;
    }

    // ** DCEL::edgeCount
    template<typename TIndex>
    int DCEL<TIndex>::edgeCount( void ) const
    {
        return m_edgeCount;
    }

    // ** DCEL::edgeCount
    template<typename TIndex>
    const typename DCEL<TIndex>::Edge* DCEL<TIndex>::edge( int index ) const
    {
        return &m_edges[index];
    }

    // ** DCEL::create
    template<typename TIndex>
    DCEL<TIndex> DCEL<TIndex>::create( const typename IndexBuffer& indexBuffer )
    {
        typedef std::pair<TIndex, TIndex>           EdgeVertices;
        typedef std::multimap<EdgeVertices, Edge*>  EdgeCache;

        int       faceCount  = indexBuffer.size() / 3;
        int       edgeCount  = faceCount * 3;
        Edge*     edges      = new Edge[edgeCount];
        EdgeCache cache;

        memset( edges, 0, sizeof( Edge ) * edgeCount );

        for( TIndex face = 0; face < faceCount; face++ ) {
            Edge*         faceEdges   = &edges[face * 3];
            const TIndex* faceIndices = &indexBuffer[face * 3];

            for( int i = 0; i < 3; i++ ) {
                faceEdges[i].m_face   = face;
                faceEdges[i].m_vertex = faceIndices[i];
            }

            for( int i = 0; i < 3; i++ ) {
                Edge& edge = faceEdges[i];

                edge.m_next = &faceEdges[(i + 1) % 3];
                cache.insert( std::pair<EdgeVertices, Edge*>( EdgeVertices( edge.m_vertex, edge.m_next->m_vertex ), &edge ) );
            }
        }

        for( int i = 0; i < edgeCount; i++ ) {
            Edge& e = edges[i];

            EdgeCache::iterator z = cache.find( EdgeVertices( e.m_next->m_vertex, e.m_vertex ) );
            if( z != cache.end() && &e != z->second ) {
                assert( &e != z->second );
                e.setTwin( z->second );
            }
        }

        return DCEL<TIndex>( edges, edgeCount );
    }

	//! TriMesh represents an indexed triangular mesh.
	template<typename TVertex, typename TIndex = unsigned short>
	class TriMesh {
	public:

		//! Container type to store mesh vertices.
		typedef std::vector<TVertex> Vertices;

		//! Container type to store mesh indices.
		typedef std::vector<TIndex> Indices;

        //! DCEL type.
        typedef DCEL<TIndex> Dcel;

		//! Triangular mesh face.
		struct Face {
							            //! Constructs Face instance.
							            Face( Vertices& vertices, Indices& indices, unsigned int index )
								            : m_vertices( vertices ), m_indices( indices ), m_index( index ) {}

			//! Returns a vertex by index.
			TVertex&		            operator[]( TIndex index ) { return vertex( index ); }
			const TVertex&              operator[]( TIndex index ) const { return vertex( index ); }

			unsigned int	            m_index;	//!< Face index.
			Vertices&		            m_vertices;	//!< Mesh vertices.
			Indices&		            m_indices;	//!< Mesh indices.

			//! Calculates face normal.
			Vec3			            normal( void ) const;

            //! Flattens the triangle to a 2D space.
            void                        flatten( const Vec3& axis, Vec2& a, Vec2& b, Vec2& c ) const;

			//! Returns the face vertex by index.
			const TVertex&	            vertex( TIndex index ) const;
			TVertex&		            vertex( TIndex index );
		};

        //! Mesh chart.
        struct Chart {
                                        //! Constructs Chart instance.
                                        Chart( TriMesh<TVertex, TIndex>& mesh )
                                            : m_mesh( mesh ) {}

            //! Adds a new face to chart.
            void                        add( unsigned int index );

            //! Returns face count.
            int                         faceCount( void ) const;

            //! Returns chart face by index.
            Face                        face( int index ) const;

            //! Calculates avarage chart normal.
            Vec3                        normal( void ) const;

            TriMesh<TVertex, TIndex>&   m_mesh;     //!< Parent trimesh.
            std::vector<unsigned int>   m_faces;    //!< Chart faces.
        };

						//! Constructs TriMesh instance.
						TriMesh( Vertices& vertices, Indices& indices );

        //! Returns a DCEL data structure for this mesh.
        Dcel            dcel( void ) const;   

		//! Returns the total number of mesh faces.
		int				faceCount( void ) const;

		//! Returns a mesh face by index.
		Face			face( int index ) const;

	private:

		//! Mesh vertices.
		Vertices&		m_vertices;

		//! Mesh indices.
		Indices&		m_indices;
	};

	// ** TriMesh::TriMesh
	template<typename TVertex, typename TIndex>
	TriMesh<TVertex, TIndex>::TriMesh( Vertices& vertices, Indices& indices ) : m_vertices( vertices ), m_indices( indices )
	{
	}

	// ** TriMesh::dcel
	template<typename TVertex, typename TIndex>
	typename TriMesh<TVertex, TIndex>::Dcel TriMesh<TVertex, TIndex>::dcel( void ) const
	{
		return Dcel::create( m_indices );
	}

	// ** TriMesh::faceCount
	template<typename TVertex, typename TIndex>
	int TriMesh<TVertex, TIndex>::faceCount( void ) const
	{
		return ( int )m_indices.size() / 3;
	}

	// ** TriMesh::faceCount
	template<typename TVertex, typename TIndex>
	typename TriMesh<TVertex, TIndex>::Face TriMesh<TVertex, TIndex>::face( int index ) const
	{
		return Face( m_vertices, m_indices, index );
	}

	// ** TriMesh::Chart::add
	template<typename TVertex, typename TIndex>
	void TriMesh<TVertex, TIndex>::Chart::add( unsigned int index )
	{
        if( std::find( m_faces.begin(), m_faces.end(), index ) != m_faces.end() ) {
            return;
        }

        m_faces.push_back( index );
	}

	// ** TriMesh::Chart::faceCount
	template<typename TVertex, typename TIndex>
    int TriMesh<TVertex, TIndex>::Chart::faceCount( void ) const
	{
        return ( int )m_faces.size();
	}

	// ** TriMesh::Chart::face
	template<typename TVertex, typename TIndex>
    typename TriMesh<TVertex, TIndex>::Face TriMesh<TVertex, TIndex>::Chart::face( int index ) const
	{
        return m_mesh.face( m_faces[index] );
	}

	// ** TriMesh::Chart::normal
	template<typename TVertex, typename TIndex>
	Vec3 TriMesh<TVertex, TIndex>::Chart::normal( void ) const
	{
        Vec3 n( 0.0f, 0.0f, 0.0f );

        for( int i = 0; i < faceCount(); i++ ) {
            n += face( i ).normal();
        }

        n /= ( float )m_faces.size();
        n.normalize();

        return n;
	}

	// ** TriMesh::Face::flatten
	template<typename TVertex, typename TIndex>
	void TriMesh<TVertex, TIndex>::Face::flatten( const Vec3& axis, Vec2& a, Vec2& b, Vec2& c ) const
	{
        Plane plane( axis );

        const Vec3& v0 = vertex( 0 ).position;
        const Vec3& v1 = vertex( 1 ).position;
        const Vec3& v2 = vertex( 2 ).position;
/*
        Vec3 N = normal();
        Vec3 T = v1 - v0; T.normalize();
        Vec3 B = N % T;   B.normalize();

        Matrix4 Tr;
        Tr.setRow( 0, Vec4( T.x, T.y, T.z, 0.0f ) );
        Tr.setRow( 1, Vec4( B.x, B.y, B.z, 0.0f ) );
        Tr.setRow( 2, Vec4( N.x, N.y, N.z, 0.0f ) );
        Tr.setRow( 3, Vec4( 0, 0, 0, 1.0f ) );

        Vec3 r0 = Tr.inversed().rotate( v0 );
        Vec3 r1 = Tr.inversed().rotate( v1 );
        Vec3 r2 = Tr.inversed().rotate( v2 );

        assert( fabs( r0.z - r1.z ) < 0.1f && fabs( r1.z - r2.z ) < 0.1f && fabs( r2.z - r0.z ) < 0.1f );

        a = Vec2( r0.x, r0.y );
        b = Vec2( r1.x, r1.y );
        c = Vec2( r2.x, r2.y );
*/

        Vec3 p0 = plane * v0;
        Vec3 p1 = plane * v1;
        Vec3 p2 = plane * v2;

        if( axis.x == 1.0f ) {
            a = Vec2( p0.y, p0.z );
            b = Vec2( p1.y, p1.z );
            c = Vec2( p2.y, p2.z );
        }
        else if( axis.y == 1.0f ) {
            a = Vec2( p0.x, p0.z );
            b = Vec2( p1.x, p1.z );
            c = Vec2( p2.x, p2.z );
        }
        else {
            a = Vec2( p0.x, p0.y );
            b = Vec2( p1.x, p1.y );
            c = Vec2( p2.x, p2.y );
        }
	}

	// ** TriMesh::Face::normal
	template<typename TVertex, typename TIndex>
	Vec3 TriMesh<TVertex, TIndex>::Face::normal( void ) const
	{
		assert( m_index * 3 + 3 <= m_indices.size() );
		TIndex*	    indices = &m_indices[m_index * 3];
		const Vec3&	v0		= m_vertices[indices[0]].position;
		const Vec3&	v1		= m_vertices[indices[1]].position;
		const Vec3&	v2		= m_vertices[indices[2]].position;

		Vec3 normal = (v1 - v0) % (v2 - v0);
		normal.normalize();

		return normal;
	}

	// ** TriMesh::Face::vertex
	template<typename TVertex, typename TIndex>
	const TVertex& TriMesh<TVertex, TIndex>::Face::vertex( TIndex index ) const
	{
		assert( m_index * 3 + 3 <= m_indices.size() );
		TIndex* indices = &m_indices[m_index * 3];
		return  m_vertices[indices[index]];
	}

	// ** TriMesh::Face::vertex
	template<typename TVertex, typename TIndex>
	TVertex& TriMesh<TVertex, TIndex>::Face::vertex( TIndex index )
	{
		assert( m_index * 3 + 3 <= m_indices.size() );
		TIndex* indices = &m_indices[m_index * 3];
		return  m_vertices[indices[index]];
	}

    //! MeshIndexer helps to build a vertex/index buffer pair from an input stream of vertices.
    template<typename TVertex, typename TCompare, typename TIndex = unsigned short>
    class MeshIndexer {
    public:

        //! Container type to store the indices.
        typedef std::vector<TIndex>     IndexBuffer;

        //! Container type to store the vertices.
        typedef std::vector<TVertex>    VertexBuffer;

        //! Adds a new vertex and returns it's index.
        TIndex                          operator += ( const TVertex& vertex );

        //! Returns the built index buffer.
        const IndexBuffer&              indexBuffer( void ) const;
		IndexBuffer&					indexBuffer( void );

        //! Returns the built vertex buffer.
        const VertexBuffer&             vertexBuffer( void ) const;
		VertexBuffer&					vertexBuffer( void );

    private:

        //! Container type to store added vertices.
        typedef std::map<TVertex, TIndex, TCompare>   VertexCache;

        VertexCache                     m_cache;            //!< Vertices added to an indexer.
        VertexBuffer                    m_vertexBuffer;     //!< Built vertex buffer.
        IndexBuffer                     m_indexBuffer;      //!< Built index buffer.
    };

    // ** MeshIndexer::indexBuffer
    template<typename TVertex, typename TCompare, typename TIndex>
    const typename MeshIndexer<TVertex, TCompare, TIndex>::IndexBuffer& MeshIndexer<TVertex, TCompare, TIndex>::indexBuffer( void ) const
    {
        return m_indexBuffer;
    }

    // ** MeshIndexer::indexBuffer
    template<typename TVertex, typename TCompare, typename TIndex>
    typename MeshIndexer<TVertex, TCompare, TIndex>::IndexBuffer& MeshIndexer<TVertex, TCompare, TIndex>::indexBuffer( void )
    {
        return m_indexBuffer;
    }

    // ** MeshIndexer::vertexBuffer
    template<typename TVertex, typename TCompare, typename TIndex>
    const typename MeshIndexer<TVertex, TCompare, TIndex>::VertexBuffer& MeshIndexer<TVertex, TCompare, TIndex>::vertexBuffer( void ) const
    {
        return m_vertexBuffer;
    }

    // ** MeshIndexer::vertexBuffer
    template<typename TVertex, typename TCompare, typename TIndex>
    typename MeshIndexer<TVertex, TCompare, TIndex>::VertexBuffer& MeshIndexer<TVertex, TCompare, TIndex>::vertexBuffer( void )
    {
        return m_vertexBuffer;
    }

    // ** MeshIndexer::add
    template<typename TVertex, typename TCompare, typename TIndex>
    TIndex MeshIndexer<TVertex, TCompare, TIndex>::operator += ( const TVertex& vertex )
    {
        VertexCache::iterator i   = m_cache.find( vertex );
        TIndex                idx = 0;

        if( i != m_cache.end() ) {
            idx = i->second;
        } else {
            idx = m_vertexBuffer.size();
            m_cache[vertex] = idx;
            m_vertexBuffer.push_back( vertex );
        }

        m_indexBuffer.push_back( idx );
        return idx;
    }

FOO_END_NAMESPACE

#endif  /*  !defined( __Foo_Mesh_H__ )  */
