#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <vector>
#include <vulkan/vulkan_core.h>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <cstdlib>
#include <iostream>
#include <stdexcept>

const uint32_t WIDHT = 800;
const uint32_t HEIGHT = 600;

const std::vector<const char *> validationLayers = {
    "VK_LAYER_KHRONOS_validation"};

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

class HelloTriangleApplication {
public:
  void run() {
    initWindow();
    initVulkan();
    mainLoop();
    cleanup();
  }

private:
  void initWindow() {
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    window = glfwCreateWindow(WIDHT, HEIGHT, "Vulkan", nullptr, nullptr);
  }

  void createInstance() {
    if (enableValidationLayers && !checkValidationLayerSupport()) {
      throw std::runtime_error(
          "validation layers requested, but not available!");
    }
    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Hello Triangle";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    if (enableValidationLayers) {
      addValidationLayers(createInfo);
    } else {
      createInfo.enabledLayerCount = 0;
    }

    getExtensions(createInfo);

    if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
      throw std::runtime_error("Failed to create instance!");
    }
  }

  void addValidationLayers(VkInstanceCreateInfo &createInfo) {
    createInfo.enabledLayerCount =
        static_cast<uint32_t>(validationLayers.size());
    createInfo.ppEnabledLayerNames = validationLayers.data();
  }

  void getExtensions(VkInstanceCreateInfo &createInfo) {
    uint32_t glfwExtensionCount = 0;
    const char **glfwExtensions;

    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    createInfo.enabledExtensionCount = glfwExtensionCount;
    createInfo.ppEnabledExtensionNames = glfwExtensions;
    createInfo.enabledLayerCount = 0;

    std::vector<VkExtensionProperties> extensions(0);
    getSupportedExtensions(extensions);
    checkIfRequiredExtensionsAreSupported(extensions);
  }

  void getSupportedExtensions(std::vector<VkExtensionProperties> &extensions) {
    uint32_t extensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
    extensions.resize(extensionCount);
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount,
                                           extensions.data());

    std::cout << "Available extensions: \n";
    for (const auto &extension : extensions) {
      std::cout << "\t" << extension.extensionName << "\n";
    }
  }

  void checkIfRequiredExtensionsAreSupported(
      std::vector<VkExtensionProperties> &supportedExtensions) {

    uint32_t requiredExtensionsLength = 0;

    const char **requiredExtensions =
        glfwGetRequiredInstanceExtensions(&requiredExtensionsLength);

    if (requiredExtensions == NULL) {
      throw std::runtime_error(
          "Seems like Vulkan is not available on the current machine");
    }

    std::cout << "Required extensions: \n";

    for (int i = 0; i < requiredExtensionsLength; ++i) {
      std::cout << "\t" << requiredExtensions[i] << "\n";
      bool found = false;

      for (const auto &extension : supportedExtensions) {
        if (strcmp(extension.extensionName, requiredExtensions[i]) == 0) {
          found = true;
          continue;
        }
      }

      if (!found) {
        char error[256];
        sprintf(error, "One required extension is not supported: %s",
                requiredExtensions[i]);
        throw std::runtime_error(error);
      }
    }
  }

  bool checkValidationLayerSupport() {
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    bool layerFound;

    for (auto &neededLayer : validationLayers) {
      layerFound = false;
      for (auto &supportedLayer : availableLayers) {
        if (strcmp(neededLayer, supportedLayer.layerName) == 0) {
          layerFound = true;
          break;
        }
      }
      if (!layerFound) {
        return false;
      }
    }

    return true;
  }

  void initVulkan() {
    createInstance();
    pickPhysicalDevice();
  }

  void pickPhysicalDevice() {
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
    if (deviceCount == 0) {
      throw std::runtime_error("failed to find GPUs with Vulkan support!");
    }
    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());
    for (const auto &device : devices) {
      if (isDeviceSuitable(device)) {
        physicalDevice = device;
        break;
      }
    }
    if (physicalDevice == VK_NULL_HANDLE) {
      throw std::runtime_error("failed to find a suitable GPU!");
    }
  }

  bool isDeviceSuitable(VkPhysicalDevice device) { return true; }

  void mainLoop() {
    while (!glfwWindowShouldClose(window)) {
      glfwPollEvents();
    }
  }

  void cleanup() {
    vkDestroyInstance(instance, nullptr);

    glfwDestroyWindow(window);
    glfwTerminate();
  }

private:
  GLFWwindow *window;
  VkInstance instance;
  VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
};

int main() {
  HelloTriangleApplication app;

  try {
    app.run();
  } catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
