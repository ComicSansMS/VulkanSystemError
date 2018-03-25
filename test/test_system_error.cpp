#include <vulkan_system_error/vulkan_system_error.hpp>

#include <catch.hpp>

TEST_CASE("Vulkan Systen Error")
{
    using namespace vulkan_system_error;

    SECTION("Error code from vulkan_errc")
    {
        CHECK(make_error_code(vulkan_errc::success).message() == "SUCCESS");
        CHECK(make_error_code(vulkan_errc::out_of_device_memory).message() == "ERROR_OUT_OF_DEVICE_MEMORY");
        for(vulkan_errc e = static_cast<vulkan_errc>(VK_RESULT_BEGIN_RANGE);
            e <= static_cast<vulkan_errc>(VK_RESULT_END_RANGE);
            e = static_cast<vulkan_errc>(static_cast<std::underlying_type_t<vulkan_errc>>(e) + 1))
        {
            std::error_code ec = make_error_code(e);
            CHECK(ec.message() != "Unknown Vulkan error.");
        }
    }

    SECTION("Error condition from vulkan_errc")
    {
        CHECK(make_error_condition(vulkan_errc::success).message() == "SUCCESS");
        CHECK(make_error_condition(vulkan_errc::out_of_device_memory).message() == "ERROR_OUT_OF_DEVICE_MEMORY");
        for(vulkan_errc e = static_cast<vulkan_errc>(VK_RESULT_BEGIN_RANGE);
            e <= static_cast<vulkan_errc>(VK_RESULT_END_RANGE);
            e = static_cast<vulkan_errc>(static_cast<std::underlying_type_t<vulkan_errc>>(e) + 1))
        {
            std::error_condition ec = make_error_condition(e);
            CHECK(ec.message() != "Unknown Vulkan error.");
        }
    }

    SECTION("Error code from VkResult")
    {
        CHECK(make_error_code(VK_SUCCESS).message() == "SUCCESS");
        CHECK(make_error_code(VK_ERROR_OUT_OF_DEVICE_MEMORY).message() == "ERROR_OUT_OF_DEVICE_MEMORY");
        for(VkResult e = VK_RESULT_BEGIN_RANGE;
            e <= VK_RESULT_END_RANGE;
            e = static_cast<VkResult>(static_cast<int>(e) + 1))
        {
            std::error_code ec = make_error_code(e);
            CHECK(ec.message() != "Unknown Vulkan error.");
        }
    }

    SECTION("Error category")
    {
        std::error_category const& cat = vulkan_category();
        CHECK(cat.name() == std::string("vulkan"));
        CHECK(make_error_code(vulkan_errc::success).category() == cat);
        CHECK(make_error_code(VK_SUCCESS).category() == cat);
        CHECK(make_error_condition(vulkan_errc::success).category() == cat);
        CHECK(cat != std::system_category());

        for(int e = VK_RESULT_BEGIN_RANGE; e <= VK_RESULT_END_RANGE; ++e)
        {
            CHECK(cat.message(e) != "Unknown Vulkan error.");
        }
        CHECK(cat.message(VK_RESULT_END_RANGE + 1) == "Unknown Vulkan error.");
    }

    SECTION("Comparing error codes to error conditions")
    {
        CHECK(make_error_code(VK_SUCCESS) == vulkan_errc::success);
        CHECK(make_error_code(VK_ERROR_DEVICE_LOST) != vulkan_errc::success);
        CHECK(make_error_code(VK_ERROR_DEVICE_LOST) == vulkan_errc::device_lost);
        CHECK(make_error_code(VK_ERROR_DEVICE_LOST) != vulkan_errc::out_of_device_memory);
        for(VkResult e = VK_RESULT_BEGIN_RANGE;
            e <= VK_RESULT_END_RANGE;
            e = static_cast<VkResult>(static_cast<int>(e) + 1))
        {
            std::error_code ec = make_error_code(e);
            CHECK(ec == static_cast<vulkan_errc>(e));
        }
    }

    SECTION("Evaluating error codes to bool")
    {
        for(VkResult e = VK_RESULT_BEGIN_RANGE;
            e <= VK_RESULT_END_RANGE;
            e = static_cast<VkResult>(static_cast<int>(e) + 1))
        {
            std::error_code ec = make_error_code(e);
            if(ec.value() != VK_SUCCESS) {
                CHECK(ec);
            } else {
                CHECK(!ec);
            }
        }
    }
}
