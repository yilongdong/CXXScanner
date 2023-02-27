#pragma once
#include <string>
#include <utility>
#include <vector>
#include "model/CXXInclusion.h"
#include "graph/Digraph.hpp"
#include <regex>
namespace CXXScanner::reporter {
    class Reporter {
    private:

        struct InclusionNodeData {
            explicit InclusionNodeData(std::filesystem::path filename) : path(std::move(filename)) {}
            std::filesystem::path path;
        };

        using InclusionGraph = CXXScanner::graph::Digraph<std::string, InclusionNodeData, CXXScanner::graph::Empty>;
        using InclusionNode = InclusionGraph::node_t;
        using InclusionEdge = InclusionGraph::edge_t;
        using InclusionID = InclusionGraph::ID;

        std::string m_name;
        std::filesystem::path m_savePath;
        InclusionGraph m_digraph;
        std::vector<std::regex> m_filterRegexes;
    public:
        Reporter();
        template<typename Iter> Reporter& loadInclusions(Iter first, Iter last) {
            std::for_each(first, last, [this](CXXScanner::model::CXXInclusion const& inclusion) {
                this->loadInclusion(inclusion);
            });
            return *this;
        }
        Reporter& loadInclusion(CXXScanner::model::CXXInclusion const& inclusion);
        Reporter& setName(std::string const& name);
        Reporter& savePath(std::filesystem::path filename);

        void doReport();
    };
}
