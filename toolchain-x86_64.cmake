# toolchain-x86_64.cmake
# 设置目标系统为裸机（无操作系统）[reference:5]
set(CMAKE_SYSTEM_NAME Generic)
# 设置目标处理器架构
set(CMAKE_SYSTEM_PROCESSOR x86_64)

# 指定交叉编译器，这里以 x86_64 裸机工具链为例
# 你可以根据实际安装的编译器修改，如 i686-elf-gcc
set(CMAKE_C_COMPILER gcc)
set(CMAKE_ASM_COMPILER nasm) # 如果使用 NASM

# 告诉 CMake 不要尝试为宿主机系统进行链接测试
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)