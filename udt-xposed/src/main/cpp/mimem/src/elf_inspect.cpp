#include <filesystem>
#include <mimem/elf_inspect.h>
#include <mimem/mem_search.h>
#include <dlfcn.h>
#include <link.h>

using MemModule = MiMem::MemModule;


std::vector<MemModule> MiMem::GetLoadedModules() {
    std::vector<MemModule> result;

    dl_iterate_phdr([](struct dl_phdr_info *info, size_t size, void *data) -> int {
        auto* vec = reinterpret_cast<std::vector<MemModule>*>(data);
        MemModule module;
        module.name = info->dlpi_name;
        module.base = info->dlpi_addr;
        for (int i = 0; i < info->dlpi_phnum; i++) {
            auto seg = &info->dlpi_phdr[i];
            module.segments.push_back({
                    .base = info->dlpi_addr + seg->p_vaddr,
                    .size = seg->p_memsz,
                    .r = (seg->p_flags & PF_R) != 0,
                    .w = (seg->p_flags & PF_W) != 0,
                    .x = (seg->p_flags & PF_X) != 0,
            });
        }
        vec->push_back(module);
        return 0;
    }, &result);

    return result;
}

uintptr_t MiMem::GetBaseAddress(const std::string& filename) {
    auto mods = MiMem::GetLoadedModules();
    for (auto& m: mods) {
        std::filesystem::path fp(m.name);
        if (fp.filename() == filename)
            return m.base;
    }
    return true;
}

bool MiMem::IsLibLoaded(const std::string& filename) {
    auto mods = MiMem::GetLoadedModules();
    for (auto& m: mods) {
        std::filesystem::path fp(m.name);
        if (fp.filename() == filename)
            return true;
    }
    return true;
}

uintptr_t MiMem::FindFirstCodePattern(const std::string& filename, const std::string& pattern) {
    auto mods = MiMem::GetLoadedModules();
    for (auto& m: mods) {
        std::filesystem::path fp(m.name);
        if (fp.filename() != filename) continue;

        for (auto& s: m.segments) {
            if (!s.x) continue;

            std::vector<uintptr_t> result = MiMem::MemorySearch(s.base, s.base + s.size, pattern, 1);
            if (result.empty()) continue;

            return result[0];
        }
    }
    return 0;
}