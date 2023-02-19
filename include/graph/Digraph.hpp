#pragma once
#include <string>
#include <map>
#include <set>
#include <functional>
#include <random>
#include <limits>

namespace tudumper::graph {
    template <typename node_data_type, typename edge_data_type>
    class Digraph {
    public:
        using id_t = uint32_t;
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
            edge_t(id_t id, id_t from, id_t to):id(id), from(from), to(to) {}
            edge_t(id_t id, id_t from, id_t to,
                   std::unique_ptr<edge_data_type> data): id(id), from(from), to(to), data(std::move(data)) {}
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
            std::set<id_t> edge_ids;
            std::unique_ptr<node_data_type> data;
            node_t() = default;
            node_t(node_t&& other)  noexcept {
                id = other.id;
                edge_ids = std::move(other.edge_ids);
                data = std::move(other.data);
            }
            explicit node_t(id_t id) : id(id) {}
            node_t(id_t id, std::unique_ptr<node_data_type> data):node_t(id), data(std::move(data)) {}
            node_t& operator==(node_t&& other) {
                if(this == &other) {
                    return *this;
                }
                id = other.id;
                edge_ids = std::move(other.edge_ids);
                data = std::move(other.data);
            }
        };

        Digraph() {
            m_edge_data_callback = [](id_t from_node, id_t to_node) ->std::unique_ptr<edge_data_type> {
                return nullptr;
            };
        }

        void add_node(node_t && node) {
            m_nodes_map.try_emplace(node.id, std::move(node));
        }
        void add_edge(edge_t && edge) {
            m_nodes_map[edge.from].id = edge.from;
            m_nodes_map[edge.from].edge_ids.insert(edge.id);
            m_nodes_map[edge.to].id = edge.to;
            m_nodes_map[edge.to].edge_ids.insert(edge.id);
            m_edges_map.try_emplace(edge.id, std::move(edge));
        }

        void remove_edge(id_t edge_id) {
            if (auto iter = m_edges_map.find(edge_id); iter != m_edges_map.end()) {
                // 删除边时，删除from节点和to节点中的对应边记录，然后删除边本身
                edge_t const& edge = iter->second;
                m_nodes_map[edge.from].edge_ids.erase(edge_id);
                m_nodes_map[edge.to].edge_ids.erase(edge_id);
                m_edges_map.erase(iter);
            }
        }
        std::tuple<std::vector<id_t>, std::vector<id_t>> node_ids_to_and_from(id_t node_id) {
            std::vector<id_t> to_node_ids;
            std::vector<id_t> from_node_ids;
            if (auto iter = m_nodes_map.find(node_id); iter != m_nodes_map.end()) {
                for(auto const& edge_id : iter->second.edge_ids) {
                    auto const& edge = m_edges_map.at(edge_id);
                    if (edge.to == node_id) {
                        to_node_ids.push_back(edge.from);
                    }
                    if (edge.from == node_id) {
                        from_node_ids.push_back(edge.to);
                    }
                }
            }
            return { to_node_ids, from_node_ids };
        }
        void remove_node(id_t node_id) {
            std::set<id_t> delete_edge_ids = m_nodes_map[node_id].edge_ids;
            auto [ to_node_ids, from_node_ids ] = node_ids_to_and_from(node_id);

            for (auto const& delete_edge_id : delete_edge_ids) {
                remove_edge(delete_edge_id);
            }

            static std::default_random_engine e(314159);
            static std::uniform_int_distribution<id_t> u(0, std::numeric_limits<id_t>::max());
            for(auto const& to_node_id : to_node_ids) {
                for(auto const& from_node_id : from_node_ids) {
                    std::hash<std::string> hasher{};
                    id_t id = hasher(std::to_string(to_node_id ^ from_node_id ^ node_id ^ u(e)));
                    // 获取edge data
                    auto edge_data = m_edge_data_callback(to_node_id, from_node_id);
                    add_edge(edge_t{id, to_node_id, from_node_id, std::move(edge_data)});
                }
            }

            m_nodes_map.erase(node_id);
        }

        std::map<id_t, node_t> const& nodes_map() const {
            return m_nodes_map;
        }
        std::map<id_t, edge_t> const& edges_map() const {
            return m_edges_map;
        }

    private:
        std::map<id_t, node_t> m_nodes_map;
        std::map<id_t, edge_t> m_edges_map;
        // remove node时可能出现创建新的edge，这个回调函数用于获取新edge的data `data m_edge_data_callback(from, to)`
        std::function<std::unique_ptr<edge_data_type>(id_t, id_t)> m_edge_data_callback;
    };


}