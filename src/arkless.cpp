#include <rex/runtime/guest/context.h>
#include <rex/runtime/guest/types.h>
#include <rex/logging.h>
#include <filesystem>
#include <system_error>
#include <string_view>

// handles the .. folders in the ARK
static std::filesystem::path SanitizePath(const char* cc) {
    std::filesystem::path result;
    for (const auto& part : std::filesystem::path(cc)) {
        if (part == "..") {
            result /= "(..)";
        } else {
            result /= part;
        }
    }
    return result;
}

void NewFileHook(PPCRegister& r3, PPCRegister& r4) {
    uint32_t cc_addr = r3.u32;
    if (!cc_addr || !rex::runtime::guest::g_memory_base) return;

    const char* cc = reinterpret_cast<const char*>(rex::runtime::guest::g_memory_base + cc_addr);
    if (!cc || !*cc) return;

    std::error_code ec;
    std::filesystem::path sanitized = SanitizePath(cc);

    bool exists = std::filesystem::exists(sanitized, ec);

    if (!exists && sanitized.begin() != sanitized.end() && *sanitized.begin() != "assets") {
        exists = std::filesystem::exists(std::filesystem::path("assets") / sanitized, ec);
    }

    if (exists) {
        REXLOG_INFO("NewFile: {} [flags={:#x}]", cc, r4.u32);
        r4.u64 = r4.u32 | 0x10000;
    } else {
        REXLOG_INFO("NewFile: {} (ARK) [flags={:#x}]", cc, r4.u32);
    }
}