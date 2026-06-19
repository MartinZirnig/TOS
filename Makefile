MAKEFLAGS += --no-print-directory

CC = gcc
ASM = nasm
LD = ld

DUMP = dump
BUILD = build
DISK = $(BUILD)/os_disk.img

INCLUDE_DIRS = \
    src/shared \
    src/shared/Math \
    src/shared/Paging \
    src/shared/Context

INCLUDES = $(addprefix -I,$(INCLUDE_DIRS))

# ---------------------------------------------------------------------
# SPECIFICKÉ PŘÍZNAKY PRO 32-BIT (BOOTSTRAP) A 64-BIT (KERNEL)
# ---------------------------------------------------------------------

# Společný základ pro C kompilátor (freestanding OS vývoj)
CFLAGS_BASE = -ffreestanding -nostdinc -fno-pic -fno-pie -fno-stack-protector -nostdlib -mno-red-zone -mno-mmx -mno-80387 $(INCLUDES)

# 32-bit (Bootstrap) flagy
CFLAGS_32  = -m32 $(CFLAGS_BASE)
ASMFLAGS_32 = -f elf32
LDFLAGS_32  = -m elf_i386 --no-warn-rwx-segments --oformat binary -Map=$(BUILD)/bootstrap.map

CFLAGS_64 = -m64 -mcmodel=kernel -fno-plt -g3 -O0 $(CFLAGS_BASE)
ASMFLAGS_64 = -f elf64 -g -F dwarf
LDFLAGS_64  = -m elf_x86_64 --no-warn-rwx-segments -z noexecstack -Map=$(BUILD)/kernel.map


# ---------------------------------------------------------------------
# DETEKCE ZDROJÁKŮ A EXPLICITNÍ SEPARACE OBJEKTŮ SE SUFFIXY
# ---------------------------------------------------------------------
C_SOURCES   = $(shell find src -name "*.c")
ASM_SOURCES = $(shell find src -name "*.asm")

# Objekty Bootloaderu (pouze hlavní soubor, dostane __asm)
BOOTLOADER_OBJS = $(BUILD)/bootloader/bootloader__asm.o

# Objekty Bootstrapu (vše v src/bootstrap/ kompilujeme jako 32-bit)
BOOTSTRAP_C_SRCS   = $(shell find src/bootstrap -name "*.c")
BOOTSTRAP_ASM_SRCS = $(shell find src/bootstrap -name "*.asm")

# Vstupní bod je teď bootstrap__asm.o
BOOTSTRAP_ENTRY    = $(BUILD)/bootstrap/bootstrap__asm.o

# Generování objektů se suffixy __c a __asm
BOOTSTRAP_REST     = $(patsubst src/%.c,$(BUILD)/%__c.o,$(BOOTSTRAP_C_SRCS)) \
                     $(patsubst src/%.asm,$(BUILD)/%__asm.o,$(BOOTSTRAP_ASM_SRCS))

# Složíme objekty tak, aby bootstrap__asm.o (vstupní bod) byl VŽDY na prvním místě!
BOOTSTRAP_OBJS     = $(BOOTSTRAP_ENTRY) $(filter-out $(BOOTSTRAP_ENTRY),$(BOOTSTRAP_REST))

# Objekty Kernelu (vše v src/kernel/ kompilujeme jako 64-bit)
KERNEL_C_SRCS   = $(shell find src/kernel -name "*.c")
KERNEL_ASM_SRCS = $(shell find src/kernel -name "*.asm")

# !!! POZOR: Zde případně uprav název souboru podle tvé reálné struktury !!!
KERNEL_ENTRY    = $(BUILD)/kernel/kernel__asm.o

KERNEL_REST     = $(patsubst src/%.c,$(BUILD)/%__c.o,$(KERNEL_C_SRCS)) \
                  $(patsubst src/%.asm,$(BUILD)/%__asm.o,$(KERNEL_ASM_SRCS))

# Složíme objekty tak, aby vstupní bod kernelu byl VŽDY na prvním místě!
KERNEL_OBJS     = $(KERNEL_ENTRY) $(filter-out $(KERNEL_ENTRY),$(KERNEL_REST))

# Objekty Shared (Adresář src/shared/ se zkompiluje 2x - jednou pro 32b, jednou pro 64b)
SHARED_C_SRCS   = $(shell find src/shared -name "*.c")
SHARED_ASM_SRCS = $(shell find src/shared -name "*.asm")

SHARED_OBJS_32  = $(patsubst src/%.c,$(BUILD)/%__c_32.o,$(SHARED_C_SRCS)) \
                  $(patsubst src/%.asm,$(BUILD)/%__asm_32.o,$(SHARED_ASM_SRCS))

