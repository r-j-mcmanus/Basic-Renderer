#include <memory>

#include "SceneNodeBuilder.h"
#include "EventHandler.h"
#include "EventListenerInterface.h"


SceneNodeBuilder::SceneNodeBuilder() {
    node = std::make_unique<SceneNode>();
}


std::unique_ptr<SceneNode> SceneNodeBuilder::build() {
    

    //for (const auto& it: node->components) {
    //    auto listener = std::dynamic_pointer_cast<EventListenerInterface>(it.second); // will return null if the component does not inherit from EventListenerInterface
    //    if (listener && eventHandler) {
    //        eventHandler->registerObserver(listener);
    //    }
    //}


    // Call onBuild for each component
    for (const auto& it : node->components) {
        it->onBuild(*node);
    }

    std::unique_ptr<SceneNode> temp_unique_ptr = std::move(node);
    node = std::make_unique<SceneNode>();
    
    return temp_unique_ptr;
}

