#ifndef VULKAN_SYSTEM_ERROR_INCLUDE_GUARD_SYSTEM_ERROR_HPP
#define VULKAN_SYSTEM_ERROR_INCLUDE_GUARD_SYSTEM_ERROR_HPP

#include <vulkan/vulkan_core.h>

#include <system_error>
#include <type_traits>

namespace vulkan_system_error {

enum class vulkan_errc {
    success                     = VK_SUCCESS,
    not_ready                   = VK_NOT_READY,
    timeout                     = VK_TIMEOUT,
    event_set                   = VK_EVENT_SET,
    event_reset                 = VK_EVENT_RESET,
    incomplete                  = VK_INCOMPLETE,
    out_of_host_memory          = VK_ERROR_OUT_OF_HOST_MEMORY,
    out_of_device_memory        = VK_ERROR_OUT_OF_DEVICE_MEMORY,
    initialization_failed       = VK_ERROR_INITIALIZATION_FAILED,
    device_lost                 = VK_ERROR_DEVICE_LOST,
    memory_map_failed           = VK_ERROR_MEMORY_MAP_FAILED,
    layer_not_present           = VK_ERROR_LAYER_NOT_PRESENT,
    extension_not_present       = VK_ERROR_EXTENSION_NOT_PRESENT,
    feature_not_present         = VK_ERROR_FEATURE_NOT_PRESENT,
    incompatible_driver         = VK_ERROR_INCOMPATIBLE_DRIVER,
    too_many_objects            = VK_ERROR_TOO_MANY_OBJECTS,
    format_not_supported        = VK_ERROR_FORMAT_NOT_SUPPORTED,
    fragmented_pool             = VK_ERROR_FRAGMENTED_POOL,
    out_of_pool_memory          = VK_ERROR_OUT_OF_POOL_MEMORY,
    invalid_external_handle     = VK_ERROR_INVALID_EXTERNAL_HANDLE,
    surface_lost_KHR            = VK_ERROR_SURFACE_LOST_KHR,
    native_window_in_use_KHR    = VK_ERROR_NATIVE_WINDOW_IN_USE_KHR,
    suboptimal_KHR              = VK_SUBOPTIMAL_KHR,
    out_of_date_KHR             = VK_ERROR_OUT_OF_DATE_KHR,
    incompatible_display_KHR    = VK_ERROR_INCOMPATIBLE_DISPLAY_KHR,
    validation_failed_EXT       = VK_ERROR_VALIDATION_FAILED_EXT,
    invalid_shader_NV           = VK_ERROR_INVALID_SHADER_NV,
    not_permitted_EXT           = VK_ERROR_NOT_PERMITTED_EXT,
    out_of_pool_memory_KHR      = VK_ERROR_OUT_OF_POOL_MEMORY_KHR,
    invalid_external_handle_KHR = VK_ERROR_INVALID_EXTERNAL_HANDLE_KHR
};

std::error_category const& vulkan_category();

inline std::error_code make_error_code(VkResult e) {
    return std::error_code(e, vulkan_category());
}

inline std::error_code make_error_code(vulkan_errc e) {
    return std::error_code(static_cast<int>(e), vulkan_category());
}

inline std::error_condition make_error_condition(vulkan_errc e) {
    return std::error_condition(static_cast<int>(e), vulkan_category());
}

namespace detail {
class vulkan_category_impl : public std::error_category {
public:
    ~vulkan_category_impl() override = default;
    char const* name() const noexcept override;
    std::string message(int ev) const override;
};

inline char const* vulkan_category_impl::name() const noexcept {
    return "vulkan";
}

inline std::string vulkan_category_impl::message(int ev) const {
#define VULKAN_SYSTEM_ERROR_VKRESULT_PRINT_CASE(ec) case VK_ ##ec: return #ec
    switch(ev) {
    default: return "Unknown Vulkan error.";
    VULKAN_SYSTEM_ERROR_VKRESULT_PRINT_CASE(SUCCESS);
    VULKAN_SYSTEM_ERROR_VKRESULT_PRINT_CASE(NOT_READY);
    VULKAN_SYSTEM_ERROR_VKRESULT_PRINT_CASE(TIMEOUT);
    VULKAN_SYSTEM_ERROR_VKRESULT_PRINT_CASE(EVENT_SET);
    VULKAN_SYSTEM_ERROR_VKRESULT_PRINT_CASE(EVENT_RESET);
    VULKAN_SYSTEM_ERROR_VKRESULT_PRINT_CASE(INCOMPLETE);
    VULKAN_SYSTEM_ERROR_VKRESULT_PRINT_CASE(ERROR_OUT_OF_HOST_MEMORY);
    VULKAN_SYSTEM_ERROR_VKRESULT_PRINT_CASE(ERROR_OUT_OF_DEVICE_MEMORY);
    VULKAN_SYSTEM_ERROR_VKRESULT_PRINT_CASE(ERROR_INITIALIZATION_FAILED);
    VULKAN_SYSTEM_ERROR_VKRESULT_PRINT_CASE(ERROR_DEVICE_LOST);
    VULKAN_SYSTEM_ERROR_VKRESULT_PRINT_CASE(ERROR_MEMORY_MAP_FAILED);
    VULKAN_SYSTEM_ERROR_VKRESULT_PRINT_CASE(ERROR_LAYER_NOT_PRESENT);
    VULKAN_SYSTEM_ERROR_VKRESULT_PRINT_CASE(ERROR_EXTENSION_NOT_PRESENT);
    VULKAN_SYSTEM_ERROR_VKRESULT_PRINT_CASE(ERROR_FEATURE_NOT_PRESENT);
    VULKAN_SYSTEM_ERROR_VKRESULT_PRINT_CASE(ERROR_INCOMPATIBLE_DRIVER);
    VULKAN_SYSTEM_ERROR_VKRESULT_PRINT_CASE(ERROR_TOO_MANY_OBJECTS);
    VULKAN_SYSTEM_ERROR_VKRESULT_PRINT_CASE(ERROR_FORMAT_NOT_SUPPORTED);
    VULKAN_SYSTEM_ERROR_VKRESULT_PRINT_CASE(ERROR_FRAGMENTED_POOL);
    VULKAN_SYSTEM_ERROR_VKRESULT_PRINT_CASE(ERROR_OUT_OF_POOL_MEMORY);
    VULKAN_SYSTEM_ERROR_VKRESULT_PRINT_CASE(ERROR_INVALID_EXTERNAL_HANDLE);
    VULKAN_SYSTEM_ERROR_VKRESULT_PRINT_CASE(ERROR_SURFACE_LOST_KHR);
    VULKAN_SYSTEM_ERROR_VKRESULT_PRINT_CASE(ERROR_NATIVE_WINDOW_IN_USE_KHR);
    VULKAN_SYSTEM_ERROR_VKRESULT_PRINT_CASE(SUBOPTIMAL_KHR);
    VULKAN_SYSTEM_ERROR_VKRESULT_PRINT_CASE(ERROR_OUT_OF_DATE_KHR);
    VULKAN_SYSTEM_ERROR_VKRESULT_PRINT_CASE(ERROR_INCOMPATIBLE_DISPLAY_KHR);
    VULKAN_SYSTEM_ERROR_VKRESULT_PRINT_CASE(ERROR_VALIDATION_FAILED_EXT);
    VULKAN_SYSTEM_ERROR_VKRESULT_PRINT_CASE(ERROR_INVALID_SHADER_NV);
    VULKAN_SYSTEM_ERROR_VKRESULT_PRINT_CASE(ERROR_NOT_PERMITTED_EXT);
    }
#undef VULKAN_SYSTEM_ERROR_VKRESULT_PRINT_CASE
}
}
}

namespace std
{
template <>
struct is_error_code_enum<vulkan_system_error::vulkan_errc> : public true_type {};
}

#ifdef VULKAN_SYSTEM_ERROR_INSTANTIATE_IMPLEMENTATION
namespace vulkan_system_error {
std::error_category const& vulkan_category() {
    static detail::vulkan_category_impl ret;
    return ret;
}
}
#endif
#endif
