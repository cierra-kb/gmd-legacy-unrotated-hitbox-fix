#include <cmath>    // std::fabs
#include <cstdint>  // std::uintptr_t
#include <dlfcn.h>  // dlsym, RTLD_NOW
#include <dobby.h>  // DobbyHook
#include <utility>  // std::swap

namespace mem {
template <typename T>
T& field_from_offset(void* base, std::uintptr_t offset) {
    return *reinterpret_cast<T*>(reinterpret_cast<std::uintptr_t>(base) + offset);
}
} // namespace mem

struct CCSize {
    char inherited[0x18];
    float width, height;
};

struct CCRect {
    char inherited[0x18];
    char origin[0x20];
    CCSize size;
};

const CCRect (*TRAM_GameObject_getObjectRect)(void* self, float scaleX, float scaleY);
const CCRect GameObject_getObjectRect(void* self, float scaleX, float scaleY) {
    CCRect objectRect = TRAM_GameObject_getObjectRect(self, scaleX, scaleY);
    float rotation = std::fabs(mem::field_from_offset<float>(self, 0x20));

    if (rotation == 90.0f || rotation == 270.0f) {
        std::swap(objectRect.size.width, objectRect.size.height);
    }

    return objectRect;
}

[[gnu::constructor]]
int main() {
    void* handle = dlopen("libgame.so", RTLD_NOW);

    DobbyHook(
        dlsym(handle, "_ZN10GameObject13getObjectRectEff"),
        reinterpret_cast<void*>(GameObject_getObjectRect),
        reinterpret_cast<void**>(&TRAM_GameObject_getObjectRect)
    );
}
