
#include "vulkan.hpp"
#include "commandpool.hpp"
#include "core/physicalDevice.hpp"
#include "swapchain.hpp"
#include <GLFW/glfw3.h>
#include <cstdint>
#include <iostream>
#include <ostream>
#include <vulkan/vulkan_core.h>


void mouse_callback(GLFWwindow* window, double xpos, double ypos);

int Vulkan::init_vulkan()
{
  m_window_info.window = create_window(&m_window_info.frame_buffer_resized);
  m_last_time = glfwGetTime();
  glfwSetCursorPosCallback(m_window_info.window, mouse_callback);

  create_instance(m_vk_instance, m_validation_layers); //Instance of vulkan, set up validation layers
  setup_debug_messenger(m_vk_instance, &m_debug_messenger);
  create_surface(m_vk_instance, m_window_info);
  pick_physical_device(m_vk_instance, m_devices, m_window_info.surface, m_device_extensions);
  create_logical_device(m_devices, m_graphics_queue, m_present_queue, m_validation_layers, m_device_extensions, m_window_info.surface);
  create_swap_chain(m_devices, m_swapchain_info, m_window_info);
  create_image_views(m_devices.logical_device, m_swapchain_info);
  create_renderpass(m_devices.logical_device, m_graphics_info.render_pass, m_swapchain_info.swap_chain_image_format);
  create_descriptor_set_layout(m_devices.logical_device, m_descriptor_set_info.descriptor_set_layout, m_graphics_info.pipeline_layout);

  create_graphics_pipelines(m_devices.logical_device, m_graphics_info, m_swapchain_info.swap_chain_extent, m_descriptor_set_info.descriptor_set_layout);


  create_framebuffers(m_devices.logical_device, m_swapchain_info, m_graphics_info.render_pass);
  create_command_pool(m_devices, m_window_info.surface, m_command_pool);
  //create_image_texture(m_devices, m_image_texture_info, m_graphics_queue, m_command_pool, "../images/sanic.png");
  //create_texture_image_view(m_devices.logical_device, m_image_texture_info);
  //create_texture_sampler(m_devices, m_image_texture_info.texture_sampler);
  create_vertex_buffer(m_devices, m_buffer_info, m_graphics_queue, m_command_pool, m_vertices);
  create_index_buffer(m_devices, m_buffer_info, m_graphics_queue, m_command_pool, m_indices);
  create_uniform_buffers(m_devices, m_buffer_info);
  create_descriptor_pool(m_devices.logical_device, m_descriptor_set_info.descriptor_pool);

  create_descriptor_sets(m_devices.logical_device, m_buffer_info.uniform_buffers, m_image_texture_info, m_descriptor_set_info);
  create_command_buffers(m_devices.logical_device, m_command_pool, m_buffer_info.command_buffers);

  create_sync_objects(m_devices.logical_device, m_sync_info);
  return 1;
}

int Vulkan::update()
{

  if (!glfwWindowShouldClose(m_window_info.window))
  {
    glfwPollEvents();
    float current_time = glfwGetTime();
    m_delta_time = static_cast<float>(current_time - m_last_time);
    m_last_time = current_time;

    /* GenLogInfo("DT: {}", m_delta_time); */

    process_input(m_window_info.window, m_delta_time);
    draw_frame();
  }
  vkDeviceWaitIdle(m_devices.logical_device);

  return !glfwWindowShouldClose(m_window_info.window);
}



