Vulkan System Error
===================

A single-header implementation of C++ system error classes for Vulkan error codes.

Defines a `std::error_category` for Vulkan errors and provides functions for creating `std::error_code`s and `std::error_condition`s from Vulkan `VkResult` values.

How to use
----------

`#include` the `vulkan_system_error.hpp` header.

In *exactly* one `.cpp` file, `#define VULKAN_SYSTEM_ERROR_INSTANTIATE_IMPLEMENTATION` before including the header to instantiate the implementation for the `error_category`.

Error codes can be created by from the `vulkan_system_error::errc` enum values or directly from `VkResult` values:

    std::error_code ec = make_error_code(vulkan_system_error::success);
    // or equivalently
    std::error_code ec = vulkan_system_error::make_error_code(VK_SUCCESS);

Error codes can be implicitly converted to `bool` or compared directly to `vulkan_system_error::errc` enum values:

    assert(!vulkan_system_error::make_error_code(VK_SUCCESS));
    assert(!vulkan_system_error::make_error_code(VK_ERROR_DEVICE_LOST));
    assert(vulkan_system_error::make_error_code(VK_SUCCESS) == vulkan_system_error::success);

Known Issues
------------

While Vulkan interprets only negative values as hard errors, `std::error_code` considers any `0` value an error. Therefore, only error codes constructed from `VK_SUCCESS` will evaluate as success. There is no distinction between soft errors like `VK_NOT_READY` and hard errors like `VK_ERROR_DEVICE_LOST`.
