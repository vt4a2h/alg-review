#include <iostream>

#include "node.h"
#include "graph.h"

// Route between two nodes
namespace rbn
{
    bool routeExists(g::IntGraph const& graph, g::IntVertex::Ptr const& start, g::IntVertex::Ptr const& end)
    {
        if (start == end)
            return true;

        // Just for sure mark all nodes as unvisited
        for (auto && n : graph.verticies())
            n.second->setState(g::IntVertex::Unvisited);

        std::queue<g::IntVertex::Ptr> verteciesQueue;

        start->setState(g::IntVertex::Visiting);
        verteciesQueue.push(start);

        g::IntVertex::Ptr visitingVertex;
        while (!verteciesQueue.empty()) {
            visitingVertex = verteciesQueue.front();
            verteciesQueue.pop();

            if (visitingVertex) {
                for (auto && v : visitingVertex->linkedVertices()) {
                    if (v->state() == g::IntVertex::Unvisited) {
                        if (v == end)
                            return true;
                        else {
                            v->setState(g::IntVertex::Visiting);
                            verteciesQueue.push(v);
                        }
                    }
                }

                visitingVertex->setState(g::IntVertex::Visited);
            }
        }

        return false;
    }
}

// Minimal tree (create minimal tree from sorted array with unique elements)
namespace mt
{

}

int main(int /*argc*/, char */*argv*/[])
{
    // 1
//    g::IntGraph graph;
//    auto first = graph.addVertex(1);
//    auto second = graph.addVertex(2);
//    auto third = graph.addVertex(3);
//    auto fourth = graph.addVertex(4);

//    graph.addEdge(1, 2);
//    graph.addEdge(1, 3);
//    graph.addEdge(2, 3);
//    graph.addEdge(3, 4);

//    std::cout << std::boolalpha << rbn::routeExists(graph, fourth, first) << std::endl;

//    graph.dump("/home/vt4a2h/Projects/alg/graph.dot");
//    system("cd /home/vt4a2h/Projects/alg/ && dot graph.dot -Tsvg > graph.svg ");

    // 2


    return 0;
}