SHARED_OBJS_64  = $(patsubst src/%.c,$(BUILD)/%__c_64.o,$(SHARED_C_SRCS)) \
                  $(patsubst src/%.asm,$(BUILD)/%__asm_64.o,$(SHARED_ASM_SRCS))


.PHONY: all clean run env-init env-fill docker-build docker-run build run-kill gdb dump dump-clean

# Hlavní cíl sestaví všechny 3 binárky
all: $(BUILD)/bootloader.bin $(BUILD)/bootstrap.bin $(BUILD)/kernel.bin

# ---------------------------------------------------------------------
# KOMPILACE PRO BOOTLOADER
# ---------------------------------------------------------------------
$(BUILD)/bootloader/%__asm.o: src/bootloader/%.asm
	@mkdir -p $(dir $@)
	@$(ASM) -f elf32 $< -o $@

# ---------------------------------------------------------------------
# KOMPILACE PRO BOOTSTRAP (Čistý 32-bit s rozlišením suffixů)
# ---------------------------------------------------------------------
$(BUILD)/bootstrap/%__c.o: src/bootstrap/%.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS_32) -c $< -o $@

$(BUILD)/bootstrap/%__asm.o: src/bootstrap/%.asm
	@mkdir -p $(dir $@)
	@$(ASM) $(ASMFLAGS_32) $< -o $@

# Kompilace sdílených souborů pro 32-bit bootstrap
$(BUILD)/shared/%__c_32.o: src/shared/%.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS_32) -c $< -o $@

$(BUILD)/shared/%__asm_32.o: src/shared/%.asm
	@mkdir -p $(dir $@)
	@$(ASM) $(ASMFLAGS_32) $< -o $@

# ---------------------------------------------------------------------
# KOMPILACE PRO KERNEL (Čistý 64-bit s rozlišením suffixů)
# ---------------------------------------------------------------------
$(BUILD)/kernel/%__c.o: src/kernel/%.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS_64) -c $< -o $@

$(BUILD)/kernel/%__asm.o: src/kernel/%.asm
	@mkdir -p $(dir $@)
	@$(ASM) $(ASMFLAGS_64) $< -o $@

# Kompilace sdílených souborů pro 64-bit kernel
$(BUILD)/shared/%__c_64.o: src/shared/%.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS_64) -c $< -o $@

$(BUILD)/shared/%__asm_64.o: src/shared/%.asm
	@mkdir -p $(dir $@)
	@$(ASM) $(ASMFLAGS_64) $< -o $@


# ---------------------------------------------------------------------
# PRAVIDLA PRO LINKOVÁNÍ
# ---------------------------------------------------------------------

# 1) STAGE 1: Bootloader
$(BUILD)/bootloader.bin: $(BOOTLOADER_OBJS)
	@mkdir -p $(BUILD)
	@echo ""
	@echo "=================================================="
	@echo "   LINKING STAGE 1: Bootloader (0x7C00) [16-bit]"
	@echo "=================================================="
	@$(LD) $(LDFLAGS_32) -T linker/bootloader.ld -o $@ $^

# 2) STAGE 2: Bootstrap (Linkujeme 32-bitové objekty + 32-bit shared)
$(BUILD)/bootstrap.bin: $(BOOTSTRAP_OBJS) $(SHARED_OBJS_32)
	@mkdir -p $(BUILD)
	@echo ""
	@echo "=================================================="
	@echo "   LINKING STAGE 2: Bootstrap (0x8000) [32-bit]"
	@echo "=================================================="
	@$(LD) $(LDFLAGS_32) -T linker/bootstrap.ld -o $@ $^

# 3) STAGE 3: Kernel (Nejprve sestavíme ELF64 a z něj vytáhneme čistou binárku přes objcopy)
$(BUILD)/kernel.bin: $(KERNEL_OBJS) $(SHARED_OBJS_64)
	@mkdir -p $(BUILD)
	@echo ""
	@echo "=================================================="
	@echo "   LINKING STAGE 3: Kernel (ELF64)"
	@echo "=================================================="
	@$(LD) $(LDFLAGS_64) -T linker/kernel.ld -o $(BUILD)/kernel.elf $^
	@echo "   EXTRACTING BINARY FROM ELF"
	@objcopy -O binary $(BUILD)/kernel.elf $@

dump:
	@mkdir -p $(DUMP)
	@objdump -d -M intel,intel-mnemonic $(BUILD)/kernel.elf > $(DUMP)/kernel_elf.txt
	@objdump -D -M intel,intel-mnemonic -b binary -m i386:x86-64 $(BUILD)/kernel.bin > $(DUMP)/kernel.txt
	@objdump -D -M intel,intel-mnemonic -b binary -m i386 $(BUILD)/bootstrap.bin > $(DUMP)/bootstrap.txt
	@objdump -D -M intel,intel-mnemonic -b binary -m i386 $(BUILD)/bootloader.bin > $(DUMP)/bootloader.txt

