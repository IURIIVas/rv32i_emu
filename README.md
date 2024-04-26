rv32i_emu
---------
RISC V base integer instruction asm emulator  
This is a learning project to practise C programming skills and RISC V assembly.

### Building from repository
___
```
git clone https://github.com/IURIIVas/rv32i_emu.git
cd rv32i_emu
make riscv_emu
```
### Run
___
Pass the path to the binary file as the first argument to run

To compile riscv asembly to binary file
```
riscv64-unknown-elf-gcc -march=rv32i -mabi=ilp32 -o "path/to/output" "path/to/file.s"
riscv64-unknown-elf-objcopy -O binary "path/to/output" "path/to/output.bin"
```
Run emulator
```
riscv_emu "path/to/output.bin"
```
You can check printed regmap after the program ends

### Sources
Big thanks to [fmash16 emulator](https://github.com/fmash16/riscv_emulator) for parser idea
