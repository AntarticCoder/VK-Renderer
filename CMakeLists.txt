cmake_minimum_required(VERSION 3.25)

set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER gcc)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin) 
set(CMAKE_CXX_FLAGS "-std=c++17 -O3")

add_subdirectory(libs/glfw)
add_subdirectory(libs/glm)

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

project(VK-Renderer CXX C)

add_executable(VK-Renderer
    ${CMAKE_SOURCE_DIR}/src/main.cpp

    ${CMAKE_SOURCE_DIR}/src/app-context/vk_application_context.cpp
)

target_include_directories(VK-Renderer PRIVATE
    ${CMAKE_SOURCE_DIR}/libs/glfw/include

    ${CMAKE_SOURCE_DIR}/libs/glm/glm
    ${CMAKE_SOURCE_DIR}/libs/glm/glm/detail
    ${CMAKE_SOURCE_DIR}/libs/glm/glm/ext
    ${CMAKE_SOURCE_DIR}/libs/glm/glm/gtc
    ${CMAKE_SOURCE_DIR}/libs/glm/glm/gtx
    ${CMAKE_SOURCE_DIR}/libs/glm/glm/simd

    ${Vulkan_INCLUDE_DIRS}
    
    ${CMAKE_SOURCE_DIR}/libs
    ${CMAKE_SOURCE_DIR}/src
)

add_compile_options(-Wno-dev)
target_link_libraries(VK-Renderer PUBLIC ${Vulkan_LIBRARIES} glfw glm stdc++)