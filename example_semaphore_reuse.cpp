#include <iostream>
#include <vector>

// 模拟VkSemaphoreCreateInfo的结构
struct VkSemaphoreCreateInfo {
    int sType;
    void* pNext;
    int flags;
};

// 模拟vkCreateSemaphore函数
void* vkCreateSemaphore(void* device, VkSemaphoreCreateInfo* info, void* allocator) {
    std::cout << "Creating semaphore with sType: " << info->sType << std::endl;
    return (void*)0x12345678; // 返回一个假句柄
}

int main() {
    std::vector<void*> semaphores(3);
    
    // 方法1: 复用结构体（你的代码方式）
    std::cout << "=== 方法1: 复用结构体 ===" << std::endl;
    VkSemaphoreCreateInfo semaphoreInfo = {};
    semaphoreInfo.sType = 1; // VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO
    
    for (size_t i = 0; i < 3; i++) {
        // 每次调用时，vkCreateSemaphore会复制semaphoreInfo的内容
        semaphores[i] = vkCreateSemaphore(nullptr, &semaphoreInfo, nullptr);
    }
    
    // 方法2: 每次都创建新的结构体（不必要）
    std::cout << "\n=== 方法2: 每次都创建新结构体 ===" << std::endl;
    for (size_t i = 0; i < 3; i++) {
        VkSemaphoreCreateInfo newInfo = {};
        newInfo.sType = 1;
        semaphores[i] = vkCreateSemaphore(nullptr, &newInfo, nullptr);
    }
    
    return 0;
}
