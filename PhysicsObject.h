#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <vulkan/vulkan.h>

class PhysicsObject {
public:
    struct Vertex {
        glm::vec2 position;
        glm::vec3 color;
    };

    PhysicsObject(const std::vector<Vertex>& vertices, float mass = 1.0f);
    ~PhysicsObject();

    // Physics property setters
    void setPosition(const glm::vec2& pos);
    void setVelocity(const glm::vec2& vel);
    void setMass(float m);
    void setElasticity(float e);
    void setFriction(float f);

    // Physics property getters
    glm::vec2 getPosition() const { return position; }
    glm::vec2 getVelocity() const { return velocity; }
    float getMass() const { return mass; }
    float getElasticity() const { return elasticity; }
    float getFriction() const { return friction; }
    const std::vector<Vertex>& getVertices() const { return vertices; }

    // Physics simulation
    void applyForce(const glm::vec2& force);
    void applyImpulse(const glm::vec2& impulse);
    void update(float deltaTime);

    // Collision detection
    bool checkCollision(const PhysicsObject& other) const;
    void resolveCollision(PhysicsObject& other);

    // Deformation
    void deform(const glm::vec2& impactPoint, float force);

private:
    std::vector<Vertex> vertices;
    std::vector<Vertex> originalVertices;
    glm::vec2 position;
    glm::vec2 velocity;
    glm::vec2 acceleration;
    float mass;
    float elasticity;
    float friction;
    float deformation;

    // Bounding box
    glm::vec2 minBounds;
    glm::vec2 maxBounds;

    void updateBounds();
    bool pointInTriangle(const glm::vec2& point, const glm::vec2& a, const glm::vec2& b, const glm::vec2& c) const;
};