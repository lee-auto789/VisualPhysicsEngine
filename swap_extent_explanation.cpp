#include <iostream>
#include <algorithm>

// 模拟Vulkan结构
struct VkExtent2D {
    uint32_t width;
    uint32_t height;
};

struct VkSurfaceCapabilitiesKHR {
    VkExtent2D currentExtent;
    VkExtent2D minImageExtent;
    VkExtent2D maxImageExtent;
    // 其他字段...
};

// 模拟std::clamp函数
template<typename T>
T clamp(T value, T min, T max) {
    return std::min(std::max(value, min), max);
}

class SwapExtentChooser {
public:
    void demonstrateExtentSelection() {
        std::cout << "=== 交换链尺寸选择演示 ===" << std::endl;
        
        // 场景1: 系统指定尺寸
        demonstrateSystemSpecifiedExtent();
        
        // 场景2: 自定义尺寸
        demonstrateCustomExtent();
        
        // 场景3: 边界情况
        demonstrateBoundaryCases();
    }

private:
    void demonstrateSystemSpecifiedExtent() {
        std::cout << "\n--- 场景1: 系统指定尺寸 ---" << std::endl;
        
        // 模拟系统指定了具体尺寸
        VkSurfaceCapabilitiesKHR capabilities = {
            {1920, 1080},  // currentExtent: 系统建议1920x1080
            {1, 1},        // minImageExtent
            {4096, 4096}   // maxImageExtent
        };
        
        std::cout << "系统建议尺寸: " << capabilities.currentExtent.width 
                  << "x" << capabilities.currentExtent.height << std::endl;
        
        VkExtent2D result = chooseSwapExtent(capabilities);
        std::cout << "选择结果: " << result.width << "x" << result.height << std::endl;
        std::cout << "说明: 直接使用系统建议的尺寸" << std::endl;
    }
    
    void demonstrateCustomExtent() {
        std::cout << "\n--- 场景2: 自定义尺寸 ---" << std::endl;
        
        // 模拟系统允许自定义尺寸
        VkSurfaceCapabilitiesKHR capabilities = {
            {0xFFFFFFFF, 0xFFFFFFFF},  // currentExtent: UINT32_MAX 表示允许自定义
            {100, 100},                // minImageExtent: 最小100x100
            {2048, 2048}               // maxImageExtent: 最大2048x2048
        };
        
        std::cout << "系统允许自定义尺寸" << std::endl;
        std::cout << "最小尺寸: " << capabilities.minImageExtent.width 
                  << "x" << capabilities.minImageExtent.height << std::endl;
        std::cout << "最大尺寸: " << capabilities.maxImageExtent.width 
                  << "x" << capabilities.maxImageExtent.height << std::endl;
        
        // 测试不同的自定义尺寸
        testCustomSize(capabilities, 800, 600);   // 正常范围
        testCustomSize(capabilities, 50, 50);     // 小于最小值
        testCustomSize(capabilities, 3000, 3000); // 大于最大值
    }
    
    void testCustomSize(const VkSurfaceCapabilitiesKHR& capabilities, uint32_t width, uint32_t height) {
        std::cout << "\n   测试自定义尺寸: " << width << "x" << height << std::endl;
        
        VkExtent2D actualExtent = {width, height};
        actualExtent.width = clamp(actualExtent.width, 
                                  capabilities.minImageExtent.width, 
                                  capabilities.maxImageExtent.width);
        actualExtent.height = clamp(actualExtent.height, 
                                   capabilities.minImageExtent.height, 
                                   capabilities.maxImageExtent.height);
        
        std::cout << "   调整后尺寸: " << actualExtent.width << "x" << actualExtent.height << std::endl;
        
        if (actualExtent.width != width || actualExtent.height != height) {
            std::cout << "   ✓ 尺寸被限制在有效范围内" << std::endl;
        } else {
            std::cout << "   - 尺寸在有效范围内，无需调整" << std::endl;
        }
    }
    
