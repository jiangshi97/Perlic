#include "cppl.hpp"

// 普通 new（分配失败返回 nullptr，因为异常被禁用）
void* operator new(size_t size) {
    return kmalloc(size);
}

void* operator new[](size_t size) {
    return kmalloc(size);
}


// 对应的 delete
void operator delete(void* ptr) noexcept {
    kfree(ptr);
}

void operator delete[](void* ptr) noexcept {
    kfree(ptr);
}

// 带大小参数的 C++14 版本（可选，但为了兼容性可以加上）
void operator delete(void* ptr, size_t) noexcept {
    kfree(ptr);
}

void operator delete[](void* ptr, size_t) noexcept {
    kfree(ptr);
}