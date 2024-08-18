#include "../../core.h"


void update_uniform_buffer(std::vector<void*>& uniform_buffers_mapped, const uint32_t& current_image, const VkExtent2D& extent, Gen_Camera camera);
void create_uniform_buffers(Gen_Devices& devices, Gen_Buffers& buffer_info);