void mouse_callback(GLFWwindow* window, double xpos, double ypos) 
{
    Vulkan* vulkan_app = static_cast<Vulkan*>(glfwGetWindowUserPointer(window));

    /* GenLogInfo("Mouse was moved: xpos={}, ypos={}", xpos, ypos); */

    if (!vulkan_app)
    {
        GenLogCritical("Cant get vulkan instance!");
    }
        

    static float yaw = -90.0f;  // Yaw starts pointing along the negative Z-axis
    static float pitch = 0.0f;

    static double lastX = 400, lastY = 300;  // Initially set to the center of the window
    static bool firstMouse = true;

    static float mouse_sens = 1.25f;

    if (firstMouse) 
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;  // Reversed since y-coordinates go from bottom to top
    lastX = xpos;
    lastY = ypos;

    xoffset *= mouse_sens;
    yoffset *= mouse_sens;

    yaw   += xoffset;
    pitch += yoffset;

    /* Constrain the pitch so the screen doesn't flip */
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    // Update the camera front vector
    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    vulkan_app->set_cam_front(glm::normalize(front));  // Call the class method
}

void Vulkan::set_cam_front(glm::vec3 front)
{
    m_cam.cam_front = front;
}

void Vulkan::process_input(GLFWwindow* window, float dt)
{
    float velocity  = m_ms * dt;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        m_cam.cam_pos += m_cam.cam_front * velocity;  // Move forward
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        m_cam.cam_pos -= m_cam.cam_front * velocity;  // Move backward
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        m_cam.cam_pos -= glm::normalize(glm::cross(m_cam.cam_front , m_cam.cam_up)) * velocity; // Move left
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        m_cam.cam_pos += glm::normalize(glm::cross(m_cam.cam_front, m_cam.cam_up)) * velocity; // Move right
}


void Vulkan::draw_frame()
{
    // Wait for the previous frame to finish
    vkWaitForFences(m_devices.logical_device, 1, &m_sync_info.in_flight_fences[m_current_frame], VK_TRUE, UINT64_MAX); // Wait for fence that tells us we are not rendering anything currently

    // Acquire new image from swapchain
    uint32_t image_index;
    VkResult result = vkAcquireNextImageKHR(m_devices.logical_device, m_swapchain_info.swap_chain,
                                            UINT64_MAX, m_sync_info.image_available_semaphores[m_current_frame],
                                            VK_NULL_HANDLE, &image_index);

    // Check if we need to recreate swapchain, and if we do, we recreate it
    if (result == VK_ERROR_OUT_OF_DATE_KHR)
    {
        recreate_swapchain(m_devices, m_swapchain_info, m_window_info, m_graphics_info.render_pass);
    }
    else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
    {
        GenLogCritical("Failed to acquire swap chain image! In vulkan.cpp:draw_frame()");
    }

    update_uniform_buffer(m_buffer_info.uniform_buffers_mapped, m_current_frame, m_swapchain_info.swap_chain_extent, m_cam);
    vkResetFences(m_devices.logical_device, 1, &m_sync_info.in_flight_fences[m_current_frame]); // Reset manually

    // Record command buffer that draws scene to image
    vkResetCommandBuffer(m_buffer_info.command_buffers[m_current_frame], 0);

    record_command_buffer(m_graphics_info, m_swapchain_info, m_buffer_info, m_descriptor_set_info, m_indices, m_current_frame, image_index);

    // Submit recorded command buffer
    VkSubmitInfo submit_info{};
    submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore wait_semaphores[] = {m_sync_info.image_available_semaphores[m_current_frame]};
    VkPipelineStageFlags wait_stages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submit_info.waitSemaphoreCount = 1;
    submit_info.pWaitSemaphores = wait_semaphores;
    submit_info.pWaitDstStageMask = wait_stages;

    submit_info.commandBufferCount = 1;
    submit_info.pCommandBuffers = &m_buffer_info.command_buffers[m_current_frame];

    VkSemaphore signal_semaphores[] = {m_sync_info.render_finished_semaphores[m_current_frame]};
    submit_info.signalSemaphoreCount = 1;
    submit_info.pSignalSemaphores = signal_semaphores;

    if (vkQueueSubmit(m_graphics_queue, 1, &submit_info, m_sync_info.in_flight_fences[m_current_frame]) != VK_SUCCESS)
    {
        GenLogCritical("Failed to submit draw command buffer! In window.cpp");
    }

    // Present swap chain image
    VkPresentInfoKHR present_info{};
    present_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    present_info.waitSemaphoreCount = 1;
    present_info.pWaitSemaphores = signal_semaphores;

    VkSwapchainKHR swap_chains[] = {m_swapchain_info.swap_chain};
    present_info.swapchainCount = 1;
    present_info.pSwapchains = swap_chains;
    present_info.pImageIndices = &image_index;
    present_info.pResults = nullptr;

    result = vkQueuePresentKHR(m_present_queue, &present_info); // Oh my LOOORD

    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || m_window_info.frame_buffer_resized)
    {
        m_window_info.frame_buffer_resized = false; // Reset the flag of resetting
        recreate_swapchain(m_devices, m_swapchain_info, m_window_info, m_graphics_info.render_pass);
    }
    else if (result != VK_SUCCESS)
    {
        GenLogCritical("Failed to present swap chain image! In vulkan.cpp:draw_frame()");
    }

    m_current_frame = (m_current_frame + 1) % 2;
}

