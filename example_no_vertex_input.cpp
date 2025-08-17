#include <vulkan/vulkan.h>
#include <vector>

// 示例：全屏四边形渲染（禁用顶点输入）
void createFullscreenQuadPipeline(VkDevice device, VkRenderPass renderPass) {
    // 全屏四边形顶点着色器（使用内置变量）
    const char* vertexShaderSource = R"(
        #version 450
        
        void main() {
            // 使用 gl_VertexIndex 生成全屏四边形
            vec2 positions[4] = vec2[](
                vec2(-1.0, -1.0),  // 左下
                vec2( 1.0, -1.0),  // 右下
                vec2(-1.0,  1.0),  // 左上
                vec2( 1.0,  1.0)   // 右上
            );
            
            gl_Position = vec4(positions[gl_VertexIndex], 0.0, 1.0);
        }
    )";
    
    // 片元着色器（后处理效果）
    const char* fragmentShaderSource = R"(
        #version 450
        
        layout(location = 0) out vec4 outColor;
        
        void main() {
            // 使用 gl_FragCoord 计算UV坐标
            vec2 uv = gl_FragCoord.xy / vec2(800, 600);
            
            // 简单的渐变效果
            outColor = vec4(uv.x, uv.y, 0.5, 1.0);
        }
    )";
    
    // 创建着色器模块...
    
    // 关键：禁用顶点输入
    VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputInfo.vertexBindingDescriptionCount = 0;    // 没有顶点绑定
    vertexInputInfo.vertexAttributeDescriptionCount = 0;  // 没有顶点属性
    
    // 其他管线状态...
    
    // 渲染时使用 vkCmdDraw 而不是 vkCmdBindVertexBuffers
    // vkCmdDraw(commandBuffer, 4, 1, 0, 0); // 绘制4个顶点
}

// 示例：GPU粒子系统（禁用顶点输入）
void createParticleSystemPipeline(VkDevice device, VkRenderPass renderPass) {
    // 顶点着色器从存储缓冲区读取粒子数据
    const char* vertexShaderSource = R"(
        #version 450
        
        layout(set = 0, binding = 0) buffer ParticleBuffer {
            vec4 particles[]; // xyz=position, w=size
        } particleBuffer;
        
        void main() {
            // 从存储缓冲区读取粒子数据
            vec4 particle = particleBuffer.particles[gl_VertexIndex];
            
            gl_Position = vec4(particle.xyz, 1.0);
            gl_PointSize = particle.w;
        }
    )";
    
    // 禁用顶点输入，因为数据来自存储缓冲区
    VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputInfo.vertexBindingDescriptionCount = 0;
    vertexInputInfo.vertexAttributeDescriptionCount = 0;
    
    // 渲染时
    // vkCmdDraw(commandBuffer, particleCount, 1, 0, 0);
}

// 示例：程序化几何体生成
void createProceduralGeometryPipeline(VkDevice device, VkRenderPass renderPass) {
    // 顶点着色器根据索引生成球体
    const char* vertexShaderSource = R"(
        #version 450
        
        void main() {
            // 根据顶点索引生成球体坐标
            int vertexIndex = gl_VertexIndex;
            int ring = vertexIndex / 36;
            int segment = vertexIndex % 36;
            
            float phi = float(ring) * 2.0 * 3.14159 / 18.0;
            float theta = float(segment) * 3.14159 / 18.0;
            
            vec3 pos = vec3(
                sin(theta) * cos(phi),
                sin(theta) * sin(phi),
                cos(theta)
            );
            
            gl_Position = vec4(pos, 1.0);
        }
    )";
    
    // 禁用顶点输入，因为几何体是程序化生成的
    VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputInfo.vertexBindingDescriptionCount = 0;
    vertexInputInfo.vertexAttributeDescriptionCount = 0;
    
    // 渲染时
    // vkCmdDraw(commandBuffer, sphereVertexCount, 1, 0, 0);
}
