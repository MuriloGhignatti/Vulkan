#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <vector>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <cstdlib>
#include <iostream>
#include <stdexcept>

const uint32_t WIDHT = 800;
const uint32_t HEIGHT = 600;

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
    getExtensions(createInfo);

    if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
      throw std::runtime_error("Failed to create instance!");
    }
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

  void initVulkan() { createInstance(); }

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
