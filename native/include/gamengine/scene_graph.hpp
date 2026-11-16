#pragma once

#include <unordered_map>
#include <cstdint>
#include <optional>

namespace GAMENGINE {

using NodeID = std::uint64_t;

struct Vec3 {
    double x = 0;
    double y = 0;
    double z = 0;
};

struct Transform {
    Vec3 position;
    Vec3 rotation;
    Vec3 scale{1, 1, 1};
};

class SceneGraph {
public:
    struct Node {
        NodeID id;
        NodeID parent = 0;
        Transform local;
    };

    NodeID create_node();

    bool remove_node(NodeID id);

    bool set_parent(
        NodeID child,
        NodeID parent
    );

    bool set_transform(
        NodeID id,
        const Transform& transform
    );

    std::optional<Transform> local_transform(
        NodeID id
    ) const;

    std::optional<Transform> world_transform(
        NodeID id
    ) const;

    std::size_t size() const {
        return nodes.size();
    }

private:
    std::unordered_map<NodeID, Node> nodes;
    NodeID nextID = 1;

    bool is_ancestor(NodeID ancestor, NodeID node) const;
};

}
