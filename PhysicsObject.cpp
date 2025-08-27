#include "PhysicsObject.h"
#include <algorithm>
#include <iostream>
#include <cstring>

PhysicsObject::PhysicsObject(const std::vector<Vertex>& verts, float m)
    : vertices(verts), originalVertices(verts), mass(m), elasticity(0.8f), friction(0.1f), deformation(0.0f) {
    updateBounds();
}

PhysicsObject::~PhysicsObject() {}

void PhysicsObject::setPosition(const glm::vec2& pos) {
    position = pos;
    updateBounds();
}

void PhysicsObject::setVelocity(const glm::vec2& vel) {
    velocity = vel;
}

void PhysicsObject::setMass(float m) {
    mass = m;
}

void PhysicsObject::setElasticity(float e) {
    elasticity = std::clamp(e, 0.0f, 1.0f);
}

void PhysicsObject::setFriction(float f) {
    friction = std::clamp(f, 0.0f, 1.0f);
}

void PhysicsObject::applyForce(const glm::vec2& force) {
    acceleration += force / mass;
}

void PhysicsObject::applyImpulse(const glm::vec2& impulse) {
    velocity += impulse / mass;
}

void PhysicsObject::update(float deltaTime) {
    // 更新速度和位置
    velocity += acceleration * deltaTime;
    position += velocity * deltaTime;

    // 重置加速度
    acceleration = glm::vec2(0.0f);

    // 更新顶点位置
    for (size_t i = 0; i < vertices.size(); i++) {
        vertices[i].position = originalVertices[i].position + position;
    }

    updateBounds();
}

void PhysicsObject::updateBounds() {
    if (vertices.empty()) return;

    minBounds = maxBounds = vertices[0].position;
    for (const auto& vertex : vertices) {
        minBounds.x = std::min(minBounds.x, vertex.position.x);
        minBounds.y = std::min(minBounds.y, vertex.position.y);
        maxBounds.x = std::max(maxBounds.x, vertex.position.x);
        maxBounds.y = std::max(maxBounds.y, vertex.position.y);
    }
}

bool PhysicsObject::checkCollision(const PhysicsObject& other) const {
    // 简单的AABB碰撞检测
    return !(maxBounds.x < other.minBounds.x || minBounds.x > other.maxBounds.x ||
             maxBounds.y < other.minBounds.y || minBounds.y > other.maxBounds.y);
}

void PhysicsObject::resolveCollision(PhysicsObject& other) {
    // 计算碰撞响应
    glm::vec2 normal = glm::normalize(position - other.position);
    float relativeVelocity = glm::dot(velocity - other.velocity, normal);

    if (relativeVelocity > 0) return; // 物体正在分离

    float restitution = (elasticity + other.elasticity) * 0.5f;
    float impulse = -(1.0f + restitution) * relativeVelocity;
    impulse /= 1.0f/mass + 1.0f/other.mass;

    glm::vec2 impulseVector = impulse * normal;

    applyImpulse(impulseVector);
    other.applyImpulse(-impulseVector);

    // 分离物体以防止重叠
    float overlap = 0.1f;
    glm::vec2 separation = normal * overlap;
    position += separation;
    other.position -= separation;
}

void PhysicsObject::deform(const glm::vec2& impactPoint, float force) {
    // 简单的变形效果：根据冲击力调整顶点位置
    deformation += force * 0.01f;
    deformation = std::min(deformation, 0.3f); // 限制最大变形

    for (size_t i = 0; i < vertices.size(); i++) {
        glm::vec2 toImpact = impactPoint - vertices[i].position;
        float distance = glm::length(toImpact);
        if (distance > 0.001f) {
            float deformationFactor = deformation / (1.0f + distance);
            vertices[i].position += glm::normalize(toImpact) * deformationFactor * 0.1f;
        }
    }
}

bool PhysicsObject::pointInTriangle(const glm::vec2& point, const glm::vec2& a, const glm::vec2& b, const glm::vec2& c) const {
    float area = 0.5f * (-b.y * c.x + a.y * (-b.x + c.x) + a.x * (b.y - c.y) + b.x * c.y);
    float s = 1.0f / (2.0f * area) * (a.y * c.x - a.x * c.y + (c.y - a.y) * point.x + (a.x - c.x) * point.y);
    float t = 1.0f / (2.0f * area) * (a.x * b.y - a.y * b.x + (a.y - b.y) * point.x + (b.x - a.x) * point.y);
    return s >= 0 && t >= 0 && (s + t) <= 1;
}