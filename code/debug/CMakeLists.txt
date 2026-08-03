add_library(debug_obj OBJECT
    serial.c
    sprint.c
    test.c
    # ... 其他汇编文件
)

target_compile_options(debug_obj PRIVATE
    $<$<COMPILE_LANGUAGE:C>:-ffreestanding -fno-stack-protector -fno-builtin>
)

target_include_directories(debug_obj PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
)