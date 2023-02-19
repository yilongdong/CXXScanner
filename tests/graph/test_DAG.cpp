#include <gtest/gtest.h>
#include "graph/Digraph.hpp"
#include <fstream>
#include <fmt/format.h>

struct Node {
    std::string label;
};

struct Edge {
    std::string label;
};

TEST(dag, dag_add_node) {
    using graph_t = typename tudumper::graph::Digraph<std::string, Node, Edge>;
    using edge_t = typename graph_t::edge_t;
    using node_t = typename graph_t::node_t;
    graph_t digraph;
    digraph.add_node(node_t("11"));
    digraph.add_node(node_t{"22"});
    digraph.add_node(node_t{"33"});

    digraph.add_edge(edge_t{"10","22","33"});
    digraph.add_edge(edge_t{"20","11","33"});
    digraph.add_edge(edge_t{"30","33","44"});
    digraph.add_edge(edge_t{"40","33","55"});
    digraph.add_edge(edge_t{"50","33","66"});
    digraph.add_edge(edge_t{"60","55","66"});
    digraph.add_edge(edge_t{"70","66","77"});
    digraph.remove_node("33");
    std::ofstream fout("./test_dag.dot");
    std::string result;
    for(auto const& [id, node] : digraph.nodes_map()) {
        result += fmt::format("\t{};\n", node.id);
    }
    for(auto const& [id, edge] : digraph.edges_map()) {
        result += fmt::format("\t{}->{};\n", edge.from, edge.to);
    }

    fout << fmt::format("digraph G {{\n{}}}", result);
    fout.close();
}


int main(int argc, char* argv[]) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}