cmake_minimum_required(VERSION 3.22)

set(CMAKE_C_COMPILER riscv32-unknown-elf-gcc)
set(CMAKE_CXX_COMPILER riscv32-unknown-elf-g++)
set(CMAKE_ASM_COMPILER  riscv32-unknown-elf-gcc)
set(CMAKE_AR riscv32-unknown-elf-ar)
set(CMAKE_OBJCOPY riscv32-unknown-elf-objcopy)
set(CMAKE_OBJDUMP riscv32-unknown-elf-objdump)
set(SIZE riscv32-unknown-elf-size)

#set(CMAKE_C_COMPILER riscv-none-elf-gcc)
#set(CMAKE_CXX_COMPILER riscv-none-elf-g++)
#set(CMAKE_ASM_COMPILER  riscv-none-elf-gcc)
#set(CMAKE_AR riscv-none-elf-ar)
#set(CMAKE_OBJCOPY riscv-none-elf-objcopy)
#set(CMAKE_OBJDUMP riscv-none-elf-objdump)
#set(SIZE riscv-none-elf-size)
set(VERBOSE on)

project(RiscVTest C ASM)
set(CMAKE_C_STANDARD 11)

#add_compile_options(-march=rv32im_zba_zbb_zbc_zbs_zicsr -mabi=ilp32)
add_compile_options(-march=rv32im_zicsr -mabi=ilp32)
add_compile_options(-Og -Wall  -Wstrict-prototypes)
add_compile_options(-ffunction-sections
        -fdata-sections
        -mno-fdiv
        -mstrict-align
        -mbranch-cost=10
        -fno-peephole2
        -g -Wl,--gc-sections)

include_directories(
        .
        include
        ../../../../EclipseCPP/LatheCPP/include
        ../../lib/include
)

set(LINKER_SCRIPT /home/eric/neorv32/sw/common/neorv32.ld)
add_link_options(-march=rv32i_zicsr -mabi=ilp32)
add_link_options(-nostartfiles)
add_link_options(-T ${LINKER_SCRIPT})

set(SOURCES
        axisCtl.c
        fpga.c
        Lathe.c
        remCmd.c
        remSerial.c
        riscvFunc.c
)

set(OBJECTS
        ../../lib/source/neorv32_cfs.c
        ../../lib/source/neorv32_cpu.c
        ../../lib/source/neorv32_cpu_cfu.c
        ../../lib/source/neorv32_crc.c
        ../../lib/source/neorv32_dma.c
        ../../lib/source/neorv32_gpio.c
        ../../lib/source/neorv32_gptmr.c
        ../../lib/source/neorv32_mtime.c
        ../../lib/source/neorv32_neoled.c
        ../../lib/source/neorv32_onewire.c
        ../../lib/source/neorv32_pwm.c
        ../../lib/source/neorv32_rte.c
        ../../lib/source/neorv32_sdi.c
        ../../lib/source/neorv32_slink.c
        ../../lib/source/neorv32_spi.c
        ../../lib/source/neorv32_trng.c
        ../../lib/source/neorv32_twi.c
        ../../lib/source/neorv32_uart.c
        ../../lib/source/neorv32_wdt.c
        ../../lib/source/neorv32_xip.c
        ../../lib/source/neorv32_xirq.c
        ../../lib/source/syscalls.c
        ../../common/crt0.S)

add_executable(${PROJECT_NAME}.elf ${SOURCES} ${LINKER_SCRIPT} ${OBJECTS})

#add_custom_command(
#        POST_BUILD
#        COMMAND "../../image_gen/image_gen -app_img main.bin neorv32_application_image.vhd"
#)
