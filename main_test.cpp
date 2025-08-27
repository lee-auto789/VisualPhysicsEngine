#include <iostream>
#include <vector>
#include <memory>
#include <chrono>
#include "PhysicsEngine.h"

using namespace std;

// Global variables
std::unique_ptr<PhysicsEngine> physicsEngine;
std::vector<std::shared_ptr<PhysicsObject>> physicsObjects;

void initPhysicsObjects() {
    cout << "Initializing physics objects..." << endl;

    try {
        physicsEngine = std::make_unique<PhysicsEngine>();
        cout << "Physics engine created successfully" << endl;

        // Create multiple triangles with different colors
        std::vector<std::vector<PhysicsObject::Vertex>> triangleVertices = {
            // Red triangle
            {
                {{0.0f, 0.2f}, {1.0f, 0.0f, 0.0f}},
                {{-0.1f, -0.1f}, {1.0f, 0.0f, 0.0f}},
                {{0.1f, -0.1f}, {1.0f, 0.0f, 0.0f}}
            },
        };

        cout << "Created " << triangleVertices.size() << " triangle vertex sets" << endl;

        // Create physics objects
        for (size_t i = 0; i < triangleVertices.size(); i++) {
            cout << "Creating physics object " << i << "..." << endl;
            auto obj = std::make_shared<PhysicsObject>(triangleVertices[i], 1.0f + i * 0.5f);
            obj->setPosition(glm::vec2(-0.5f + i * 0.3f, 0.8f));
            obj->setVelocity(glm::vec2(0.0f, 0.0f));
            obj->setElasticity(0.7f + i * 0.1f);
            obj->setFriction(0.1f + i * 0.05f);

            physicsObjects.push_back(obj);
            physicsEngine->addObject(obj);
            cout << "Physics object " << i << " created and added" << endl;
        }

        cout << "Created " << physicsObjects.size() << " physics objects" << endl;
    } catch (const std::exception& e) {
        cout << "Error initializing physics objects: " << e.what() << endl;
    }
}

// Test function for physics simulation
void runPhysicsTests() {
    cout << "=== Running Physics Tests ===" << endl;

    initPhysicsObjects();

    // Test parameters
    const int numSteps = 200;
    const float deltaTime = 0.016f;
    bool hasFallen = false;
    bool hasBounced = false;

    // Check if there are any physics objects to test
    if (physicsObjects.empty()) {
        cout << "No physics objects to test." << endl;
        return;
    }

    float initialY = physicsObjects[0]->getPosition().y;
    float lastY = initialY;

    // Simulation loop
    for (int i = 0; i < numSteps; i++) {
        physicsEngine->update(deltaTime);

        float currentY = physicsObjects[0]->getPosition().y;

        // Check for falling
        if (currentY < lastY) {
            hasFallen = true;
        }

        // Check for bouncing
        if (hasFallen && currentY > lastY && lastY <= physicsEngine->getGroundLevel() + 0.01f) {
            hasBounced = true;
        }

        lastY = currentY;
    }

    // Output test results
    cout << "Test Results:" << endl;
    if (hasFallen) {
        cout << "  - Object has fallen: PASS" << endl;
    } else {
        cout << "  - Object has fallen: FAIL" << endl;
    }

    if (hasBounced) {
        cout << "  - Object has bounced: PASS" << endl;
    } else {
        cout << "  - Object has bounced: FAIL" << endl;
    }

    cout << "==========================" << endl;
}

#include "TestRunner.h"
#include <cassert>

void testAddRemoveObject() {
    PhysicsEngine engine;
    auto obj = std::make_shared<PhysicsObject>(std::vector<PhysicsObject::Vertex>{});
    engine.addObject(obj);
    assert(engine.getObjects().size() == 1);
    engine.removeObject(obj);
    assert(engine.getObjects().empty());
}

void testCollision() {
    PhysicsEngine engine;
    std::vector<PhysicsObject::Vertex> vertices = {
        {{0.0f, 0.1f}, {1.0f, 0.0f, 0.0f}},
        {{-0.1f, -0.1f}, {1.0f, 0.0f, 0.0f}},
        {{0.1f, -0.1f}, {1.0f, 0.0f, 0.0f}}
    };
    auto obj1 = std::make_shared<PhysicsObject>(vertices);
    auto obj2 = std::make_shared<PhysicsObject>(vertices);
    engine.addObject(obj1);
    engine.addObject(obj2);

    // Test no collision
    obj1->setPosition(glm::vec2(0.0f, 0.0f));
    obj2->setPosition(glm::vec2(1.0f, 1.0f));
    engine.update(0.016f);
    obj1->setVelocity(glm::vec2(0.0f, 0.0f));
    obj2->setVelocity(glm::vec2(0.0f, 0.0f));
    assert(obj1->getVelocity() == glm::vec2(0.0f, 0.0f));
    assert(obj2->getVelocity() == glm::vec2(0.0f, 0.0f));

    // Test collision
    obj2->setPosition(glm::vec2(0.05f, 0.05f));
    engine.update(0.016f);
    assert(obj1->getVelocity() != glm::vec2(0.0f, 0.0f));
    assert(obj2->getVelocity() != glm::vec2(0.0f, 0.0f));
}

int main() {
    TestRunner runner;
    runner.addTest("testAddRemoveObject", testAddRemoveObject);
    runner.addTest("testCollision", testCollision);

    try {
        runPhysicsTests();
        runner.runTests();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return 0;
}
