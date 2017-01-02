#pragma once

#include <memory>
#include <vector>
#include <map>
#include <queue>
#include <fstream>
#include <algorithm>
#include <assert.h>

namespace g
{
    template <class T>
    class Vertex
    {
    public: // Types
        using Ptr = std::shared_ptr<Vertex<T>>;
        using WPtr = std::weak_ptr<Vertex<T>>;
        using LinkedVerticesW = std::vector<WPtr>;
        using LinkedVertices  = std::vector<Ptr>;

        enum State { Unvisited, Visited, Visiting };

    public: // Methods
        Vertex(T const& v) : m_data(v), m_state(Unvisited) {}

        T data() const { return m_data; }
        LinkedVertices linkedVertices() const
        {
            LinkedVertices result(m_linkedVerticies.size());
            std::transform(m_linkedVerticies.begin(), m_linkedVerticies.end(),
                           result.begin(), [](auto &&v) { return v.lock(); });
            return result;
        }

        void linkTo(Ptr const& vertex) { m_linkedVerticies.push_back(vertex); }

        State state() const { return m_state; }
        void setState(State const& state) { m_state = state; }

        friend std::ostream & operator << (std::ostream & out, Ptr const & vertex)
        {
            assert(vertex);
            for (auto && v : vertex->linkedVertices())
                out << vertex->data() << " -> " << v->data() << ";\n";

            return out;
        }

    private: // Data
        T m_data;
        State m_state;
        LinkedVerticesW m_linkedVerticies;
    };

    using IntVertex = Vertex<int>;

    template <class T, bool directed = true>
    class Graph
    {
    public: // Types
        using VerteciesMap = std::map<T, typename Vertex<T>::Ptr>;

    public: // Methods
        typename Vertex<T>::Ptr addVertex(T const& v)
        {
            auto it = m_vertices.find(v);
            if (it == m_vertices.end())
                return m_vertices[v] = std::make_shared<Vertex<T>>(v);
            else
                throw std::invalid_argument("Vertex already exists.");
        }

        void addEdge(T const& from, T const& to)
        {
            auto fromIt = m_vertices.find(from);
            auto toIt   = m_vertices.find(to);

            if (fromIt == m_vertices.end() || toIt == m_vertices.end())
                throw std::invalid_argument("Cannot make an edge between non-existing vertices.");

            if (directed)
                fromIt->second->linkTo(toIt->second);
            else
                throw std::logic_error("Is not implemented yet.");
        }

        VerteciesMap verticies() const { return m_vertices; }

        void dump(std::string const& path)
        {
            std::ofstream out(path, std::ofstream::out);

            out << "digraph {\n";
            for (auto && v : m_vertices)
                out << v.second;
            out << "}\n";

            out.close();
        }

    private: // Data
        VerteciesMap m_vertices;
    };

    using IntGraph = Graph<int>;
}
