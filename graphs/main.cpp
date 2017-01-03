#include <iostream>
#include <list>
#include <numeric>

#include <boost/optional.hpp>

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
namespace bst
{
    using namespace Tree;

    namespace details
    {
        IntNodePtr createMinimalBSTImpl(std::vector<int> const& array, int from, int to)
        {
            if (to < from)
                return nullptr;

            int mid = (from + to) / 2;
            auto node = std::make_shared<IntNode>(array.at(mid));
            node->setLeftChild(createMinimalBSTImpl(array, from, mid - 1));
            node->setRightChild(createMinimalBSTImpl(array, mid + 1, to));
            return node;
        }
    }

    IntNodePtr createMinimalBST(std::vector<int> const& array)
    {
        return details::createMinimalBSTImpl(array, 0, array.size() - 1);
    }
}

// List of depth. For binary tree, algorithm to create a linked list of all nodes on each depth
// using preorder traversal algorithm
namespace lod
{
    using namespace Tree;
    using NodesList = std::list<IntNodePtr>;
    using NodesListsArray = std::vector<NodesList>;

    namespace details
    {
        void createLevelLinkedListImpl(IntNodePtr const& root, NodesListsArray & lists, std::size_t level)
        {
            if (!root)
                return;

            // Check if level not in the list
            if (lists.size() == level) {
                lists.push_back(NodesList());
                lists.back().push_back(root);
            } else
                lists[level].push_back(root);

            createLevelLinkedListImpl(root->mLeftChild, lists, level + 1);
            createLevelLinkedListImpl(root->mRightChild, lists, level + 1);
        }
    }

    NodesListsArray createLevelLinkedList(IntNodePtr const& root)
    {
        NodesListsArray lists;
        details::createLevelLinkedListImpl(root, lists, 0);
        return lists;
    }
}

// Check if tree is balanced (difference between subtrees no more than one)
namespace bt
{
    using namespace Tree;

    namespace details
    {
        static const int ERROR_TAG = std::numeric_limits<int>::min();

        int checkHeight(IntNodePtr const& root)
        {
            if (!root)
                return -1;

            int leftHeight = checkHeight(root->mLeftChild);
            if (leftHeight == ERROR_TAG)
                return ERROR_TAG; // Propagate error to the top

            int rightHeight = checkHeight(root->mRightChild);
            if (rightHeight == ERROR_TAG)
                return ERROR_TAG; // Propagate error to the top

            if (std::abs(leftHeight - rightHeight) > 1)
                return ERROR_TAG; // Pass error back
            else
                return std::max(leftHeight, rightHeight) + 1;
        }
    }

    bool isBalanced(IntNodePtr const& root)
    {
        return details::checkHeight(root) != details::ERROR_TAG;
    }
}

// Check if tree is a valid BST
namespace vbst
{
    using namespace Tree;

    namespace details
    {
        using Int = boost::optional<int>;
        bool checkBSTImpl(IntNodePtr const& n, Int min = Int(), Int max = Int())
        {
            if (!n)
                return true;

            if ((min && n->mKey <= min) || (max && n->mKey > max))
                return false;

            if (!checkBSTImpl(n->mLeftChild, min, n->mKey) ||
                !checkBSTImpl(n->mRightChild, n->mKey, max))
                return false;

            return true;
        }
    }

    bool checkBST(IntNodePtr const& root)
    {
        return details::checkBSTImpl(root);
    }
}

// Successor (find "next" node)
namespace sr
{
    using namespace Tree;

    namespace details
    {
        IntNodePtr leftMostChild(IntNodePtr node)
        {
            if (!node)
                return nullptr;

            while (node->mLeftChild)
                node = node->mLeftChild;

            return node;
        }
    }

    IntNodePtr successor(IntNodePtr const& node)
    {
        if (!node)
            return nullptr;

        // If there are right child just return leftmost child shich will be the next node
        if (node->mRightChild)
            return details::leftMostChild(node->mRightChild);
        else {
            auto current = node;
            auto parent  = node->parent();

            // Go up until we're on left node instead of right
            while (parent && parent->mLeftChild != current) {
                current = parent;
                parent  = parent->parent();
            }

            return parent;
        }
    }
}

// Topological sort.
// Given list of projects and list of dependencies (pairs second depends on first). Find bild order
// or error is there is no valid build order.
namespace ts
{
    using namespace g;
    using ProjectsVector     = std::vector<std::string>;
    using DependenciesVector = std::vector<std::pair<std::string, std::string>>;

    namespace details
    {
        using Project = Vertex<std::string>;
        using OrderedProjects = std::list<Project::Ptr>;