void Vulkan::cleanup()
{
    cleanup_swap_chain(m_devices.logical_device, m_swapchain_info);

    vkDestroyPipeline(m_devices.logical_device, m_graphics_info.graphics_pipeline, nullptr);
    vkDestroyPipelineLayout(m_devices.logical_device, m_graphics_info.pipeline_layout, nullptr);
    vkDestroyRenderPass(m_devices.logical_device, m_graphics_info.render_pass, nullptr);

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    {
        vkDestroyBuffer(m_devices.logical_device, m_buffer_info.uniform_buffers[i], nullptr);
        vkFreeMemory(m_devices.logical_device, m_buffer_info.uniform_buffers_memory[i], nullptr);
    }

    vkDestroyDescriptorPool(m_devices.logical_device, m_descriptor_set_info.descriptor_pool, nullptr);
    vkDestroySampler(m_devices.logical_device, m_image_texture_info.texture_sampler, nullptr);
    vkDestroyImageView(m_devices.logical_device, m_image_texture_info.texture_image_view, nullptr);
    vkDestroyImage(m_devices.logical_device, m_image_texture_info.image, nullptr);
    vkFreeMemory(m_devices.logical_device, m_image_texture_info.texture_image_memory, nullptr);
    vkDestroyDescriptorSetLayout(m_devices.logical_device, m_descriptor_set_info.descriptor_set_layout, nullptr);

    vkDestroyBuffer(m_devices.logical_device, m_buffer_info.index_buffer, nullptr);
    vkFreeMemory(m_devices.logical_device, m_buffer_info.index_buffer_memory, nullptr);

    vkDestroyBuffer(m_devices.logical_device, m_buffer_info.vertex_buffer, nullptr);
    vkFreeMemory(m_devices.logical_device, m_buffer_info.vertex_buffer_memory, nullptr);

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    {
        vkDestroySemaphore(m_devices.logical_device, m_sync_info.image_available_semaphores[i], nullptr);
        vkDestroySemaphore(m_devices.logical_device, m_sync_info.render_finished_semaphores[i], nullptr);
        vkDestroyFence(m_devices.logical_device, m_sync_info.in_flight_fences[i], nullptr);
    }

    vkDestroyCommandPool(m_devices.logical_device, m_command_pool, nullptr);
    vkDeviceWaitIdle(m_devices.logical_device);
    vkDestroyDevice(m_devices.logical_device, nullptr);

    if (DEBUG)
    {
        destroy_debug_utils_messengerEXT(m_vk_instance, m_debug_messenger, nullptr);
    }

    vkDestroySurfaceKHR(m_vk_instance, m_window_info.surface, nullptr);
    vkDestroyInstance(m_vk_instance, nullptr);

    glfwDestroyWindow(m_window_info.window);
    glfwTerminate();
}

