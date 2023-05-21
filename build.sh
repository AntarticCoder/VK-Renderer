clear

export VULKAN_SDK=/Users/nick/VulkanSDK/1.3.243.0/macOS
export VK_LAYER_PATH=/Users/nick/VulkanSDK/1.3.243.0/macOS/share/vulkan/explicit_layer.d
export VK_ICD_FILENAMES=/Users/nick/VulkanSDK/1.3.243.0/macOS/share/vulkan/icd.d/MoltenVK_icd.json

cmake -DCMAKE_BUILD_TYPE=Debug .
make

rm cmake_install.cmake
rm Makefile