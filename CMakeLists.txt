cmake_minimum_required(VERSION 3.25)

set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER gcc)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin) 
set(CMAKE_CXX_FLAGS "-std=c++17 -O3")

add_compile_options(-O0)

find_package(Vulkan REQUIRED)

if (DEFINED VULKAN_SDK_PATH)
    set(Vulkan_INCLUDE_DIRS "${VULKAN_SDK_PATH}/Include")
    set(Vulkan_LIBRARIES "${VULKAN_SDK_PATH}/Lib")
    set(Vulkan_FOUND "True")
else()
    find_package(Vulkan REQUIRED)
    message(STATUS "Found Vulkan: $ENV{VULKAN_SDK}")
endif()

if (NOT Vulkan_FOUND)
	message(FATAL_ERROR "Could not find Vulkan library!")
else()
	message(STATUS "Using Vulkan lib at: ${Vulkan_LIBRARIES}")
endif()

file(GLOB IMGUI_BASE_FILES ${CMAKE_SOURCE_DIR}/libs/imgui/*.cpp)
set(IMGUI_VULKAN_FILES ${CMAKE_SOURCE_DIR}/libs/imgui/backends/imgui_impl_vulkan.cpp)
set(IMGUI_WINDOW_FILES ${CMAKE_SOURCE_DIR}/libs/imgui/backends/imgui_impl_glfw.cpp)

add_subdirectory(libs/glfw)
add_subdirectory(libs/glm)
add_subdirectory(libs/VulkanMemoryAllocator)

project(VK-Renderer CXX C)

add_executable(VK-Renderer
    ${CMAKE_SOURCE_DIR}/src/main.cpp

    ${CMAKE_SOURCE_DIR}/src/api/app-context/vk_instance.cpp
    ${CMAKE_SOURCE_DIR}/src/api/app-context/vk_device.cpp
    ${CMAKE_SOURCE_DIR}/src/api/app-context/vk_swapchain.cpp
    ${CMAKE_SOURCE_DIR}/src/api/app-context/vk_window.cpp

    ${CMAKE_SOURCE_DIR}/src/api/descriptors/vk_descriptor_set.cpp
    ${CMAKE_SOURCE_DIR}/src/api/descriptors/vk_descriptor_set_layout.cpp

    ${CMAKE_SOURCE_DIR}/src/api/commands/vk_command_pool.cpp
    ${CMAKE_SOURCE_DIR}/src/api/commands/vk_command_buffer.cpp

    ${CMAKE_SOURCE_DIR}/src/api/graphics-pipeline/vk_shader_module.cpp
    ${CMAKE_SOURCE_DIR}/src/api/graphics-pipeline/vk_renderpass.cpp
    ${CMAKE_SOURCE_DIR}/src/api/graphics-pipeline/vk_graphics_pipeline.cpp

    ${CMAKE_SOURCE_DIR}/src/api/memory/vk_buffer.cpp
    ${CMAKE_SOURCE_DIR}/src/api/memory/vk_vma_allocator.cpp

    ${CMAKE_SOURCE_DIR}/src/renderer/vk_renderer.cpp

    ${IMGUI_BASE_FILES}
    ${IMGUI_VULKAN_FILES}
    ${IMGUI_WINDOW_FILES}
)

target_include_directories(VK-Renderer PRIVATE
    ${CMAKE_SOURCE_DIR}/libs/glfw/include

    ${CMAKE_SOURCE_DIR}/libs/glm/glm
    ${CMAKE_SOURCE_DIR}/libs/glm/glm/detail
    ${CMAKE_SOURCE_DIR}/libs/glm/glm/ext
    ${CMAKE_SOURCE_DIR}/libs/glm/glm/gtc
    ${CMAKE_SOURCE_DIR}/libs/glm/glm/gtx
    ${CMAKE_SOURCE_DIR}/libs/glm/glm/simd

    ${CMAKE_SOURCE_DIR}/libs/VulkanMemoryAllocator/include

    ${CMAKE_SOURCE_DIR}/libs/imgui

    ${Vulkan_INCLUDE_DIRS}
    
    ${CMAKE_SOURCE_DIR}/libs
    ${CMAKE_SOURCE_DIR}/src
)

add_compile_options(-Wno-dev)
target_link_libraries(VK-Renderer PUBLIC Vulkan::Vulkan glfw glm VulkanMemoryAllocator stdc++)

set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CMAKE_COMMAND} -E time")
set_property(TARGET VK-Renderer PROPERTY RULE_LAUNCH_COMPILE "${CMAKE_COMMAND} -E time")