void PhysicsObject::update(float deltaTime) {
    // Update position and velocity
    position += velocity * deltaTime;
    velocity += gravity * deltaTime;
}

void PhysicsObject::draw(VkCommandBuffer commandBuffer) {
    // Draw the object
}

void PhysicsObject::applyForce(glm::vec3 force) {
    // Apply a force to the object
}

void PhysicsObject::applyTorque(glm::vec3 torque) {
    // Apply a torque to the object
}

void PhysicsObject::applyImpulse(glm::vec3 impulse) {

}

// 添加摩擦力函数
void applyFriction(float frictionCoefficient) {
    // 摩擦力只在物体运动时才有
    if (vx != 0.0f || vy != 0.0f) {
        
        // 计算当前速度的大小
        float speed = sqrt(vx*vx + vy*vy);
        
        // 摩擦力大小 = 摩擦系数 × 重力
        float frictionMagnitude = frictionCoefficient * mass * 9.8f;
        
        // 摩擦力方向与速度方向相反
        float frictionX = -(vx / speed) * frictionMagnitude;
        float frictionY = -(vy / speed) * frictionMagnitude;
        
        // 施加摩擦力
        applyForce(frictionX, frictionY);
    }
}