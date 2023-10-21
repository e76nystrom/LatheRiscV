#!/bin/bash

#make USER_FLAGS+=-DUART0_SIM_MODE exe

make exe

#../../image_gen/image_gen -app_img main.bin ../../../../Altera/LatheNew/neorv32/neorv32_application_image.vhd

if [ "$(expr substr $(uname -s) 1 5)" == "Linux" ]; then
 ../../image_gen/image_gen -app_img main.bin neorv32_application_image.vhd
 riscv32-unknown-elf-objdump --disassemble-all main.elf >tmp.lst
else
 ../../image_gen/image_gen.exe -app_img main.bin neorv32_application_image.vhd
 riscv-none-elf-objdump --disassemble-all main.elf >tmp.lst
fi
