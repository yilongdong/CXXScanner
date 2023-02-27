#pragma once
#include <string>
#include <map>
#include <set>
#include <functional>
#include <random>
#include <limits>
#include <utility>
#include "utility/log.h"

namespace CXXScanner::graph {
    struct Empty {};
    template <typename id_t, typename node_data_type, typename edge_data_type>
    class Digraph {
    public:
//        using id_t = id_t;
        using ID = id_t;
        struct edge_t {
            id_t id{};
            id_t from{};
            id_t to{};
            std::unique_ptr<edge_data_type> data;
            edge_t() = default;
            edge_t(edge_t&& other)  noexcept {
                id = other.id;
                from = other.from;
                to = other.to;
                data = std::move(other.data);
            }
            edge_t(id_t id, id_t from, id_t to):id(std::move(id)), from(std::move(from)), to(std::move(to)) {}
            edge_t(id_t id, id_t from, id_t to, std::unique_ptr<edge_data_type> data)
                : id(std::move(id)), from(std::move(from)), to(std::move(to)), data(std::move(data)) {}
            edge_t& operator==(edge_t&& other) {
                if(this == &other) {
                    return *this;
                }
                id = other.id;
                from = other.from;
                to = other.to;
                data = std::move(other.data);
                return *this;
            }
        };
        struct node_t {
            id_t id{};
            std::set<id_t> in_edge_ids;
            std::set<id_t> out_edge_ids;
            std::unique_ptr<node_data_type> data;
            node_t() = default;
            node_t(node_t&& other)  noexcept {
                id = other.id;
                in_edge_ids = std::move(other.in_edge_ids);
                out_edge_ids = std::move(other.out_edge_ids);
                data = std::move(other.data);
            }
            explicit node_t(id_t id) : id(std::move(id)) {}
            node_t(id_t id, std::unique_ptr<node_data_type> data):id(std::move(id)), data(std::move(data)) {}
            node_t& operator==(node_t&& other) {
                if(this == &other) {
                    return *this;
                }
                id = other.id;
                in_edge_ids = std::move(other.in_edge_ids);
                out_edge_ids = std::move(other.out_edge_ids);
                data = std::move(other.data);
            }
        };

        Digraph() {
            m_edge_data_callback = [](id_t const& from_node, id_t const& to_node) ->std::unique_ptr<edge_data_type> {
                return nullptr;
            };
            m_new_edge_id_callback = [](id_t const& from, id_t const& to) -> id_t {
                return fmt::format("from {} to {}", from, to);
            };
        }

        void add_node(node_t && node) {
            m_nodes_map.try_emplace(node.id, std::move(node));
        }
        void add_edge(edge_t && edge) {
            m_nodes_map.at(edge.from).id = edge.from;
            m_nodes_map.at(edge.from).out_edge_ids.insert(edge.id);
            m_nodes_map.at(edge.to).id = edge.to;
            m_nodes_map.at(edge.to).in_edge_ids.insert(edge.id);
            m_edges_map.try_emplace(edge.id, std::move(edge));
        }

        void remove_edge(id_t const& edge_id) {
            if (auto iter = m_edges_map.find(edge_id); iter != m_edges_map.end()) {
                edge_t const& edge = iter->second;

                m_nodes_map.at(edge.from).out_edge_ids.erase(edge_id);
                m_nodes_map.at(edge.to).in_edge_ids.erase(edge_id);
                m_edges_map.erase(iter);
            }
        }


        ///
        /// \param node_id
        /// \param with_merge_edge 为true时，在删除节点时会合并依赖路径，但是耗时严重
        void remove_node(id_t const& node_id, bool with_merge_edge = false) {
            if(m_nodes_map.find(node_id) == m_nodes_map.end()) {
                return;
            }

            auto const out_edge_ids = m_nodes_map.at(node_id).out_edge_ids;
            auto const in_edge_ids = m_nodes_map.at(node_id).in_edge_ids;

            // 这段代码非常耗时
            if(with_merge_edge) {
                std::vector<id_t> from_node_ids, to_node_ids;
                std::transform(out_edge_ids.begin(), out_edge_ids.end(), std::back_inserter(to_node_ids),
                               [this](id_t edge_id) {
                                   return m_edges_map.at(edge_id).to;
                               });
                std::transform(in_edge_ids.begin(), in_edge_ids.end(), std::back_inserter(from_node_ids),
                               [this](id_t edge_id) {
                                   return m_edges_map.at(edge_id).from;
                               });

                for (auto const &from_node_id: from_node_ids) {
                    for (auto const &to_node_id: to_node_ids) {
                        id_t edge_id = m_new_edge_id_callback(from_node_id, to_node_id);
                        add_edge(edge_t{edge_id, from_node_id, to_node_id});
                    }
                }
            }

            for (auto const delete_in_edge_id : in_edge_ids) {
                remove_edge(delete_in_edge_id);
            }
            for (auto const delete_out_edge_id : out_edge_ids) {
                remove_edge(delete_out_edge_id);
            }

            m_nodes_map.erase(node_id);
        }

        std::map<id_t, node_t> const& nodes_map() const {
            return m_nodes_map;
        }
        std::map<id_t, edge_t> const& edges_map() const {
            return m_edges_map;
        }
        std::function<id_t(id_t const&, id_t const&)>& new_edge_id_callback() {
            return m_new_edge_id_callback;
        }
    private:
        std::map<id_t, node_t> m_nodes_map;
        std::map<id_t, edge_t> m_edges_map;
        // remove node时可能出现创建新的edge，这个回调函数用于获取新edge的data `data m_edge_data_callback(from, to)`
        std::function<std::unique_ptr<edge_data_type>(id_t, id_t)> m_edge_data_callback;
        std::function<id_t(id_t const&, id_t const&)> m_new_edge_id_callback;
    };


}