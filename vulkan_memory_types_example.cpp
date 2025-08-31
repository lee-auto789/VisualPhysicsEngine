#include <iostream>
#include <vector>
#include <iomanip>

// 模拟Vulkan内存类型和属性
struct VkMemoryPropertyFlags {
    static const uint32_t DEVICE_LOCAL_BIT = 0x00000001;
    static const uint32_t HOST_VISIBLE_BIT = 0x00000002;
    static const uint32_t HOST_COHERENT_BIT = 0x00000004;
    static const uint32_t HOST_CACHED_BIT = 0x00000008;
    static const uint32_t LAZILY_ALLOCATED_BIT = 0x00000010;
    static const uint32_t PROTECTED_BIT = 0x00000020;
    static const uint32_t DEVICE_COHERENT_BIT_AMD = 0x00000040;
    static const uint32_t DEVICE_UNCACHED_BIT_AMD = 0x00000080;
};

struct VkMemoryType {
    uint32_t propertyFlags;
    uint32_t heapIndex;
};

struct VkMemoryHeap {
    uint64_t size;
    uint32_t flags;
};

struct VkPhysicalDeviceMemoryProperties {
    uint32_t memoryTypeCount;
    VkMemoryType memoryTypes[32];
    uint32_t memoryHeapCount;
    VkMemoryHeap memoryHeaps[16];
};

// 模拟GPU内存属性
VkPhysicalDeviceMemoryProperties getMockMemoryProperties() {
    VkPhysicalDeviceMemoryProperties props = {};
    
    // 典型的GPU内存类型配置
    props.memoryTypeCount = 8;
    props.memoryHeapCount = 2;
    
    // 堆0: 设备本地内存 (VRAM)
    props.memoryHeaps[0].size = 8ULL * 1024 * 1024 * 1024; // 8GB
    props.memoryHeaps[0].flags = 0; // 设备本地
    
    // 堆1: 系统内存
    props.memoryHeaps[1].size = 16ULL * 1024 * 1024 * 1024; // 16GB
    props.memoryHeaps[1].flags = 0; // 系统内存
    
    // 内存类型0: 设备本地内存 (最快)
    props.memoryTypes[0].propertyFlags = VkMemoryPropertyFlags::DEVICE_LOCAL_BIT;
    props.memoryTypes[0].heapIndex = 0;
    
    // 内存类型1: 设备本地 + 主机可见 (用于频繁更新的数据)
    props.memoryTypes[1].propertyFlags = VkMemoryPropertyFlags::DEVICE_LOCAL_BIT | 
                                        VkMemoryPropertyFlags::HOST_VISIBLE_BIT;
    props.memoryTypes[1].heapIndex = 0;
    
    // 内存类型2: 主机可见 + 主机一致 (用于顶点缓冲区)
    props.memoryTypes[2].propertyFlags = VkMemoryPropertyFlags::HOST_VISIBLE_BIT | 
                                        VkMemoryPropertyFlags::HOST_COHERENT_BIT;
    props.memoryTypes[2].heapIndex = 1;
    
    // 内存类型3: 主机可见 + 主机缓存 (用于纹理上传)
    props.memoryTypes[3].propertyFlags = VkMemoryPropertyFlags::HOST_VISIBLE_BIT | 
                                        VkMemoryPropertyFlags::HOST_CACHED_BIT;
    props.memoryTypes[3].heapIndex = 1;
    
    // 内存类型4: 延迟分配 (用于渲染目标)
    props.memoryTypes[4].propertyFlags = VkMemoryPropertyFlags::DEVICE_LOCAL_BIT | 
                                        VkMemoryPropertyFlags::LAZILY_ALLOCATED_BIT;
    props.memoryTypes[4].heapIndex = 0;
    
    // 内存类型5: 保护内存
    props.memoryTypes[5].propertyFlags = VkMemoryPropertyFlags::PROTECTED_BIT;
    props.memoryTypes[5].heapIndex = 0;
    
    // 内存类型6: 设备一致 (AMD扩展)
    props.memoryTypes[6].propertyFlags = VkMemoryPropertyFlags::DEVICE_LOCAL_BIT | 
                                        VkMemoryPropertyFlags::DEVICE_COHERENT_BIT_AMD;
    props.memoryTypes[6].heapIndex = 0;
    
    // 内存类型7: 设备未缓存 (AMD扩展)
    props.memoryTypes[7].propertyFlags = VkMemoryPropertyFlags::DEVICE_LOCAL_BIT | 
                                        VkMemoryPropertyFlags::DEVICE_UNCACHED_BIT_AMD;
    props.memoryTypes[7].heapIndex = 0;
    
    return props;
}

// 模拟findMemoryType函数
uint32_t findMemoryType(uint32_t typeFilter, uint32_t properties, const VkPhysicalDeviceMemoryProperties& memProperties) {
    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
        if ((typeFilter & (1 << i)) && 
            (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
            return i;
        }
    }
    throw std::runtime_error("failed to find suitable memory type!");
}

