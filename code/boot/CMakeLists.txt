# arch/x86_64/CMakeLists.txt
# 创建一个静态库，包含所有汇编源文件
add_library(boot_obj OBJECT
    boot.asm
    bootc.c
    # ... 其他汇编文件
)

target_compile_options(boot_obj PRIVATE 
    $<$<COMPILE_LANGUAGE:C>:-ffreestanding -fno-stack-protector -fno-builtin -Werror>
)
# 对于 NASM 源文件，可能需要设置其语言属性
# 如果使用 gas (GNU Assembler)，CMake 通常能自动识别
set_source_files_properties(boot.asm PROPERTIES LANGUAGE ASM_NASM)

target_include_directories(boot_obj PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
)