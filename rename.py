
import os
import re

# Define the directories to scan and the file extensions to look for
project_directory = "src"
file_extensions = [".cpp", ".hpp"]

# Exclusion lists for Vulkan, GLM, and spdlog functions
vulkan_exclusions = {
    "vkCreateInstance", "vkDestroyInstance", "vkEnumeratePhysicalDevices", "vkGetPhysicalDeviceFeatures",
    "vkGetPhysicalDeviceFormatProperties", "vkGetPhysicalDeviceImageFormatProperties", "vkGetPhysicalDeviceProperties",
    "vkGetPhysicalDeviceQueueFamilyProperties", "vkGetPhysicalDeviceMemoryProperties", "vkCreateDevice", 
    "vkDestroyDevice", "vkEnumerateDeviceExtensionProperties", "vkGetDeviceProcAddr", "vkCreateShaderModule",
    # Add more Vulkan API calls as needed
}

glm_exclusions = {
    "glm::vec2", "glm::vec3", "glm::vec4", "glm::mat4", "glm::translate", "glm::rotate", "glm::scale",
    "glm::perspective", "glm::lookAt", "glm::normalize", "glm::cross", "glm::dot", 
    "glm::inverse", "glm::transpose", "glm::length",
    # Add more GLM functions and types as needed
}

spdlog_exclusions = {
    "spdlog::info", "spdlog::warn", "spdlog::error", "spdlog::critical", "spdlog::debug",
    "spdlog::trace", "spdlog::get", "spdlog::set_level", "spdlog::flush_on",
    # Add more spdlog functions as needed
}

# Combine all exclusions into a single set
exclusions = vulkan_exclusions | glm_exclusions | spdlog_exclusions

# Regular expression to match camelCase identifiers
camel_case_pattern = re.compile(r'\b[a-z]+[A-Z][a-zA-Z]*\b')

def camel_to_snake(name):
    """Convert a camelCase name to snake_case."""
    return re.sub(r'([a-z])([A-Z])', r'\1_\2', name).lower()

def rename_identifiers_in_file(filepath):
    with open(filepath, 'r') as file:
        content = file.read()

    # Find all camelCase identifiers
    identifiers = camel_case_pattern.findall(content)

    # Replace each camelCase identifier with its snake_case version, if not excluded
    for identifier in identifiers:
        if identifier not in exclusions:
            content = re.sub(r'\b' + re.escape(identifier) + r'\b', camel_to_snake(identifier), content)

    # Write the updated content back to the file
    with open(filepath, 'w') as file:
        file.write(content)

def recursively_rename_in_directory(directory):
    for root, _, files in os.walk(directory):
        for file in files:
            if any(file.endswith(ext) for ext in file_extensions):
                filepath = os.path.join(root, file)
                rename_identifiers_in_file(filepath)

# Run the renaming process
recursively_rename_in_directory(project_directory)

print("Renaming complete!")

