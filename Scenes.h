#pragma once
#include "Scene.h"

class SingleTriangleScene : public Scene {
public:
    SingleTriangleScene();
};

class MultipleTrianglesScene : public Scene {
public:
    MultipleTrianglesScene();
};

class InteractiveScene : public Scene {
public:
    InteractiveScene();
    void onClick(double x, double y);
};
