class PhysicsObject {
    public:
        PhysicsObject();
        ~PhysicsObject();

        void applyForce(glm::vec3 force);
        void applyTorque(glm::vec3 torque);
        void applyImpulse(glm::vec3 impulse);
        void applyAngularImpulse(glm::vec3 angularImpulse);
        void applyGravity(glm::vec3 gravity);
        void applyAirResistance(float airResistance);
        void applyBuoyancy(float buoyancy);

        void setPosition(glm::vec3 position);
        void setVelocity(glm::vec3 velocity);
        void setMass(float mass);
        void setRadius(float radius);
        void setDensity(float density);
        void setFriction(float friction);

        void update(float deltaTime);
        void draw(VkCommandBuffer commandBuffer);

    private:
        glm::vec3 position;
        glm::vec3 velocity;
        float mass;
        float radius;
        float density;
        float friction;
        float elasticity;
        float restitution;
        float gravity;
        float airResistance;
        float buoyancy;
};