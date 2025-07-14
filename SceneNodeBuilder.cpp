#include "SceneNodeBuilder.h"


SceneNodeBuilder::SceneNodeBuilder() {
    node = std::make_unique<SceneNode>();
}


std::unique_ptr<SceneNode> SceneNodeBuilder::build() {
    std::unique_ptr<SceneNode> temp_unique_ptr = std::move(node);
    node = std::make_unique<SceneNode>();
    return temp_unique_ptr;
}

