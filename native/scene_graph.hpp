
#pragma once

#include "terrain.hpp"

#include <algorithm>
#include <memory>
#include <string>
#include <vector>

namespace gamengine {

struct SceneTransform {
    Vec3 position{0, 0, 0};
    Vec3 rotation{0, 0, 0};
    Vec3 scale{1, 1, 1};
};

class SceneNode {
public:
    explicit SceneNode(std::string name)
        : name_(std::move(name)) {}

    SceneNode& createChild(std::string name) {
        auto child = std::make_unique<SceneNode>(
            std::move(name)
        );

        child->parent_ = this;

        children_.push_back(std::move(child));

        return *children_.back();
    }

    bool removeChild(SceneNode* node) {
        auto it = std::find_if(
            children_.begin(),
            children_.end(),
            [node](const auto& child) {
                return child.get() == node;
            }
        );

        if (it == children_.end())
            return false;

        (*it)->parent_ = nullptr;
        children_.erase(it);

        return true;
    }

    Vec3 worldPosition() const {
        Vec3 result = transform_.position;

        const SceneNode* current = parent_;

        while (current) {
            result.x += current->transform_.position.x;
            result.y += current->transform_.position.y;
            result.z += current->transform_.position.z;

            current = current->parent_;
        }

        return result;
    }

    SceneTransform& transform() {
        return transform_;
    }

    const SceneTransform& transform() const {
        return transform_;
    }

    const std::string& name() const {
        return name_;
    }

    void setName(std::string name) {
        name_ = std::move(name);
    }

    SceneNode* parent() const {
        return parent_;
    }

    const std::vector<std::unique_ptr<SceneNode>>&
    children() const {
        return children_;
    }

private:
    std::string name_;
    SceneTransform transform_;

    SceneNode* parent_ = nullptr;

    std::vector<std::unique_ptr<SceneNode>> children_;
};

class SceneGraph {
public:
    SceneGraph()
        : root_("GAMENGINE_ROOT") {}

    SceneNode& root() {
        return root_;
    }

    void clear() {
        root_ = SceneNode("GAMENGINE_ROOT");
    }

private:
    SceneNode root_;
};

} // namespace gamengine
