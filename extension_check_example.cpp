#include <iostream>
#include <vector>
#include <set>
#include <string>

// 模拟Vulkan扩展属性结构
struct VkExtensionProperties {
    char extensionName[256];
    uint32_t specVersion;
};

// 模拟扩展检查过程
class ExtensionChecker {
public:
    void demonstrateExtensionCheck() {
        std::cout << "=== Vulkan扩展检查演示 ===" << std::endl;
        
        // 1. 程序需要的扩展
        std::vector<const char*> requiredExtensions = {
            "VK_KHR_swapchain",
            "VK_KHR_maintenance1"
        };
        
        std::cout << "\n1. 程序需要的扩展:" << std::endl;
        for (const auto& ext : requiredExtensions) {
            std::cout << "   - " << ext << std::endl;
        }
        
        // 2. GPU支持的扩展（模拟）
        std::vector<VkExtensionProperties> availableExtensions = {
            {"VK_KHR_swapchain", 1},
            {"VK_KHR_maintenance1", 1},
            {"VK_EXT_debug_marker", 1},
            {"VK_KHR_dedicated_allocation", 1}
        };
        
        std::cout << "\n2. GPU支持的扩展:" << std::endl;
        for (const auto& ext : availableExtensions) {
            std::cout << "   - " << ext.extensionName << std::endl;
        }
        
        // 3. 检查过程
        std::cout << "\n3. 检查过程:" << std::endl;
        
        // 创建需要扩展的集合
        std::set<std::string> requiredSet(requiredExtensions.begin(), requiredExtensions.end());
        
        std::cout << "   初始需要扩展数量: " << requiredSet.size() << std::endl;
        
        // 遍历GPU支持的扩展，从需要列表中删除
        for (const auto& extension : availableExtensions) {
            std::cout << "   检查扩展: " << extension.extensionName << std::endl;
            
            auto it = requiredSet.find(extension.extensionName);
            if (it != requiredSet.end()) {
                std::cout << "   ✓ 找到匹配，从需要列表中删除" << std::endl;
                requiredSet.erase(it);
            } else {
                std::cout << "   - 不在需要列表中，跳过" << std::endl;
            }
            
            std::cout << "   剩余需要扩展数量: " << requiredSet.size() << std::endl;
        }
        
        // 4. 结果
        std::cout << "\n4. 检查结果:" << std::endl;
        if (requiredSet.empty()) {
            std::cout << "   ✓ 所有需要的扩展都支持！" << std::endl;
        } else {
            std::cout << "   ✗ 缺少以下扩展:" << std::endl;
            for (const auto& missing : requiredSet) {
                std::cout << "     - " << missing << std::endl;
            }
        }
    }
    
    void demonstrateFailureCase() {
        std::cout << "\n=== 失败案例演示 ===" << std::endl;
        
        // 程序需要的扩展
        std::vector<const char*> requiredExtensions = {
            "VK_KHR_swapchain",
            "VK_KHR_maintenance1",
            "VK_KHR_imageless_framebuffer"  // 这个扩展GPU不支持
        };
        
        // GPU支持的扩展（缺少一个）
        std::vector<VkExtensionProperties> availableExtensions = {
            {"VK_KHR_swapchain", 1},
            {"VK_KHR_maintenance1", 1}
            // 注意：缺少 VK_KHR_imageless_framebuffer
        };
        
        std::cout << "程序需要: VK_KHR_swapchain, VK_KHR_maintenance1, VK_KHR_imageless_framebuffer" << std::endl;
        std::cout << "GPU支持: VK_KHR_swapchain, VK_KHR_maintenance1" << std::endl;
        
        std::set<std::string> requiredSet(requiredExtensions.begin(), requiredExtensions.end());
        
        for (const auto& extension : availableExtensions) {
            requiredSet.erase(extension.extensionName);
        }
        
        std::cout << "检查结果: ";
        if (requiredSet.empty()) {
            std::cout << "✓ 支持所有扩展" << std::endl;
        } else {
            std::cout << "✗ 缺少扩展: ";
            for (const auto& missing : requiredSet) {
                std::cout << missing << " ";
            }
            std::cout << std::endl;
        }
    }
    
    void explainAlgorithm() {
        std::cout << "\n=== 算法解释 ===" << std::endl;
        
        std::cout << "\n1. 算法思路:" << std::endl;
        std::cout << "   - 创建需要扩展的集合" << std::endl;
        std::cout << "   - 遍历GPU支持的扩展" << std::endl;
        std::cout << "   - 从需要集合中删除已找到的扩展" << std::endl;
        std::cout << "   - 最后检查需要集合是否为空" << std::endl;
        
        std::cout << "\n2. 为什么使用 std::set?" << std::endl;
        std::cout << "   - 快速查找: O(log n)" << std::endl;
        std::cout << "   - 快速删除: O(log n)" << std::endl;
        std::cout << "   - 自动去重" << std::endl;
        
        std::cout << "\n3. 为什么使用 erase()?" << std::endl;
        std::cout << "   - 标记已找到的扩展" << std::endl;
        std::cout << "   - 避免重复检查" << std::endl;
        std::cout << "   - 最终剩余的就是缺少的扩展" << std::endl;
        
        std::cout << "\n4. 时间复杂度:" << std::endl;
        std::cout << "   - 遍历GPU扩展: O(n)" << std::endl;
        std::cout << "   - 每次查找和删除: O(log m)" << std::endl;
        std::cout << "   - 总体: O(n * log m)" << std::endl;
        std::cout << "   其中 n = GPU扩展数量, m = 需要扩展数量" << std::endl;
    }
};

int main() {
    ExtensionChecker checker;
    
    checker.demonstrateExtensionCheck();
    checker.demonstrateFailureCase();
    checker.explainAlgorithm();
    
    std::cout << "\n=== 总结 ===" << std::endl;
    std::cout << "这段代码是Vulkan扩展兼容性检查的核心算法" << std::endl;
    std::cout << "通过集合操作高效地验证GPU是否支持所有需要的扩展" << std::endl;
    std::cout << "确保程序在运行时不会因为缺少扩展而失败" << std::endl;
    
    return 0;
}
