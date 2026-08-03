add_library(kernel_obj OBJECT
    gates.c
    trap.c
    mem.c
    traps.asm
    # ... 其他汇编文件
)

# 对于 NASM 源文件，可能需要设置其语言属性
# 如果使用 gas (GNU Assembler)，CMake 通常能自动识别
set_source_files_properties(traps.asm PROPERTIES LANGUAGE ASM_NASM)

target_compile_options(kernel_obj PRIVATE
    $<$<COMPILE_LANGUAGE:C>:-ffreestanding -fno-stack-protector -fno-builtin>
)

target_include_directories(kernel_obj PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
)