    void demonstrateBoundaryCases() {
        std::cout << "\n--- 场景3: 边界情况 ---" << std::endl;
        
        // 边界情况1: 最小尺寸
        VkSurfaceCapabilitiesKHR capabilities1 = {
            {0xFFFFFFFF, 0xFFFFFFFF},
            {800, 600},    // 最小800x600
            {1920, 1080}   // 最大1920x1080
        };
        
        std::cout << "边界情况1: 请求尺寸小于最小值" << std::endl;
        testCustomSize(capabilities1, 400, 300);
        
        // 边界情况2: 最大尺寸
        std::cout << "\n边界情况2: 请求尺寸大于最大值" << std::endl;
        testCustomSize(capabilities1, 2560, 1440);
        
        // 边界情况3: 系统指定特殊尺寸
        VkSurfaceCapabilitiesKHR capabilities2 = {
            {1366, 768},   // 系统指定1366x768
            {1, 1},
            {4096, 4096}
        };
        
        std::cout << "\n边界情况3: 系统指定特殊尺寸" << std::endl;
        VkExtent2D result = chooseSwapExtent(capabilities2);
        std::cout << "结果: " << result.width << "x" << result.height << std::endl;
        std::cout << "说明: 忽略程序请求的尺寸，使用系统指定尺寸" << std::endl;
    }
    
    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) {
        if (capabilities.currentExtent.width != UINT32_MAX) {
            return capabilities.currentExtent;
        } else {
            VkExtent2D actualExtent = {800, 600}; // 模拟WIDTH=800, HEIGHT=600
            actualExtent.width = clamp(actualExtent.width, 
                                      capabilities.minImageExtent.width, 
                                      capabilities.maxImageExtent.width);
            actualExtent.height = clamp(actualExtent.height, 
                                       capabilities.minImageExtent.height, 
                                       capabilities.maxImageExtent.height);
            return actualExtent;
        }
    }
};

// 解释为什么需要这个函数
class ExtentExplanation {
public:
    void explainWhyNeeded() {
        std::cout << "\n=== 为什么需要这个函数？ ===" << std::endl;
        
        std::cout << "\n1. 窗口系统兼容性:" << std::endl;
        std::cout << "   - 不同操作系统对窗口尺寸有不同的要求" << std::endl;
        std::cout << "   - 某些系统会强制指定特定的图像尺寸" << std::endl;
        std::cout << "   - 程序必须遵循这些限制" << std::endl;
        
        std::cout << "\n2. 硬件限制:" << std::endl;
        std::cout << "   - GPU有最小和最大图像尺寸限制" << std::endl;
        std::cout << "   - 超出范围会导致创建失败" << std::endl;
        std::cout << "   - 需要确保尺寸在硬件支持范围内" << std::endl;
        
        std::cout << "\n3. 性能考虑:" << std::endl;
        std::cout << "   - 图像尺寸影响内存使用" << std::endl;
        std::cout << "   - 过大尺寸会浪费内存" << std::endl;
        std::cout << "   - 过小尺寸可能影响显示质量" << std::endl;
        
        std::cout << "\n4. 跨平台兼容性:" << std::endl;
        std::cout << "   - Windows: 通常允许自定义尺寸" << std::endl;
        std::cout << "   - Linux: 可能强制特定尺寸" << std::endl;
        std::cout << "   - macOS: 有特殊的尺寸要求" << std::endl;
    }
    
    void explainClampFunction() {
        std::cout << "\n=== std::clamp函数的作用 ===" << std::endl;
        
        std::cout << "\nstd::clamp(value, min, max) 的作用:" << std::endl;
        std::cout << "- 如果 value < min，返回 min" << std::endl;
        std::cout << "- 如果 value > max，返回 max" << std::endl;
        std::cout << "- 如果 min <= value <= max，返回 value" << std::endl;
        
        std::cout << "\n示例:" << std::endl;
        std::cout << "clamp(50, 100, 200) = " << clamp(50, 100, 200) << std::endl;
        std::cout << "clamp(150, 100, 200) = " << clamp(150, 100, 200) << std::endl;
        std::cout << "clamp(250, 100, 200) = " << clamp(250, 100, 200) << std::endl;
    }
};

int main() {
    SwapExtentChooser chooser;
    chooser.demonstrateExtentSelection();
    
    ExtentExplanation explanation;
    explanation.explainWhyNeeded();
    explanation.explainClampFunction();
    
    std::cout << "\n=== 总结 ===" << std::endl;
    std::cout << "这个函数确保交换链图像尺寸符合系统和硬件要求" << std::endl;
    std::cout << "通过智能选择尺寸，确保程序在不同平台上都能正常运行" << std::endl;
    std::cout << "是Vulkan跨平台兼容性的重要组成部分" << std::endl;
    
    return 0;
}
