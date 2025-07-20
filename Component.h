#pragma once
class SceneNode; // Forward declaration


class Component {
friend class SceneNodeBuilder;

public:
    virtual ~Component() = default;
    virtual void onBuild(SceneNode& node) {}; // Default: do nothing but allows for comonnets to querery the node on build
    virtual void fixedUpdate(float dt) {};

protected:
    SceneNode* parent;
};
