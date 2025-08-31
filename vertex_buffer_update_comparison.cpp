#include <iostream>
#include <vector>
#include <chrono>

// 模拟顶点数据结构
struct Vertex {
    float x, y;     // 位置
    float r, g, b;  // 颜色
};

// 模拟Vulkan API
class VulkanAPI {
public:
    // 方法1: 直接内存映射 (你的代码使用的方式)
    void updateVertexBuffer_DirectMapping(std::vector<Vertex>& newVertices) {
        std::cout << "=== 方法1: 直接内存映射 ===" << std::endl;
        
        // 1. 映射GPU内存到CPU地址空间
        void* mappedData = mapMemory();
        std::cout << "1. 映射GPU内存到CPU地址空间" << std::endl;
        
        // 2. 直接复制数据
        memcpy(mappedData, newVertices.data(), newVertices.size() * sizeof(Vertex));
        std::cout << "2. 直接复制数据到GPU内存" << std::endl;
        
        // 3. 解除映射
        unmapMemory();
        std::cout << "3. 解除内存映射" << std::endl;
        
        std::cout << "优点: 简单直接，适合频繁更新" << std::endl;
        std::cout << "缺点: 需要HOST_VISIBLE内存类型" << std::endl;
    }
    
    // 方法2: 暂存缓冲区 (传统方式)
    void updateVertexBuffer_StagingBuffer(std::vector<Vertex>& newVertices) {
        std::cout << "\n=== 方法2: 暂存缓冲区 ===" << std::endl;
        
        // 1. 创建暂存缓冲区
        std::cout << "1. 创建暂存缓冲区" << std::endl;
        
        // 2. 复制数据到暂存缓冲区
        std::cout << "2. 复制数据到暂存缓冲区" << std::endl;
        
        // 3. 记录复制命令
        std::cout << "3. 记录GPU复制命令" << std::endl;
        
        // 4. 提交命令缓冲区
        std::cout << "4. 提交命令缓冲区" << std::endl;
        
        // 5. 等待GPU完成
        std::cout << "5. 等待GPU完成复制" << std::endl;
        
        // 6. 清理暂存缓冲区
        std::cout << "6. 清理暂存缓冲区" << std::endl;
        
        std::cout << "优点: 可以使用DEVICE_LOCAL内存，性能更好" << std::endl;
        std::cout << "缺点: 复杂，需要命令缓冲区" << std::endl;
    }
    
    // 方法3: 统一内存 (现代GPU)
    void updateVertexBuffer_UnifiedMemory(std::vector<Vertex>& newVertices) {
        std::cout << "\n=== 方法3: 统一内存 ===" << std::endl;
        
        // 1. 直接写入统一内存
        std::cout << "1. 直接写入统一内存" << std::endl;
        
        // 2. 数据自动同步
        std::cout << "2. 数据自动在CPU和GPU间同步" << std::endl;
        
        std::cout << "优点: 最简单，性能好" << std::endl;
        std::cout << "缺点: 需要支持统一内存的GPU" << std::endl;
    }

private:
    void* mapMemory() {
        // 模拟映射
        return malloc(1024);
    }
    
    void unmapMemory() {
        // 模拟解除映射
    }
};

int main() {
    VulkanAPI vulkan;
    std::vector<Vertex> vertices = {
        {0.0f, 0.5f, 1.0f, 0.0f, 0.0f},
        {-0.5f, -0.5f, 0.0f, 1.0f, 0.0f},
        {0.5f, -0.5f, 0.0f, 0.0f, 1.0f}
    };
    
    // 演示三种更新方式
    vulkan.updateVertexBuffer_DirectMapping(vertices);
    vulkan.updateVertexBuffer_StagingBuffer(vertices);
    vulkan.updateVertexBuffer_UnifiedMemory(vertices);
    
    std::cout << "\n=== 性能对比 ===" << std::endl;
    std::cout << "直接映射: 适合频繁更新，简单但内存类型受限" << std::endl;
    std::cout << "暂存缓冲区: 适合大块数据，复杂但性能好" << std::endl;
    std::cout << "统一内存: 最简单，但需要硬件支持" << std::endl;
    
    return 0;
}
