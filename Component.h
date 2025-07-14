#pragma once
class SceneNode; // Forward declaration


class Component {
public:
    virtual ~Component() = default;

    // Optional: Called when the component is attached to a node
    void onAttach(SceneNode& node) {}

    // Optional: Called when the component is detached or node is destroyed
    void onDetach(SceneNode& node) {}
};