dump-clean:
	@rm -rf $(DUMP)

# ---------------------------------------------------------------------
# Disk Environment Management & QEMU
# ---------------------------------------------------------------------

env-init:
	@mkdir -p $(BUILD)
	@rm -f $(DISK)
	@qemu-img create -f raw $(DISK) 10G
	@echo "Disk image initialized: $(DISK) (10GB)"

env-fill: all
	@if [ ! -f $(DISK) ]; then echo "Error: Disk image does not exist. Run 'make env-init' first."; exit 1; fi
	@echo ""
	@echo "=================================================="
	@echo "   FLASHING BINARIES TO DISK IMAGE"
	@echo "=================================================="
	@if [ ! -f "$(BUILD)/bootstrap.bin" ]; then echo "ERROR: bootstrap.bin missing!"; exit 1; fi
	@BOOTSTRAP_SIZE=$$(stat -c%s "$(BUILD)/bootstrap.bin"); \
	if [ "$$BOOTSTRAP_SIZE" -gt 32256 ]; then \
		echo "ERROR: bootstrap.bin is too large ($$BOOTSTRAP_SIZE bytes). Max size is 32256 bytes (63 sectors)."; \
		exit 1; \
	fi
	@if [ ! -f "$(BUILD)/kernel.bin" ]; then echo "ERROR: kernel.bin missing! Sestavení ELF64 nebo objcopy selhalo."; exit 1; fi
	@KERNEL_SIZE=$$(stat -c%s "$(BUILD)/kernel.bin"); \
	if [ "$$KERNEL_SIZE" -gt 2097152 ]; then \
		echo "ERROR: kernel.bin is too large ($$KERNEL_SIZE bytes). Max size is 2MB (1 HugePage)."; \
		exit 1; \
	fi
	@echo "-> Writing Bootloader to sector 0..."
	@dd if=$(BUILD)/bootloader.bin of=$(DISK) bs=512 count=1 conv=notrunc 2>/dev/null
	@echo "-> Writing Bootstrap to sector 1 (max 63 sectors)..."
	@dd if=$(BUILD)/bootstrap.bin of=$(DISK) bs=512 seek=1 conv=notrunc 2>/dev/null
	@echo "-> Writing Kernel to sector 64 (aligned to HugePage)..."
	@dd if=$(BUILD)/kernel.bin of=$(DISK) bs=512 seek=64 conv=notrunc 2>/dev/null
	@echo "Done! Disk image is ready to boot."

run:
	@echo "Launching QEMU..."
	@qemu-system-x86_64 \
			-drive format=raw,file=$(DISK) \
			-m 4G \
			-device isa-debug-exit,iobase=0xf4,iosize=0x04 \
			-debugcon file:debug.log \
			-d int,cpu_reset \
			-D qemu.log \
			-no-reboot \
			-no-shutdown \
			-s -S

run-kill:
	@echo "Launching QEMU..."
	@qemu-system-x86_64 \
			-drive format=raw,file=$(DISK) \
			-m 4G \
			-device isa-debug-exit,iobase=0xf4,iosize=0x04 \
			-debugcon file:debug.log \
			-d int,cpu_reset \
			-D qemu.log \
			-no-reboot \
			-s -S
	@tac qemu.log | head -n 1
	@tac debug.log | head -n 1

run-alone:
	@echo "Launching QEMU..."
	@qemu-system-x86_64 \
			-drive format=raw,file=$(DISK) \
			-m 4G \
			-device isa-debug-exit,iobase=0xf4,iosize=0x04 \
			-debugcon file:debug.log \
			-d int,cpu_reset \
			-D qemu.log \
			-no-reboot 
	@tac qemu.log | head -n 1
	@tac debug.log | head -n 1

run-monitor:
	@echo "Launching QEMU..."
	@qemu-system-x86_64 \
			-drive format=raw,file=$(DISK) \
			-m 4G \
			-device isa-debug-exit,iobase=0xf4,iosize=0x04 \
			-debugcon file:debug.log \
			-d int,cpu_reset \
			-D qemu.log \
			-no-reboot  \
			-monitor stdio
	@tac qemu.log | head -n 1
	@tac debug.log | head -n 1

gdb:
	@gdb-multiarch -x gdb/gdb_init.script

clean:
	@echo "Cleaning up build directory..."
	@rm -rf $(BUILD)
	@rm qemu.log -rf
	@rm debug.log -rf

docker-build:
	@docker build -t my-os-dev docker/

docker-run:
	@docker run -it --user $(shell id -u):$(shell id -g) -v $(PWD):/os my-os-dev

build: 
	@clear
	@make clean
	@make dump-clean
	@make all
	@make dump
	@make env-init
	@make env-fill