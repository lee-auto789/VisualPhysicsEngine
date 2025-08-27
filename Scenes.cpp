#include "Scenes.h"

SingleTriangleScene::SingleTriangleScene() {
    // Create a single triangle
    std::vector<PhysicsObject::Vertex> vertices = {
        {{0.0f, 0.2f}, {1.0f, 0.0f, 0.0f}},
        {{-0.1f, -0.1f}, {1.0f, 0.0f, 0.0f}},
        {{0.1f, -0.1f}, {1.0f, 0.0f, 0.0f}}
    };
    auto obj = std::make_shared<PhysicsObject>(vertices, 1.0f);
    obj->setPosition(glm::vec2(0.0f, 0.8f));
    addObject(obj);
}

MultipleTrianglesScene::MultipleTrianglesScene() {
    // Create multiple triangles
    std::vector<std::vector<PhysicsObject::Vertex>> triangleVertices = {
        // Red triangle
        {
            {{0.0f, 0.2f}, {1.0f, 0.0f, 0.0f}},
            {{-0.1f, -0.1f}, {1.0f, 0.0f, 0.0f}},
            {{0.1f, -0.1f}, {1.0f, 0.0f, 0.0f}}
        },
        // Green triangle
        {
            {{0.0f, 0.4f}, {0.0f, 1.0f, 0.0f}},
            {{-0.1f, 0.1f}, {0.0f, 1.0f, 0.0f}},
            {{0.1f, 0.1f}, {0.0f, 1.0f, 0.0f}}
        },
        // Blue triangle
        {
            {{0.0f, 0.6f}, {0.0f, 0.0f, 1.0f}},
            {{-0.1f, 0.3f}, {0.0f, 0.0f, 1.0f}},
            {{0.1f, 0.3f}, {0.0f, 0.0f, 1.0f}}
        },
        // Yellow triangle
        {
            {{0.0f, 0.8f}, {1.0f, 1.0f, 0.0f}},
            {{-0.1f, 0.5f}, {1.0f, 1.0f, 0.0f}},
            {{0.1f, 0.5f}, {1.0f, 1.0f, 0.0f}}
        }
    };

    for (size_t i = 0; i < triangleVertices.size(); i++) {
        auto obj = std::make_shared<PhysicsObject>(triangleVertices[i], 1.0f + i * 0.5f);
        obj->setPosition(glm::vec2(-0.5f + i * 0.3f, 0.8f));
        obj->setVelocity(glm::vec2(0.0f, 0.0f));
        obj->setElasticity(0.7f + i * 0.1f);
        obj->setFriction(0.1f + i * 0.05f);
        addObject(obj);
    }
}

InteractiveScene::InteractiveScene() {
    // Create a single triangle that can be interacted with
    std::vector<PhysicsObject::Vertex> vertices = {
        {{0.0f, 0.2f}, {1.0f, 1.0f, 1.0f}},
        {{-0.1f, -0.1f}, {1.0f, 1.0f, 1.0f}},
        {{0.1f, -0.1f}, {1.0f, 1.0f, 1.0f}}
    };
    auto obj = std::make_shared<PhysicsObject>(vertices, 1.0f);
    obj->setPosition(glm::vec2(0.0f, 0.0f));
    addObject(obj);
}

void InteractiveScene::onClick(double x, double y) {
    // Apply a force to the object when the user clicks
    if (!getObjects().empty()) {
        auto obj = getObjects()[0];
        glm::vec2 force = glm::vec2(x, y) - obj->getPosition();
        obj->applyImpulse(force * 5.0f);
    }
}
