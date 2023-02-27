#include "reporter/reporter.h"
#include <algorithm>
#include <fmt/format.h>
#include <fstream>

CXXScanner::reporter::Reporter::Reporter() {
    m_filterRegexes = {
            std::regex("^/Library/Developer/CommandLineTools/SDKs/.*"),
            std::regex(R"(^/usr/local/bin/\.\./include/c\+\+/.*)"),
            std::regex("^/usr/local/include/gflags/.*"),
            std::regex("^/usr/local/include/llvm/.*"),
            std::regex("^/usr/local/include/llvm-c/.*"),
            std::regex("^/usr/local/include/clang/.*"),
            std::regex("^/usr/local/include/clang-c/.*"),
            std::regex("^/usr/local/include/gtest/.*"),
            std::regex("^/Users/dongyilong/projects/Clion/TUDumper/build/.*"),
            std::regex("^/opt/homebrew/.*"),
    };
}

CXXScanner::reporter::Reporter &
CXXScanner::reporter::Reporter::loadInclusion(const CXXScanner::model::CXXInclusion &inclusion) {
    InclusionID fromNodeId = std::to_string(std::filesystem::hash_value(inclusion.loc.path));
    InclusionID toNodeId = std::to_string(std::filesystem::hash_value(inclusion.filename));
    auto fromNodeData = std::make_unique<InclusionNodeData>(inclusion.loc.path);
    auto toNodeData = std::make_unique<InclusionNodeData>(inclusion.filename);
    InclusionNode fromNode{fromNodeId, std::move(fromNodeData) };
    InclusionNode toNode{toNodeId, std::move(toNodeData) };
    m_digraph.add_node(std::move(fromNode));
    m_digraph.add_node(std::move(toNode));
    InclusionID edgeId = m_digraph.new_edge_id_callback()(fromNodeId, toNodeId);
    m_digraph.add_edge(InclusionEdge(edgeId, fromNodeId, toNodeId));
    return *this;
}

CXXScanner::reporter::Reporter &CXXScanner::reporter::Reporter::setName(const std::string &name) {
    m_name = name;
    return *this;
}

CXXScanner::reporter::Reporter &CXXScanner::reporter::Reporter::savePath(std::filesystem::path filename) {
    m_savePath = std::move(filename);
    return *this;
}

void CXXScanner::reporter::Reporter::doReport() {
    std::set<CXXScanner::reporter::Reporter::InclusionID> remove_node_ids;
    for(auto const& [id, node] : m_digraph.nodes_map()) {
        if (node.data == nullptr) continue;
        std::string filename = node.data->path.u8string();
        bool shouldRemove = std::any_of(m_filterRegexes.begin(), m_filterRegexes.end(), [&filename](auto const& regex) {
            return std::regex_match(filename, regex);
        });
        if (shouldRemove) {
            remove_node_ids.insert(id);
        }
    }
    for (auto const& id : remove_node_ids) {
        bool with_merge_edge = false;
        m_digraph.remove_node(id, with_merge_edge);
    }
    std::string nodesContent;
    for(auto const& [id, node] : m_digraph.nodes_map()) {
        nodesContent += fmt::format("\t{}[label=\"{}\"];\n", id, node.data->path.u8string());
    }
    std::string edgesContent;
    for(auto const& [id, edge] : m_digraph.edges_map()) {
        edgesContent += fmt::format("\t{}->{};\n", edge.from, edge.to);
    }
    std::ofstream fout(m_savePath.u8string());
    fout << fmt::format("digraph \"{}\" {{\n{}{}}}", m_name, nodesContent, edgesContent);
    fout.close();
    LOG_INFO("报告输出到{}", m_savePath.u8string());
}
