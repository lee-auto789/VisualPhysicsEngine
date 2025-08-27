#include "Renderer.h"
#include <cstring>

void Renderer::updateVertexBuffer(VkDevice device, VkDeviceMemory vertexBufferMemory, const std::vector<PhysicsObject::Vertex>& vertices) {
    void* data;
    vkMapMemory(device, vertexBufferMemory, 0, vertices.size() * sizeof(PhysicsObject::Vertex), 0, &data);
    memcpy(data, vertices.data(), vertices.size() * sizeof(PhysicsObject::Vertex));
    vkUnmapMemory(device, vertexBufferMemory);
}

void Renderer::draw(VkCommandBuffer commandBuffer, VkBuffer vertexBuffer, uint32_t vertexCount) {
    VkBuffer vertexBuffers[] = {vertexBuffer};
    VkDeviceSize offsets[] = {0};
    vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
    vkCmdDraw(commandBuffer, vertexCount, 1, 0, 0);
}