// 打印内存属性标志
void printMemoryFlags(uint32_t flags) {
    std::vector<std::string> flagNames;
    
    if (flags & VkMemoryPropertyFlags::DEVICE_LOCAL_BIT) flagNames.push_back("DEVICE_LOCAL");
    if (flags & VkMemoryPropertyFlags::HOST_VISIBLE_BIT) flagNames.push_back("HOST_VISIBLE");
    if (flags & VkMemoryPropertyFlags::HOST_COHERENT_BIT) flagNames.push_back("HOST_COHERENT");
    if (flags & VkMemoryPropertyFlags::HOST_CACHED_BIT) flagNames.push_back("HOST_CACHED");
    if (flags & VkMemoryPropertyFlags::LAZILY_ALLOCATED_BIT) flagNames.push_back("LAZILY_ALLOCATED");
    if (flags & VkMemoryPropertyFlags::PROTECTED_BIT) flagNames.push_back("PROTECTED");
    if (flags & VkMemoryPropertyFlags::DEVICE_COHERENT_BIT_AMD) flagNames.push_back("DEVICE_COHERENT_AMD");
    if (flags & VkMemoryPropertyFlags::DEVICE_UNCACHED_BIT_AMD) flagNames.push_back("DEVICE_UNCACHED_AMD");
    
    for (const auto& name : flagNames) {
        std::cout << name << " ";
    }
}

int main() {
    std::cout << "=== Vulkan 内存类型详解 ===" << std::endl;
    
    VkPhysicalDeviceMemoryProperties memProps = getMockMemoryProperties();
    
    std::cout << "\n--- 内存堆信息 ---" << std::endl;
    for (uint32_t i = 0; i < memProps.memoryHeapCount; i++) {
        std::cout << "堆 " << i << ": " 
                  << (memProps.memoryHeaps[i].size / (1024*1024*1024)) << "GB"
                  << (memProps.memoryHeaps[i].flags ? " (设备本地)" : " (系统内存)")
                  << std::endl;
    }
    
    std::cout << "\n--- 内存类型信息 ---" << std::endl;
    for (uint32_t i = 0; i < memProps.memoryTypeCount; i++) {
        std::cout << "类型 " << i << " (堆" << memProps.memoryTypes[i].heapIndex << "): ";
        printMemoryFlags(memProps.memoryTypes[i].propertyFlags);
        std::cout << std::endl;
    }
    
    std::cout << "\n--- 常见使用场景 ---" << std::endl;
    
    // 场景1: 顶点缓冲区 (你的代码中使用的)
    std::cout << "\n1. 顶点缓冲区:" << std::endl;
    uint32_t vertexBufferType = findMemoryType(0xFF, 
        VkMemoryPropertyFlags::HOST_VISIBLE_BIT | VkMemoryPropertyFlags::HOST_COHERENT_BIT, 
        memProps);
    std::cout << "   选择内存类型: " << vertexBufferType << std::endl;
    std::cout << "   原因: 需要CPU频繁更新顶点数据" << std::endl;
    
    // 场景2: 纹理/图像
    std::cout << "\n2. 纹理/图像:" << std::endl;
    uint32_t textureType = findMemoryType(0xFF, 
        VkMemoryPropertyFlags::DEVICE_LOCAL_BIT, 
        memProps);
    std::cout << "   选择内存类型: " << textureType << std::endl;
    std::cout << "   原因: 需要GPU快速访问，CPU很少更新" << std::endl;
    
    // 场景3: 暂存缓冲区
    std::cout << "\n3. 暂存缓冲区:" << std::endl;
    uint32_t stagingType = findMemoryType(0xFF, 
        VkMemoryPropertyFlags::HOST_VISIBLE_BIT | VkMemoryPropertyFlags::HOST_CACHED_BIT, 
        memProps);
    std::cout << "   选择内存类型: " << stagingType << std::endl;
    std::cout << "   原因: 用于CPU到GPU的数据传输" << std::endl;
    
    // 场景4: 渲染目标
    std::cout << "\n4. 渲染目标:" << std::endl;
    uint32_t renderTargetType = findMemoryType(0xFF, 
        VkMemoryPropertyFlags::DEVICE_LOCAL_BIT | VkMemoryPropertyFlags::LAZILY_ALLOCATED_BIT, 
        memProps);
    std::cout << "   选择内存类型: " << renderTargetType << std::endl;
    std::cout << "   原因: 延迟分配，节省内存" << std::endl;
    
    std::cout << "\n--- 内存属性说明 ---" << std::endl;
    std::cout << "DEVICE_LOCAL: GPU本地内存，访问最快" << std::endl;
    std::cout << "HOST_VISIBLE: CPU可以访问" << std::endl;
    std::cout << "HOST_COHERENT: CPU和GPU内存一致" << std::endl;
    std::cout << "HOST_CACHED: CPU缓存，读取快" << std::endl;
    std::cout << "LAZILY_ALLOCATED: 延迟分配，节省内存" << std::endl;
    std::cout << "PROTECTED: 受保护内存，用于DRM" << std::endl;
    
    return 0;
}
