#include <iostream>
using namespace std;

void initVulkan() {
    cout << "initVulkan" << endl;
    VkApplicationInfo appInfo = {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Hello Triangle";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    VkInstance instance;
    VkResult result = vkCreateInstance(&createInfo, nullptr, &instance);
    if (result != VK_SUCCESS) {
        throw runtime_error("failed to create instance!");
    }

    uint32_t extensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
    cout << "extensionCount = " << extensionCount << endl;

    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
    cout << "deviceCount = " << deviceCount << endl;

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

    VkPhysicalDevice selectedDevice = VK_NULL_HANDLE;
    for (const auto& device : devices) {
        VkPhysicalDeviceProperties deviceProperties;
        vkGetPhysicalDeviceProperties(device, &deviceProperties);
        cout << "deviceProperties.deviceName = " << deviceProperties.deviceName.data() << endl;
        if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
            cout << "Discrete GPU" << endl;
            selectedDevice = device;
            break;
        }
    }

    if (selectedDevice == VK_NULL_HANDLE) {
        throw runtime_error("No discrete GPU found!");
    }

    VkPhysicalDeviceProperties deviceProperties;
    vkGetPhysicalDeviceProperties(selectedDevice, &deviceProperties);
    cout << "deviceProperties.deviceName = " << deviceProperties.deviceName.data() << endl;

    VkPhysicalDeviceFeatures deviceFeatures;
    vkGetPhysicalDeviceFeatures(selectedDevice, &deviceFeatures);
    cout << "deviceFeatures.geometryShader = " << deviceFeatures.geometryShader << endl;

    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(selectedDevice, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilyProperties(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(selectedDevice, &queueFamilyCount, queueFamilyProperties.data());

    VkDeviceQueueCreateInfo queueCreateInfo = {};
    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex = 0;
    queueCreateInfo.queueCount = 1;

    VkDevice device;
    vkCreateDevice(selectedDevice, &queueCreateInfo, nullptr, &device);

    VkQueue queue;
    vkGetDeviceQueue(device, 0, 0, &queue);

    vkDestroyDevice(device, nullptr);


}

void mainLoop() {

}

int main() {
    int a = 10;
    int b = 20;
    int c = a + b;
    cout << "c = " << c << endl;
    return 0;
}



