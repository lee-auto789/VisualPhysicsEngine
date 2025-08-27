#pragma once
#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include <vulkan/vulkan.h>
#include "PhysicsObject.h"

// Physics Engine Class
class PhysicsEngine {
public:
    PhysicsEngine();
    ~PhysicsEngine();
    
    // Add physics objects
    void addObject(std::shared_ptr<PhysicsObject> obj);
    void removeObject(std::shared_ptr<PhysicsObject> obj);
    
    // Physics simulation
    void update(float deltaTime);
    void setGravity(const glm::vec2& g) { gravity = g; }
    void setGroundLevel(float level) { groundLevel = level; }
    float getGroundLevel() const { return groundLevel; }
    
    // Collision detection and response
    void checkCollisions();
    

private:
    std::vector<std::shared_ptr<PhysicsObject>> objects;
    glm::vec2 gravity;
    float groundLevel;
    float airResistance;
    
    void applyGroundCollision(std::shared_ptr<PhysicsObject> obj);
    void applyAirResistance(std::shared_ptr<PhysicsObject> obj);
};
