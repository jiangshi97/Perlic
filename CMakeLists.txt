set(CMAKE_ASM_NASM_COMPILER nasm)

# 根目录 CMakeLists.txt
cmake_minimum_required(VERSION 3.13)
# 声明项目，支持 C 和汇编语言
project(Perlic C ASM_NASM)

# 启用汇编语言支持
enable_language(ASM)

# 设置 C 标准
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)


# 添加子目录，实现模块化管理
add_subdirectory(code/boot)
add_subdirectory(code/debug)
add_subdirectory(code/kernel)

# 创建最终的内核可执行文件目标
add_executable(kernel
    $<TARGET_OBJECTS:boot_obj>
    $<TARGET_OBJECTS:debug_obj>
    $<TARGET_OBJECTS:kernel_obj>)



# 指定链接器脚本[reference:10]
target_link_options(kernel PRIVATE
-Wl,--no-gc-sections
-static
    -nostdlib
    -nostartfiles
    -nodefaultlibs
    -no-pie -fno-pie
     -T ${CMAKE_CURRENT_SOURCE_DIR}/code/kernel.lds)




# 可选：添加自定义目标，生成ISO镜像或直接运行QEMU
# add_custom_target(run-qemu ...
add_custom_target(build-iso
    COMMAND cmake --build ${CMAKE_SOURCE_DIR}/build
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/build/kernel ${CMAKE_SOURCE_DIR}/target/live
    COMMAND grub-mkrescue -o ${CMAKE_SOURCE_DIR}/build/my_os.iso ${CMAKE_SOURCE_DIR}/target
    VERBATIM
)

add_custom_target(run-qemu
    COMMAND cmake --build ${CMAKE_SOURCE_DIR}/build
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/build/kernel ${CMAKE_SOURCE_DIR}/target/live
    COMMAND grub-mkrescue -o ${CMAKE_SOURCE_DIR}/build/my_os.iso ${CMAKE_SOURCE_DIR}/target
    COMMAND qemu-system-x86_64 -D ${CMAKE_SOURCE_DIR}/qemu-debug-log -d cpu_reset -serial stdio -cdrom ${CMAKE_SOURCE_DIR}/build/my_os.iso
    VERBATIM
)

add_custom_target(run-bochs
    COMMAND cmake --build ${CMAKE_SOURCE_DIR}/build
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/build/kernel ${CMAKE_SOURCE_DIR}/target/live
    COMMAND grub-mkrescue -o ${CMAKE_SOURCE_DIR}/build/my_os.iso ${CMAKE_SOURCE_DIR}/target
    COMMAND bochs -f ${CMAKE_SOURCE_DIR}/bochsrc.txt -q
    VERBATIM
)