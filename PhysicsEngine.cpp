#include "PhysicsEngine.h"
#include <algorithm>
#include <iostream>

// PhysicsObject 实现
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

void PhysicsObject::updateVertexBuffer(VkDevice device, VkDeviceMemory vertexBufferMemory) {
    // 更新GPU内存中的顶点数据
    void* data;
    vkMapMemory(device, vertexBufferMemory, 0, vertices.size() * sizeof(Vertex), 0, &data);
    memcpy(data, vertices.data(), vertices.size() * sizeof(Vertex));
    vkUnmapMemory(device, vertexBufferMemory);
}

void PhysicsObject::draw(VkCommandBuffer commandBuffer, VkBuffer vertexBuffer) {
    VkBuffer vertexBuffers[] = {vertexBuffer};
    VkDeviceSize offsets[] = {0};
    vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
    vkCmdDraw(commandBuffer, static_cast<uint32_t>(vertices.size()), 1, 0, 0);
}

bool PhysicsObject::pointInTriangle(const glm::vec2& point, const glm::vec2& a, const glm::vec2& b, const glm::vec2& c) const {
    float area = 0.5f * (-b.y * c.x + a.y * (-b.x + c.x) + a.x * (b.y - c.y) + b.x * c.y);
    float s = 1.0f / (2.0f * area) * (a.y * c.x - a.x * c.y + (c.y - a.y) * point.x + (a.x - c.x) * point.y);
    float t = 1.0f / (2.0f * area) * (a.x * b.y - a.y * b.x + (a.y - b.y) * point.x + (b.x - a.x) * point.y);
    return s >= 0 && t >= 0 && (s + t) <= 1;
}

// PhysicsEngine 实现
PhysicsEngine::PhysicsEngine() : gravity(0.0f, -9.8f), groundLevel(-0.8f), airResistance(0.02f) {}

PhysicsEngine::~PhysicsEngine() {}

void PhysicsEngine::addObject(std::shared_ptr<PhysicsObject> obj) {
    objects.push_back(obj);
}

void PhysicsEngine::removeObject(std::shared_ptr<PhysicsObject> obj) {
    objects.erase(std::remove(objects.begin(), objects.end(), obj), objects.end());
}

void PhysicsEngine::update(float deltaTime) {
    for (auto& obj : objects) {
        // 应用重力
        obj->applyForce(gravity * obj->getMass());
        
        // 应用空气阻力
        applyAirResistance(obj);
        
        // 更新物理对象
        obj->update(deltaTime);
        
        // 检查地面碰撞
        applyGroundCollision(obj);
    }
    
    // 检查对象间的碰撞
    checkCollisions();
}

void PhysicsEngine::checkCollisions() {
    for (size_t i = 0; i < objects.size(); i++) {
        for (size_t j = i + 1; j < objects.size(); j++) {
            if (objects[i]->checkCollision(*objects[j])) {
                objects[i]->resolveCollision(*objects[j]);
                
                // 应用变形效果
                glm::vec2 impactPoint = (objects[i]->getPosition() + objects[j]->getPosition()) * 0.5f;
                float impactForce = glm::length(objects[i]->getVelocity() - objects[j]->getVelocity());
                objects[i]->deform(impactPoint, impactForce);
                objects[j]->deform(impactPoint, impactForce);
            }
        }
    }
}

void PhysicsEngine::applyGroundCollision(std::shared_ptr<PhysicsObject> obj) {
    // 检查是否与地面碰撞
    if (obj->getPosition().y < groundLevel) {
        obj->setPosition(glm::vec2(obj->getPosition().x, groundLevel));
        
        // 应用反弹和摩擦力
        glm::vec2 vel = obj->getVelocity();
        vel.y = -vel.y * obj->getElasticity();
        vel.x *= (1.0f - obj->getFriction());
        obj->setVelocity(vel);
    }
}

void PhysicsEngine::applyAirResistance(std::shared_ptr<PhysicsObject> obj) {
    glm::vec2 vel = obj->getVelocity();
    glm::vec2 resistance = -vel * airResistance;
    obj->applyForce(resistance);
}

void PhysicsEngine::renderAll(VkCommandBuffer commandBuffer, VkBuffer vertexBuffer) {
    for (auto& obj : objects) {
        obj->draw(commandBuffer, vertexBuffer);
    }
}
