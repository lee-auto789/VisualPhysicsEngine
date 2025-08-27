#include "PhysicsEngine.h"
#include <algorithm>
#include <iostream>
#include <cstring>


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

const std::vector<std::shared_ptr<PhysicsObject>>& PhysicsEngine::getObjects() const {
    return objects;
}
