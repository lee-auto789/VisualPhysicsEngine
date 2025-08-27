#pragma once
#include <vector>
#include <memory>
#include "PhysicsObject.h"

class Scene {
public:
    Scene();
    ~Scene();

    void addObject(std::shared_ptr<PhysicsObject> obj);
    void removeObject(std::shared_ptr<PhysicsObject> obj);
    const std::vector<std::shared_ptr<PhysicsObject>>& getObjects() const;

private:
    std::vector<std::shared_ptr<PhysicsObject>> objects;
};
