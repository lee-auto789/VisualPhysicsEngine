#pragma once
#include <vulkan/vulkan.h>
#include "PhysicsObject.h"

class Renderer {
public:
    void updateVertexBuffer(VkDevice device, VkDeviceMemory vertexBufferMemory, const std::vector<PhysicsObject::Vertex>& vertices);
    void draw(VkCommandBuffer commandBuffer, VkBuffer vertexBuffer, uint32_t vertexCount);
};
