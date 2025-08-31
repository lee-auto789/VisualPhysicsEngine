#include <iostream>
#include <vector>
#include <thread>
#include <chrono>

// 模拟同步对象
struct SyncObject {
    int id;
    bool isSignaled = false;
    
    SyncObject(int i) : id(i) {}
    
    void signal() { 
        isSignaled = true; 
        std::cout << "SyncObject " << id << " signaled" << std::endl;
    }
    
    void wait() { 
        while (!isSignaled) {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
        std::cout << "SyncObject " << id << " waited" << std::endl;
    }
    
    void reset() { 
        isSignaled = false; 
        std::cout << "SyncObject " << id << " reset" << std::endl;
    }
};

int main() {
    const int MAX_FRAMES_IN_FLIGHT = 3;
    
    // 方法1: 错误 - 只用一个同步对象
    std::cout << "=== 错误方法: 只用一个同步对象 ===" << std::endl;
    SyncObject singleSync(0);
    
    // 模拟3帧同时进行
    for (int frame = 0; frame < 3; frame++) {
        std::cout << "Frame " << frame << " 开始" << std::endl;
        
        // 问题：所有帧都在等待同一个对象！
        singleSync.wait();  // 帧0等待，帧1也等待，帧2也等待
        
        // 模拟渲染
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        
        singleSync.signal();  // 只有最后一个帧会触发
        std::cout << "Frame " << frame << " 完成" << std::endl;
    }
    
    std::cout << "\n=== 正确方法: 每帧独立的同步对象 ===" << std::endl;
    
    // 方法2: 正确 - 每帧独立的同步对象
    std::vector<SyncObject> frameSyncs;
    for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        frameSyncs.emplace_back(i);
    }
    
    int currentFrame = 0;
    for (int frame = 0; frame < 3; frame++) {
        std::cout << "Frame " << frame << " 使用 SyncObject " << currentFrame << std::endl;
        
        // 每帧等待自己的同步对象
        frameSyncs[currentFrame].wait();
        
        // 模拟渲染
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        
        frameSyncs[currentFrame].signal();
        std::cout << "Frame " << frame << " 完成" << std::endl;
        
        // 移动到下一个同步对象
        currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
    }
    
    return 0;
}
