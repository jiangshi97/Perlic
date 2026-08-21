add_library(cpplib_obj OBJECT
    cppl.cpp
    cpplib.cpp
)

# 对于 NASM 源文件，可能需要设置其语言属性
# 如果使用 gas (GNU Assembler)，CMake 通常能自动识别
# set_source_files_properties(traps.asm PROPERTIES LANGUAGE ASM_NASM)

target_compile_options(cpplib_obj PRIVATE
    $<$<COMPILE_LANGUAGE:C>:-ffreestanding -fno-stack-protector -fno-builtin -Werror>
    $<$<COMPILE_LANGUAGE:CXX>:
        -ffreestanding
        -fno-stack-protector
        -fno-builtin
        -fno-exceptions      # 禁用 C++ 异常
        -fno-rtti            # 禁用运行时类型信息
        -std=c++11           # 或 c++14/17,按需选择
        -Werror
        -nostdinc++          # 可选，防止搜索系统 C++ 头文件(如果你自备 libc++)
    >
)

target_include_directories(cpplib_obj PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/kernel
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/driver
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/cpplib
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/clib
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/boot
)