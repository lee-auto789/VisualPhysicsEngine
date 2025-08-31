#include <iostream>
#include <vector>
#include <glm/glm.hpp>

// 模拟Vulkan推送常量结构
struct PushConstants {
    glm::mat4 mvp;        // 模型-视图-投影矩阵 (64字节)
    glm::vec4 timeData;   // 时间相关数据 (16字节)
    // 总计: 80字节 < 128字节限制
};

// 模拟顶点着色器中的推送常量使用
class VertexShader {
public:
    void processVertex(const PushConstants& pushConstants, glm::vec3& position) {
        // 应用MVP变换
        glm::vec4 worldPos = pushConstants.mvp * glm::vec4(position, 1.0f);
        position = glm::vec3(worldPos);
        
        // 应用时间动画
        float time = pushConstants.timeData.x;
        position.y += sin(time * 2.0f) * 0.1f; // 简单的上下摆动
    }
};

// 模拟Vulkan推送常量设置
class PushConstantsManager {
public:
    void setupPushConstants() {
        std::cout << "=== 推送常量设置示例 ===" << std::endl;
        
        // 1. 定义推送常量范围
        setupPushConstantRange();
        
        // 2. 创建管线布局
        createPipelineLayout();
        
        // 3. 在渲染循环中使用
        demonstrateUsage();
    }

private:
    void setupPushConstantRange() {
        std::cout << "\n1. 设置推送常量范围:" << std::endl;
        std::cout << "   - 阶段: VK_SHADER_STAGE_VERTEX_BIT" << std::endl;
        std::cout << "   - 偏移: 0" << std::endl;
        std::cout << "   - 大小: sizeof(PushConstants) = " << sizeof(PushConstants) << " 字节" << std::endl;
        std::cout << "   - 限制: 128字节 (GPU限制)" << std::endl;
    }
    
    void createPipelineLayout() {
        std::cout << "\n2. 创建管线布局:" << std::endl;
        std::cout << "   - 包含推送常量范围" << std::endl;
        std::cout << "   - 用于图形管线创建" << std::endl;
    }
    
    void demonstrateUsage() {
        std::cout << "\n3. 渲染循环中的使用:" << std::endl;
        
        float time = 0.0f;
        for (int frame = 0; frame < 3; frame++) {
            std::cout << "\n   帧 " << frame << ":" << std::endl;
            
            // 更新推送常量数据
            PushConstants pushConstants;
            updatePushConstants(pushConstants, time);
            
            // 推送到GPU
            std::cout << "   - 更新MVP矩阵" << std::endl;
            std::cout << "   - 更新时间数据: " << time << std::endl;
            std::cout << "   - 推送到GPU: vkCmdPushConstants()" << std::endl;
            
            // 渲染
            std::cout << "   - 执行渲染命令" << std::endl;
            
            time += 0.016f; // 16ms per frame
        }
    }
    
    void updatePushConstants(PushConstants& pushConstants, float time) {
        // 设置MVP矩阵
        pushConstants.mvp = glm::mat4(1.0f); // 简化示例
        
        // 设置时间数据
        pushConstants.timeData = glm::vec4(time, 0.0f, 0.0f, 0.0f);
    }
};

// 对比推送常量与其他数据传递方式
class DataTransferComparison {
public:
    void compareMethods() {
        std::cout << "\n=== 数据传递方式对比 ===" << std::endl;
        
        std::cout << "\n1. 推送常量 (Push Constants):" << std::endl;
        std::cout << "   优点:" << std::endl;
        std::cout << "   - 访问速度最快 (GPU常量内存)" << std::endl;
        std::cout << "   - 设置简单，无需缓冲区" << std::endl;
        std::cout << "   - 适合频繁更新的小数据" << std::endl;
        std::cout << "   缺点:" << std::endl;
        std::cout << "   - 大小限制 (通常128字节)" << std::endl;
        std::cout << "   - 只能传递简单数据" << std::endl;
        std::cout << "   适用: 变换矩阵、时间、颜色等" << std::endl;
        
        std::cout << "\n2. 统一缓冲区 (Uniform Buffer):" << std::endl;
        std::cout << "   优点:" << std::endl;
        std::cout << "   - 大小限制较大" << std::endl;
        std::cout << "   - 可以传递复杂数据结构" << std::endl;
        std::cout << "   缺点:" << std::endl;
        std::cout << "   - 需要创建和管理缓冲区" << std::endl;
        std::cout << "   - 访问速度较慢" << std::endl;
        std::cout << "   适用: 大量数据、复杂结构" << std::endl;
        
        std::cout << "\n3. 存储缓冲区 (Storage Buffer):" << std::endl;
        std::cout << "   优点:" << std::endl;
        std::cout << "   - 大小几乎无限制" << std::endl;
        std::cout << "   - 可以读写" << std::endl;
        std::cout << "   缺点:" << std::endl;
        std::cout << "   - 访问速度最慢" << std::endl;
        std::cout << "   - 需要复杂的内存管理" << std::endl;
        std::cout << "   适用: 大量数据、计算着色器" << std::endl;
    }
};

// 在你的物理引擎中的应用示例
class PhysicsEnginePushConstants {
public:
    void demonstratePhysicsUsage() {
        std::cout << "\n=== 物理引擎中的推送常量应用 ===" << std::endl;
        
        std::cout << "\n1. 相机变换:" << std::endl;
        std::cout << "   - 视图矩阵 (16个float)" << std::endl;
        std::cout << "   - 投影矩阵 (16个float)" << std::endl;
        std::cout << "   - 用途: 2D/3D视角变换" << std::endl;
        
        std::cout << "\n2. 时间动画:" << std::endl;
        std::cout << "   - 当前时间 (1个float)" << std::endl;
        std::cout << "   - 时间间隔 (1个float)" << std::endl;
        std::cout << "   - 用途: 物理动画、粒子效果" << std::endl;
        
        std::cout << "\n3. 全局参数:" << std::endl;
        std::cout << "   - 重力方向 (3个float)" << std::endl;
        std::cout << "   - 缩放因子 (1个float)" << std::endl;
        std::cout << "   - 用途: 物理模拟参数" << std::endl;
        
        std::cout << "\n4. 渲染设置:" << std::endl;
        std::cout << "   - 背景颜色 (4个float)" << std::endl;
        std::cout << "   - 光照强度 (1个float)" << std::endl;
        std::cout << "   - 用途: 视觉效果控制" << std::endl;
    }
};

int main() {
    PushConstantsManager manager;
    manager.setupPushConstants();
    
    DataTransferComparison comparison;
    comparison.compareMethods();
    
    PhysicsEnginePushConstants physics;
    physics.demonstratePhysicsUsage();
    
    std::cout << "\n=== 总结 ===" << std::endl;
    std::cout << "推送常量是Vulkan中传递小量数据到着色器的高效方式" << std::endl;
    std::cout << "特别适合传递变换矩阵、时间、颜色等频繁更新的数据" << std::endl;
    std::cout << "在你的物理引擎中，可以用于传递相机变换、时间动画等参数" << std::endl;
    
    return 0;
}
