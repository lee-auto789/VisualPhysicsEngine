#include "Scene.h"
#include <algorithm>

Scene::Scene() {}

Scene::~Scene() {}

void Scene::addObject(std::shared_ptr<PhysicsObject> obj) {
    objects.push_back(obj);
}

void Scene::removeObject(std::shared_ptr<PhysicsObject> obj) {
    objects.erase(std::remove(objects.begin(), objects.end(), obj), objects.end());
}

const std::vector<std::shared_ptr<PhysicsObject>>& Scene::getObjects() const {
    return objects;
}