        bool doDFS(Project::Ptr const& project, OrderedProjects & projects)
        {
            // Cycle detected
            if (project->state() == Project::Visiting)
                return false;

            if (project->state() == Project::Unvisited) {
                project->setState(Project::Visiting);

                for (auto && child : project->linkedVertices())
                    if (!doDFS(child, projects))
                        return false; // Propagate cycle

                project->setState(Project::Visited);
                projects.push_front(project);
            }

            return true;
        }

        decltype(auto) fillGraph(ProjectsVector const& projects,
                                 DependenciesVector const& dependencies)
        {
            auto graph = std::make_unique<StrGraph>();

            for (auto && project : projects)
                graph->addVertex(project);

            // Second depends on first
            for (auto && edge : dependencies)
                graph->addEdge(edge.second, edge.first);

            return graph;
        }
    }

    ProjectsVector orderedProjects(ProjectsVector     const& projects,
                                   DependenciesVector const& dependencies)
    {
        auto graph = details::fillGraph(projects, dependencies);

        details::OrderedProjects op;
        for (auto && project : graph->verticies())
            if (project.second->state() == details::Project::Unvisited)
                if (!details::doDFS(project.second, op))
                    return ProjectsVector();

        ProjectsVector result(op.size());
        std::transform(op.begin(), op.end(), result.begin(), [](auto && p) { return p->data(); });
        return result;
    }
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
//    try {
//        std::vector<int> v {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
//        auto tree = bst::createMinimalBST(v);
//        tree->dump("/home/vt4a2h/Projects/alg/graph.dot");
//        system("cd /home/vt4a2h/Projects/alg/ && dot graph.dot -Tsvg > graph.svg ");
//    } catch (std::exception const& e) {
//        std::cout << e.what() << std::endl;
//    }

    // 3
//    try {
//        std::vector<int> v {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
//        auto tree = bst::createMinimalBST(v);

//        auto lists = lod::createLevelLinkedList(tree);
//        for (auto && list : lists) {
//            for (auto && n : list) {
//                std::cout << n->mKey << "\t";
//            }
//            std::cout << std::endl;
//        }
//    } catch (std::exception const& e) {
//        std::cout << e.what() << std::endl;
//    }

    // 4
//    try {
//        std::vector<int> v {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
//        auto tree = bst::createMinimalBST(v);

//        // Must be balanced
//        std::cout << "Is balanced: " << std::boolalpha << bt::isBalanced(tree) << std::endl;

//        // Disbalance somehow
//        Tree::IntNodePtr min = tree->mLeftChild;
//        while (min->mLeftChild)
//            min = min->mLeftChild;

//        std::cout << "Min element: " << min->mKey << std::endl;

//        min->makeLeftChild(-1)->makeLeftChild(-2);

//        std::cout << "Is balanced: " << std::boolalpha << bt::isBalanced(tree) << std::endl;
//    } catch (std::exception const& e) {
//        std::cout << e.what() << std::endl;
//    }

    // 5
//    try {
//        std::vector<int> v {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
//        auto tree = bst::createMinimalBST(v);

//        std::cout << "Is BST: " << std::boolalpha << vbst::checkBST(tree) << std::endl;

//        Tree::IntNodePtr min = tree->mLeftChild;
//        while (min->mLeftChild)
//            min = min->mLeftChild;
//        min->makeLeftChild(100)->makeLeftChild(200);

//        std::cout << "Is BST: " << std::boolalpha << bt::isBalanced(tree) << std::endl;
//    } catch (std::exception const& e) {
//        std::cout << e.what() << std::endl;
//    }

    // 6
//    try {
//        std::vector<int> v {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
//        auto tree = bst::createMinimalBST(v);

//        Tree::IntNodePtr min = tree->mLeftChild;
//        while (min->mLeftChild)
//            min = min->mLeftChild;

//        // 1 -> 2
//        std::cout << min->mKey << " -> " << sr::successor(min)->mKey << std::endl;

//        // 2 -> 3
//        auto two = min->parent();
//        std::cout << two->mKey << " -> " << sr::successor(two)->mKey << std::endl;

//        // 3 -> 4
//        auto three = sr::successor(min)->mRightChild;
//        std::cout << three->mKey << " -> " << sr::successor(three)->mKey << std::endl;

//    } catch (std::exception const& e) {
//        std::cout << e.what() << std::endl;
//    }

    // 7
    ts::ProjectsVector pv {"a", "b", "c", "d", "e", "f"};
    ts::DependenciesVector dv {{"a", "d"}, {"f", "b"}, {"b", "d"}, {"f", "a"}, {"d", "c"}};
    try {
        auto orderedProjects = ts::orderedProjects(pv, dv);
        for (auto && p : orderedProjects)
            std::cout << p << "\t";
        std::cout << std::endl;
    } catch (std::exception const& e) {
        std::cout << e.what() << std::endl;
    }

    return 0;
}
