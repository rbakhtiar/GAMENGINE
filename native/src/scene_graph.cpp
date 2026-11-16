#include <gamengine/scene_graph.hpp>

namespace GAMENGINE {

NodeID SceneGraph::create_node() {
    NodeID id = nextID++;

    nodes.emplace(
        id,
        Node{id, 0, Transform{}}
    );

    return id;
}

bool SceneGraph::remove_node(NodeID id) {
    if (!nodes.contains(id))
        return false;

    for (auto& [childID, node] : nodes) {
        if (node.parent == id)
            node.parent = 0;
    }

    nodes.erase(id);
    return true;
}

bool SceneGraph::is_ancestor(
    NodeID ancestor,
    NodeID node
) const {
    while (node != 0) {
        if (node == ancestor)
            return true;

        auto it = nodes.find(node);

        if (it == nodes.end())
            return false;

        node = it->second.parent;
    }

    return false;
}

bool SceneGraph::set_parent(
    NodeID child,
    NodeID parent
) {
    if (!nodes.contains(child))
        return false;

    if (parent != 0 && !nodes.contains(parent))
        return false;

    if (child == parent)
        return false;

    if (parent != 0 && is_ancestor(child, parent))
        return false;

    nodes[child].parent = parent;
    return true;
}

bool SceneGraph::set_transform(
    NodeID id,
    const Transform& transform
) {
    auto it = nodes.find(id);

    if (it == nodes.end())
        return false;

    it->second.local = transform;
    return true;
}

std::optional<Transform> SceneGraph::local_transform(
    NodeID id
) const {
    auto it = nodes.find(id);

    if (it == nodes.end())
        return std::nullopt;

    return it->second.local;
}

std::optional<Transform> SceneGraph::world_transform(
    NodeID id
) const {
    auto it = nodes.find(id);

    if (it == nodes.end())
        return std::nullopt;

    Transform result = it->second.local;

    NodeID parent = it->second.parent;

    while (parent != 0) {
        auto p = nodes.find(parent);

        if (p == nodes.end())
            return std::nullopt;

        const Transform& t = p->second.local;

        result.position.x =
            t.position.x + result.position.x * t.scale.x;

        result.position.y =
            t.position.y + result.position.y * t.scale.y;

        result.position.z =
            t.position.z + result.position.z * t.scale.z;

        result.rotation.x += t.rotation.x;
        result.rotation.y += t.rotation.y;
        result.rotation.z += t.rotation.z;

        result.scale.x *= t.scale.x;
        result.scale.y *= t.scale.y;
        result.scale.z *= t.scale.z;

        parent = p->second.parent;
    }

    return result;
}

}
